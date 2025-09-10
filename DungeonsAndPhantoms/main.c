#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// Estructuras de datos
typedef struct {
    int filas;
    int columnas;
    int vidas_inicio;
    int max_fantasmas;
    int max_premios;
    int max_vidas_extra;
} Configuracion;

typedef struct {
    int x;
    int y;
} Posicion;

typedef struct {
    char **tablero;
    int filas;
    int columnas;
    Posicion entrada;
    Posicion salida;
    Posicion jugador;
    Posicion *fantasmas;
    int num_fantasmas;
    Posicion *premios;
    int num_premios;
    Posicion *vidas_extra;
    int num_vidas_extra;
} Laberinto;

typedef struct {
    int vidas;
    int puntos;
    int premios_capturados;
} EstadoJugador;

// Prototipos de funciones
void leerConfiguracion(Configuracion *config);
void generarLaberinto(Laberinto *lab, Configuracion config);
void guardarLaberinto(Laberinto lab, const char *filename);
void inicializarJuego(Laberinto *lab, EstadoJugador *estado, Configuracion config);
void mostrarTablero(Laberinto lab, EstadoJugador estado);
int moverJugador(Laberinto *lab, EstadoJugador *estado, char direccion);
void moverFantasmas(Laberinto *lab, EstadoJugador *estado);
int juegoTerminado(EstadoJugador estado, Laberinto lab);
void liberarMemoria(Laberinto *lab);

int main() {
    srand(time(NULL));

    Configuracion config;
    Laberinto laberinto;
    EstadoJugador estado;

    // Leer configuración
    leerConfiguracion(&config);

    // Generar laberinto
    generarLaberinto(&laberinto, config);
    guardarLaberinto(laberinto, "laberinto.txt");

    // Inicializar juego
    inicializarJuego(&laberinto, &estado, config);

    // Bucle principal del juego
    char movimiento;
    int juego_activo = 1;

    printf("=== LABERINTOS Y FANTASMAS ===\n");
    printf("Controles: w(arriba), s(abajo), a(izquierda), d(derecha), q(salir)\n\n");

    while (juego_activo && estado.vidas > 0) {
        mostrarTablero(laberinto, estado);

        printf("Movimiento: ");
        scanf(" %c", &movimiento);

        if (movimiento == 'q') {
            break;
        }

        // Mover jugador
        if (moverJugador(&laberinto, &estado, movimiento)) {
            // Mover fantasmas
            moverFantasmas(&laberinto, &estado);

            // Verificar si el juego terminó
            if (juegoTerminado(estado, laberinto)) {
                juego_activo = 0;
            }
        }
    }

    // Mostrar resultado final
    mostrarTablero(laberinto, estado);
    if (estado.vidas <= 0) {
        printf("\n¡PERDISTE! Te quedaste sin vidas.\n");
    } else if (!juego_activo) {
        printf("\n¡GANASTE! Llegaste a la salida con %d puntos.\n", estado.puntos);
    }

    printf("Puntos finales: %d\n", estado.puntos);
    printf("Premios capturados: %d\n", estado.premios_capturados);

    liberarMemoria(&laberinto);
    return 0;
}

void leerConfiguracion(Configuracion *config) {
    FILE *archivo = fopen("config.txt", "r");
    if (archivo == NULL) {
        // Valores por defecto si no existe el archivo
        config->filas = 15;
        config->columnas = 15;
        config->vidas_inicio = 3;
        config->max_fantasmas = 5;
        config->max_premios = 10;
        config->max_vidas_extra = 3;
        printf("Usando configuración por defecto (archivo config.txt no encontrado)\n");
        return;
    }

    char linea[100];
    while (fgets(linea, sizeof(linea), archivo)) {
        if (strstr(linea, "filas:")) {
            sscanf(linea, "filas: %d", &config->filas);
        } else if (strstr(linea, "columnas:")) {
            sscanf(linea, "columnas: %d", &config->columnas);
        } else if (strstr(linea, "vidas_inicio:")) {
            sscanf(linea, "vidas_inicio: %d", &config->vidas_inicio);
        } else if (strstr(linea, "maximo_numero_fantasmas:")) {
            sscanf(linea, "maximo_numero_fantasmas: %d", &config->max_fantasmas);
        } else if (strstr(linea, "maximo_numero_premios:")) {
            sscanf(linea, "maximo_numero_premios: %d", &config->max_premios);
        } else if (strstr(linea, "maximo_vidas_extra:")) {
            sscanf(linea, "maximo_vidas_extra: %d", &config->max_vidas_extra);
        }
    }

    fclose(archivo);
}

