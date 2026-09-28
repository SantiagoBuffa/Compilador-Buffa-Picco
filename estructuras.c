#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estructuras.h"


Node *createNode(NodeType type, char *value) {

    Node *node = malloc(sizeof(Node));

    if (node == NULL) {
        fprintf(stderr, "Error: couldn't allocate memory (tree)\n");
        exit(EXIT_FAILURE);
    }

    node->type = type;

    if (value != NULL) {
        node->value = strdup(value);
    } else {
        node->value = NULL;
    }

    node->children = NULL;
    node->child_count = 0;

    return node;
}


void addChild(Node *parent, Node *child) {

    if (parent == NULL || child == NULL)
        return;

    parent->children = realloc(
        parent->children,
        (parent->child_count + 1) * sizeof(Node *)
    );

    if (parent->children == NULL) {
        fprintf(stderr, "Error: couldn't allocate memory (tree)\n");
        exit(EXIT_FAILURE);
    }

    parent->children[parent->child_count] = child;
    parent->child_count++;
}


const char *TypeName(NodeType type) {

    switch (type) {

        case PROGRAM_NODE:    return "PROGRAM";
        case VAR_DECL_NODE:    return "VAR_DECL";
        case METHOD_NODE:      return "METHOD";
        case PARAMETER_NODE:   return "PARAMETER";
        case BLOCK_NODE:      return "BLOCK";

        case ASSIGNMENT_NODE:  return "ASSIGNMENT";
        case IF_NODE:          return "IF";
        case WHILE_NODE:       return "WHILE";
        case RETURN_NODE:      return "RETURN";
        case CALL_NODE:     return "CALL";

        case OPERATION_NODE:   return "OPERATION";
        case ID_NODE:          return "ID";
        case LITERAL_NODE:     return "LITERAL";

        default:               return "UNKNOWN";
    }
}


void printASTrec(Node *node, int level) {

    if (node == NULL)
        return;

    for (int i = 0; i < level; i++)
        printf("  ");

    printf("%s", TypeName(node->type));

    if (node->value != NULL)
        printf(" (%s)", node->value);

    printf("\n");

    for (int i = 0; i < node->child_count; i++) {
        printASTrec(node->children[i], level + 1);
    }
}


void printAST(Node *root) {
    printASTrec(root, 0);
}


void freeAST(Node *root) {

    if (root == NULL)
        return;

    for (int i = 0; i < root->child_count; i++) {
        freeAST(root->children[i]);
    }

    free(root->children);
    free(root->value);
    free(root);
}