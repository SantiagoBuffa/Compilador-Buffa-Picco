#ifndef AST_OPTIMIZER_H
#define AST_OPTIMIZER_H

#include <stdbool.h>
#include "estructuras.h"

// Configuration flags for enabling/disabling specific optimization passes
typedef struct {
    bool constant_folding; // Constant folding & constant propagation
    bool short_circuit;    // Short-circuit logical optimizations (&&, ||)
    bool dead_code_elim;   // Dead code elimination (unreachable code, while(false), dead branches)
    bool algebraic;        // Strength reduction & algebraic simplifications (* 2, / 2, identities)
} OptConfig;

// Traverses and optimizes the AST in-place according to the specified configuration.
// Returns the root of the optimized AST.
Node* optimize_ast_with_config(Node* root, OptConfig config);

// Convenience wrapper that enables all optimizations.
Node* optimize_ast(Node* root);

#endif
