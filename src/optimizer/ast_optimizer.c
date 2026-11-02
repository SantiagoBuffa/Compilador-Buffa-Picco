#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "ast_optimizer.h"
#include "estructuras.h"
#include "symtab.h"

#define MAX_OPTIMIZATION_PASSES 10

// Representation of a constant value
typedef struct {
    bool is_const;
    DataType type;
    union {
        int ival;
        double fval;
        bool bval;
    } val;
} ConstVal;

// Entry in the constant environment
typedef struct EnvEntry {
    char* name;
    Symbol* sym;
    ConstVal cval;
    struct EnvEntry* next;
} EnvEntry;

// Environment tracking known constant values of variables
typedef struct ConstEnv {
    EnvEntry* head;
} ConstEnv;

static ConstEnv* env_create(void) {
    ConstEnv* env = (ConstEnv*)malloc(sizeof(ConstEnv));
    if (!env) {
        fprintf(stderr, "Error: couldn't allocate memory for ConstEnv\n");
        exit(EXIT_FAILURE);
    }
    env->head = NULL;
    return env;
}

static void env_free(ConstEnv* env) {
    if (!env) return;
    EnvEntry* curr = env->head;
    while (curr) {
        EnvEntry* next = curr->next;
        free(curr->name);
        free(curr);
        curr = next;
    }
    free(env);
}

static ConstEnv* env_clone(ConstEnv* src) {
    ConstEnv* copy = env_create();
    if (!src) return copy;
    
    EnvEntry* curr = src->head;
    EnvEntry** tail_ptr = &copy->head;
    while (curr) {
        EnvEntry* new_entry = (EnvEntry*)malloc(sizeof(EnvEntry));
        new_entry->name = strdup(curr->name);
        new_entry->sym = curr->sym;
        new_entry->cval = curr->cval;
        new_entry->next = NULL;
        *tail_ptr = new_entry;
        tail_ptr = &new_entry->next;
        curr = curr->next;
    }
    return copy;
}

static ConstVal env_get(ConstEnv* env, const char* name) {
    ConstVal def;
    def.is_const = false;
    def.type = TYPE_UNKNOWN;
    if (!env || !name) return def;

    EnvEntry* curr = env->head;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            return curr->cval;
        }
        curr = curr->next;
    }
    return def;
}

static void env_set_const(ConstEnv* env, Symbol* sym, const char* name, ConstVal cval) {
    if (!env || !name) return;

    EnvEntry* curr = env->head;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            curr->cval = cval;
            if (sym) curr->sym = sym;
            return;
        }
        curr = curr->next;
    }

    // Insert new entry
    EnvEntry* new_entry = (EnvEntry*)malloc(sizeof(EnvEntry));
    new_entry->name = strdup(name);
    new_entry->sym = sym;
    new_entry->cval = cval;
    new_entry->next = env->head;
    env->head = new_entry;
}

static void env_set_unknown(ConstEnv* env, const char* name) {
    if (!env || !name) return;

    EnvEntry* curr = env->head;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            curr->cval.is_const = false;
            return;
        }
        curr = curr->next;
    }

    EnvEntry* new_entry = (EnvEntry*)malloc(sizeof(EnvEntry));
    new_entry->name = strdup(name);
    new_entry->sym = NULL;
    new_entry->cval.is_const = false;
    new_entry->cval.type = TYPE_UNKNOWN;
    new_entry->next = env->head;
    env->head = new_entry;
}

static void env_invalidate_globals(ConstEnv* env) {
    if (!env) return;
    EnvEntry* curr = env->head;
    while (curr) {
        if (curr->sym && curr->sym->is_global) {
            curr->cval.is_const = false;
        }
        curr = curr->next;
    }
}

static bool const_val_equal(ConstVal a, ConstVal b) {
    if (!a.is_const || !b.is_const) return false;
    if (a.type != b.type) return false;
    switch (a.type) {
        case TYPE_INT:     return a.val.ival == b.val.ival;
        case TYPE_FLOAT:   return a.val.fval == b.val.fval;
        case TYPE_BOOLEAN: return a.val.bval == b.val.bval;
        default:           return false;
    }
}

