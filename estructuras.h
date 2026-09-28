#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

typedef enum {
    PROGRAM_NODE,

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


typedef struct Node {
    NodeType type;
    char *value;

    struct Node **children;
    int child_count;
} Node;


/* Crear un nodo sin hijos */
Node *createNode(NodeType type, char *value);

/* Agregar un hijo */
void addChild(Node *parent, Node *child);

/* Mostrar el árbol */
void printTree(Node *root);

/* Liberar memoria */
void freeTree(Node *root);

#endif