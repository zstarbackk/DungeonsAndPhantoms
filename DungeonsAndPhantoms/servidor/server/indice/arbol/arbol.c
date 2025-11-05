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
        if(!(*p)->dato){
            free(*p);
            *p = NULL;
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

int alturaArbol(tArbol *p){
    if(!*p)return 0;
    return (1 + MAX(alturaArbol(&(*p)->der), alturaArbol(&(*p)->izq)));
}

int contarNodos(tArbol *p){
    if(!*p) return 0;
    return (1 + contarNodos(&(*p)->der) + contarNodos(&(*p)->izq));
}

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
int buscarNodo2(tArbol *p, void *dato, const void *clave, unsigned tam, CMP cmp){
    int comp;
    if(!*p) return 0;
    if((comp = cmp(clave, (*p)->dato)) == 0){
        memcpy(dato, (*p)->dato, MIN(tam, (*p)->tam));
        return 1;
    }
    else{
        if(comp < 0){
            return buscarNodo2(&(*p)->izq, dato, clave, tam, cmp);
        }
        else{
            return buscarNodo2(&(*p)->der, dato, clave, tam, cmp);
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
    info = malloc(tam);
    if(!info){
        return 0;
    }
    while(leer(info, pf, &tam) == tam){
        insertarEnArbolBRec(p, info, tam, cmp);
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

    pDat = fopen(nombArchDat, "rb");
    if(!pDat) return 0;
    pInd = fopen(nombArchInd, "wb");
    if(!pInd){
        fclose(pDat);
        return 0;
    }
    crearArbolB(&arbol);

    if(!cargarArbolDesdeArchivoDesordenado(&arbol, pDat, tam, leer, cmp)){
        fclose(pDat);
        fclose(pInd);
        vaciarArbol(&arbol);
        return 0;
    }
    if(!cargarArchivoDesdeArbol(&arbol, pInd)){
        fclose(pDat);
        fclose(pInd);
        vaciarArbol(&arbol);
        return 0;
    }

    fclose(pDat);
    fclose(pInd);
    vaciarArbol(&arbol);
    return 1;
}
int cargarArbolDesdeArchivoOrdenado(tArbol *p, FILE *pf, unsigned tam){
    int cantReg;
    if(!pf || *p) return 0;
    fseek(pf, 0, SEEK_END);
    cantReg = (ftell(pf)/tam);

    return __cargarArbolDesdeArchivoOrdenado(p, pf, tam, 0, cantReg - 1);
}
int __cargarArbolDesdeArchivoOrdenado(tArbol *p, FILE *pf, unsigned tam, int li, int ls){
    int med = (li+ls)/2;

    if(li > ls) return 1;

    (*p) = (tNodo*)malloc(sizeof(tNodo));
    if(!(*p)) return 0;

    (*p)->dato = malloc(tam);
    if(!(*p)->dato){
        free(*p);
        return 0;
    }
    fseek(pf, med * tam, SEEK_SET);
    if(!fread((*p)->dato, tam, 1, pf)){
        free((*p)->dato);
        free(*p);
        return 1;
    }
    (*p)->tam = tam;
    (*p)->der = NULL;
    (*p)->izq = NULL;

    if(!__cargarArbolDesdeArchivoOrdenado(&(*p)->izq, pf, tam, li, med - 1))
        return 0;
    if(!__cargarArbolDesdeArchivoOrdenado(&(*p)->der, pf, tam, med + 1, ls))
        return 0;

    return 1;
}

int buscarEnArbolIndice(tArbol *p, void *dato, const void *clave, FILE *pf, LEER leer, CMP cmp){
    tArbol *pBuscado = buscarNodo(p, clave, cmp);
    if(!pBuscado) return 0;
    if(!leer(dato, pf, (*pBuscado)->dato)) return 0;
    return 1;
}


int verNodo(tArbol *p, void *dato, unsigned tam){
    if(!*p) return 0;
    memcpy(dato, (*p)->dato, MIN(tam, (*p)->tam));
    return 1;
}

int verMenorNodo(tArbol *p, void *dato, unsigned tam){
    if(!*p) return 0;
    while((*p)->izq != NULL)
        p = &(*p)->izq;
    memcpy(dato, (*p)->dato, MIN(tam, (*p)->tam));
    return 1;
}
