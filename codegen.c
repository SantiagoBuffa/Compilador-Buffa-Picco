#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "codegen.h"
#include "estructuras.h"
#include "symtab.h"
#include "tac.h"

// Prototypes for internal recursive visitors
static char* gen_expr(Node* node, TacList* list);
static void gen_stmt(Node* node, TacList* list);
static void gen_method(Node* node, TacList* list);
static void gen_declarations(Node* node, TacList* list);

static char* get_node_tac_name(Node* id_node) {
    if (!id_node) return NULL;
    if (id_node->symbol && id_node->symbol->tac_name) {
        return id_node->symbol->tac_name;
    }
    return id_node->value;
}

static char* gen_expr(Node* node, TacList* list) {
    if (!node) return NULL;

    switch (node->type) {
        case LITERAL_NODE: {
            char* t = new_temp();
            if (strcmp(node->value, "true") == 0) {
                tac_append(list, tac_create_instr(TAC_COPY, t, "1", NULL));
            } else if (strcmp(node->value, "false") == 0) {
                tac_append(list, tac_create_instr(TAC_COPY, t, "0", NULL));
            } else {
                tac_append(list, tac_create_instr(TAC_COPY, t, node->value, NULL));
            }
            return t;
        }

        case ID_NODE: {
            char* name = get_node_tac_name(node);
            return name ? strdup(name) : strdup("unknown");
        }

        case CALL_NODE: {
            // Function call as an expression
            int arg_count = 0;
            Node* arg_list = (node->child_count > 0) ? node->children[0] : NULL;
            if (arg_list && !(arg_list->value && strcmp(arg_list->value, "empty") == 0)) {
                arg_count = arg_list->child_count;
            }

            char** arg_temps = NULL;
            if (arg_count > 0) {
                arg_temps = (char**)malloc(arg_count * sizeof(char*));
                for (int i = 0; i < arg_count; i++) {
                    arg_temps[i] = gen_expr(arg_list->children[i], list);
                }
                for (int i = 0; i < arg_count; i++) {
                    tac_append(list, tac_create_instr(TAC_PARAM, NULL, arg_temps[i], NULL));
                    free(arg_temps[i]);
                }
                free(arg_temps);
            }

            char count_buf[32];
            snprintf(count_buf, sizeof(count_buf), "%d", arg_count);
            char* t_res = new_temp();
            tac_append(list, tac_create_instr(TAC_CALL, t_res, node->value, count_buf));
            return t_res;
        }

        case OPERATION_NODE: {
            char* op = node->value;

            // Unary operators
            if (strcmp(op, "- (unary)") == 0) {
                char* arg = gen_expr(node->children[0], list);
                char* t_res = new_temp();
                tac_append(list, tac_create_instr(TAC_NEG, t_res, arg, NULL));
                free(arg);
                return t_res;
            } else if (strcmp(op, "!") == 0) {
                char* arg = gen_expr(node->children[0], list);
                char* t_res = new_temp();
                tac_append(list, tac_create_instr(TAC_NOT, t_res, arg, NULL));
                free(arg);
                return t_res;
            }

            // Short-circuit logical AND (&&)
            if (strcmp(op, "&&") == 0) {
                char* t_res = new_temp();
                char* l_end = new_label();

                char* left_val = gen_expr(node->children[0], list);
                tac_append(list, tac_create_instr(TAC_COPY, t_res, left_val, NULL));
                tac_append(list, tac_create_instr(TAC_IF_FALSE, l_end, t_res, NULL));
                free(left_val);

                char* right_val = gen_expr(node->children[1], list);
                tac_append(list, tac_create_instr(TAC_COPY, t_res, right_val, NULL));
                free(right_val);

                tac_append(list, tac_create_instr(TAC_LABEL, l_end, NULL, NULL));
                free(l_end);
                return t_res;
            }

            // Short-circuit logical OR (||)
            if (strcmp(op, "||") == 0) {
                char* t_res = new_temp();
                char* l_end = new_label();

                char* left_val = gen_expr(node->children[0], list);
                tac_append(list, tac_create_instr(TAC_COPY, t_res, left_val, NULL));
                tac_append(list, tac_create_instr(TAC_IF_TRUE, l_end, t_res, NULL));
                free(left_val);

                char* right_val = gen_expr(node->children[1], list);
                tac_append(list, tac_create_instr(TAC_COPY, t_res, right_val, NULL));
                free(right_val);

                tac_append(list, tac_create_instr(TAC_LABEL, l_end, NULL, NULL));
                free(l_end);
                return t_res;
            }

            // Binary arithmetic and relational operators
            char* left_val = gen_expr(node->children[0], list);
            char* right_val = gen_expr(node->children[1], list);
            char* t_res = new_temp();

            TacType tt = TAC_ADD;
            if (strcmp(op, "+") == 0) tt = TAC_ADD;
            else if (strcmp(op, "-") == 0) tt = TAC_SUB;
            else if (strcmp(op, "*") == 0) tt = TAC_MUL;
            else if (strcmp(op, "/") == 0) tt = TAC_DIV;
            else if (strcmp(op, "%") == 0) tt = TAC_MOD;
            else if (strcmp(op, "<") == 0) tt = TAC_LT;
            else if (strcmp(op, ">") == 0) tt = TAC_GT;
            else if (strcmp(op, "==") == 0) tt = TAC_EQ;

            tac_append(list, tac_create_instr(tt, t_res, left_val, right_val));
            free(left_val);
            free(right_val);
            return t_res;
        }

        default:
            return NULL;
    }
}

