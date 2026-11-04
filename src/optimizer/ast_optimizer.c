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

// Creates a new constant environment
static ConstEnv* env_create(void) {
    ConstEnv* env = (ConstEnv*)malloc(sizeof(ConstEnv));
    if (!env) {
        fprintf(stderr, "Error: couldn't allocate memory for ConstEnv\n");
        exit(EXIT_FAILURE);
    }
    env->head = NULL;
    return env;
}

// frees up the constant environment and its entries
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

// Clones the constant environment, creating a deep copy
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

// Retrieves the constant value of a variable from the environment, or returns an unknown value if not found
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

// Sets a variable in the environment to a known constant value
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

// Sets a variable in the environment to an unknown value (not a constant)
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

// Invalidates all global variables in the environment, marking them as unknown
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

// Compares two constant values for equality, considering their types and values
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

// Checks if an expression tree contains function calls (side effects)
static bool has_side_effects(Node* node) {
    if (!node) return false;
    if (node->type == CALL_NODE) return true;
    for (int i = 0; i < node->child_count; i++) {
        if (has_side_effects(node->children[i])) return true;
    }
    return false;
}

// Replaces a binary operation node in-place with one of its children (0 for left, 1 for right)
static void replace_node_with_child(Node* parent, int keep_child_index) {
    if (!parent || parent->child_count < 2) return;
    int discard_child_index = 1 - keep_child_index;
    Node* keep = parent->children[keep_child_index];
    Node* discard = parent->children[discard_child_index];

    // Free the discarded child subtree
    freeAST(discard);

    // Free parent's own value and children array
    if (parent->value) free(parent->value);
    free(parent->children);

    // Adopt keep child's properties into parent
    parent->type = keep->type;
    parent->value = keep->value;
    parent->symbol = keep->symbol;
    parent->eval_type = keep->eval_type;
    parent->children = keep->children;
    parent->child_count = keep->child_count;

    // Free only the wrapper struct of keep
    free(keep);
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

// Retrieves the variable name from an ID_NODE, preferring the TAC name if available
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
static void optimize_statement_list(Node* stmt_list, ConstEnv* env, bool* changed);
static bool stmt_guarantees_exit(Node* stmt);

// Recursively walks and folds an expression tree
// For now: constant folding of arithmetic and boolean expressions, and variable replacement with known constants
static void fold_expr(Node* node, ConstEnv* env, bool* changed) {
    if (!node) return;

    // constant propagation: replace ID_NODE with known constant value if available
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

    // if the node is a call node, optimize its parameters (minimize their expressions) and invalidate globals
    // in case of side effects.
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

        // Minor optimization: if the operation is unary, fold its single child first
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

        // if !, minimize the child first, then check if it's a boolean constant
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
            bool left_is_const = get_const_val(node->children[0], &left_val) && left_val.type == TYPE_BOOLEAN;

            // 1. false && ... -> false
            if (left_is_const && !left_val.val.bval) {
                replace_with_bool_literal(node, false);
                *changed = true;
                return;
            }

            // 2. true && expr -> expr
            if (left_is_const && left_val.val.bval) {
                fold_expr(node->children[1], env, changed);
                replace_node_with_child(node, 1);
                *changed = true;
                fold_expr(node, env, changed);
                return;
            }

            fold_expr(node->children[1], env, changed);
            ConstVal right_val;
            bool right_is_const = get_const_val(node->children[1], &right_val) && right_val.type == TYPE_BOOLEAN;

            // 3. Both are constants
            if (left_is_const && right_is_const) {
                replace_with_bool_literal(node, left_val.val.bval && right_val.val.bval);
                *changed = true;
                return;
            }

            // 4. expr && true -> expr
            if (right_is_const && right_val.val.bval) {
                replace_node_with_child(node, 0);
                *changed = true;
                fold_expr(node, env, changed);
                return;
            }

            // 5. expr && false -> false (only if expr has no side effects)
            if (right_is_const && !right_val.val.bval && !has_side_effects(node->children[0])) {
                replace_with_bool_literal(node, false);
                *changed = true;
                return;
            }

            return;
        }

        // Short-circuit logical OR (||)
        if (strcmp(op, "||") == 0) {
            fold_expr(node->children[0], env, changed);
            ConstVal left_val;
            bool left_is_const = get_const_val(node->children[0], &left_val) && left_val.type == TYPE_BOOLEAN;

            // 1. true || ... -> true
            if (left_is_const && left_val.val.bval) {
                replace_with_bool_literal(node, true);
                *changed = true;
                return;
            }

            // 2. false || expr -> expr
            if (left_is_const && !left_val.val.bval) {
                fold_expr(node->children[1], env, changed);
                replace_node_with_child(node, 1);
                *changed = true;
                fold_expr(node, env, changed);
                return;
            }

            fold_expr(node->children[1], env, changed);
            ConstVal right_val;
            bool right_is_const = get_const_val(node->children[1], &right_val) && right_val.type == TYPE_BOOLEAN;

            // 3. Both are constants
            if (left_is_const && right_is_const) {
                replace_with_bool_literal(node, left_val.val.bval || right_val.val.bval);
                *changed = true;
                return;
            }

            // 4. expr || false -> expr
            if (right_is_const && !right_val.val.bval) {
                replace_node_with_child(node, 0);
                *changed = true;
                fold_expr(node, env, changed);
                return;
            }

            // 5. expr || true -> true (only if expr has no side effects)
            if (right_is_const && right_val.val.bval && !has_side_effects(node->children[0])) {
                replace_with_bool_literal(node, true);
                *changed = true;
                return;
            }

            return;
        }

        // optimization for arithmetic and relational operators: fold both children first
        fold_expr(node->children[0], env, changed);
        fold_expr(node->children[1], env, changed);

        ConstVal l, r;
        if (!get_const_val(node->children[0], &l) || !get_const_val(node->children[1], &r)) {
            return;
        }

        // Arithmetic operators
        if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0 ||
            strcmp(op, "*") == 0 || strcmp(op, "/") == 0 || strcmp(op, "%") == 0) {

                // If both operands are integers, perform integer arithmetic
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
                // If either operand is a float, perform floating-point arithmetic
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
// its objective is to optimize the statement and update the constant environment accordingly
static void optimize_stmt(Node* node, ConstEnv* env, bool* changed) {
    if (!node) return;

    switch (node->type) {
        // Handle assignment statements: fold the expression, and if it's a constant, update the environment
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

        // Handle function calls: fold arguments and invalidate globals
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

        // if a return has an expression, fold it
        // no need to update the enviroment
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

            // this folds the condition expression, for ex 5>3 turns into true
            fold_expr(cond_node, env, changed);

            ConstVal cond_val;
            // if condition is a compile-time constant boolean, we can optimize the branches
            if (get_const_val(cond_node, &cond_val) && cond_val.type == TYPE_BOOLEAN) {
                // condition is true
                if (cond_val.val.bval) { 
                    
                    optimize_block(then_node, env, changed);
                    if (has_else) {
                        // optimize the else branch in a cloned environment, since it won't be executed
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

                // Merge the environments from the then and else branches
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
            optimize_statement_list(node, env, changed);
            break;
        }

        default:
            break;
    }
}

// Checks if a statement guarantees termination/exit (return or if-else where all branches return)
static bool stmt_guarantees_exit(Node* stmt) {
    if (!stmt) return false;

    if (stmt->type == RETURN_NODE) {
        return true;
    }

    if (stmt->type == BLOCK_NODE) {
        if (stmt->child_count > 1 && stmt->children[1]->type == STATEMENT_LIST_NODE) {
            return stmt_guarantees_exit(stmt->children[1]);
        }
        return false;
    }

    if (stmt->type == STATEMENT_LIST_NODE) {
        for (int i = 0; i < stmt->child_count; i++) {
            if (stmt_guarantees_exit(stmt->children[i])) {
                return true;
            }
        }
        return false;
    }

    if (stmt->type == IF_NODE) {
        // Both branches must exist and exit
        if (stmt->child_count > 2) {
            bool if_exits = stmt_guarantees_exit(stmt->children[1]);
            bool else_exits = stmt_guarantees_exit(stmt->children[2]);
            return if_exits && else_exits;
        }
        return false;
    }

    return false;
}

// Optimizes a statement list with dead code elimination (unreachable code after return, dead branches, dead loops)
static void optimize_statement_list(Node* stmt_list, ConstEnv* env, bool* changed) {
    if (!stmt_list || stmt_list->type != STATEMENT_LIST_NODE) return;

    for (int i = 0; i < stmt_list->child_count; i++) {
        Node* stmt = stmt_list->children[i];

        // 1. Optimize the current statement (expressions, inner blocks, assignments, etc.)
        optimize_stmt(stmt, env, changed);

        // 2. Dead loop elimination: while (false)
        if (stmt->type == WHILE_NODE) {
            ConstVal cond_val;
            if (get_const_val(stmt->children[0], &cond_val) &&
                cond_val.type == TYPE_BOOLEAN && !cond_val.val.bval) {
                // Loop condition is false: remove the entire while statement
                freeAST(stmt);
                for (int k = i; k < stmt_list->child_count - 1; k++) {
                    stmt_list->children[k] = stmt_list->children[k + 1];
                }
                stmt_list->child_count--;
                i--;
                *changed = true;
                continue;
            }
        }

        // 3. Dead conditional elimination: if (true) / if (false)
        if (stmt->type == IF_NODE) {
            ConstVal cond_val;
            if (get_const_val(stmt->children[0], &cond_val) && cond_val.type == TYPE_BOOLEAN) {
                bool has_else = (stmt->child_count > 2);
                if (cond_val.val.bval) {
                    // if (true): keep then branch, prune else branch if it exists
                    Node* then_block = stmt->children[1];
                    freeAST(stmt->children[0]);
                    if (has_else) freeAST(stmt->children[2]);
                    if (stmt->value) free(stmt->value);
                    free(stmt->children);
                    free(stmt);

                    stmt_list->children[i] = then_block;
                    *changed = true;
                    // Optimize the newly adopted then_block with current environment
                    optimize_stmt(stmt_list->children[i], env, changed);
                } else {
                    // if (false):
                    if (has_else) {
                        // Keep else branch, prune then branch
                        Node* else_block = stmt->children[2];
                        freeAST(stmt->children[0]);
                        freeAST(stmt->children[1]);
                        if (stmt->value) free(stmt->value);
                        free(stmt->children);
                        free(stmt);

                        stmt_list->children[i] = else_block;
                        *changed = true;
                        // Optimize the newly adopted else_block with current environment
                        optimize_stmt(stmt_list->children[i], env, changed);
                    } else {
                        // No else branch: remove entire if statement
                        freeAST(stmt);
                        for (int k = i; k < stmt_list->child_count - 1; k++) {
                            stmt_list->children[k] = stmt_list->children[k + 1];
                        }
                        stmt_list->child_count--;
                        i--;
                        *changed = true;
                        continue;
                    }
                }
            }
        }

        // 4. Dead code elimination after an exit statement (return or guaranteed-exit if-else)
        if (stmt_guarantees_exit(stmt_list->children[i])) {
            if (i + 1 < stmt_list->child_count) {
                for (int k = i + 1; k < stmt_list->child_count; k++) {
                    freeAST(stmt_list->children[k]);
                }
                stmt_list->child_count = i + 1;
                *changed = true;
            }
            break; // No further statements are reachable in this block
        }
    }
}

// Optimizes a block or list of statements
static void optimize_block(Node* node, ConstEnv* env, bool* changed) {
    if (!node) return;
    if (node->type == BLOCK_NODE) {
        if (node->child_count > 1 && node->children[1]->type == STATEMENT_LIST_NODE) {
            optimize_statement_list(node->children[1], env, changed);
        } else {
            for (int i = 0; i < node->child_count; i++) {
                optimize_stmt(node->children[i], env, changed);
            }
        }
    } else if (node->type == STATEMENT_LIST_NODE) {
        optimize_statement_list(node, env, changed);
    } else {
        optimize_stmt(node, env, changed);
    }
}

// Optimizes a method declaration (uses a fixed-point iteration to handle loops and conditionals)
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
                // Optimize each method declaration
                optimize_method(decls->children[i]);
            }
        }
    }

    return root;
}
