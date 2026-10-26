#ifndef ASM_H
#define ASM_H

#include <stdio.h>
#include "tac.h"

// Generates 64-bit GNU-style x86-64 assembly in AT&T syntax from the TAC list
void asm_generate(FILE *out, TacList *tac);

#endif
