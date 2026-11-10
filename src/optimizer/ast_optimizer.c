#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "ast_optimizer.h"
#include "estructuras.h"
#include "symtab.h"
#include "const_env.h"
#include "ast_opt_utils.h"

#define MAX_OPTIMIZATION_PASSES 10

// Currently active optimization configuration
static OptConfig current_config;

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
    if (current_config.constant_folding && node->type == ID_NODE) {
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
            if (current_config.constant_folding) {
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
            }
            return;
        }

        // if !, minimize the child first, then check if it's a boolean constant
        if (strcmp(op, "!") == 0) {
            fold_expr(node->children[0], env, changed);
            if (current_config.constant_folding) {
                ConstVal arg;
                if (get_const_val(node->children[0], &arg) && arg.type == TYPE_BOOLEAN) {
                    replace_with_bool_literal(node, !arg.val.bval);
                    *changed = true;
                }
            }
            return;
        }

        // Short-circuit logical AND (&&)
        if (strcmp(op, "&&") == 0) {
            fold_expr(node->children[0], env, changed);
            ConstVal left_val;
            bool left_is_const = get_const_val(node->children[0], &left_val) && left_val.type == TYPE_BOOLEAN;

            if (current_config.short_circuit) {
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
            }

            fold_expr(node->children[1], env, changed);
            ConstVal right_val;
            bool right_is_const = get_const_val(node->children[1], &right_val) && right_val.type == TYPE_BOOLEAN;

            // 3. Both are constants
            if ((current_config.constant_folding || current_config.short_circuit) && left_is_const && right_is_const) {
                replace_with_bool_literal(node, left_val.val.bval && right_val.val.bval);
                *changed = true;
                return;
            }

            if (current_config.short_circuit) {
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
            }

            return;
        }

        // Short-circuit logical OR (||)
        if (strcmp(op, "||") == 0) {
            fold_expr(node->children[0], env, changed);
            ConstVal left_val;
            bool left_is_const = get_const_val(node->children[0], &left_val) && left_val.type == TYPE_BOOLEAN;

            if (current_config.short_circuit) {
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
            }

            fold_expr(node->children[1], env, changed);
            ConstVal right_val;
            bool right_is_const = get_const_val(node->children[1], &right_val) && right_val.type == TYPE_BOOLEAN;

            // 3. Both are constants
            if ((current_config.constant_folding || current_config.short_circuit) && left_is_const && right_is_const) {
                replace_with_bool_literal(node, left_val.val.bval || right_val.val.bval);
                *changed = true;
                return;
            }

            if (current_config.short_circuit) {
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
            }

            return;
        }

        // optimization for arithmetic and relational operators: fold both children first
        fold_expr(node->children[0], env, changed);
        fold_expr(node->children[1], env, changed);

        ConstVal l, r;
        bool has_l = get_const_val(node->children[0], &l);
        bool has_r = get_const_val(node->children[1], &r);

        // 1. If BOTH operands are constants, perform constant folding
        if (current_config.constant_folding && has_l && has_r) {
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

        // 2. Algebraic simplifications and strength reduction (when at least one child is not a constant)
        if (current_config.algebraic) {
            // Operator: Multiplication (*)
        if (strcmp(op, "*") == 0) {
            // A. Absorbing element: expr * 0 or 0 * expr -> 0 (only if expr has no side effects)
            if (has_r && ((r.type == TYPE_INT && r.val.ival == 0) || (r.type == TYPE_FLOAT && r.val.fval == 0.0))) {
                if (!has_side_effects(node->children[0])) {
                    if (r.type == TYPE_FLOAT || node->children[0]->eval_type == TYPE_FLOAT) {
                        replace_with_float_literal(node, 0.0);
                    } else {
                        replace_with_int_literal(node, 0);
                    }
                    *changed = true;
                    return;
                }
            }
            if (has_l && ((l.type == TYPE_INT && l.val.ival == 0) || (l.type == TYPE_FLOAT && l.val.fval == 0.0))) {
                if (!has_side_effects(node->children[1])) {
                    if (l.type == TYPE_FLOAT || node->children[1]->eval_type == TYPE_FLOAT) {
                        replace_with_float_literal(node, 0.0);
                    } else {
                        replace_with_int_literal(node, 0);
                    }
                    *changed = true;
                    return;
                }
            }

            // B. Neutral element: expr * 1 -> expr, 1 * expr -> expr
            if (has_r && ((r.type == TYPE_INT && r.val.ival == 1) || (r.type == TYPE_FLOAT && r.val.fval == 1.0))) {
                replace_node_with_child(node, 0);
                *changed = true;
                fold_expr(node, env, changed);
                return;
            }
            if (has_l && ((l.type == TYPE_INT && l.val.ival == 1) || (l.type == TYPE_FLOAT && l.val.fval == 1.0))) {
                replace_node_with_child(node, 1);
                *changed = true;
                fold_expr(node, env, changed);
                return;
            }

            // C. Strength reduction: x * 2 or 2 * x -> x + x (for ID_NODE or pure expressions)
            if (has_r && r.type == TYPE_INT && r.val.ival == 2) {
                Node* left = node->children[0];
                if (!has_side_effects(left) && left->type == ID_NODE) {
                    freeAST(node->children[1]);
                    node->children[1] = deep_copy_ast(left);
                    free(node->value);
                    node->value = strdup("+");
                    *changed = true;
                    return;
                }
            }
            if (has_l && l.type == TYPE_INT && l.val.ival == 2) {
                Node* right = node->children[1];
                if (!has_side_effects(right) && right->type == ID_NODE) {
                    freeAST(node->children[0]);
                    node->children[0] = deep_copy_ast(right);
                    free(node->value);
                    node->value = strdup("+");
                    *changed = true;
                    return;
                }
            }

            return;
        }

        // Operator: Division (/)
        if (strcmp(op, "/") == 0) {
            // Cannot optimize if divisor is 0 (preserve potential division by zero error)
            if (has_r && ((r.type == TYPE_INT && r.val.ival == 0) || (r.type == TYPE_FLOAT && r.val.fval == 0.0))) {
                return;
            }

            // A. Zero dividend: 0 / expr -> 0 (only if expr has no side effects)
            if (has_l && ((l.type == TYPE_INT && l.val.ival == 0) || (l.type == TYPE_FLOAT && l.val.fval == 0.0))) {
                if (!has_side_effects(node->children[1])) {
                    if (l.type == TYPE_FLOAT || node->children[1]->eval_type == TYPE_FLOAT) {
                        replace_with_float_literal(node, 0.0);
                    } else {
                        replace_with_int_literal(node, 0);
                    }
                    *changed = true;
                    return;
                }
            }

            // B. Neutral element: expr / 1 -> expr
            if (has_r && ((r.type == TYPE_INT && r.val.ival == 1) || (r.type == TYPE_FLOAT && r.val.fval == 1.0))) {
                replace_node_with_child(node, 0);
                *changed = true;
                fold_expr(node, env, changed);
                return;
            }

            // C. Pattern cancellation: (expr * C) / C -> expr or (C * expr) / C -> expr
            if (has_r && r.type == TYPE_INT && r.val.ival != 0) {
                Node* left = node->children[0];
                if (left && left->type == OPERATION_NODE && strcmp(left->value, "*") == 0 && left->child_count == 2) {
                    ConstVal c0, c1;
                    bool c0_is_const = get_const_val(left->children[0], &c0);
                    bool c1_is_const = get_const_val(left->children[1], &c1);

                    if (c0_is_const && c0.type == TYPE_INT && c0.val.ival == r.val.ival) {
                        replace_node_with_subtree(node, left->children[1]);
                        *changed = true;
                        fold_expr(node, env, changed);
                        return;
                    }
                    if (c1_is_const && c1.type == TYPE_INT && c1.val.ival == r.val.ival) {
                        replace_node_with_subtree(node, left->children[0]);
                        *changed = true;
                        fold_expr(node, env, changed);
                        return;
                    }
                }

                // D. Pattern cancellation: (x + x) / 2 -> x
                if (r.val.ival == 2 && left && left->type == OPERATION_NODE &&
                    strcmp(left->value, "+") == 0 && left->child_count == 2) {
                    if (are_ast_nodes_equal(left->children[0], left->children[1])) {
                        replace_node_with_subtree(node, left->children[0]);
                        *changed = true;
                        fold_expr(node, env, changed);
                        return;
                    }
                }
            }

            return;
        }

        // Operator: Addition (+)
        if (strcmp(op, "+") == 0) {
            // Neutral element: expr + 0 -> expr
            if (has_r && ((r.type == TYPE_INT && r.val.ival == 0) || (r.type == TYPE_FLOAT && r.val.fval == 0.0))) {
                replace_node_with_child(node, 0);
                *changed = true;
                fold_expr(node, env, changed);
                return;
            }
            // Neutral element: 0 + expr -> expr
            if (has_l && ((l.type == TYPE_INT && l.val.ival == 0) || (l.type == TYPE_FLOAT && l.val.fval == 0.0))) {
                replace_node_with_child(node, 1);
                *changed = true;
                fold_expr(node, env, changed);
                return;
            }
            return;
        }

        // Operator: Subtraction (-)
        if (strcmp(op, "-") == 0) {
            // Neutral element: expr - 0 -> expr
            if (has_r && ((r.type == TYPE_INT && r.val.ival == 0) || (r.type == TYPE_FLOAT && r.val.fval == 0.0))) {
                replace_node_with_child(node, 0);
                *changed = true;
                fold_expr(node, env, changed);
                return;
            }
            // Identity: x - x -> 0 (only if x has no side effects)
            if (are_ast_nodes_equal(node->children[0], node->children[1]) && !has_side_effects(node->children[0])) {
                if (node->children[0]->eval_type == TYPE_FLOAT) {
                    replace_with_float_literal(node, 0.0);
                } else {
                    replace_with_int_literal(node, 0);
                }
                *changed = true;
                return;
            }
            return;
        }
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
            if (current_config.constant_folding && get_const_val(expr_node, &cval)) {
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
            if (current_config.dead_code_elim && get_const_val(cond_node, &cond_val) && cond_val.type == TYPE_BOOLEAN) {
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
        if (current_config.dead_code_elim && stmt->type == WHILE_NODE) {
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
        if (current_config.dead_code_elim && stmt->type == IF_NODE) {
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
        if (current_config.dead_code_elim && stmt_guarantees_exit(stmt_list->children[i])) {
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

// Top-level entry point for AST optimization with explicit configuration
Node* optimize_ast_with_config(Node* root, OptConfig config) {
    if (!root) return NULL;

    current_config = config;

    // If no optimizations are enabled, return root directly without doing any work
    if (!current_config.constant_folding &&
        !current_config.short_circuit &&
        !current_config.dead_code_elim &&
        !current_config.algebraic) {
        return root;
    }

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

// Convenience wrapper that enables all optimizations
Node* optimize_ast(Node* root) {
    OptConfig all_opts = {
        .constant_folding = true,
        .short_circuit = true,
        .dead_code_elim = true,
        .algebraic = true
    };
    return optimize_ast_with_config(root, all_opts);
}
