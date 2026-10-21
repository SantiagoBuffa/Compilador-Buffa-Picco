#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "estructuras.h"

// It begins the semantic analysis by traversing the AST.
// Returns the number of semantic errors found (0 if valid).
int analyze_semantics(Node* root, bool require_main);

#endif
