#ifndef AST_OPTIMIZER_H
#define AST_OPTIMIZER_H

#include "estructuras.h"

// Traverses and optimizes the AST in-place using Constant Folding and Constant Propagation.
// Returns the root of the optimized AST.
Node* optimize_ast(Node* root);

#endif
