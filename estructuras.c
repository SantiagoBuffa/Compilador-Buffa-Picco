#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estructuras.h"

// creates a new Node for the AST. all initialized to null by default.
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
    
    node->symbol = NULL;

    node->children = NULL;
    node->child_count = 0;

    return node;
}

// add child to parent node. reallocates memory for the children array.
void addChild(Node *parent, Node *child) {

    if (parent == NULL) {
        fprintf(stderr, "Error: trying to add child to NULL parent\n");
        return;
    }

    if (child == NULL) {
        fprintf(stderr, "Error: trying to add NULL child\n");
        return;
    }
    Node **new_children = realloc(
        parent->children,
        (parent->child_count + 1) * sizeof(Node *)
    );

    if (new_children == NULL) {
        fprintf(stderr, "Error: couldn't allocate memory (tree)\n");
        exit(EXIT_FAILURE);
    }

    parent->children = new_children;

    parent->children[parent->child_count] = child;
    parent->child_count++;
}

// adds a child to the beginning of the parent's children array.
void prependChild(Node *parent, Node *child) {
    if (parent == NULL) {
        fprintf(stderr, "Error: trying to prepend child to NULL parent\n");
        return;
    }
    if (child == NULL) {
        fprintf(stderr, "Error: trying to prepend NULL child\n");
        return;
    }
    Node **new_children = realloc(
        parent->children,
        (parent->child_count + 1) * sizeof(Node *)
    );
    if (new_children == NULL) {
        fprintf(stderr, "Error: couldn't allocate memory (tree)\n");
        exit(EXIT_FAILURE);
    }
    parent->children = new_children;
    for (int i = parent->child_count; i > 0; i--) {
        parent->children[i] = parent->children[i - 1];
    }
    parent->children[0] = child;
    parent->child_count++;
}

// auxiliary function to get the name of a node type as a string. useful for printing the AST.
const char *typeName(NodeType type) {

    switch (type) {

        case PROGRAM_NODE:    return "PROGRAM";
        case VAR_DECL_NODE:    return "VAR_DECL";
        case METHOD_NODE:      return "METHOD";
        case PARAMETER_NODE:   return "PARAMETER";
        case BLOCK_NODE:      return "BLOCK";

        case DECLARATIONS_NODE:     return "DECLARATIONS";
        case VAR_DECL_LIST_NODE:    return "VAR_DECL_LIST";
        case METHOD_DECL_LIST_NODE: return "METHOD_DECL_LIST";
        case PARAM_LIST_NODE:       return "PARAM_LIST";
        case STATEMENT_LIST_NODE:   return "STATEMENT_LIST";
        case ARG_LIST_NODE:         return "ARG_LIST";
        case ID_LIST_NODE:          return "ID_LIST";

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

// recursive function to print the AST with indentation based on the level of the node.
void printASTrec(Node *node, int level) {

    if (node == NULL)
        return;

    for (int i = 0; i < level; i++)
        printf("  ");

    printf("%s", typeName(node->type));

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

// recursive function to free the AST. frees the children first, then the node itself.
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