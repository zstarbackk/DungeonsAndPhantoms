#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED
#include <stdlib.h>
#include <string.h>

typedef struct sNodo
{
    void           *info;
    unsigned        tamInfo;
    struct sNodo   *sig;
} tNodo;

typedef struct
{
    tNodo  *pri,
           *ult;
} tCola;

void crearCola(tCola *p);
int  colaLlena(const tCola *p, unsigned cantBytes);
int  ponerEnCola(tCola *p, const void *d, unsigned cantBytes);
int  verPrimeroCola(const tCola *p, void *d, unsigned cantBytes);
int  colaVacia(const tCola *p);
int  sacarDeCola(tCola *p, void *d, unsigned cantBytes);
void vaciarCola(tCola *p);




#endif // COLA_H_INCLUDED