// Merges env_a and env_b into dest. If a variable has identical constant value in both, keep it; else unknown.
static void env_merge(ConstEnv* dest, ConstEnv* env_a, ConstEnv* env_b) {
    if (!env_a || !env_b || !dest) return;

    EnvEntry* curr = env_a->head;
    while (curr) {
        ConstVal b_val = env_get(env_b, curr->name);
        if (const_val_equal(curr->cval, b_val)) {
            env_set_const(dest, curr->sym, curr->name, curr->cval);
        } else {
            env_set_unknown(dest, curr->name);
        }
        curr = curr->next;
    }

    // Also check variables present in env_b but not in env_a
    EnvEntry* curr_b = env_b->head;
    while (curr_b) {
        ConstVal a_val = env_get(env_a, curr_b->name);
        if (!a_val.is_const) {
            env_set_unknown(dest, curr_b->name);
        }
        curr_b = curr_b->next;
    }
}

// Replaces the contents of dest with src
static void env_copy_into(ConstEnv* dest, ConstEnv* src) {
    if (!dest || !src) return;
    // Clear dest entries
    EnvEntry* curr = dest->head;
    while (curr) {
        EnvEntry* next = curr->next;
        free(curr->name);
        free(curr);
        curr = next;
    }
    dest->head = NULL;

    EnvEntry* s = src->head;
    EnvEntry** tail_ptr = &dest->head;
    while (s) {
        EnvEntry* e = (EnvEntry*)malloc(sizeof(EnvEntry));
        e->name = strdup(s->name);
        e->sym = s->sym;
        e->cval = s->cval;
        e->next = NULL;
        *tail_ptr = e;
        tail_ptr = &e->next;
        s = s->next;
    }
}

// Helper functions for mutating AST nodes into literals in-place
static void replace_with_int_literal(Node* node, int val) {
    for (int i = 0; i < node->child_count; i++) {
        freeAST(node->children[i]);
    }
    if (node->children) {
        free(node->children);
        node->children = NULL;
    }
    node->child_count = 0;

    if (node->value) free(node->value);
    char buf[64];
    snprintf(buf, sizeof(buf), "%d", val);
    node->value = strdup(buf);
    node->type = LITERAL_NODE;
    node->eval_type = TYPE_INT;
    node->symbol = NULL;
}

static void replace_with_float_literal(Node* node, double val) {
    for (int i = 0; i < node->child_count; i++) {
        freeAST(node->children[i]);
    }
    if (node->children) {
        free(node->children);
        node->children = NULL;
    }
    node->child_count = 0;

    if (node->value) free(node->value);
    char buf[64];
    snprintf(buf, sizeof(buf), "%g", val);
    if (strchr(buf, '.') == NULL && strchr(buf, 'e') == NULL && strchr(buf, 'E') == NULL) {
        strcat(buf, ".0");
    }
    node->value = strdup(buf);
    node->type = LITERAL_NODE;
    node->eval_type = TYPE_FLOAT;
    node->symbol = NULL;
}

static void replace_with_bool_literal(Node* node, bool val) {
    for (int i = 0; i < node->child_count; i++) {
        freeAST(node->children[i]);
    }
    if (node->children) {
        free(node->children);
        node->children = NULL;
    }
    node->child_count = 0;

    if (node->value) free(node->value);
    node->value = strdup(val ? "true" : "false");
    node->type = LITERAL_NODE;
    node->eval_type = TYPE_BOOLEAN;
    node->symbol = NULL;
}

// Extracts a constant value from a LITERAL_NODE
static bool get_const_val(Node* node, ConstVal* out) {
    if (!node || node->type != LITERAL_NODE || !node->value) return false;

    if (strcmp(node->value, "true") == 0) {
        out->is_const = true;
        out->type = TYPE_BOOLEAN;
        out->val.bval = true;
        return true;
    }
    if (strcmp(node->value, "false") == 0) {
        out->is_const = true;
        out->type = TYPE_BOOLEAN;
        out->val.bval = false;
        return true;
    }
    if (node->eval_type == TYPE_FLOAT || strchr(node->value, '.') != NULL) {
        out->is_const = true;
        out->type = TYPE_FLOAT;
        out->val.fval = atof(node->value);
        return true;
    }
    out->is_const = true;
    out->type = TYPE_INT;
    out->val.ival = atoi(node->value);
    return true;
}

static const char* get_node_var_name(Node* id_node) {
    if (!id_node) return NULL;
    if (id_node->symbol && id_node->symbol->tac_name) {
        return id_node->symbol->tac_name;
    }
    return id_node->value;
}

// Forward declarations
static void fold_expr(Node* node, ConstEnv* env, bool* changed);
static void optimize_stmt(Node* node, ConstEnv* env, bool* changed);
static void optimize_block(Node* node, ConstEnv* env, bool* changed);

