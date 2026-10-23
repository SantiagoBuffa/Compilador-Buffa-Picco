#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "asm.h"
#include "tac.h"

// Linked list of global symbols
typedef struct GlobalSymbol {
    char *name;
    struct GlobalSymbol *next;
} GlobalSymbol;

static GlobalSymbol *global_symbols = NULL;

static void add_global_symbol(const char *name) {
    if (!name) return;
    GlobalSymbol *curr = global_symbols;
    while (curr) {
        if (strcmp(curr->name, name) == 0) return;
        curr = curr->next;
    }
    GlobalSymbol *node = malloc(sizeof(GlobalSymbol));
    node->name = strdup(name);
    node->next = global_symbols;
    global_symbols = node;
}

static bool is_global(const char *name) {
    if (!name) return false;
    GlobalSymbol *curr = global_symbols;
    while (curr) {
        if (strcmp(curr->name, name) == 0) return true;
        curr = curr->next;
    }
    return false;
}

static void free_global_symbols(void) {
    GlobalSymbol *curr = global_symbols;
    while (curr) {
        GlobalSymbol *next = curr->next;
        free(curr->name);
        free(curr);
        curr = next;
    }
    global_symbols = NULL;
}

// Checks if an operand string is a numeric integer literal
static bool is_number(const char *str) {
    if (!str || *str == '\0') return false;
    int i = 0;
    if (str[i] == '-' || str[i] == '+') i++;
    if (str[i] == '\0') return false;
    while (str[i]) {
        if (!isdigit((unsigned char)str[i])) return false;
        i++;
    }
    return true;
}

// Linked list for local variables, parameters, and temporaries in the stack frame
typedef struct StackVar {
    char *name;
    int offset;
    struct StackVar *next;
} StackVar;

typedef struct {
    const char *func_name;
    StackVar *vars;
    int total_locals;
    int frame_size;
} FuncContext;

static void register_local(FuncContext *ctx, const char *name) {
    if (!name || is_number(name) || is_global(name)) return;
    if (strcmp(name, "true") == 0 || strcmp(name, "false") == 0) return;

    StackVar *curr = ctx->vars;
    while (curr) {
        if (strcmp(curr->name, name) == 0) return;
        curr = curr->next;
    }

    ctx->total_locals++;
    StackVar *var = malloc(sizeof(StackVar));
    var->name = strdup(name);
    var->offset = -8 * ctx->total_locals;
    var->next = ctx->vars;
    ctx->vars = var;
}

static int get_stack_offset(FuncContext *ctx, const char *name) {
    StackVar *curr = ctx->vars;
    while (curr) {
        if (strcmp(curr->name, name) == 0) return curr->offset;
        curr = curr->next;
    }
    // Fallback if not pre-registered
    register_local(ctx, name);
    return ctx->vars->offset;
}

static void free_func_context(FuncContext *ctx) {
    StackVar *curr = ctx->vars;
    while (curr) {
        StackVar *next = curr->next;
        free(curr->name);
        free(curr);
        curr = next;
    }
    ctx->vars = NULL;
    ctx->total_locals = 0;
}

// Helpers for loading and storing operands
static void load_operand(FILE *out, const char *arg, const char *reg, FuncContext *ctx) {
    if (!arg) return;
    if (strcmp(arg, "true") == 0) {
        fprintf(out, "    movq $1, %s\n", reg);
    } else if (strcmp(arg, "false") == 0) {
        fprintf(out, "    movq $0, %s\n", reg);
    } else if (is_number(arg)) {
        fprintf(out, "    movq $%s, %s\n", arg, reg);
    } else if (is_global(arg)) {
        fprintf(out, "    movq %s(%%rip), %s\n", arg, reg);
    } else {
        int offset = get_stack_offset(ctx, arg);
        fprintf(out, "    movq %d(%%rbp), %s\n", offset, reg);
    }
}

static void store_operand(FILE *out, const char *reg, const char *res, FuncContext *ctx) {
    if (!res) return;
    if (is_global(res)) {
        fprintf(out, "    movq %s, %s(%%rip)\n", reg, res);
    } else {
        int offset = get_stack_offset(ctx, res);
        fprintf(out, "    movq %s, %d(%%rbp)\n", reg, offset);
    }
}

// System V AMD64 ABI: First 6 integer/pointer argument registers
static const char *arg_regs[6] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};

