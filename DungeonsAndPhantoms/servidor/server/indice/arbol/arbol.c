#include "arbol.h"

void crearArbolB(tArbol *p){
    *p = NULL;
}
int insertarEnArbolBRec(tArbol *p, void *dato, unsigned tam, CMP cmp){
    int comp = 0;
    if(!*p){
        *p = (tNodo*)malloc(sizeof(tNodo));
        if(!*p) return 0;
        (*p)->dato = malloc(tam);
        if((*p)->dato == NULL){
            free(*p);
            return 0;
        }
        memcpy((*p)->dato, dato, tam);
        (*p)->tam = tam;
        (*p)->der = NULL;
        (*p)->izq = NULL;
        return 1;
    }

    if((comp = cmp(dato, (*p)->dato)) == 0){
        return 0;
    }
    else{
        if(comp < 0){
            return insertarEnArbolBRec(&(*p)->izq, dato, tam, cmp);
        }
        else{
            return insertarEnArbolBRec(&(*p)->der, dato, tam, cmp);
        }
    }
}
int insertarEnArbolBIt(tArbol *p, void *dato, unsigned tam, CMP cmp){
    int comp = 1;
    while(*p){
        if((comp = cmp(dato, (*p)->dato)) < 0)
            p = &(*p)->izq;
        else
            p = &(*p)->der;
        if(comp == 0) return 0;
    }

    *p = (tNodo*)malloc(sizeof(tNodo));
    if(!*p) return 0;
    (*p)->dato = malloc(tam);
    if((*p)->dato == NULL){
        free(*p);
        return 0;
    }
    memcpy((*p)->dato, dato, tam);
    (*p)->tam = tam;
    (*p)->der = NULL;
    (*p)->izq = NULL;
    //(*p)->der = (*p)->izq = NULL;
    return 1;

}
void recorrerArbolInOrden(tArbol *p, void acc(void*)){
    if(!*p) return;
    recorrerArbolInOrden(&(*p)->izq, acc);
    acc((*p)->dato);
    recorrerArbolInOrden(&(*p)->der, acc);
}
void recorrerArbolPosOrden(tArbol *p, void acc(void*)){
    if(!*p) return;
    recorrerArbolPosOrden(&(*p)->izq, acc);
    recorrerArbolPosOrden(&(*p)->der, acc);
    acc((*p)->dato);
}
void recorrerArbolPreOrden(tArbol *p, void acc(void*)){
    if(!*p) return;
    acc((*p)->dato);
    recorrerArbolPreOrden(&(*p)->izq, acc);
    recorrerArbolPreOrden(&(*p)->der, acc);
}
int contarHojas(tArbol *p){
    if(!*p) return 0;
    if((*p)->izq == NULL && (*p)->der == NULL) return 1;
    return (contarHojas(&(*p)->izq) + contarHojas(&(*p)->der));
}
int contarNoHojas(tArbol *p){
    if(!*p) return 0;
    if((*p)->izq == NULL && (*p)->der == NULL) return 0;
    return (1 + contarHojas(&(*p)->izq) + contarHojas(&(*p)->der));
}
int alturaArbol(tArbol *p){
    if(!*p)return 0;
    return (1 + MAX(alturaArbol(&(*p)->der), alturaArbol(&(*p)->izq)));
}
//int alturaArbol(tArbol *p){
//    int altDer, altIzq;
//    if(!*p)return 0;
//    altDer = alturaArbol(&(*p)->der);
//    altIzq = alturaArbol(&(*p)->izq);
//    return (1 + MAX(altIzq, altDer));
//}
///Incluido el nivel
void verHastaNivel(tArbol *p, int nivel, void acc(void *)){
    if(!*p || nivel == -1) return;
    acc((*p)->dato);
    verHastaNivel(&(*p)->izq, nivel - 1, acc);
    verHastaNivel(&(*p)->der, nivel - 1, acc);
}
void verNodosDeNivel(tArbol *p, int nivel, void acc(void *)){
    if(!*p) return;
    if(nivel == 0){
        acc((*p)->dato);
        return;
    }
    verNodosDeNivel(&(*p)->izq, nivel - 1, acc);
    verNodosDeNivel(&(*p)->der, nivel - 1, acc);
}
///Incluido el nivel
void verNodosDesdeNivel(tArbol *p, int nivel, void acc(void*)){
    if(!*p) return;
    if(nivel <= 0) acc((*p)->dato);
    verNodosDesdeNivel(&(*p)->izq, nivel - 1, acc);
    verNodosDesdeNivel(&(*p)->der, nivel - 1, acc);
}
tArbol* mayorHojaIt(tArbol *p){
    if(!*p) return NULL;
    while((*p)->der || (*p)->izq){
        while((*p)->der)
            p = &(*p)->der;
        if((*p)->izq)
            p = &(*p)->izq;
    }
    return p;
}
tArbol* mayorHojaRec(tArbol *p){
    if(!*p) return NULL;
    if(!(*p)->der && !(*p)->izq) return p;
    if(!(*p)->der) p = &(*p)->izq;
    return mayorHojaRec(&(*p)->der);
}
tArbol* menorHojaIt(tArbol *p){
    if(!*p) return NULL;
    while((*p)->der || (*p)->izq){
        while((*p)->izq)
            p = &(*p)->izq;
        if((*p)->der)
            p = &(*p)->der;
    }
    return p;
}
tArbol* menorHojaRec(tArbol *p){
    if(!*p) return NULL;
    if(!(*p)->der && !(*p)->izq) return p;
    if(!(*p)->izq) p = &(*p)->der;
    return menorHojaRec(&(*p)->izq);
}
int contarNodos(tArbol *p){
    if(!*p) return 0;
    return (1 + contarNodos(&(*p)->der) + contarNodos(&(*p)->izq));
}
int esArbolBalanceado(tArbol *p){
    int altDer, altIzq;
    if(!*p) return 1;
    altIzq = alturaArbol(&(*p)->izq);
    altDer = alturaArbol(&(*p)->der);
    if(altIzq != altDer && altIzq != (altDer + 1) && (altIzq + 1) != altDer)
        return 0;
    return (esArbolBalanceado(&(*p)->izq) && esArbolBalanceado(&(*p)->der));
}

