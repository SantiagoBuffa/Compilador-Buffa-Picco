#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estructuras.h"


Nodo *crearNodo(TipoNodo tipo, char *valor) {

    Nodo *nodo = malloc(sizeof(Nodo));

    if (nodo == NULL) {
        fprintf(stderr, "Error: no se pudo reservar memoria\n");
        exit(EXIT_FAILURE);
    }

    nodo->tipo = tipo;

    if (valor != NULL) {
        nodo->valor = strdup(valor);
    } else {
        nodo->valor = NULL;
    }

    nodo->hijos = NULL;
    nodo->cantidad_hijos = 0;

    return nodo;
}


void agregarHijo(Nodo *padre, Nodo *hijo) {

    if (padre == NULL || hijo == NULL)
        return;

    padre->hijos = realloc(
        padre->hijos,
        (padre->cantidad_hijos + 1) * sizeof(Nodo *)
    );

    if (padre->hijos == NULL) {
        fprintf(stderr, "Error: no se pudo reservar memoria\n");
        exit(EXIT_FAILURE);
    }

    padre->hijos[padre->cantidad_hijos] = hijo;
    padre->cantidad_hijos++;
}


const char *nombreTipo(TipoNodo tipo) {

    switch (tipo) {

        case NODO_PROGRAMA:    return "PROGRAMA";
        case NODO_VAR_DECL:    return "VAR_DECL";
        case NODO_METODO:      return "METODO";
        case NODO_PARAMETRO:   return "PARAMETRO";
        case NODO_BLOQUE:      return "BLOQUE";

        case NODO_ASIGNACION:  return "ASIGNACION";
        case NODO_IF:          return "IF";
        case NODO_WHILE:       return "WHILE";
        case NODO_RETURN:      return "RETURN";
        case NODO_LLAMADA:     return "LLAMADA";

        case NODO_OPERACION:   return "OPERACION";
        case NODO_ID:          return "ID";
        case NODO_LITERAL:     return "LITERAL";

        default:               return "DESCONOCIDO";
    }
}


void imprimirArbolRec(Nodo *nodo, int nivel) {

    if (nodo == NULL)
        return;

    for (int i = 0; i < nivel; i++)
        printf("  ");

    printf("%s", nombreTipo(nodo->tipo));

    if (nodo->valor != NULL)
        printf(" (%s)", nodo->valor);

    printf("\n");

    for (int i = 0; i < nodo->cantidad_hijos; i++) {
        imprimirArbolRec(nodo->hijos[i], nivel + 1);
    }
}


void imprimirArbol(Nodo *raiz) {
    imprimirArbolRec(raiz, 0);
}


void liberarArbol(Nodo *raiz) {

    if (raiz == NULL)
        return;

    for (int i = 0; i < raiz->cantidad_hijos; i++) {
        liberarArbol(raiz->hijos[i]);
    }

    free(raiz->hijos);
    free(raiz->valor);
    free(raiz);
}