void generarLaberinto(Laberinto *lab, Configuracion config) {
    // Inicializar estructura
    lab->filas = config.filas;
    lab->columnas = config.columnas;

    // Reservar memoria para el tablero
    lab->tablero = (char **)malloc(lab->filas * sizeof(char *));
    for (int i = 0; i < lab->filas; i++) {
        lab->tablero[i] = (char *)malloc(lab->columnas * sizeof(char));
    }

    // Crear paredes externas y caminos internos
    for (int i = 0; i < lab->filas; i++) {
        for (int j = 0; j < lab->columnas; j++) {
            if (i == 0 || i == lab->filas - 1 || j == 0 || j == lab->columnas - 1) {
                lab->tablero[i][j] = '#'; // Paredes externas
            } else {
                lab->tablero[i][j] = '.'; // Caminos internos
            }
        }
    }

    // Colocar entrada (siempre en un borde)
    int borde = rand() % 4;
    switch (borde) {
        case 0: // Borde superior
            lab->entrada.x = 0;
            lab->entrada.y = 1 + rand() % (lab->columnas - 2);
            break;
        case 1: // Borde inferior
            lab->entrada.x = lab->filas - 1;
            lab->entrada.y = 1 + rand() % (lab->columnas - 2);
            break;
        case 2: // Borde izquierdo
            lab->entrada.x = 1 + rand() % (lab->filas - 2);
            lab->entrada.y = 0;
            break;
        case 3: // Borde derecho
            lab->entrada.x = 1 + rand() % (lab->filas - 2);
            lab->entrada.y = lab->columnas - 1;
            break;
    }
    lab->tablero[lab->entrada.x][lab->entrada.y] = 'E';

    // Colocar salida (en borde opuesto)
    int borde_opuesto = (borde + 2) % 4;
    switch (borde_opuesto) {
        case 0: // Borde superior
            lab->salida.x = 0;
            lab->salida.y = 1 + rand() % (lab->columnas - 2);
            break;
        case 1: // Borde inferior
            lab->salida.x = lab->filas - 1;
            lab->salida.y = 1 + rand() % (lab->columnas - 2);
            break;
        case 2: // Borde izquierdo
            lab->salida.x = 1 + rand() % (lab->filas - 2);
            lab->salida.y = 0;
            break;
        case 3: // Borde derecho
            lab->salida.x = 1 + rand() % (lab->filas - 2);
            lab->salida.y = lab->columnas - 1;
            break;
    }
    lab->tablero[lab->salida.x][lab->salida.y] = 'S';

    // Colocar algunos obstáculos internos
    for (int i = 0; i < (lab->filas * lab->columnas) / 10; i++) {
        int x = 1 + rand() % (lab->filas - 2);
        int y = 1 + rand() % (lab->columnas - 2);
        if (lab->tablero[x][y] == '.') {
            lab->tablero[x][y] = '#';
        }
    }

    // Reservar memoria para elementos del juego
    lab->fantasmas = (Posicion *)malloc(config.max_fantasmas * sizeof(Posicion));
    lab->premios = (Posicion *)malloc(config.max_premios * sizeof(Posicion));
    lab->vidas_extra = (Posicion *)malloc(config.max_vidas_extra * sizeof(Posicion));

    // Colocar fantasmas
    lab->num_fantasmas = config.max_fantasmas;
    for (int i = 0; i < lab->num_fantasmas; i++) {
        int x, y;
        do {
            x = 1 + rand() % (lab->filas - 2);
            y = 1 + rand() % (lab->columnas - 2);
        } while (lab->tablero[x][y] != '.');

        lab->fantasmas[i].x = x;
        lab->fantasmas[i].y = y;
        lab->tablero[x][y] = 'F';
    }

    // Colocar premios
    lab->num_premios = config.max_premios;
    for (int i = 0; i < lab->num_premios; i++) {
        int x, y;
        do {
            x = 1 + rand() % (lab->filas - 2);
            y = 1 + rand() % (lab->columnas - 2);
        } while (lab->tablero[x][y] != '.');

        lab->premios[i].x = x;
        lab->premios[i].y = y;
        lab->tablero[x][y] = 'P';
    }

    // Colocar vidas extra
    lab->num_vidas_extra = config.max_vidas_extra;
    for (int i = 0; i < lab->num_vidas_extra; i++) {
        int x, y;
        do {
            x = 1 + rand() % (lab->filas - 2);
            y = 1 + rand() % (lab->columnas - 2);
        } while (lab->tablero[x][y] != '.');

        lab->vidas_extra[i].x = x;
        lab->vidas_extra[i].y = y;
        lab->tablero[x][y] = 'V';
    }
}

void guardarLaberinto(Laberinto lab, const char *filename) {
    FILE *archivo = fopen(filename, "w");
    if (archivo == NULL) {
        printf("Error al crear el archivo laberinto.txt\n");
        return;
    }

    for (int i = 0; i < lab.filas; i++) {
        for (int j = 0; j < lab.columnas; j++) {
            fprintf(archivo, "%c", lab.tablero[i][j]);
        }
        fprintf(archivo, "\n");
    }

    fclose(archivo);
    printf("Laberinto guardado en %s\n", filename);
}

