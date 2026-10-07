#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "semantic.h"
#include "symtab.h"

static Symbol* current_method = NULL;

static DataType str_to_dtype(const char* str) {
    if(!str) return TYPE_UNKNOWN;
    if(strcmp(str, "int") == 0) return TYPE_INT;
    if(strcmp(str, "float") == 0) return TYPE_FLOAT;
    if(strcmp(str, "boolean") == 0) return TYPE_BOOLEAN;
    if(strcmp(str, "void") == 0) return TYPE_VOID;
    return TYPE_UNKNOWN;
}

static const char* dtype_to_str(DataType type) {
    switch (type) {
        case TYPE_INT: return "int";
        case TYPE_FLOAT: return "float";
        case TYPE_BOOLEAN: return "boolean";
        case TYPE_VOID: return "void";
        default: return "unknown";
    }
}

static void try_truncate_literal(Node* literal_node, DataType target_type) {
    if (literal_node->type == LITERAL_NODE && target_type == TYPE_INT) {
        if (strchr(literal_node->value, '.') != NULL) {
            float fval = atof(literal_node->value);
            int ival = (int)fval;
            char buf[64];
            snprintf(buf, sizeof(buf), "%d", ival);
            free(literal_node->value);
            literal_node->value = strdup(buf);
            literal_node->eval_type = TYPE_INT; 
            fprintf(stderr, "Warning: Implicit truncation from float to int in literal. Truncated to %d.\n", ival);
        }
    } else if (literal_node->type == LITERAL_NODE && target_type == TYPE_FLOAT) {
        if (strchr(literal_node->value, '.') == NULL && strcmp(literal_node->value, "true") != 0 && strcmp(literal_node->value, "false") != 0) {
            char buf[64];
            snprintf(buf, sizeof(buf), "%s.0", literal_node->value);
            free(literal_node->value);
            literal_node->value = strdup(buf);
            literal_node->eval_type = TYPE_FLOAT;
            fprintf(stderr, "Warning: Implicit coercion from int to float in literal.\n");
        }
    }
}

static bool check_all_paths_return(Node* node) {
    if (!node) return false;
    
    if (node->type == RETURN_NODE) {
        return true;
    }
    
    if (node->type == IF_NODE) {
        // Si tiene bloque 'else' (hijo 2 existe)
        if (node->child_count > 2) {
            bool if_returns = check_all_paths_return(node->children[1]);
            bool else_returns = check_all_paths_return(node->children[2]);
            return if_returns && else_returns;
        }
        return false;
    }
    
    if (node->type == BLOCK_NODE || node->type == STATEMENT_LIST_NODE) {
        for (int i = 0; i < node->child_count; i++) {
            if (check_all_paths_return(node->children[i])) {
                return true;
            }
        }
    }
    
    return false;
}

