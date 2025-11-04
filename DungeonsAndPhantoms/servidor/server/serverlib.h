#ifndef SERVERLIB_H_INCLUDED
#define SERVERLIB_H_INCLUDED

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <winsock2.h>
#include <stdlib.h>
#include "cola.h"
#include "indice//arbol//arbol.h"
#define PORT 12345
#define BUFFER_SIZE 2048
typedef struct{
    int id;
    char usuario[16];
    char contrasenia[16];
    unsigned puntosTotales;
    int cantPartidas;
}tUsuario;
typedef struct{
    int id;
    int usuarioId;
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

// Inicializa Winsock
int initWinsock();

// Crea el socket del servidor
SOCKET crearServerSocket();

// Procesa el mensaje recibido y genera la respuesta
void procesarRequest(tArbol* indiceUsuario, tArbol* indiceRanked, const char *request, char *response);

// Ejecuta el bucle principal del servidor
void runServer();
///
void avisarSeCerroBien();
int chequearEstado();
void login(tArbol *indice, char * text);
void getRank(char * text);
void getStats(char * text);
void postGame(char * text);

int leerDatosArchivoUsuario(void* dest, FILE *arch, void *param);
int leerDatosArchivoPartida(void* dest, FILE *arch, void *param);

int compararIndPar(const void*d1, const void *d2);
int compararIndPer(const void*d1, const void *d2);
int leerDatosIdxUsuario(void* dest, FILE *arch, void *param);
int leerDatosIdxPartida(void* dest, FILE *arch, void *param);


void mostrarUsuario(void* el);
void mostrarRanked(void* el);
#endif // SERVERLIB_H_INCLUDED
