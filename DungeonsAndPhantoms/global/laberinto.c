#include "laberinto.h"


void leerConfiguracion(tConfiguracion *config) {
    FILE *archivo = fopen("global/config.txt", "r");
    if (archivo == NULL) {
        // Valores por defecto si no existe el archivo
        printf("Inicializado con valores por defecto");
        config->filas = 15;
        config->columnas = 15;
        config->vidasInicio = 1;
        config->maxFantasmas = 2;
        config->maxPremios = 10;
        config->maxVidasExtra = 3;
        config->obstaculosEliminados = 0.1;
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
        } else if(strstr(linea, "porcentajes_obstaculos:")){
            sscanf(linea, "porcentajes_obstaculos: %f", &config->obstaculosEliminados);
        }
    }

    fclose(archivo);
}
int esValida(int x, int y, int filas, int columnas) {
    return x >= 0 && x < filas && y >= 0 && y < columnas;
}

// Función recursiva para generar el laberinto con DFS
void generarLaberintoDFS(char **tablero, int x, int y, int filas, int columnas, int **visitado) {
    // Direcciones: arriba, derecha, abajo, izquierda
    int dx[] = {-2, 0, 2, 0};
    int dy[] = {0, 2, 0, -2};
    int direcciones[] = {0, 1, 2, 3};

    // Mezclar direcciones para aleatoriedad
    for (int i = 0; i < 4; i++) {
        int j = rand() % 4;
        int temp = direcciones[i];
        direcciones[i] = direcciones[j];
        direcciones[j] = temp;
    }

    // Marcar celda actual como visitada y camino
    visitado[x][y] = 1;
    tablero[x][y] = '.';

    // Explorar en todas las direcciones
    for (int i = 0; i < 4; i++) {
        int dir = direcciones[i];
        int nx = x + dx[dir];
        int ny = y + dy[dir];

        if (esValida(nx, ny, filas, columnas) && !visitado[nx][ny]) {
            // Quitar la pared entre la celda actual y la nueva
            tablero[x + dx[dir]/2][y + dy[dir]/2] = '.';
            generarLaberintoDFS(tablero, nx, ny, filas, columnas, visitado);
        }
    }
}

