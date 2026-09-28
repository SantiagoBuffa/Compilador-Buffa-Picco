#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

typedef enum {
    NODO_PROGRAMA,

    NODO_VAR_DECL,
    NODO_METODO,
    NODO_PARAMETRO,
    NODO_BLOQUE,

    NODO_ASIGNACION,
    NODO_IF,
    NODO_WHILE,
    NODO_RETURN,
    NODO_LLAMADA,

    NODO_OPERACION,
    NODO_ID,
    NODO_LITERAL
} TipoNodo;


typedef struct Nodo {
    TipoNodo tipo;
    char *valor;

    struct Nodo **hijos;
    int cantidad_hijos;
} Nodo;


/* Crear un nodo sin hijos */
Nodo *crearNodo(TipoNodo tipo, char *valor);

/* Agregar un hijo */
void agregarHijo(Nodo *padre, Nodo *hijo);

/* Mostrar el árbol */
void imprimirArbol(Nodo *raiz);

/* Liberar memoria */
void liberarArbol(Nodo *raiz);

#endif