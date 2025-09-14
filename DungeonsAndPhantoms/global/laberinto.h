#ifndef LABERINTO_H_INCLUDED
#define LABERINTO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <conio.h>
#include "estructuras.h"

// Prototipos laberinto.c
void leerConfiguracion(tConfiguracion *config);
int generarLaberinto(tLaberinto *lab, tConfiguracion config);
void guardarLaberinto(tLaberinto lab, const char *filename);
void mostrarTablero(tLaberinto lab, tEstadoJugador estado);
int cantParedesInternas(tLaberinto *lab);
void eliminarObstaculos(tLaberinto *lab, int paredesAEliminar);
int esParedCritica(char **tablero, int x, int y, int filas, int columnas);

// Prototipos juego.c
void inicializarEstado(tLaberinto *lab, tEstadoJugador *estado, tConfiguracion config);
void liberarMemoria(tLaberinto *lab);
int juegoTerminado(tEstadoJugador estado, tLaberinto lab);
void moverFantasmas(tLaberinto *lab, tEstadoJugador *estado);
int moverJugador(tLaberinto *lab, tEstadoJugador *estado, char direccion);
int iniciarJuego();
void movimientoAleatorio(tLaberinto *lab, int numFantasma);
#endif // LABERINTO_H_INCLUDED
