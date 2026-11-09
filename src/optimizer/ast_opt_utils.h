#ifndef AST_OPT_UTILS_H
#define AST_OPT_UTILS_H

#include <stdbool.h>
#include "estructuras.h"
#include "const_env.h"

// AST node mutation helpers (in-place transformations)
void replace_with_int_literal(Node* node, int val);
void replace_with_float_literal(Node* node, double val);
void replace_with_bool_literal(Node* node, bool val);
void replace_node_with_child(Node* parent, int keep_child_index);
void replace_node_with_subtree(Node* parent, Node* replacement);

// AST inspection and copying helpers
Node* deep_copy_ast(Node* root);
bool are_ast_nodes_equal(Node* a, Node* b);
bool has_side_effects(Node* node);
bool get_const_val(Node* node, ConstVal* out);
const char* get_node_var_name(Node* id_node);

#endif // AST_OPT_UTILS_H