int esArbolCompleto(tArbol *p){
    if(!*p) return 1;
    if(alturaArbol(&(*p)->izq) != alturaArbol(&(*p)->der)) return 0;
    return (esArbolCompleto(&(*p)->izq) && esArbolCompleto(&(*p)->der));
}

//int esArbolAVL(tArbol *p){
//
//}
void vaciarArbol(tArbol *p){
    if(!*p) return;
    vaciarArbol(&(*p)->izq);
    vaciarArbol(&(*p)->der);
    free((*p)->dato);
    free(*p);
}
tArbol* buscarNodo(tArbol *p, const void *clave, CMP cmp){
    int comp;
    if(!*p) return NULL;
    if((comp = cmp(clave, (*p)->dato)) == 0){
        return p;
    }
    else{
        if(comp < 0){
            return buscarNodo(&(*p)->izq, clave, cmp);
        }
        else{
            return buscarNodo(&(*p)->der, clave, cmp);
        }
    }
}
tArbol* mayorNodo(tArbol *p){
    if(!*p) return NULL;
    if(!(*p)->der) return p;
    return mayorNodo(&(*p)->der);
}
tArbol* menorNodo(tArbol *p){
    if(!*p) return NULL;
    if(!(*p)->izq) return p;
    return menorNodo(&(*p)->izq);
}
int eliminarNodo(tArbol *p,const void *clave, void *dato, unsigned tam, CMP cmp){
    tArbol *raizElim;

    if(!*p) return 0;
    raizElim = buscarNodo(p, clave, cmp);
    if(!raizElim) return 0;

    memcpy(dato, (*raizElim)->dato, MIN((*raizElim)->tam, tam));
    free((*raizElim)->dato);

    if((*raizElim)->izq == NULL && (*raizElim)->der == NULL)
        *raizElim = NULL;
    else
        eliminarRaiz(raizElim);
    return 1;
}