// Recursively walks and folds an expression tree
static void fold_expr(Node* node, ConstEnv* env, bool* changed) {
    if (!node) return;

    if (node->type == ID_NODE) {
        const char* name = get_node_var_name(node);
        ConstVal cval = env_get(env, name);
        if (cval.is_const) {
            if (cval.type == TYPE_INT) {
                replace_with_int_literal(node, cval.val.ival);
            } else if (cval.type == TYPE_FLOAT) {
                replace_with_float_literal(node, cval.val.fval);
            } else if (cval.type == TYPE_BOOLEAN) {
                replace_with_bool_literal(node, cval.val.bval);
            }
            *changed = true;
        }
        return;
    }

    if (node->type == CALL_NODE) {
        if (node->child_count > 0 && node->children[0]->type == ARG_LIST_NODE) {
            Node* arg_list = node->children[0];
            for (int i = 0; i < arg_list->child_count; i++) {
                fold_expr(arg_list->children[i], env, changed);
            }
        }
        // Function call side effects invalidate global variables
        env_invalidate_globals(env);
        return;
    }

    if (node->type == OPERATION_NODE) {
        char* op = node->value;

        // Unary operators
        if (strcmp(op, "- (unary)") == 0) {
            fold_expr(node->children[0], env, changed);
            ConstVal arg;
            if (get_const_val(node->children[0], &arg)) {
                if (arg.type == TYPE_INT) {
                    replace_with_int_literal(node, -arg.val.ival);
                    *changed = true;
                } else if (arg.type == TYPE_FLOAT) {
                    replace_with_float_literal(node, -arg.val.fval);
                    *changed = true;
                }
            }
            return;
        }

        if (strcmp(op, "!") == 0) {
            fold_expr(node->children[0], env, changed);
            ConstVal arg;
            if (get_const_val(node->children[0], &arg) && arg.type == TYPE_BOOLEAN) {
                replace_with_bool_literal(node, !arg.val.bval);
                *changed = true;
            }
            return;
        }

        // Short-circuit logical AND (&&)
        if (strcmp(op, "&&") == 0) {
            fold_expr(node->children[0], env, changed);
            ConstVal left_val;
            if (get_const_val(node->children[0], &left_val) && left_val.type == TYPE_BOOLEAN) {
                if (!left_val.val.bval) {
                    // false && ... is always false
                    replace_with_bool_literal(node, false);
                    *changed = true;
                    return;
                }
            }

            fold_expr(node->children[1], env, changed);
            ConstVal right_val;
            if (get_const_val(node->children[0], &left_val) && get_const_val(node->children[1], &right_val)) {
                replace_with_bool_literal(node, left_val.val.bval && right_val.val.bval);
                *changed = true;
            }
            return;
        }

        // Short-circuit logical OR (||)
        if (strcmp(op, "||") == 0) {
            fold_expr(node->children[0], env, changed);
            ConstVal left_val;
            if (get_const_val(node->children[0], &left_val) && left_val.type == TYPE_BOOLEAN) {
                if (left_val.val.bval) {
                    // true || ... is always true
                    replace_with_bool_literal(node, true);
                    *changed = true;
                    return;
                }
            }

            fold_expr(node->children[1], env, changed);
            ConstVal right_val;
            if (get_const_val(node->children[0], &left_val) && get_const_val(node->children[1], &right_val)) {
                replace_with_bool_literal(node, left_val.val.bval || right_val.val.bval);
                *changed = true;
            }
            return;
        }

        // Binary arithmetic and relational operators
        fold_expr(node->children[0], env, changed);
        fold_expr(node->children[1], env, changed);

        ConstVal l, r;
        if (!get_const_val(node->children[0], &l) || !get_const_val(node->children[1], &r)) {
            return;
        }

        // Arithmetic operators
        if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0 ||
            strcmp(op, "*") == 0 || strcmp(op, "/") == 0 || strcmp(op, "%") == 0) {

            if (l.type == TYPE_INT && r.type == TYPE_INT) {
                if (strcmp(op, "+") == 0) {
                    replace_with_int_literal(node, l.val.ival + r.val.ival);
                    *changed = true;
                } else if (strcmp(op, "-") == 0) {
                    replace_with_int_literal(node, l.val.ival - r.val.ival);
                    *changed = true;
                } else if (strcmp(op, "*") == 0) {
                    replace_with_int_literal(node, l.val.ival * r.val.ival);
                    *changed = true;
                } else if (strcmp(op, "/") == 0) {
                    if (r.val.ival != 0) {
                        replace_with_int_literal(node, l.val.ival / r.val.ival);
                        *changed = true;
                    }
                } else if (strcmp(op, "%") == 0) {
                    if (r.val.ival != 0) {
                        replace_with_int_literal(node, l.val.ival % r.val.ival);
                        *changed = true;
                    }
                }
            } else if (l.type == TYPE_FLOAT || r.type == TYPE_FLOAT) {
                double lf = (l.type == TYPE_FLOAT) ? l.val.fval : (double)l.val.ival;
                double rf = (r.type == TYPE_FLOAT) ? r.val.fval : (double)r.val.ival;

                if (strcmp(op, "+") == 0) {
                    replace_with_float_literal(node, lf + rf);
                    *changed = true;
                } else if (strcmp(op, "-") == 0) {
                    replace_with_float_literal(node, lf - rf);
                    *changed = true;
                } else if (strcmp(op, "*") == 0) {
                    replace_with_float_literal(node, lf * rf);
                    *changed = true;
                } else if (strcmp(op, "/") == 0) {
                    if (rf != 0.0) {
                        replace_with_float_literal(node, lf / rf);
                        *changed = true;
                    }
                }
            }
            return;
        }

        // Relational operators
        if (strcmp(op, "<") == 0) {
            if (l.type == TYPE_FLOAT || r.type == TYPE_FLOAT) {
                double lf = (l.type == TYPE_FLOAT) ? l.val.fval : (double)l.val.ival;
                double rf = (r.type == TYPE_FLOAT) ? r.val.fval : (double)r.val.ival;
                replace_with_bool_literal(node, lf < rf);
            } else {
                replace_with_bool_literal(node, l.val.ival < r.val.ival);
            }
            *changed = true;
            return;
        }

        if (strcmp(op, ">") == 0) {
            if (l.type == TYPE_FLOAT || r.type == TYPE_FLOAT) {
                double lf = (l.type == TYPE_FLOAT) ? l.val.fval : (double)l.val.ival;
                double rf = (r.type == TYPE_FLOAT) ? r.val.fval : (double)r.val.ival;
                replace_with_bool_literal(node, lf > rf);
            } else {
                replace_with_bool_literal(node, l.val.ival > r.val.ival);
            }
            *changed = true;
            return;
        }

        if (strcmp(op, "==") == 0) {
            if (l.type == TYPE_BOOLEAN && r.type == TYPE_BOOLEAN) {
                replace_with_bool_literal(node, l.val.bval == r.val.bval);
            } else if (l.type == TYPE_FLOAT || r.type == TYPE_FLOAT) {
                double lf = (l.type == TYPE_FLOAT) ? l.val.fval : (double)l.val.ival;
                double rf = (r.type == TYPE_FLOAT) ? r.val.fval : (double)r.val.ival;
                replace_with_bool_literal(node, lf == rf);
            } else {
                replace_with_bool_literal(node, l.val.ival == r.val.ival);
            }
            *changed = true;
            return;
        }
    }
}