int generarLaberinto(tLaberinto *lab, tConfiguracion config) {
    int i, j, borde, paredesAEliminar,bordeOpuesto, x, y, startX, startY, paredesInternas;
    srand(time(NULL));

    // Asegurar dimensiones impares para el algoritmo
    if (config.filas % 2 == 0) config.filas++;
    if (config.columnas % 2 == 0) config.columnas++;

    // Inicializar estructura
    lab->filas = config.filas;
    lab->columnas = config.columnas;

    // Reservar memoria para el tablero
    lab->tablero = (char **)malloc(lab->filas * sizeof(char *));
    if (!lab->tablero) return 0;

    for (i = 0; i < lab->filas; i++) {
        lab->tablero[i] = (char *)malloc(lab->columnas * sizeof(char));
        if (!lab->tablero[i]) {
            for (int k = 0; k < i; k++) {
                free(lab->tablero[k]);
            }
            free(lab->tablero);
            return 0;
        }
    }

    // Inicializar todo como paredes
    for (i = 0; i < lab->filas; i++) {
        for (j = 0; j < lab->columnas; j++) {
            lab->tablero[i][j] = '#';
        }
    }

    // Crear matriz de visitados para el algoritmo DFS
    int **visitado = (int **)malloc(lab->filas * sizeof(int *));
    for (i = 0; i < lab->filas; i++) {
        visitado[i] = (int *)malloc(lab->columnas * sizeof(int));
        for (j = 0; j < lab->columnas; j++) {
            visitado[i][j] = 0;
        }
    }

    // Generar laberinto empezando desde una celda impar (para mantener el patrón)
    startX = 1;
    startY = 1;
    generarLaberintoDFS(lab->tablero, startX, startY, lab->filas, lab->columnas, visitado);

    // Liberar matriz de visitados
    for (i = 0; i < lab->filas; i++) {
        free(visitado[i]);
    }
    free(visitado);
    ///Cuantas paredes internas hay

    paredesInternas = cantParedesInternas(lab);

    paredesAEliminar = (int)(config.obstaculosEliminados*paredesInternas);
    eliminarObstaculos(lab, paredesAEliminar);

    ///

    // Colocar entrada (siempre en un borde)
    borde = rand() % 4;
    switch (borde) {
        case 0: // Borde superior
            do {
                lab->entrada.x = 0;
                lab->entrada.y = 1 + rand() % (lab->columnas - 2);
            } while (lab->tablero[1][lab->entrada.y] != '.');
            break;
        case 1: // Borde inferior
            do {
                lab->entrada.x = lab->filas - 1;
                lab->entrada.y = 1 + rand() % (lab->columnas - 2);
            } while (lab->tablero[lab->filas - 2][lab->entrada.y] != '.');
            break;
        case 2: // Borde izquierdo
            do {
                lab->entrada.x = 1 + rand() % (lab->filas - 2);
                lab->entrada.y = 0;
            } while (lab->tablero[lab->entrada.x][1] != '.');
            break;
        case 3: // Borde derecho
            do {
                lab->entrada.x = 1 + rand() % (lab->filas - 2);
                lab->entrada.y = lab->columnas - 1;
            } while (lab->tablero[lab->entrada.x][lab->columnas - 2] != '.');
            break;
    }
    lab->tablero[lab->entrada.x][lab->entrada.y] = 'E';

    // Colocar salida (en borde opuesto)
    bordeOpuesto = (borde + 2) % 4;
    switch (bordeOpuesto) {
        case 0: // Borde superior
            do {
                lab->salida.x = 0;
                lab->salida.y = 1 + rand() % (lab->columnas - 2);
            } while (lab->tablero[1][lab->salida.y] != '.' ||
                    (lab->salida.x == lab->entrada.x && lab->salida.y == lab->entrada.y));
            break;
        case 1: // Borde inferior
            do {
                lab->salida.x = lab->filas - 1;
                lab->salida.y = 1 + rand() % (lab->columnas - 2);
            } while (lab->tablero[lab->filas - 2][lab->salida.y] != '.' ||
                    (lab->salida.x == lab->entrada.x && lab->salida.y == lab->entrada.y));
            break;
        case 2: // Borde izquierdo
            do {
                lab->salida.x = 1 + rand() % (lab->filas - 2);
                lab->salida.y = 0;
            } while (lab->tablero[lab->salida.x][1] != '.' ||
                    (lab->salida.x == lab->entrada.x && lab->salida.y == lab->entrada.y));
            break;
        case 3: // Borde derecho
            do {
                lab->salida.x = 1 + rand() % (lab->filas - 2);
                lab->salida.y = lab->columnas - 1;
            } while (lab->tablero[lab->salida.x][lab->columnas - 2] != '.' ||
                    (lab->salida.x == lab->entrada.x && lab->salida.y == lab->entrada.y));
            break;
    }
    lab->tablero[lab->salida.x][lab->salida.y] = 'S';

    // Asegurar que la entrada y salida estén conectadas al laberinto
    if (lab->entrada.x == 0) lab->tablero[1][lab->entrada.y] = '.';
    else if (lab->entrada.x == lab->filas - 1) lab->tablero[lab->filas - 2][lab->entrada.y] = '.';
    else if (lab->entrada.y == 0) lab->tablero[lab->entrada.x][1] = '.';
    else lab->tablero[lab->entrada.x][lab->columnas - 2] = '.';

    if (lab->salida.x == 0) lab->tablero[1][lab->salida.y] = '.';
    else if (lab->salida.x == lab->filas - 1) lab->tablero[lab->filas - 2][lab->salida.y] = '.';
    else if (lab->salida.y == 0) lab->tablero[lab->salida.x][1] = '.';
    else lab->tablero[lab->salida.x][lab->columnas - 2] = '.';

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
    for (i = 0; i < lab->numPremios; i++) {
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
    for (i = 0; i < lab->numVidasExtra; i++) {
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
int cantParedesInternas(tLaberinto *lab){
    int paredesInternasActuales = 0, i, j;
    for (i = 1; i < lab->filas - 1; i++) {
        for (j = 1; j < lab->columnas - 1; j++) {
            if (lab->tablero[i][j] == '#') {
                paredesInternasActuales++;
            }
        }
    }
    return paredesInternasActuales;
}
void eliminarObstaculos(tLaberinto * lab, int aEliminar){
    int eliminadas = 0, x, y;
    while(eliminadas < aEliminar){
        x = 1 + rand() % (lab->filas - 2);
        y = 1 + rand() % (lab->columnas - 2);
        if (lab->tablero[x][y] == '#' &&
            !esParedCritica(lab->tablero, x, y, lab->filas, lab->columnas)) {
            lab->tablero[x][y] = '.';
            eliminadas++;
        }
    }
}
int esParedCritica(char **tablero, int x, int y, int filas, int columnas) {
    // Una pared es crítica si al eliminarla se crea un área cerrada
    // Verificar si es una pared que conecta dos áreas diferentes
    int caminosAdyacentes = 0;

    // Contar celdas adyacentes que son caminos
    if (x > 0 && tablero[x-1][y] == '.') caminosAdyacentes++;
    if (x < filas-1 && tablero[x+1][y] == '.') caminosAdyacentes++;
    if (y > 0 && tablero[x][y-1] == '.') caminosAdyacentes++;
    if (y < columnas-1 && tablero[x][y+1] == '.') caminosAdyacentes++;

    // Si hay más de 2 caminos adyacentes, probablemente no sea crítica
    return caminosAdyacentes < 2;
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
}


void mostrarTablero(tLaberinto *lab, tEstadoJugador *estado) {
    system("cls");
    printf("=== LABERINTOS Y FANTASMAS ===\n");
    printf("Controles: w(arriba), s(abajo), a(izquierda), d(derecha), q(salir)\n\n");

    printf("Vidas: %d | Puntos: %d | Premios: %d\n\n", estado->vidas, estado->puntos, estado->premiosCapturados);

    for (int i = 0; i < lab->filas; i++) {
        for (int j = 0; j < lab->columnas; j++) {
            if (i == lab->jugador.x && j == lab->jugador.y) {
                printf("J "); // Mostrar jugador
            } else {
                printf("%c ", lab->tablero[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}


