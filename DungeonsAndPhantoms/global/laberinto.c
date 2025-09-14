#include "laberinto.h"


void leerConfiguracion(tConfiguracion *config) {
    FILE *archivo = fopen("config.txt", "r");
    if (archivo == NULL) {
        // Valores por defecto si no existe el archivo
        config->filas = 15;
        config->columnas = 15;
        config->vidasInicio = 3;
        config->maxFantasmas = 5;
        config->maxPremios = 10;
        config->maxVidasExtra = 3;
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
            sscanf(linea, "vidas_inicio: %d", &config->vidasInicio);
        } else if (strstr(linea, "maximo_numero_fantasmas:")) {
            sscanf(linea, "maximo_numero_fantasmas: %d", &config->maxFantasmas);
        } else if (strstr(linea, "maximo_numero_premios:")) {
            sscanf(linea, "maximo_numero_premios: %d", &config->maxPremios);
        } else if (strstr(linea, "maximo_vidas_extra:")) {
            sscanf(linea, "maximo_vidas_extra: %d", &config->maxVidasExtra);
        }
    }

    fclose(archivo);
}

int generarLaberinto(tLaberinto *lab, tConfiguracion config) {
    int i, j, borde, bordeOpuesto, x, y;
    srand(time(NULL));
    // Inicializar estructura
    lab->filas = config.filas;
    lab->columnas = config.columnas;

    // Reservar memoria para el tablero
    lab->tablero = (char **)malloc(lab->filas * sizeof(char *));
    if(!lab->tablero)
        return 0;

    for (i = 0; i < lab->filas; i++) {
        *((lab->tablero) + i) = (char *)malloc(lab->columnas * sizeof(char));
        if(!(lab->tablero + i)){
            while(i>=0){
                free(lab->tablero + i);
            }
            return 0;
        }
    }

    // Crear paredes externas y caminos internos
    for (i = 0; i < lab->filas; i++) {
        for (j = 0; j < lab->columnas; j++) {
            if (i == 0 || i == lab->filas - 1 || j == 0 || j == lab->columnas - 1) {
                lab->tablero[i][j] = '#'; // Paredes externas
            } else {
                lab->tablero[i][j] = '.'; // Caminos internos
            }
        }
    }

    // Colocar entrada (siempre en un borde)
    borde = rand() % 4;
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
    bordeOpuesto = (borde + 2) % 4;
    switch (bordeOpuesto) {
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
    for (i = 0; i < (lab->filas * lab->columnas) / 10; i++) {
        x = 1 + rand() % (lab->filas - 2);
        y = 1 + rand() % (lab->columnas - 2);
        if (lab->tablero[x][y] == '.') {
            lab->tablero[x][y] = '#';
        }
    }

    // Reservar memoria para elementos del juego
    lab->fantasmas = (tPosicion *)malloc(config.maxFantasmas * sizeof(tPosicion));
    lab->premios = (tPosicion *)malloc(config.maxPremios * sizeof(tPosicion));
    lab->vidasExtra = (tPosicion *)malloc(config.maxVidasExtra * sizeof(tPosicion));

    // Colocar fantasmas
    lab->numFantasmas = config.maxFantasmas;
    for (i = 0; i < lab->numFantasmas; i++) {
        do {
            x = 1 + rand() % (lab->filas - 2);
            y = 1 + rand() % (lab->columnas - 2);
        } while (lab->tablero[x][y] != '.');

        lab->fantasmas[i].x = x;
        lab->fantasmas[i].y = y;
        lab->tablero[x][y] = 'F';
    }

    // Colocar premios
    lab->numPremios = config.maxPremios;
    for (int i = 0; i < lab->numPremios; i++) {
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
    lab->numVidasExtra = config.maxVidasExtra;
    for (int i = 0; i < lab->numVidasExtra; i++) {
        int x, y;
        do {
            x = 1 + rand() % (lab->filas - 2);
            y = 1 + rand() % (lab->columnas - 2);
        } while (lab->tablero[x][y] != '.');

        lab->vidasExtra[i].x = x;
        lab->vidasExtra[i].y = y;
        lab->tablero[x][y] = 'V';
    }
    return 1;
}

void guardarLaberinto(tLaberinto lab, const char *filename) {
    int i, j;
    FILE *archivo = fopen(filename, "w");
    if (archivo == NULL) {
        printf("Error al crear el archivo laberinto.txt\n");
        return;
    }

    for (i = 0; i < lab.filas; i++) {
        for (j = 0; j < lab.columnas; j++) {
            fprintf(archivo, "%c", lab.tablero[i][j]);
        }
        fprintf(archivo, "\n");
    }

    fclose(archivo);
    printf("Laberinto guardado en %s\n", filename);
}


void mostrarTablero(tLaberinto lab, tEstadoJugador estado) {
    system("cls");
    printf("=== LABERINTOS Y FANTASMAS ===\n");
    printf("Controles: w(arriba), s(abajo), a(izquierda), d(derecha), q(salir)\n\n");

    printf("Vidas: %d | Puntos: %d | Premios: %d\n\n", estado.vidas, estado.puntos, estado.premiosCapturados);

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