void inicializarJuego(Laberinto *lab, EstadoJugador *estado, Configuracion config) {
    // Posicionar jugador en la entrada
    lab->jugador = lab->entrada;

    // Inicializar estado del jugador
    estado->vidas = config.vidas_inicio;
    estado->puntos = 0;
    estado->premios_capturados = 0;
}

void mostrarTablero(Laberinto lab, EstadoJugador estado) {
    system("cls"); // Para Linux/Mac. Usar "cls" para Windows

    printf("Vidas: %d | Puntos: %d | Premios: %d\n\n", estado.vidas, estado.puntos, estado.premios_capturados);

    for (int i = 0; i < lab.filas; i++) {
        for (int j = 0; j < lab.columnas; j++) {
            if (i == lab.jugador.x && j == lab.jugador.y) {
                printf("J "); // Mostrar jugador
            } else {
                printf("%c ", lab.tablero[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

int moverJugador(Laberinto *lab, EstadoJugador *estado, char direccion) {
    int nuevo_x = lab->jugador.x;
    int nuevo_y = lab->jugador.y;

    switch (direccion) {
        case 'w': nuevo_x--; break;
        case 's': nuevo_x++; break;
        case 'a': nuevo_y--; break;
        case 'd': nuevo_y++; break;
        default:
            printf("Movimiento no válido\n");
            return 0;
    }

    // Verificar límites
    if (nuevo_x < 0 || nuevo_x >= lab->filas || nuevo_y < 0 || nuevo_y >= lab->columnas) {
        printf("Movimiento fuera de límites\n");
        return 0;
    }

    // Verificar colisión con pared
    if (lab->tablero[nuevo_x][nuevo_y] == '#') {
        printf("¡Choque con pared!\n");
        return 0;
    }

    // Mover jugador
    lab->jugador.x = nuevo_x;
    lab->jugador.y = nuevo_y;

    // Verificar colisión con elementos
    char elemento = lab->tablero[nuevo_x][nuevo_y];

    if (elemento == 'P') { // Premio
        estado->puntos += 10;
        estado->premios_capturados++;
        printf("¡Premio capturado! +10 puntos\n");
        lab->tablero[nuevo_x][nuevo_y] = '.'; // Remover premio

    } else if (elemento == 'V') { // Vida extra
        estado->vidas++;
        printf("¡Vida extra! +1 vida\n");
        lab->tablero[nuevo_x][nuevo_y] = '.'; // Remover vida extra

    } else if (elemento == 'F') { // Fantasma
        estado->vidas--;
        printf("¡Fantasma! -1 vida. Regresas a la entrada\n");
        lab->jugador = lab->entrada; // Regresar a entrada
        return 1;
    }

    return 1;
}

void moverFantasmas(Laberinto *lab, EstadoJugador *estado) {
    int direcciones[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Arriba, abajo, izquierda, derecha

    for (int i = 0; i < lab->num_fantasmas; i++) {
        if (lab->fantasmas[i].x == -1) continue; // Fantasma eliminado

        int dir = rand() % 4;
        int nuevo_x = lab->fantasmas[i].x + direcciones[dir][0];
        int nuevo_y = lab->fantasmas[i].y + direcciones[dir][1];

        // Verificar movimiento válido
        if (nuevo_x >= 0 && nuevo_x < lab->filas && nuevo_y >= 0 && nuevo_y < lab->columnas &&
            lab->tablero[nuevo_x][nuevo_y] != '#' && lab->tablero[nuevo_x][nuevo_y] != 'E' &&
            lab->tablero[nuevo_x][nuevo_y] != 'S') {

            // Mover fantasma
            lab->tablero[lab->fantasmas[i].x][lab->fantasmas[i].y] = '.';
            lab->fantasmas[i].x = nuevo_x;
            lab->fantasmas[i].y = nuevo_y;
            lab->tablero[nuevo_x][nuevo_y] = 'F';

            // Verificar colisión con jugador
            if (nuevo_x == lab->jugador.x && nuevo_y == lab->jugador.y) {
                estado->vidas--;
                printf("¡Fantasma te atrapó! -1 vida. Regresas a la entrada\n");
                lab->jugador = lab->entrada;
                lab->tablero[nuevo_x][nuevo_y] = '.'; // Eliminar fantasma
                lab->fantasmas[i].x = -1; // Marcar como eliminado
            }
        }
    }
}

int juegoTerminado(EstadoJugador estado, Laberinto lab) {
    // Verificar si el jugador llegó a la salida
    if (lab.jugador.x == lab.salida.x && lab.jugador.y == lab.salida.y) {
        return 1;
    }

    return 0;
}

void liberarMemoria(Laberinto *lab) {
    for (int i = 0; i < lab->filas; i++) {
        free(lab->tablero[i]);
    }
    free(lab->tablero);
    free(lab->fantasmas);
    free(lab->premios);
    free(lab->vidas_extra);
}
