#ifndef SYMTAB_H
#define SYMTAB_H

#include <stdbool.h>

// Data types of the C-TDS language
typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_BOOLEAN,
    TYPE_VOID,
    TYPE_UNKNOWN
} DataType;

// Symbol kinds
typedef enum {
    KIND_VAR,
    KIND_PARAM,
    KIND_FUNC
} SymbolKind;

// Linked list for function parameters
typedef struct ParamList {
    DataType type;
    char* name;
    struct ParamList* next;
} ParamList;

// Main Symbol structure
typedef struct Symbol {
    char* name;
    DataType type;
    SymbolKind kind;
    
    // If it is a function, stores its parameters
    ParamList* params;
    
    // Pointer to the next symbol in the same scope
    struct Symbol* next;
} Symbol;

// Symbol Table functions

/* Initializes the symbol table stack (creates the global scope) */
void symtab_init();

/* Creates a new scope level and pushes it onto the stack */
void symtab_enter_scope();

/* Pops the current scope level from the stack (hides it) */
void symtab_exit_scope();

/*
 * Inserts a symbol into the current scope (top of the stack).
 * Returns a pointer to the new Symbol.
 * Returns NULL if a symbol with that name already exists in THE SAME scope.
 */
Symbol* symtab_insert(char* name, DataType type, SymbolKind kind);

/*
 * Searches for a symbol traversing the stack from current scope to global.
 * Returns a pointer to the Symbol if found (resolving shadowing).
 * Returns NULL if not found.
 */
Symbol* symtab_lookup(char* name);

// Debugging utilities
void symtab_print();

#endif
