#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(x,y) ((x)<(y)?(x):(y))
#define MAX(x,y) ((x)>(y)?(x):(y))

typedef struct sNodo{
    void *dato;
    unsigned tam;
    struct sNodo *izq;
    struct sNodo *der;
}tNodo;

typedef tNodo* tArbol;
typedef int (*CMP)(const void*, const void*);
typedef int (*LEER)(void* dest, FILE *arch, void *param);

void crearArbolB(tArbol *p);
int insertarEnArbolBRec(tArbol *p, void *dato, unsigned tam, CMP cmp);
int insertarEnArbolBIt(tArbol *p, void *dato, unsigned tam, CMP cmp);
int contarNodos(tArbol *p);
int alturaArbol(tArbol *p);

int eliminarNodo(tArbol *p,const void *clave, void *dato, unsigned tam, CMP cmp);
void eliminarRaiz(tArbol *p);
tArbol* buscarNodo(tArbol *p, const void *clave, CMP cmp);
void vaciarArbol(tArbol *p);
int verNodo(tArbol *p, void *dato, unsigned tam);
int cargarArbolDesdeArchivoDesordenado(tArbol *p, FILE *pf, unsigned tam, LEER leer, CMP cmp);
int cargarArchivoDesdeArbol(tArbol *p, FILE *pf);
int crearArchivoIndice(const char* nombArchDat, const char* nombArchInd, unsigned tam, LEER leer, CMP cmp);
int cargarArbolDesdeArchivoOrdenado(tArbol *p, FILE *pf, unsigned tam, LEER leer);
int __cargarArbolDesdeArchivoOrdenado(tArbol *p, FILE *pf, unsigned tam, int li, int ls, LEER leer);
int buscarEnArbolIndice(tArbol *p, void *dato, const void *clave, FILE *pf, LEER leer, CMP cmp);
int verMenorNodo(tArbol *p, void *dato, unsigned tam);
int buscarNodo2(tArbol *p, void *dato, const void *clave, unsigned tam, CMP cmp);

#endif // ARBOL_H_INCLUDED
