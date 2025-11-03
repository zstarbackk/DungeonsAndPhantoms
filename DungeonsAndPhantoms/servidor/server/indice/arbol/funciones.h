#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include "arbol.h"

#define SALTO printf("\n"); \

typedef struct{
    long dni;
    char nya[21];
    char sex;
}tPersona;
typedef struct{
    long dni;
    long offset;
}tIndicePersona;

int leerDatosArchivoPer(void* dest, FILE *arch, void *param);
int leerDatosArchivoPerConIdx(void* dest, FILE *arch, void *param);
int leerDatosArchivoIdx(void* dest, FILE *arch, void *param);
int compararInt(const void*d1, const void*d2);
int compararIndPer(const void*d1, const void*d2);
int compararClaveConIndPer(const void *clave, const void *ind);
void mostrarInt(void *d);
void mostrarPersonaIdx(void *d);
void crearLote();
#endif // FUNCIONES_H_INCLUDED