static void gen_stmt(Node* node, TacList* list) {
    if (!node) return;

    switch (node->type) {
        case ASSIGNMENT_NODE: {
            Node* id_node = node->children[0];
            Node* expr_node = node->children[1];
            char* rhs = gen_expr(expr_node, list);
            char* lhs = get_node_tac_name(id_node);
            tac_append(list, tac_create_instr(TAC_COPY, lhs, rhs, NULL));
            if (rhs) free(rhs);
            break;
        }

        case CALL_NODE: {
            // Function call as statement
            int arg_count = 0;
            Node* arg_list = (node->child_count > 0) ? node->children[0] : NULL;
            if (arg_list && !(arg_list->value && strcmp(arg_list->value, "empty") == 0)) {
                arg_count = arg_list->child_count;
            }

            char** arg_temps = NULL;
            if (arg_count > 0) {
                arg_temps = (char**)malloc(arg_count * sizeof(char*));
                for (int i = 0; i < arg_count; i++) {
                    arg_temps[i] = gen_expr(arg_list->children[i], list);
                }
                for (int i = 0; i < arg_count; i++) {
                    tac_append(list, tac_create_instr(TAC_PARAM, NULL, arg_temps[i], NULL));
                    free(arg_temps[i]);
                }
                free(arg_temps);
            }

            char count_buf[32];
            snprintf(count_buf, sizeof(count_buf), "%d", arg_count);
            tac_append(list, tac_create_instr(TAC_CALL_VOID, NULL, node->value, count_buf));
            break;
        }

        case IF_NODE: {
            bool has_else = (node->value && strcmp(node->value, "else") == 0);
            Node* cond_node = node->children[0];
            Node* then_node = node->children[1];

            if (has_else) {
                Node* else_node = node->children[2];
                char* cond_res = gen_expr(cond_node, list);
                char* l_else = new_label();
                char* l_end = new_label();

                tac_append(list, tac_create_instr(TAC_IF_FALSE, l_else, cond_res, NULL));
                free(cond_res);

                gen_stmt(then_node, list);
                tac_append(list, tac_create_instr(TAC_GOTO, l_end, NULL, NULL));

                tac_append(list, tac_create_instr(TAC_LABEL, l_else, NULL, NULL));
                gen_stmt(else_node, list);

                tac_append(list, tac_create_instr(TAC_LABEL, l_end, NULL, NULL));
                free(l_else);
                free(l_end);
            } else {
                char* cond_res = gen_expr(cond_node, list);
                char* l_end = new_label();

                tac_append(list, tac_create_instr(TAC_IF_FALSE, l_end, cond_res, NULL));
                free(cond_res);

                gen_stmt(then_node, list);
                tac_append(list, tac_create_instr(TAC_LABEL, l_end, NULL, NULL));
                free(l_end);
            }
            break;
        }

        case WHILE_NODE: {
            Node* cond_node = node->children[0];
            Node* body_node = node->children[1];

            char* l_start = new_label();
            char* l_end = new_label();

            tac_append(list, tac_create_instr(TAC_LABEL, l_start, NULL, NULL));

            char* cond_res = gen_expr(cond_node, list);
            tac_append(list, tac_create_instr(TAC_IF_FALSE, l_end, cond_res, NULL));
            free(cond_res);

            gen_stmt(body_node, list);
            tac_append(list, tac_create_instr(TAC_GOTO, l_start, NULL, NULL));

            tac_append(list, tac_create_instr(TAC_LABEL, l_end, NULL, NULL));
            free(l_start);
            free(l_end);
            break;
        }

        case RETURN_NODE: {
            if (node->child_count > 0 && node->children[0] != NULL) {
                char* ret_val = gen_expr(node->children[0], list);
                tac_append(list, tac_create_instr(TAC_RETURN, NULL, ret_val, NULL));
                free(ret_val);
            } else {
                tac_append(list, tac_create_instr(TAC_RETURN, NULL, NULL, NULL));
            }
            break;
        }

        case BLOCK_NODE: {
            // Block contains children: [0] = VAR_DECL_LIST_NODE, [1] = STATEMENT_LIST_NODE
            if (node->child_count > 1 && node->children[1]->type == STATEMENT_LIST_NODE) {
                gen_stmt(node->children[1], list);
            } else {
                for (int i = 0; i < node->child_count; i++) {
                    gen_stmt(node->children[i], list);
                }
            }
            break;
        }

        case STATEMENT_LIST_NODE: {
            for (int i = 0; i < node->child_count; i++) {
                gen_stmt(node->children[i], list);
            }
            break;
        }

        default:
            break;
    }
}

