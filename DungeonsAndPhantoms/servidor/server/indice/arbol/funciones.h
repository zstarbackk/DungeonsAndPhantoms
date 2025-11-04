#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include "arbol.h"
#include "../../serverlib.h"

typedef struct{
    int id;
    char usuario[16];
    char contrasenia[16];
    unsigned puntosTotales;
    int cantPartidas;
}tUsuario;
typedef struct{
    int id;
    char usuario[16];
    unsigned puntaje;
    int movimientos;
}tPartida;
typedef struct {
    char usuario[16];
    int id;
    unsigned offset;
}tIndiceUsuarioNombre;
typedef struct{
    unsigned puntaje;
    unsigned id;
    unsigned offset;
}tIndicePartida;

void crearArchivos();

int leerDatosArchivoUsuario(void* dest, FILE *arch, void *param);
int leerDatosArchivoPartida(void* dest, FILE *arch, void *param);

int compararIndPar(const void*d1, const void *d2);
int compararIndPer(const void*d1, const void *d2);
int leerDatosIdxUsuario(void* dest, FILE *arch, void *param);
int leerDatosIdxPartida(void* dest, FILE *arch, void *param);


void mostrarUsuario(void* el);
void mostrarRanked(void* el);

#endif // FUNCIONES_H_INCLUDED
