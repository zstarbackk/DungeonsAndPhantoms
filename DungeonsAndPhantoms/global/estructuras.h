#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED
///Macros
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
///esX
#define esPared(a) ((a)=='#')
#define esCamino(a) ((a)=='.')
#define esSalida(a) ((a)=='S')
#define esFantasma(a) ((a)=='F')
#define esPremio(a) ((a)=='P')
#define esVida(a) ((a)=='V')
///Define componentes como chars
#define PARED '#'
#define CAMINO '.'
#define ENTRADA 'E'
#define SALIDA 'S'
#define PREMIO 'P'
#define VIDA 'V'

typedef struct {
    int filas;
    int columnas;
    int vidasInicio;
    int maxFantasmas;
    int maxPremios;
    int maxVidasExtra;
} tConfiguracion;

typedef struct {
    int x;
    int y;
} tPosicion;

typedef struct {
    char **tablero;
    int filas;
    int columnas;
    Posicion entrada;
    Posicion salida;
    Posicion jugador;
    Posicion *fantasmas;
    int numFantasmas;
    Posicion *premios;
    int numPremios;
    Posicion *vidasExtra;
    int numVidasExtra;
} tJuego;

typedef struct {
    int vidas;
    int puntos;
    int premiosCapturados;
    int cantMov;
} tEstadoJugador;
typedef struct{
    char usuario[20];
}tJugador;
#endif // ESTRUCTURAS_H_INCLUDED
