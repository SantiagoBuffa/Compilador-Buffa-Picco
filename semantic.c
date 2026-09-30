#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"
#include "symtab.h"

static DataType str_to_dtype(const char* str) {
    if(!str) return TYPE_UNKNOWN;
    if(strcmp(str, "int") == 0) return TYPE_INT;
    if(strcmp(str, "float") == 0) return TYPE_FLOAT;
    if(strcmp(str, "boolean") == 0) return TYPE_BOOLEAN;
    if(strcmp(str, "void") == 0) return TYPE_VOID;
    return TYPE_UNKNOWN;
}

static void traverse(Node* node) {
    if (!node) return;

    bool is_scope_creator = false;

    // Function must be inserted in the current (global) scope before creating the method's inner scope
    if (node->type == METHOD_NODE) {
        if (node->child_count > 0 && node->children[0]->type == ID_NODE) {
            char* func_name = node->children[0]->value;
            DataType ret_type = str_to_dtype(node->value);
            
            Symbol* sym = symtab_insert(func_name, ret_type, KIND_FUNC);
            if (!sym) {
                fprintf(stderr, "Semantic Error: Function '%s' already declared.\n", func_name);
            }
            node->children[0]->symbol = sym;
        }
    }

    // If it is a method or a block, we create a new scope
    if (node->type == METHOD_NODE || node->type == BLOCK_NODE) {
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
        traverse(node->children[i]);
    }

    // Post-order processing
    if (is_scope_creator) {
        printf("--- Closing Scope --- Current state of the table:\n");
        symtab_print();
        symtab_exit_scope();
    }
}

void analyze_semantics(Node* root) {
    traverse(root);
}