// Scans an AST subtree and marks any modified variables as UNKNOWN
static void collect_assigned_vars(Node* node, ConstEnv* env) {
    if (!node) return;
    if (node->type == ASSIGNMENT_NODE) {
        if (node->child_count > 0 && node->children[0]->type == ID_NODE) {
            const char* name = get_node_var_name(node->children[0]);
            if (name) env_set_unknown(env, name);
        }
    }
    for (int i = 0; i < node->child_count; i++) {
        collect_assigned_vars(node->children[i], env);
    }
}

// Optimizes a single statement
static void optimize_stmt(Node* node, ConstEnv* env, bool* changed) {
    if (!node) return;

    switch (node->type) {
        case ASSIGNMENT_NODE: {
            Node* id_node = node->children[0];
            Node* expr_node = node->children[1];

            fold_expr(expr_node, env, changed);

            ConstVal cval;
            const char* var_name = get_node_var_name(id_node);
            if (get_const_val(expr_node, &cval)) {
                // Coerce constant to destination variable's type if needed
                DataType target_type = id_node->eval_type;
                if (id_node->symbol) target_type = id_node->symbol->type;

                if (target_type == TYPE_FLOAT && cval.type == TYPE_INT) {
                    cval.type = TYPE_FLOAT;
                    cval.val.fval = (double)cval.val.ival;
                } else if (target_type == TYPE_INT && cval.type == TYPE_FLOAT) {
                    cval.type = TYPE_INT;
                    cval.val.ival = (int)cval.val.fval;
                }

                env_set_const(env, id_node->symbol, var_name, cval);
            } else {
                env_set_unknown(env, var_name);
            }
            break;
        }

        case CALL_NODE: {
            if (node->child_count > 0 && node->children[0]->type == ARG_LIST_NODE) {
                Node* arg_list = node->children[0];
                for (int i = 0; i < arg_list->child_count; i++) {
                    fold_expr(arg_list->children[i], env, changed);
                }
            }
            env_invalidate_globals(env);
            break;
        }

        case RETURN_NODE: {
            if (node->child_count > 0 && node->children[0]) {
                fold_expr(node->children[0], env, changed);
            }
            break;
        }

        case IF_NODE: {
            bool has_else = (node->child_count > 2);
            Node* cond_node = node->children[0];
            Node* then_node = node->children[1];

            fold_expr(cond_node, env, changed);

            ConstVal cond_val;
            if (get_const_val(cond_node, &cond_val) && cond_val.type == TYPE_BOOLEAN) {
                if (cond_val.val.bval) {
                    // Condition is always TRUE: then branch executes
                    optimize_block(then_node, env, changed);
                    if (has_else) {
                        ConstEnv* dummy = env_clone(env);
                        optimize_block(node->children[2], dummy, changed);
                        env_free(dummy);
                    }
                } else {
                    // Condition is always FALSE: then branch is dead
                    ConstEnv* dummy = env_clone(env);
                    optimize_block(then_node, dummy, changed);
                    env_free(dummy);
                    if (has_else) {
                        optimize_block(node->children[2], env, changed);
                    }
                }
            } else {
                // Condition is not a compile-time constant
                ConstEnv* env_then = env_clone(env);
                optimize_block(then_node, env_then, changed);

                if (has_else) {
                    ConstEnv* env_else = env_clone(env);
                    optimize_block(node->children[2], env_else, changed);

                    ConstEnv* merged = env_create();
                    env_merge(merged, env_then, env_else);
                    env_copy_into(env, merged);
                    env_free(merged);
                    env_free(env_else);
                } else {
                    ConstEnv* merged = env_create();
                    env_merge(merged, env, env_then);
                    env_copy_into(env, merged);
                    env_free(merged);
                }
                env_free(env_then);
            }
            break;
        }

        case WHILE_NODE: {
            Node* cond_node = node->children[0];
            Node* body_node = node->children[1];

            // Conservative handling: invalidate variables modified in the loop body
            collect_assigned_vars(body_node, env);

            fold_expr(cond_node, env, changed);
            optimize_block(body_node, env, changed);

            // Invalidate again after the loop
            collect_assigned_vars(body_node, env);
            break;
        }

        case BLOCK_NODE: {
            optimize_block(node, env, changed);
            break;
        }

        case STATEMENT_LIST_NODE: {
            for (int i = 0; i < node->child_count; i++) {
                optimize_stmt(node->children[i], env, changed);
            }
            break;
        }

        default:
            break;
    }
}