static void traverse(Node* node, bool skip_scope_creation) {
    if (!node) return;

    bool is_scope_creator = false;

    Symbol* saved_method = current_method;

    // Function must be inserted in the current (global) scope before creating the method's inner scope
    if (node->type == METHOD_NODE) {
        if (node->child_count > 0 && node->children[0]->type == ID_NODE) {
            char* func_name = node->children[0]->value;
            DataType ret_type = str_to_dtype(node->value);
            
            Symbol* sym = symtab_insert(func_name, ret_type, KIND_FUNC);
            if (!sym) {
                fprintf(stderr, "Semantic Error: Function '%s' already declared.\n", func_name);
            } else {
                if (node->child_count > 1 && node->children[1]->type == PARAM_LIST_NODE) {
                    Node* param_list = node->children[1];
                    ParamList* head = NULL;
                    ParamList* tail = NULL;
                    for (int i = 0; i < param_list->child_count; i++) {
                        Node* param = param_list->children[i];
                        if (param->type == PARAMETER_NODE) {
                            DataType ptype = str_to_dtype(param->value);
                            char* pname = param->children[0]->value;
                            ParamList* p = malloc(sizeof(ParamList));
                            p->type = ptype;
                            p->name = strdup(pname);
                            p->next = NULL;
                            if (!head) {
                                head = p;
                                tail = p;
                            } else {
                                tail->next = p;
                                tail = p;
                            }
                        }
                    }
                    sym->params = head;
                }
            }
            node->children[0]->symbol = sym;
            current_method = sym;
        }
    }

    // If it is a method or a block, we create a new scope
    if (!skip_scope_creation && (node->type == METHOD_NODE || node->type == BLOCK_NODE)) {
        symtab_enter_scope();
        is_scope_creator = true;
    }

    // Pre-order processing
    if (node->type == METHOD_NODE) {
        // Function declaration was handled above
    } 
    else if (node->type == VAR_DECL_NODE) {
        DataType var_type = str_to_dtype(node->value);
        // All children of a VAR_DECL_NODE are ID_NODEs (the declared variables)
        for (int i = 0; i < node->child_count; i++) {
            if (node->children[i]->type == ID_NODE) {
                char* var_name = node->children[i]->value;
                Symbol* sym = symtab_insert(var_name, var_type, KIND_VAR);
                if (!sym) {
                    fprintf(stderr, "Semantic Error: Variable '%s' already declared in this scope.\n", var_name);
                }
                node->children[i]->symbol = sym;
            }
        }
    }
    else if (node->type == PARAMETER_NODE && strcmp(node->value, "list") != 0 && strcmp(node->value, "list_empty") != 0) {
        // individual parameter
        DataType param_type = str_to_dtype(node->value);
        if (node->child_count > 0 && node->children[0]->type == ID_NODE) {
            char* param_name = node->children[0]->value;
            Symbol* sym = symtab_insert(param_name, param_type, KIND_PARAM);
            if (!sym) {
                fprintf(stderr, "Semantic Error: Parameter '%s' already declared.\n", param_name);
            }
            node->children[0]->symbol = sym;
        }
    }
    else if (node->type == ID_NODE) {
        if (node->symbol == NULL) {
            Symbol* sym = symtab_lookup(node->value);
            if (!sym) {
                fprintf(stderr, "Semantic Error: Identifier '%s' not declared.\n", node->value);
            } else {
                node->symbol = sym;
            }
        }
    }
    else if (node->type == CALL_NODE) {
        if (node->symbol == NULL) {
            Symbol* sym = symtab_lookup(node->value);
            if (!sym) {
                fprintf(stderr, "Semantic Error: Function '%s' not declared.\n", node->value);
            } else if (sym->kind != KIND_FUNC) {
                fprintf(stderr, "Semantic Error: '%s' is not a function.\n", node->value);
            } else {
                node->symbol = sym;
            }
        }
    }

    // We go through the children
    for (int i = 0; i < node->child_count; i++) {
        if (node->type == METHOD_NODE && node->children[i]->type == BLOCK_NODE) {
            traverse(node->children[i], true);
        } else {
            traverse(node->children[i], false);
        }
    }

    // Post-order processing
    if (node->type == LITERAL_NODE) {
        if (strcmp(node->value, "true") == 0 || strcmp(node->value, "false") == 0) {
            node->eval_type = TYPE_BOOLEAN;
        } else if (strchr(node->value, '.') != NULL) {
            node->eval_type = TYPE_FLOAT;
        } else {
            node->eval_type = TYPE_INT;
        }
    }
    else if (node->type == ID_NODE) {
        if (node->symbol) {
            node->eval_type = node->symbol->type;
        }
    }
    else if (node->type == CALL_NODE) {
        if (node->symbol) {
            node->eval_type = node->symbol->type;

            // Check arguments
            Node* arg_list = NULL;
            if (node->child_count > 0 && node->children[0]->type == ARG_LIST_NODE) {
                arg_list = node->children[0];
            }

            int arg_count = arg_list ? ((arg_list->value && strcmp(arg_list->value, "empty") == 0) ? 0 : arg_list->child_count) : 0;
            ParamList* p = node->symbol->params;
            int param_count = 0;
            while(p) { param_count++; p = p->next; }

            if (arg_count != param_count) {
                fprintf(stderr, "Semantic Error: Function '%s' expects %d arguments, but %d were provided.\n", node->value, param_count, arg_count);
            } else {
                p = node->symbol->params;
                for (int i = 0; i < arg_count; i++) {
                    Node* arg_expr = arg_list->children[i];
                    if (arg_expr->eval_type != p->type) {
                        if ((arg_expr->eval_type == TYPE_INT && p->type == TYPE_FLOAT) || 
                            (arg_expr->eval_type == TYPE_FLOAT && p->type == TYPE_INT)) {
                            try_truncate_literal(arg_expr, p->type);
                            if (arg_expr->type != LITERAL_NODE) {
                                fprintf(stderr, "Warning: Implicit coercion between %s and %s in argument %d of '%s'.\n", dtype_to_str(arg_expr->eval_type), dtype_to_str(p->type), i+1, node->value);
                            }
                        } else if (arg_expr->eval_type != TYPE_UNKNOWN && p->type != TYPE_UNKNOWN) {
                            fprintf(stderr, "Semantic Error: Argument %d of '%s' expects %s, but got %s.\n", i+1, node->value, dtype_to_str(p->type), dtype_to_str(arg_expr->eval_type));
                        }
                    }
                    p = p->next;
                }
            }
        }
    }
    else if (node->type == OPERATION_NODE) {
        char* op = node->value;
        if (strcmp(op, "!") == 0) {
            Node* expr = node->children[0];
            if (expr->eval_type != TYPE_BOOLEAN && expr->eval_type != TYPE_UNKNOWN) {
                fprintf(stderr, "Semantic Error: Operand of '!' must be boolean, got %s.\n", dtype_to_str(expr->eval_type));
            }
            node->eval_type = TYPE_BOOLEAN;
        } else if (strcmp(op, "- (unary)") == 0) {
            Node* expr = node->children[0];
            if (expr->eval_type != TYPE_INT && expr->eval_type != TYPE_FLOAT && expr->eval_type != TYPE_UNKNOWN) {
                fprintf(stderr, "Semantic Error: Operand of unary '-' must be int or float, got %s.\n", dtype_to_str(expr->eval_type));
            }
            node->eval_type = expr->eval_type;
        } else {
            Node* left = node->children[0];
            Node* right = node->children[1];
            
            if (strcmp(op, "&&") == 0 || strcmp(op, "||") == 0) {
                if ((left->eval_type != TYPE_BOOLEAN && left->eval_type != TYPE_UNKNOWN) || 
                    (right->eval_type != TYPE_BOOLEAN && right->eval_type != TYPE_UNKNOWN)) {
                    fprintf(stderr, "Semantic Error: Operands of '%s' must be boolean.\n", op);
                }
                node->eval_type = TYPE_BOOLEAN;
            } else if (strcmp(op, "==") == 0) {
                if (left->eval_type != right->eval_type && left->eval_type != TYPE_UNKNOWN && right->eval_type != TYPE_UNKNOWN) {
                    fprintf(stderr, "Semantic Error: Operands of '==' must have the same type, got %s and %s.\n", dtype_to_str(left->eval_type), dtype_to_str(right->eval_type));
                }
                node->eval_type = TYPE_BOOLEAN;
            } else if (strcmp(op, "<") == 0 || strcmp(op, ">") == 0) {
                if ((left->eval_type != TYPE_INT && left->eval_type != TYPE_FLOAT && left->eval_type != TYPE_UNKNOWN) || 
                    (right->eval_type != TYPE_INT && right->eval_type != TYPE_FLOAT && right->eval_type != TYPE_UNKNOWN)) {
                    fprintf(stderr, "Semantic Error: Operands of '%s' must be int or float.\n", op);
                }
                node->eval_type = TYPE_BOOLEAN;
            } else if (strcmp(op, "%") == 0) {
                if ((left->eval_type != TYPE_INT && left->eval_type != TYPE_UNKNOWN) || 
                    (right->eval_type != TYPE_INT && right->eval_type != TYPE_UNKNOWN)) {
                    fprintf(stderr, "Semantic Error: Operands of '%%' must be int.\n");
                }
                node->eval_type = TYPE_INT;
            } else if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0 || strcmp(op, "*") == 0 || strcmp(op, "/") == 0) {
                if ((left->eval_type != TYPE_INT && left->eval_type != TYPE_FLOAT && left->eval_type != TYPE_UNKNOWN) || 
                    (right->eval_type != TYPE_INT && right->eval_type != TYPE_FLOAT && right->eval_type != TYPE_UNKNOWN)) {
                    fprintf(stderr, "Semantic Error: Operands of '%s' must be int or float.\n", op);
                }
                if (left->eval_type == TYPE_FLOAT || right->eval_type == TYPE_FLOAT) {
                    node->eval_type = TYPE_FLOAT;
                } else {
                    node->eval_type = TYPE_INT;
                }
            }
        }
    }
    else if (node->type == ASSIGNMENT_NODE) {
        Node* id_node = node->children[0];
        Node* expr_node = node->children[1];
        
        if (id_node->symbol) {
            if (id_node->symbol->kind == KIND_FUNC) {
                fprintf(stderr, "Semantic Error: Cannot assign to function '%s'.\n", id_node->value);
            } else {
                DataType ltype = id_node->symbol->type;
                DataType rtype = expr_node->eval_type;
                if (ltype != rtype && ltype != TYPE_UNKNOWN && rtype != TYPE_UNKNOWN) {
                    if ((ltype == TYPE_INT && rtype == TYPE_FLOAT) || (ltype == TYPE_FLOAT && rtype == TYPE_INT)) {
                        try_truncate_literal(expr_node, ltype);
                        if (expr_node->type != LITERAL_NODE) {
                            fprintf(stderr, "Warning: Implicit coercion in assignment to '%s'.\n", id_node->value);
                        }
                    } else {
                        fprintf(stderr, "Semantic Error: Type mismatch in assignment to '%s' (expected %s, got %s).\n", 
                            id_node->value, dtype_to_str(ltype), dtype_to_str(rtype));
                    }
                }
            }
        }
        
        if (expr_node->type == CALL_NODE && expr_node->eval_type == TYPE_VOID) {
            fprintf(stderr, "Semantic Error: Method '%s' used as an expression must return a value.\n", expr_node->value);
        }
    }
    else if (node->type == RETURN_NODE) {
        Node* expr_node = (node->child_count > 0) ? node->children[0] : NULL;
        DataType expected_ret = current_method ? current_method->type : TYPE_UNKNOWN;
        
        if (expected_ret == TYPE_VOID) {
            if (expr_node != NULL) {
                fprintf(stderr, "Semantic Error: Void method '%s' cannot return a value.\n", current_method ? current_method->name : "unknown");
            }
        } else {
            if (expr_node == NULL) {
                fprintf(stderr, "Semantic Error: Non-void method '%s' must return a value of type %s.\n", 
                        current_method ? current_method->name : "unknown", dtype_to_str(expected_ret));
            } else {
                if (expr_node->eval_type != expected_ret && expr_node->eval_type != TYPE_UNKNOWN && expected_ret != TYPE_UNKNOWN) {
                    if ((expected_ret == TYPE_INT && expr_node->eval_type == TYPE_FLOAT) || 
                        (expected_ret == TYPE_FLOAT && expr_node->eval_type == TYPE_INT)) {
                        try_truncate_literal(expr_node, expected_ret);
                        if (expr_node->type != LITERAL_NODE) {
                            fprintf(stderr, "Warning: Implicit coercion in return statement.\n");
                        }
                    } else {
                        fprintf(stderr, "Semantic Error: Type mismatch in return statement (expected %s, got %s).\n", 
                            dtype_to_str(expected_ret), dtype_to_str(expr_node->eval_type));
                    }
                }
            }
        }
    }
    else if (node->type == IF_NODE || node->type == WHILE_NODE) {
        Node* expr_node = node->children[0];
        if (expr_node->eval_type != TYPE_BOOLEAN && expr_node->eval_type != TYPE_UNKNOWN) {
            fprintf(stderr, "Semantic Error: Condition of '%s' must be boolean, got %s.\n", 
                node->type == IF_NODE ? "if" : "while", dtype_to_str(expr_node->eval_type));
        }
    }
    else if (node->type == METHOD_NODE) {
        if (current_method && current_method->type != TYPE_VOID) {
            // The block node is the last child of METHOD_NODE
            Node* block_node = node->children[node->child_count - 1];
            if (!check_all_paths_return(block_node)) {
                fprintf(stderr, "Semantic Error: Function '%s' must return a value of type %s in all control paths.\n", 
                    current_method->name, dtype_to_str(current_method->type));
            }
        }
    }

    if (is_scope_creator) {
        printf("--- Closing Scope --- Current state of the table:\n");
        symtab_print();
        symtab_exit_scope();
    }
    
    current_method = saved_method;
}

void analyze_semantics(Node* root) {
    traverse(root, false);

    // Check for main function
    Symbol* main_sym = symtab_lookup("main");
    if (!main_sym) {
        fprintf(stderr, "Semantic Error: Method 'main' is missing.\n");
    } else if (main_sym->kind != KIND_FUNC) {
        fprintf(stderr, "Semantic Error: 'main' must be a function.\n");
    } else if (main_sym->params != NULL) {
        fprintf(stderr, "Semantic Error: Method 'main' must not have parameters.\n");
    }
}
