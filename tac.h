#ifndef TAC_H
#define TAC_H

#include <stdio.h>
#include <stdbool.h>

// Operation kinds in Three-Address Code (TAC)
typedef enum {
    TAC_ADD,         // res = arg1 + arg2
    TAC_SUB,         // res = arg1 - arg2
    TAC_MUL,         // res = arg1 * arg2
    TAC_DIV,         // res = arg1 / arg2
    TAC_MOD,         // res = arg1 % arg2
    TAC_NEG,         // res = - arg1
    TAC_NOT,         // res = ! arg1
    TAC_COPY,        // res = arg1
    TAC_LABEL,       // res:
    TAC_GOTO,        // goto res
    TAC_IF_TRUE,     // if arg1 goto res
    TAC_IF_FALSE,    // if_false arg1 goto res
    TAC_EQ,          // res = arg1 == arg2
    TAC_LT,          // res = arg1 < arg2
    TAC_GT,          // res = arg1 > arg2
    TAC_PARAM,       // param arg1
    TAC_CALL,        // res = call arg1, arg2
    TAC_CALL_VOID,   // call arg1, arg2
    TAC_RETURN,      // return arg1 (or return if arg1 is NULL)
    TAC_FUNC_BEGIN,  // func begin res
    TAC_FUNC_END,    // func end res
    TAC_GLOBAL_VAR   // global res
} TacType;

// Individual TAC instruction node (doubly linked list)
typedef struct TacInstr {
    TacType type;
    char *res;
    char *arg1;
    char *arg2;
    struct TacInstr *prev;
    struct TacInstr *next;
} TacInstr;

// List of TAC instructions
typedef struct TacList {
    TacInstr *head;
    TacInstr *tail;
    int count;
} TacList;

// Constructor and list manipulation
TacList* tac_list_create(void);
TacInstr* tac_create_instr(TacType type, const char *res, const char *arg1, const char *arg2);
void tac_append(TacList *list, TacInstr *instr);
void tac_concat(TacList *dest, TacList *src);
void tac_free(TacList *list);

// Output and formatting
void tac_print(FILE *out, TacList *list);

// Temporary variable and label generators
char* new_temp(void);
char* new_label(void);
void tac_reset_counters(void);

#endif