// Optimizes a block or list of statements
static void optimize_block(Node* node, ConstEnv* env, bool* changed) {
    if (!node) return;

    if (node->type == BLOCK_NODE) {
        if (node->child_count > 1 && node->children[1]->type == STATEMENT_LIST_NODE) {
            optimize_stmt(node->children[1], env, changed);
        } else {
            for (int i = 0; i < node->child_count; i++) {
                optimize_stmt(node->children[i], env, changed);
            }
        }
    } else if (node->type == STATEMENT_LIST_NODE) {
        for (int i = 0; i < node->child_count; i++) {
            optimize_stmt(node->children[i], env, changed);
        }
    } else {
        optimize_stmt(node, env, changed);
    }
}

// Optimizes a method declaration
static void optimize_method(Node* method_node) {
    if (!method_node || method_node->type != METHOD_NODE) return;

    // Last child is the block node
    Node* block_node = method_node->children[method_node->child_count - 1];
    if (!block_node || block_node->type != BLOCK_NODE) return;

    // Run passes until reaching a fixed point (no more changes) or max passes
    int pass = 0;
    bool changed = true;
    while (changed && pass < MAX_OPTIMIZATION_PASSES) {
        changed = false;
        ConstEnv* env = env_create();
        optimize_block(block_node, env, &changed);
        env_free(env);
        pass++;
    }
}

// Top-level entry point for AST optimization
Node* optimize_ast(Node* root) {
    if (!root) return NULL;

    // Navigate to DECLARATIONS_NODE
    Node* decls = (root->type == PROGRAM_NODE && root->child_count > 0) ? root->children[0] : root;
    if (decls && decls->type == DECLARATIONS_NODE) {
        for (int i = 0; i < decls->child_count; i++) {
            if (decls->children[i]->type == METHOD_NODE) {
                optimize_method(decls->children[i]);
            }
        }
    }

    return root;
}
