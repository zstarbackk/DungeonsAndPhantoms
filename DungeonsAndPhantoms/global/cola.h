#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED

#include <stdlib.h>
#include <string.h>

#define MIN(a,b) ((a)<(b)?(a):(b))

typedef struct sNodo{
    void* info;
    unsigned tam;
    struct sNodo* sig;
}tNodo;

typedef struct{
    tNodo *pri;
    tNodo *ult;
}tCola;

void crearCola(tCola *p);
void vaciarCola(tCola *p);
int ponerEnCola(tCola *p, const void *dato, unsigned tam);
int sacarDeCola(tCola *p, void *dato, unsigned tam);
int verPrimero(const tCola *p, void *dato, unsigned tam);
int colaVacia(const tCola *p);
int colaLlena(const tCola *p);

#endif // COLA_H_INCLUDED
