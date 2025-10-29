#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include "laberinto.h"

#define MSJ_CON_CONEXION "Bienvenido a Dungeons and Phantoms\n" \
                             "    1. Iniciar Partida\n    2. Ver Ranking\n    3. Ver Estadisticas Del Jugador\n 4.Salir\n>"

#define OPC_CON_CONEXION "1234"

#define MSJ_SIN_CONEXION "Bienvenido a Dungeons and Phantoms\n" \
                             "    1. Iniciar Partida\n    2. Salir\n>"

#define OPC_SIN_CONEXION "12"

// Prototipos juego.c
void inicializarEstado(tLaberinto *lab, tEstadoJugador *estado, tConfiguracion config);
void liberarMemoria(tLaberinto *lab);
int juegoTerminado(tEstadoJugador estado, tLaberinto lab);
void moverFantasmas(tLaberinto *lab, tCola *movimientos);
char moverJugador(tLaberinto *lab, tEstadoJugador *estado);
int iniciarJuego(SOCKET sock, const char *nomUsu);
char movimientoAleatorio(tLaberinto *lab, int numFantasma);
void efectuarMovimientos(tLaberinto *lab, tCola *movs, tEstadoJugador *estado);
void menuPrincipal();
int recrearMovimientos(tCola *cMovs, tLaberinto *lab);

#endif // JUEGO_H_INCLUDED
