#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

typedef enum {
    PROGRAM_NODE,

    DECLARATIONS_NODE,
    VAR_DECL_LIST_NODE,
    METHOD_DECL_LIST_NODE,
    PARAM_LIST_NODE,
    STATEMENT_LIST_NODE,
    ARG_LIST_NODE,
    ID_LIST_NODE,

    
    VAR_DECL_NODE,
    METHOD_NODE,
    PARAMETER_NODE,
    BLOCK_NODE,
    

    ASSIGNMENT_NODE,
    IF_NODE,
    WHILE_NODE,
    RETURN_NODE,
    CALL_NODE,

    OPERATION_NODE,
    ID_NODE,
    LITERAL_NODE
} NodeType;


struct Symbol;

typedef struct Node {
    NodeType type;
    char *value;
    
    struct Symbol *symbol; // Link to the Symbol Table

    struct Node **children;
    int child_count;
} Node;


/* Create a node with no children */
Node *createNode(NodeType type, char *value);

/* Add a child */
void addChild(Node *parent, Node *child);

/* Print the tree */
void printAST(Node *root);

/* Free memory */
void freeAST(Node *root);

#endif
