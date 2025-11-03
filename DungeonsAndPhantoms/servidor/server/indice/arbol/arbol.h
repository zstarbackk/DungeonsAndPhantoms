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
void recorrerArbolInOrden(tArbol *p, void acc(void*));
void recorrerArbolPosOrden(tArbol *p, void acc(void*));
void recorrerArbolPreOrden(tArbol *p, void acc(void*));
int contarHojas(tArbol *p);
int contarNoHojas(tArbol *p);
int contarNodos(tArbol *p);
int alturaArbol(tArbol *p);
tArbol* mayorHojaIt(tArbol *p);
tArbol* mayorHojaRec(tArbol *p);
tArbol* menorHojaIt(tArbol *p);
tArbol* menorHojaRec(tArbol *p);

void verHastaNivel(tArbol *p, int nivel, void acc(void *));
void verNodosDeNivel(tArbol *p, int nivel, void acc(void *));
void verNodosDesdeNivel(tArbol *p, int nivel, void acc(void*));
int esArbolCompleto(tArbol *p);
int esArbolBalanceado(tArbol *p);
tArbol* buscarNodo(tArbol *p, const void *clave, CMP cmp);
tArbol* mayorNodo(tArbol *p);
tArbol* menorNodo(tArbol *p);
int eliminarNodo(tArbol *p,const void *clave, void *dato, unsigned tam, CMP cmp);
void eliminarRaiz(tArbol *p);

int cargarArbolDesdeArchivoDesordenado(tArbol *p, FILE *pf, unsigned tam, LEER leer, CMP cmp);
int cargarArchivoDesdeArbol(tArbol *p, FILE *pf);
int crearArchivoIndice(const char* nombArchDat, const char* nombArchInd, unsigned tam, LEER leer, CMP cmp);
int cargarArbolDesdeArchivoOrdenado(tArbol *p, FILE *pf, unsigned tam, LEER leer);
int __cargarArbolDesdeArchivoOrdenado(tArbol *p, FILE *pf, unsigned tam, int li, int ls, LEER leer);
int buscarEnArbolIndice(tArbol *p, void *dato, const void *clave, FILE *pf, LEER leer, CMP cmp);
///Contar Hojas, Contar No Hojas, Cuantos Nodos Tiene Un Arbol, Clave Mayor, Clave Menor, Cantidad Nodos
///Del Sub Arbol Derecho Que Cumple Una Condicion, Cantidad De Nodos Que Cumplen Una Condicion a Partir De Una Clave,
///Promedio de los Nodos Con Clave Parcial, Buscar Un Valor No Clave Del Arbol Con Accion (Pueden Ser Varios).
#endif // ARBOL_H_INCLUDED
