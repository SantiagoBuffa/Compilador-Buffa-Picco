#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast_opt_utils.h"

// Mutates an AST node in-place into an integer literal, freeing its old children and value
void replace_with_int_literal(Node* node, int val) {
    if (!node) return;
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

// Mutates an AST node in-place into a float literal, freeing its old children and value
void replace_with_float_literal(Node* node, double val) {
    if (!node) return;
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

// Mutates an AST node in-place into a boolean literal, freeing its old children and value
void replace_with_bool_literal(Node* node, bool val) {
    if (!node) return;
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
bool has_side_effects(Node* node) {
    if (!node) return false;
    if (node->type == CALL_NODE) return true;
    for (int i = 0; i < node->child_count; i++) {
        if (has_side_effects(node->children[i])) return true;
    }
    return false;
}

// Replaces a binary operation node in-place with one of its children (0 for left, 1 for right)
void replace_node_with_child(Node* parent, int keep_child_index) {
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
bool get_const_val(Node* node, ConstVal* out) {
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
const char* get_node_var_name(Node* id_node) {
    if (!id_node) return NULL;
    if (id_node->symbol && id_node->symbol->tac_name) {
        return id_node->symbol->tac_name;
    }
    return id_node->value;
}

// Creates a deep copy of an AST subtree
Node* deep_copy_ast(Node* root) {
    if (!root) return NULL;
    Node* copy = (Node*)malloc(sizeof(Node));
    copy->type = root->type;
    copy->value = root->value ? strdup(root->value) : NULL;
    copy->symbol = root->symbol;
    copy->eval_type = root->eval_type;
    copy->child_count = root->child_count;
    if (root->child_count > 0) {
        copy->children = (Node**)malloc(root->child_count * sizeof(Node*));
        for (int i = 0; i < root->child_count; i++) {
            copy->children[i] = deep_copy_ast(root->children[i]);
        }
    } else {
        copy->children = NULL;
    }
    return copy;
}

// Checks if two AST nodes represent the same variable or literal expression
bool are_ast_nodes_equal(Node* a, Node* b) {
    if (!a || !b) return false;
    if (a->type != b->type) return false;
    if (a->type == ID_NODE) {
        const char* name_a = get_node_var_name(a);
        const char* name_b = get_node_var_name(b);
        return (name_a && name_b && strcmp(name_a, name_b) == 0);
    }
    if (a->type == LITERAL_NODE) {
        return (a->value && b->value && strcmp(a->value, b->value) == 0);
    }
    if (a->type == OPERATION_NODE) {
        if (!a->value || !b->value || strcmp(a->value, b->value) != 0) return false;
        if (a->child_count != b->child_count) return false;
        for (int i = 0; i < a->child_count; i++) {
            if (!are_ast_nodes_equal(a->children[i], b->children[i])) return false;
        }
        return true;
    }
    return false;
}

// Replaces an AST node in-place with an arbitrary replacement subtree, freeing the old children
void replace_node_with_subtree(Node* parent, Node* replacement) {
    if (!parent || !replacement) return;
    Node* keep = deep_copy_ast(replacement);
    for (int i = 0; i < parent->child_count; i++) {
        freeAST(parent->children[i]);
    }
    if (parent->value) free(parent->value);
    free(parent->children);

    parent->type = keep->type;
    parent->value = keep->value;
    parent->symbol = keep->symbol;
    parent->eval_type = keep->eval_type;
    parent->children = keep->children;
    parent->child_count = keep->child_count;
    free(keep);
}
