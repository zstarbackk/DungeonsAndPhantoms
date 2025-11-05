#ifndef SERVERLIB_H_INCLUDED
#define SERVERLIB_H_INCLUDED

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <winsock2.h>
#include <stdlib.h>
#include "cola.h"
#include "indice/arbol/arbol.h"
#include "indice/arbol/funciones.h"
#define PORT 12345
#define BUFFER_SIZE 2048

// Inicializa Winsock
int initWinsock();
// Crea el socket del servidor
SOCKET crearServerSocket();
// Procesa el mensaje recibido y genera la respuesta
void procesarRequest(tArbol *indiceUsuario, tArbol *indiceRanked, const char *request, char *response,
                     FILE *fRank, FILE *fUsuarios);
// Ejecuta el bucle principal del servidor
void runServer();

void avisarSeCerroBien();
int chequearEstado();
void login(char * text, tArbol *indice, FILE *pf);
void getRank(char * text, tArbol *p, FILE *pf);
void __getRank(char * text, tArbol *p, FILE *pf);
void getStats(char * text, tArbol *p, FILE *pf);
void postGame(char * text, tArbol *pArbolIdxPart, FILE *fRank, tArbol *pArbolIdxUsua, FILE *fUsuario);

#endif // SERVERLIB_H_INCLUDED
