#ifndef CODEGEN_H
#define CODEGEN_H

#include "estructuras.h"
#include "tac.h"

// Generates Three-Address Code (TAC) from a validated Abstract Syntax Tree (AST)
TacList* codegen_generate(Node *ast_root);

#endif
