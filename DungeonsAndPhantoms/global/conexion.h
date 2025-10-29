#ifndef CONEXION_H_INCLUDED
#define CONEXION_H_INCLUDED

#include <winsock2.h>
#include "laberinto.h"

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 2048
#define WSK_ERR -12
#define CNX_ERR -13


SOCKET iniciarConexion();

// Inicializa Winsock
int init_winsock();

// Conecta al servidor y devuelve el socket
SOCKET connect_to_server(const char *server_ip, int port);

// Envía una solicitud y recibe la respuesta
int send_request(SOCKET sock, const char *request, char *response);

// Cierra la conexión y limpia Winsock
void close_connection(SOCKET sock);

void iniciarSesion(SOCKET sock, char *nomUsu);
int cargarArchivoLocal(SOCKET sock);
int cargarResultados(SOCKET sock, const char* nomUsu, int puntaje, int cantMov);
int guardarResultadosLocal(const char* nomUsu, int puntaje, int cantMov);

#endif // CONEXION_H_INCLUDED