void eliminarRaiz(tArbol *p){
    tArbol *reemp;
    reemp = (alturaArbol(&(*p)->der) > alturaArbol(&(*p)->izq) ? menorNodo(&(*p)->der) : mayorNodo(&(*p)->izq));
    (*p)->dato = (*reemp)->dato;
    (*p)->tam = (*reemp)->tam;

    *reemp = (*reemp)->der ? (*reemp)->der : (*reemp)->izq;

}
int cargarArbolDesdeArchivoDesordenado(tArbol *p, FILE *pf, unsigned tam, LEER leer, CMP cmp){
    void *info;
    int leido;
    info = malloc(tam);
    if(!info){
        return 0;
    }
    while((leido = leer(info, pf, &tam)) > 0){
        insertarEnArbolBRec(p, info, leido, cmp);
    }
    free(info);
    return 1;
}

int cargarArchivoDesdeArbol(tArbol *p, FILE *pf){
    if(!*p)return 0;
    cargarArchivoDesdeArbol(&(*p)->izq, pf);
    fwrite((*p)->dato, (*p)->tam, 1, pf);
    cargarArchivoDesdeArbol(&(*p)->der, pf);
    return 1;
}

int crearArchivoIndice(const char* nombArchDat, const char* nombArchInd, unsigned tam, LEER leer, CMP cmp){
    tArbol arbol;
    FILE *pDat, *pInd;

    pDat = fopen(nombArchDat, "r+b");
    if(!pDat) return 0;
    pInd = fopen(nombArchInd, "wb");
    if(!pInd){
        fclose(pDat);
        return 0;
    }
    crearArbolB(&arbol);

    cargarArbolDesdeArchivoDesordenado(&arbol, pDat, tam, leer, cmp);
    cargarArchivoDesdeArbol(&arbol, pInd);

    fclose(pDat);
    fclose(pInd);
    vaciarArbol(&arbol);
    return 1;
}
int cargarArbolDesdeArchivoOrdenado(tArbol *p, FILE *pf, unsigned tam, LEER leer){
    int cantReg;
    if(!pf || *p) return 0;
    fseek(pf, 0, SEEK_END);
    cantReg = (ftell(pf)/tam) - 1;

    return __cargarArbolDesdeArchivoOrdenado(p, pf, tam, 0, cantReg, leer);
}
int __cargarArbolDesdeArchivoOrdenado(tArbol *p, FILE *pf, unsigned tam, int li, int ls, LEER leer){
    int med = (li+ls)/2;

    if(li > ls) return 1;

    (*p) = (tNodo*)malloc(sizeof(tNodo));
    if(!(*p)) return 0;

    (*p)->dato = malloc(tam);
    if(!(*p)->dato){
        free(*p);
        return 0;
    }
    if(!((*p)->tam = leer((*p)->dato, pf, &med))) return 1;
    (*p)->der = NULL;
    (*p)->izq = NULL;

    __cargarArbolDesdeArchivoOrdenado(&(*p)->izq, pf, tam, li, med - 1, leer);
    __cargarArbolDesdeArchivoOrdenado(&(*p)->der, pf, tam, med + 1, ls, leer);

    return 1;
}
//int buscarEnArbolIndice(tArbol *p, void *dato, const void *clave, FILE *pf, LEER leer){
//    tArbol *pBuscado = buscarNodo(p, clave, cmp);
//
//}
int buscarEnArbolIndice(tArbol *p, void *dato, const void *clave, FILE *pf, LEER leer, CMP cmp){
    tArbol *pBuscado = buscarNodo(p, clave, cmp);
    if(!pBuscado) return 0;
    if(!leer(dato, pf, (*pBuscado)->dato)) return 0;
    return 1;
}



