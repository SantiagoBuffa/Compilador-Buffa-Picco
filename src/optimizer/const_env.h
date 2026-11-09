#ifndef CONST_ENV_H
#define CONST_ENV_H

#include <stdbool.h>
#include "symtab.h"

// Representation of a constant value
typedef struct {
    bool is_const;
    DataType type;
    union {
        int ival;
        double fval;
        bool bval;
    } val;
} ConstVal;

// Entry in the constant environment
typedef struct EnvEntry {
    char* name;
    Symbol* sym;
    ConstVal cval;
    struct EnvEntry* next;
} EnvEntry;

// Environment tracking known constant values of variables
typedef struct ConstEnv {
    EnvEntry* head;
} ConstEnv;

// Constructor and destructor
ConstEnv* env_create(void);
void env_free(ConstEnv* env);
ConstEnv* env_clone(ConstEnv* src);

// Lookups and updates
ConstVal env_get(ConstEnv* env, const char* name);
void env_set_const(ConstEnv* env, Symbol* sym, const char* name, ConstVal cval);
void env_set_unknown(ConstEnv* env, const char* name);
void env_invalidate_globals(ConstEnv* env);

// Comparison and Lattice Meet operations
bool const_val_equal(ConstVal a, ConstVal b);
void env_merge(ConstEnv* dest, ConstEnv* env_a, ConstEnv* env_b);
void env_copy_into(ConstEnv* dest, ConstEnv* src);

#endif // CONST_ENV_H
