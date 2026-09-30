#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

// Scope stack node structure
typedef struct ScopeNode {
    Symbol* symbols;         // Linked list of symbols in this scope
    struct ScopeNode* prev;  // Pointer to previous scope (down the stack)
} ScopeNode;

// Pointer to top of stack (current scope)
static ScopeNode* top_scope = NULL;

void symtab_init() {
    // Clean up any existing scopes to avoid basic memory leaks
    while (top_scope != NULL) {
        symtab_exit_scope();
    }
    // Create global scope
    symtab_enter_scope();
}

void symtab_enter_scope() {
    ScopeNode* new_scope = (ScopeNode*)malloc(sizeof(ScopeNode));
    if (new_scope == NULL) {
        fprintf(stderr, "Error: couldn't allocate memory for scope\n");
        exit(EXIT_FAILURE);
    }
    new_scope->symbols = NULL;
    new_scope->prev = top_scope;
    top_scope = new_scope;
}

void symtab_exit_scope() {
    if (top_scope == NULL) return;

    ScopeNode* old_scope = top_scope;
    top_scope = top_scope->prev;
    free(old_scope);
}

Symbol* symtab_insert(char* name, DataType type, SymbolKind kind) {
    if (top_scope == NULL) {
        symtab_init();
    }

    // Check if symbol already exists in the same scope
    Symbol* current = top_scope->symbols;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return NULL; // Already exists in this scope
        }
        current = current->next;
    }

    // Does not exist in this scope, create it
    Symbol* new_symbol = (Symbol*)malloc(sizeof(Symbol));
    if (new_symbol == NULL) {
        fprintf(stderr, "Error: couldn't allocate memory for symbol\n");
        exit(EXIT_FAILURE);
    }
    
    new_symbol->name = strdup(name);
    new_symbol->type = type;
    new_symbol->kind = kind;
    new_symbol->params = NULL;
    new_symbol->next = NULL;
    
    // Insert at the end of current scope's list to preserve order
    if (top_scope->symbols == NULL) {
        top_scope->symbols = new_symbol;
    } else {
        Symbol* curr = top_scope->symbols;
        while (curr->next != NULL) curr = curr->next;
        curr->next = new_symbol;
    }
    
    return new_symbol;
}

Symbol* symtab_lookup(char* name) {
    ScopeNode* current_scope = top_scope;
    
    // Search from top of stack down to global scope
    while (current_scope != NULL) {
        Symbol* current_sym = current_scope->symbols;
        while (current_sym != NULL) {
            if (strcmp(current_sym->name, name) == 0) {
                return current_sym; // Found (handles shadowing automatically)
            }
            current_sym = current_sym->next;
        }
        current_scope = current_scope->prev;
    }
    
    return NULL; // Not found in any scope
}

// Helper function to convert type to string (for printing)
static const char* get_type_name(DataType type) {
    switch(type) {
        case TYPE_INT: return "int";
        case TYPE_FLOAT: return "float";
        case TYPE_BOOLEAN: return "boolean";
        case TYPE_VOID: return "void";
        default: return "unknown";
    }
}

// Helper function to print symbol table
void symtab_print() {
    printf("=== SYMBOL TABLE ===\n");
    
    // Count total depth
    int total_depth = 0;
    ScopeNode* curr = top_scope;
    while (curr != NULL) {
        total_depth++;
        curr = curr->prev;
    }
    
    // Create an array to hold pointers to scopes
    ScopeNode** scopes = (ScopeNode**)malloc(total_depth * sizeof(ScopeNode*));
    curr = top_scope;
    for (int i = total_depth - 1; i >= 0; i--) {
        scopes[i] = curr;
        curr = curr->prev;
    }
    
    // Print from Global (0) to Local (total_depth - 1)
    for (int i = 0; i < total_depth; i++) {
        printf("Scope Level %d:\n", i);
        Symbol* sym = scopes[i]->symbols;
        if (sym == NULL) {
            printf("  (empty)\n");
        }
        while (sym != NULL) {
            printf("  - %s (Type: %s)\n", sym->name, get_type_name(sym->type));
            sym = sym->next;
        }
    }
    
    free(scopes);
    printf("====================\n");
}
