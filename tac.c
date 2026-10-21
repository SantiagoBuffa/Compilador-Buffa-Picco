#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tac.h"

static int temp_counter = 0;
static int label_counter = 0;

// creates empty tac list
TacList* tac_list_create(void) {
    TacList *list = (TacList*)malloc(sizeof(TacList));
    if (!list) {
        fprintf(stderr, "Error: out of memory allocating TacList\n");
        exit(EXIT_FAILURE);
    }
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
    return list;
}

// creates a Three Adress Code instruction based on type and arguments
TacInstr* tac_create_instr(TacType type, const char *res, const char *arg1, const char *arg2) {
    TacInstr *instr = (TacInstr*)malloc(sizeof(TacInstr));
    if (!instr) {
        fprintf(stderr, "Error: out of memory allocating TacInstr\n");
        exit(EXIT_FAILURE);
    }
    instr->type = type;
    instr->res = res ? strdup(res) : NULL;
    instr->arg1 = arg1 ? strdup(arg1) : NULL;
    instr->arg2 = arg2 ? strdup(arg2) : NULL;
    instr->prev = NULL;
    instr->next = NULL;
    return instr;
}

// adds a TAC instruction to the end of the list
void tac_append(TacList *list, TacInstr *instr) {
    if (!list || !instr) return;
    if (list->head == NULL) {
        list->head = instr;
        list->tail = instr;
        instr->prev = NULL;
        instr->next = NULL;
    } else {
        list->tail->next = instr;
        instr->prev = list->tail;
        instr->next = NULL;
        list->tail = instr;
    }
    list->count++;
}

// concatenates two TAC lists, appending src to dest and clearing src
void tac_concat(TacList *dest, TacList *src) {
    if (!dest || !src || src->head == NULL) return;
    if (dest->head == NULL) {
        dest->head = src->head;
        dest->tail = src->tail;
    } else {
        dest->tail->next = src->head;
        src->head->prev = dest->tail;
        dest->tail = src->tail;
    }
    dest->count += src->count;
    src->head = NULL;
    src->tail = NULL;
    src->count = 0;
}

// frees the TAC list
void tac_free(TacList *list) {
    if (!list) return;
    TacInstr *curr = list->head;
    while (curr != NULL) {
        TacInstr *next = curr->next;
        if (curr->res) free(curr->res);
        if (curr->arg1) free(curr->arg1);
        if (curr->arg2) free(curr->arg2);
        free(curr);
        curr = next;
    }
    free(list);
}

// prints the TAC list to a file
void tac_print(FILE *out, TacList *list) {
    if (!list || !out) return;
    TacInstr *instr = list->head;
    while (instr != NULL) {
        switch (instr->type) {
            case TAC_ADD:
                fprintf(out, "    %s = %s + %s\n", instr->res, instr->arg1, instr->arg2);
                break;
            case TAC_SUB:
                fprintf(out, "    %s = %s - %s\n", instr->res, instr->arg1, instr->arg2);
                break;
            case TAC_MUL:
                fprintf(out, "    %s = %s * %s\n", instr->res, instr->arg1, instr->arg2);
                break;
            case TAC_DIV:
                fprintf(out, "    %s = %s / %s\n", instr->res, instr->arg1, instr->arg2);
                break;
            case TAC_MOD:
                fprintf(out, "    %s = %s %% %s\n", instr->res, instr->arg1, instr->arg2);
                break;
            case TAC_NEG:
                fprintf(out, "    %s = - %s\n", instr->res, instr->arg1);
                break;
            case TAC_NOT:
                fprintf(out, "    %s = ! %s\n", instr->res, instr->arg1);
                break;
            case TAC_COPY:
                fprintf(out, "    %s = %s\n", instr->res, instr->arg1);
                break;
            case TAC_LABEL:
                fprintf(out, "%s:\n", instr->res);
                break;
            case TAC_GOTO:
                fprintf(out, "    goto %s\n", instr->res);
                break;
            case TAC_IF_TRUE:
                fprintf(out, "    if %s goto %s\n", instr->arg1, instr->res);
                break;
            case TAC_IF_FALSE:
                fprintf(out, "    if_false %s goto %s\n", instr->arg1, instr->res);
                break;
            case TAC_EQ:
                fprintf(out, "    %s = %s == %s\n", instr->res, instr->arg1, instr->arg2);
                break;
            case TAC_LT:
                fprintf(out, "    %s = %s < %s\n", instr->res, instr->arg1, instr->arg2);
                break;
            case TAC_GT:
                fprintf(out, "    %s = %s > %s\n", instr->res, instr->arg1, instr->arg2);
                break;
            case TAC_PARAM:
                fprintf(out, "    param %s\n", instr->arg1);
                break;
            case TAC_RECV_PARAM:
                fprintf(out, "    recv_param %s\n", instr->res);
                break;
            case TAC_CALL:
                fprintf(out, "    %s = call %s, %s\n", instr->res, instr->arg1, instr->arg2);
                break;
            case TAC_CALL_VOID:
                fprintf(out, "    call %s, %s\n", instr->arg1, instr->arg2);
                break;
            case TAC_RETURN:
                if (instr->arg1) {
                    fprintf(out, "    return %s\n", instr->arg1);
                } else {
                    fprintf(out, "    return\n");
                }
                break;
            case TAC_FUNC_BEGIN:
                fprintf(out, "func begin %s\n", instr->res);
                break;
            case TAC_FUNC_END:
                fprintf(out, "func end %s\n\n", instr->res);
                break;
            case TAC_GLOBAL_VAR:
                fprintf(out, "global %s\n", instr->res);
                break;
            case TAC_EXTERN:
                fprintf(out, "extern %s\n", instr->res);
                break;
            default:
                break;
        }
        instr = instr->next;
    }
}
// generates a new temporary variable name

char* new_temp(void) {
    char buf[64];
    snprintf(buf, sizeof(buf), "t%d", temp_counter++);
    return strdup(buf);
}
// generates a new label name

char* new_label(void) {
    char buf[64];
    snprintf(buf, sizeof(buf), "L%d", label_counter++);
    return strdup(buf);
}
// resets the temporary variable and label counters

void tac_reset_counters(void) {
    temp_counter = 0;
    label_counter = 0;
}
