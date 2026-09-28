#include <stdio.h>
#include <assert.h>
#include "symtab.h"
#include "estructuras.h"

int main() {
    symtab_init();
    symtab_insert("x", TYPE_INT, KIND_VAR);
    Symbol* s1 = symtab_lookup("x");
    assert(s1 != NULL);
    
    symtab_enter_scope();
    symtab_insert("x", TYPE_FLOAT, KIND_VAR);
    Symbol* s2 = symtab_lookup("x");
    assert(s2 != NULL && s2 != s1 && s2->type == TYPE_FLOAT);
    
    symtab_exit_scope();
    Symbol* s3 = symtab_lookup("x");
    assert(s3 == s1);
    
    printf("Symbol Table OK!\n");
    return 0;
}
