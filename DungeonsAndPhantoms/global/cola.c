#include "cola.h"

void crearCola(tCola *p){
    p->pri = NULL;
}
void vaciarCola(tCola *p){
    tNodo *elim = p->pri;
    while(elim){
        p->pri = elim->sig;
        free(elim->info);
        free(elim);
        elim = p->pri;
    }
}
int ponerEnCola(tCola *p, const void *dato, unsigned tam){
    tNodo *nue;
    if((nue = (tNodo*)malloc(sizeof(tNodo))) == NULL || (nue->info = malloc(tam)) == NULL){
        free(nue);
        return 0;
    }
    memcpy(nue->info, dato, tam);
    nue->tam = tam;

    if(p->pri){
        nue->sig = p->ult;
        p->ult = nue;
    }
    else{
        p->pri = nue;
    }
    return 1;
}
int sacarDeCola(tCola *p, void *dato, unsigned tam){
    tNodo *elim = p->pri;
    if(!elim)
        return 0;
    memcpy(dato, elim->info, MIN(tam, elim->tam));
    p->pri = elim->sig;
    free(elim->info);
    free(elim);
    return 1;
}
int verPrimero(const tCola *p, void *dato, unsigned tam){
        tNodo *elim = p->pri;
    if(!elim)
        return 0;
    memcpy(dato, elim->info, MIN(tam, elim->tam));
    return 1;
}
int colaVacia(const tCola *p){
    return p->pri == NULL;
}
int colaLlena(const tCola *p){
    return 0;
}
