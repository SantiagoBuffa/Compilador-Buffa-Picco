#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "const_env.h"

// Creates a new constant environment
ConstEnv* env_create(void) {
    ConstEnv* env = (ConstEnv*)malloc(sizeof(ConstEnv));
    if (!env) {
        fprintf(stderr, "Error: couldn't allocate memory for ConstEnv\n");
        exit(EXIT_FAILURE);
    }
    env->head = NULL;
    return env;
}

// Frees the constant environment and all its entries
void env_free(ConstEnv* env) {
    if (!env) return;
    EnvEntry* curr = env->head;
    while (curr) {
        EnvEntry* next = curr->next;
        free(curr->name);
        free(curr);
        curr = next;
    }
    free(env);
}

// Clones the constant environment, creating a deep copy
ConstEnv* env_clone(ConstEnv* src) {
    ConstEnv* copy = env_create();
    if (!src) return copy;

    EnvEntry* curr = src->head;
    EnvEntry** tail_ptr = &copy->head;
    while (curr) {
        EnvEntry* new_entry = (EnvEntry*)malloc(sizeof(EnvEntry));
        new_entry->name = strdup(curr->name);
        new_entry->sym = curr->sym;
        new_entry->cval = curr->cval;
        new_entry->next = NULL;
        *tail_ptr = new_entry;
        tail_ptr = &new_entry->next;
        curr = curr->next;
    }
    return copy;
}

// Retrieves the constant value of a variable from the environment, or returns an unknown value if not found
ConstVal env_get(ConstEnv* env, const char* name) {
    ConstVal def;
    def.is_const = false;
    def.type = TYPE_UNKNOWN;
    if (!env || !name) return def;

    EnvEntry* curr = env->head;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            return curr->cval;
        }
        curr = curr->next;
    }
    return def;
}

// Sets a variable in the environment to a known constant value
void env_set_const(ConstEnv* env, Symbol* sym, const char* name, ConstVal cval) {
    if (!env || !name) return;

    EnvEntry* curr = env->head;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            curr->cval = cval;
            if (sym) curr->sym = sym;
            return;
        }
        curr = curr->next;
    }

    // Insert new entry at the head
    EnvEntry* new_entry = (EnvEntry*)malloc(sizeof(EnvEntry));
    new_entry->name = strdup(name);
    new_entry->sym = sym;
    new_entry->cval = cval;
    new_entry->next = env->head;
    env->head = new_entry;
}

// Sets a variable in the environment to an unknown value (not a constant)
void env_set_unknown(ConstEnv* env, const char* name) {
    if (!env || !name) return;

    EnvEntry* curr = env->head;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            curr->cval.is_const = false;
            return;
        }
        curr = curr->next;
    }

    EnvEntry* new_entry = (EnvEntry*)malloc(sizeof(EnvEntry));
    new_entry->name = strdup(name);
    new_entry->sym = NULL;
    new_entry->cval.is_const = false;
    new_entry->cval.type = TYPE_UNKNOWN;
    new_entry->next = env->head;
    env->head = new_entry;
}

// Invalidates all global variables in the environment, marking them as unknown
void env_invalidate_globals(ConstEnv* env) {
    if (!env) return;
    EnvEntry* curr = env->head;
    while (curr) {
        if (curr->sym && curr->sym->is_global) {
            curr->cval.is_const = false;
        }
        curr = curr->next;
    }
}

// Compares two constant values for equality, considering their types and values
bool const_val_equal(ConstVal a, ConstVal b) {
    if (!a.is_const || !b.is_const) return false;
    if (a.type != b.type) return false;
    switch (a.type) {
        case TYPE_INT:     return a.val.ival == b.val.ival;
        case TYPE_FLOAT:   return a.val.fval == b.val.fval;
        case TYPE_BOOLEAN: return a.val.bval == b.val.bval;
        default:           return false;
    }
}

// Merges env_a and env_b into dest. If a variable has identical constant value in both, keep it; else unknown.
void env_merge(ConstEnv* dest, ConstEnv* env_a, ConstEnv* env_b) {
    if (!env_a || !env_b || !dest) return;

    EnvEntry* curr = env_a->head;
    while (curr) {
        ConstVal b_val = env_get(env_b, curr->name);
        if (const_val_equal(curr->cval, b_val)) {
            env_set_const(dest, curr->sym, curr->name, curr->cval);
        } else {
            env_set_unknown(dest, curr->name);
        }
        curr = curr->next;
    }

    // Also check variables present in env_b but not in env_a
    EnvEntry* curr_b = env_b->head;
    while (curr_b) {
        ConstVal a_val = env_get(env_a, curr_b->name);
        if (!a_val.is_const) {
            env_set_unknown(dest, curr_b->name);
        }
        curr_b = curr_b->next;
    }
}

// Replaces the contents of dest with src
void env_copy_into(ConstEnv* dest, ConstEnv* src) {
    if (!dest || !src) return;
    // Clear dest entries
    EnvEntry* curr = dest->head;
    while (curr) {
        EnvEntry* next = curr->next;
        free(curr->name);
        free(curr);
        curr = next;
    }
    dest->head = NULL;

    EnvEntry* s = src->head;
    EnvEntry** tail_ptr = &dest->head;
    while (s) {
        EnvEntry* e = (EnvEntry*)malloc(sizeof(EnvEntry));
        e->name = strdup(s->name);
        e->sym = s->sym;
        e->cval = s->cval;
        e->next = NULL;
        *tail_ptr = e;
        tail_ptr = &e->next;
        s = s->next;
    }
}