void asm_generate(FILE *out, TacList *tac) {
    if (!out || !tac) return;

    free_global_symbols();

    // Pass 1: Collect globals and externs
    TacInstr *curr = tac->head;
    while (curr) {
        if (curr->type == TAC_GLOBAL_VAR) {
            add_global_symbol(curr->res);
        }
        curr = curr->next;
    }

    fprintf(out, "    .text\n");

    // Emit extern directives if any
    curr = tac->head;
    while (curr) {
        if (curr->type == TAC_EXTERN) {
            fprintf(out, "    .extern %s\n", curr->res);
        }
        curr = curr->next;
    }
    fprintf(out, "\n");

    // Pass 2: Emit functions
    FuncContext ctx;
    memset(&ctx, 0, sizeof(ctx));
    int recv_count = 0;

    char *call_args[256];
    int call_arg_count = 0;

    curr = tac->head;
    while (curr) {
        switch (curr->type) {
            case TAC_FUNC_BEGIN: {
                ctx.func_name = curr->res;
                ctx.vars = NULL;
                ctx.total_locals = 0;
                recv_count = 0;
                call_arg_count = 0;

                // Pre-scan this function to register all locals, parameters, and temporaries
                TacInstr *scan = curr->next;
                while (scan && scan->type != TAC_FUNC_END) {
                    if (scan->type != TAC_LABEL && scan->type != TAC_GOTO) {
                        if (scan->res && scan->type != TAC_CALL && scan->type != TAC_CALL_VOID) {
                            register_local(&ctx, scan->res);
                        } else if (scan->res && scan->type == TAC_CALL) {
                            register_local(&ctx, scan->res);
                        }
                    }
                    if (scan->type != TAC_CALL && scan->type != TAC_CALL_VOID) {
                        if (scan->arg1) register_local(&ctx, scan->arg1);
                    }
                    if (scan->arg2) register_local(&ctx, scan->arg2);
                    scan = scan->next;
                }

                // Compute 16-byte aligned frame size
                int count = ctx.total_locals;
                int frame_size = ((count * 8) + 15) & ~15;
                if (frame_size < 16) frame_size = 16;
                ctx.frame_size = frame_size;

                fprintf(out, "    .globl %s\n", ctx.func_name);
                fprintf(out, "    .type %s, @function\n", ctx.func_name);
                fprintf(out, "%s:\n", ctx.func_name);
                fprintf(out, "    enter $%d, $0\n", ctx.frame_size);
                break;
            }

            case TAC_RECV_PARAM: {
                if (recv_count < 6) {
                    store_operand(out, arg_regs[recv_count], curr->res, &ctx);
                } else {
                    int stack_offset = 16 + (recv_count - 6) * 8;
                    fprintf(out, "    movq %d(%%rbp), %%r10\n", stack_offset);
                    store_operand(out, "%r10", curr->res, &ctx);
                }
                recv_count++;
                break;
            }

            case TAC_PARAM: {
                if (call_arg_count < 256) {
                    call_args[call_arg_count++] = curr->arg1;
                }
                break;
            }

            case TAC_CALL:
            case TAC_CALL_VOID: {
                int num_args = call_arg_count;
                int reg_args = num_args < 6 ? num_args : 6;
                int stack_args = num_args > 6 ? (num_args - 6) : 0;
                bool need_align = (stack_args % 2 != 0);

                if (need_align) {
                    fprintf(out, "    subq $8, %%rsp\n");
                }

                // Push stack arguments in reverse order (7th and above)
                for (int i = num_args - 1; i >= 6; i--) {
                    load_operand(out, call_args[i], "%r10", &ctx);
                    fprintf(out, "    pushq %%r10\n");
                }

                // Load first 6 arguments into ABI registers
                for (int i = 0; i < reg_args; i++) {
                    load_operand(out, call_args[i], arg_regs[i], &ctx);
                }

                fprintf(out, "    xorl %%eax, %%eax\n");
                fprintf(out, "    call %s\n", curr->arg1);

                if (stack_args > 0) {
                    int cleanup = stack_args * 8 + (need_align ? 8 : 0);
                    fprintf(out, "    addq $%d, %%rsp\n", cleanup);
                } else if (need_align) {
                    fprintf(out, "    addq $8, %%rsp\n");
                }

                if (curr->type == TAC_CALL) {
                    store_operand(out, "%rax", curr->res, &ctx);
                }

                call_arg_count = 0;
                break;
            }

            case TAC_RETURN: {
                if (curr->arg1) {
                    load_operand(out, curr->arg1, "%rax", &ctx);
                } else if (strcmp(ctx.func_name, "main") == 0) {
                    fprintf(out, "    movq $0, %%rax\n");
                }
                fprintf(out, "    jmp .L%s_epilogue\n", ctx.func_name);
                break;
            }

            case TAC_FUNC_END: {
                if (strcmp(ctx.func_name, "main") == 0) {
                    fprintf(out, "    movq $0, %%rax\n");
                }
                fprintf(out, ".L%s_epilogue:\n", ctx.func_name);
                fprintf(out, "    leave\n");
                fprintf(out, "    ret\n");
                fprintf(out, "    .size %s, . - %s\n\n", ctx.func_name, ctx.func_name);
                free_func_context(&ctx);
                break;
            }

            case TAC_COPY: {
                load_operand(out, curr->arg1, "%r10", &ctx);
                store_operand(out, "%r10", curr->res, &ctx);
                break;
            }

            case TAC_ADD: {
                load_operand(out, curr->arg1, "%r10", &ctx);
                load_operand(out, curr->arg2, "%r11", &ctx);
                fprintf(out, "    addq %%r11, %%r10\n");
                store_operand(out, "%r10", curr->res, &ctx);
                break;
            }

            case TAC_SUB: {
                load_operand(out, curr->arg1, "%r10", &ctx);
                load_operand(out, curr->arg2, "%r11", &ctx);
                fprintf(out, "    subq %%r11, %%r10\n");
                store_operand(out, "%r10", curr->res, &ctx);
                break;
            }

            case TAC_MUL: {
                load_operand(out, curr->arg1, "%r10", &ctx);
                load_operand(out, curr->arg2, "%r11", &ctx);
                fprintf(out, "    imulq %%r11, %%r10\n");
                store_operand(out, "%r10", curr->res, &ctx);
                break;
            }

            case TAC_DIV: {
                load_operand(out, curr->arg1, "%rax", &ctx);
                fprintf(out, "    cqto\n");
                load_operand(out, curr->arg2, "%r10", &ctx);
                fprintf(out, "    idivq %%r10\n");
                store_operand(out, "%rax", curr->res, &ctx);
                break;
            }

            case TAC_MOD: {
                load_operand(out, curr->arg1, "%rax", &ctx);
                fprintf(out, "    cqto\n");
                load_operand(out, curr->arg2, "%r10", &ctx);
                fprintf(out, "    idivq %%r10\n");
                store_operand(out, "%rdx", curr->res, &ctx);
                break;
            }

            case TAC_NEG: {
                load_operand(out, curr->arg1, "%r10", &ctx);
                fprintf(out, "    negq %%r10\n");
                store_operand(out, "%r10", curr->res, &ctx);
                break;
            }

            case TAC_NOT: {
                load_operand(out, curr->arg1, "%r10", &ctx);
                fprintf(out, "    cmpq $0, %%r10\n");
                fprintf(out, "    sete %%al\n");
                fprintf(out, "    movzbq %%al, %%r10\n");
                store_operand(out, "%r10", curr->res, &ctx);
                break;
            }

            case TAC_EQ: {
                load_operand(out, curr->arg1, "%r10", &ctx);
                load_operand(out, curr->arg2, "%r11", &ctx);
                fprintf(out, "    cmpq %%r11, %%r10\n");
                fprintf(out, "    sete %%al\n");
                fprintf(out, "    movzbq %%al, %%r10\n");
                store_operand(out, "%r10", curr->res, &ctx);
                break;
            }

            case TAC_LT: {
                load_operand(out, curr->arg1, "%r10", &ctx);
                load_operand(out, curr->arg2, "%r11", &ctx);
                fprintf(out, "    cmpq %%r11, %%r10\n");
                fprintf(out, "    setl %%al\n");
                fprintf(out, "    movzbq %%al, %%r10\n");
                store_operand(out, "%r10", curr->res, &ctx);
                break;
            }

            case TAC_GT: {
                load_operand(out, curr->arg1, "%r10", &ctx);
                load_operand(out, curr->arg2, "%r11", &ctx);
                fprintf(out, "    cmpq %%r11, %%r10\n");
                fprintf(out, "    setg %%al\n");
                fprintf(out, "    movzbq %%al, %%r10\n");
                store_operand(out, "%r10", curr->res, &ctx);
                break;
            }

            case TAC_LABEL: {
                fprintf(out, ".L%s:\n", curr->res);
                break;
            }

            case TAC_GOTO: {
                fprintf(out, "    jmp .L%s\n", curr->res);
                break;
            }

            case TAC_IF_TRUE: {
                load_operand(out, curr->arg1, "%r10", &ctx);
                fprintf(out, "    cmpq $0, %%r10\n");
                fprintf(out, "    jne .L%s\n", curr->res);
                break;
            }

            case TAC_IF_FALSE: {
                load_operand(out, curr->arg1, "%r10", &ctx);
                fprintf(out, "    cmpq $0, %%r10\n");
                fprintf(out, "    je .L%s\n", curr->res);
                break;
            }

            case TAC_GLOBAL_VAR:
            case TAC_EXTERN:
                // Handled in pass 1 & pass 3
                break;

            default:
                break;
        }
        curr = curr->next;
    }

    // Pass 3: Emit global variables in .bss section
    bool has_globals = false;
    curr = tac->head;
    while (curr) {
        if (curr->type == TAC_GLOBAL_VAR) {
            if (!has_globals) {
                fprintf(out, "    .bss\n");
                has_globals = true;
            }
            fprintf(out, "    .globl %s\n", curr->res);
            fprintf(out, "    .align 8\n");
            fprintf(out, "    .type %s, @object\n", curr->res);
            fprintf(out, "    .size %s, 8\n", curr->res);
            fprintf(out, "%s:\n", curr->res);
            fprintf(out, "    .zero 8\n\n");
        }
        curr = curr->next;
    }

    free_global_symbols();
}