static void gen_method(Node* node, TacList* list) {
    if (!node || node->type != METHOD_NODE) return;

    char* func_name = node->children[0]->value;
    tac_append(list, tac_create_instr(TAC_FUNC_BEGIN, func_name, NULL, NULL));

    // The method's body block is the last child
    Node* block_node = node->children[node->child_count - 1];
    if (block_node && block_node->type == BLOCK_NODE) {
        gen_stmt(block_node, list);
    }

    tac_append(list, tac_create_instr(TAC_FUNC_END, func_name, NULL, NULL));
}

static void gen_declarations(Node* node, TacList* list) {
    if (!node) return;

    for (int i = 0; i < node->child_count; i++) {
        Node* child = node->children[i];
        if (child->type == VAR_DECL_NODE) {
            // Global variable declaration: each child is an ID_NODE
            for (int j = 0; j < child->child_count; j++) {
                Node* id_node = child->children[j];
                char* g_name = get_node_tac_name(id_node);
                tac_append(list, tac_create_instr(TAC_GLOBAL_VAR, g_name, NULL, NULL));
            }
        } else if (child->type == METHOD_NODE) {
            gen_method(child, list);
        }
    }
}

TacList* codegen_generate(Node *ast_root) {
    tac_reset_counters();
    TacList* list = tac_list_create();

    if (!ast_root) return list;

    if (ast_root->type == PROGRAM_NODE && ast_root->child_count > 0) {
        Node* decls = ast_root->children[0];
        if (decls && decls->type == DECLARATIONS_NODE) {
            gen_declarations(decls, list);
        }
    }

    return list;
}
