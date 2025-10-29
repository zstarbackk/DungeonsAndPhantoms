#ifndef SERVERLIB_H_INCLUDED
#define SERVERLIB_H_INCLUDED

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <winsock2.h>

#include "cola.h"

#define PORT 12345
#define BUFFER_SIZE 2048
typedef struct{
    char usuario[16];
    char contrasenia[16];
    unsigned puntosTotales;
    int cantPartidas;
}tUsuario;

// Inicializa Winsock
int initWinsock();

// Crea el socket del servidor
SOCKET crearServerSocket();

// Procesa el mensaje recibido y genera la respuesta
void procesarRequest(const char *request, char *response);

// Ejecuta el bucle principal del servidor
void runServer();
///
void avisarSeCerroBien();
int chequearEstado();
void login(char * text);
void getRank(char * text);
void getStats(char * text);
void postGame(char * text);


#endif // SERVERLIB_H_INCLUDED
