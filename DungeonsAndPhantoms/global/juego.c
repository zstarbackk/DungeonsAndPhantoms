#include "laberinto.h"

int iniciarJuego(){
    tConfiguracion config;
    tLaberinto laberinto;
    tEstadoJugador estado;
    // Bucle principal del juego
    char movimiento;
    int finJuego = 0;

    // Leer configuración
    leerConfiguracion(&config);

    // Generar laberinto
    if(!generarLaberinto(&laberinto, config))
        return LAB_ERR;
    guardarLaberinto(laberinto, "laberinto.txt");

    // Inicializar juego
    inicializarEstado(&laberinto, &estado, config);



    while (!finJuego && estado.vidas > 0) {

        mostrarTablero(laberinto, estado);

        fflush(stdin);
        printf("Movimiento: ");
        movimiento = getch();
        //scanf(" %c", &movimiento);

        if (movimiento == 'q') {
            break;
        }

        // Mover jugador
        if (moverJugador(&laberinto, &estado, movimiento)) {
            // Mover fantasmas
            moverFantasmas(&laberinto, &estado);

            // Verificar si el juego terminó
            finJuego = juegoTerminado(estado, laberinto);
//            if (juegoTerminado(estado, laberinto)) {
//                juegoActivo = 0;
//            }
        }
    }

    // Mostrar resultado final
    mostrarTablero(laberinto, estado);
    if (estado.vidas <= 0) {
        printf("\n¡PERDISTE! Te quedaste sin vidas.\n");
    } else if (finJuego) {
        printf("\n¡GANASTE! Llegaste a la salida con %d puntos.\n", estado.puntos);
    }

    printf("Puntos finales: %d\n", estado.puntos);
    printf("Premios capturados: %d\n", estado.premiosCapturados);

    liberarMemoria(&laberinto);
    return 1;
}
int moverJugador(tLaberinto *lab, tEstadoJugador *estado, char direccion) {
    int nuevoX = lab->jugador.x, nuevoY = lab->jugador.y;
    char elemento;

    switch (direccion) {
        case 'w': nuevoX--; break;
        case 's': nuevoX++; break;
        case 'a': nuevoY--; break;
        case 'd': nuevoY++; break;
        default:
            printf("Movimiento no valido\n");
            getch();
            return 0;
    }

    // Verificar límites
    if (nuevoX < 0 || nuevoX >= lab->filas || nuevoY < 0 || nuevoY >= lab->columnas) {
        printf("Movimiento fuera de limites\n");
        getch();
        return 0;
    }

    // Verificar colisión con pared
    if (lab->tablero[nuevoX][nuevoY] == '#') {
        printf("¡Choque con pared!\n");
        getch();
        return 0;
    }

    // Mover jugador
    lab->jugador.x = nuevoX;
    lab->jugador.y = nuevoY;

    // Verificar colisión con elementos
    elemento = lab->tablero[nuevoX][nuevoY];

    switch(elemento){
    case 'P':
        estado->puntos += 10;
        estado->premiosCapturados++;
        printf("¡Premio capturado! +10 puntos\n");
        lab->tablero[nuevoX][nuevoY] = '.'; // Remover premio
        break;
    case 'V':
        estado->puntos += 10;
        estado->premiosCapturados++;
        printf("¡Premio capturado! +10 puntos\n");
        lab->tablero[nuevoX][nuevoY] = '.'; // Remover premio
        break;
    case 'F':
        estado->vidas--;
        printf("¡Fantasma! -1 vida. Regresas a la entrada\n");
        getch();
        lab->jugador = lab->entrada; // Regresar a entrada
        break;
    }

    return 1;
}
void moverFantasmas(tLaberinto *lab, tEstadoJugador *estado) {
    int i;

    for (i = 0; i < lab->numFantasmas; i++) {
        if (lab->fantasmas[i].x == -1) continue; // Fantasma eliminado

        if(abs(lab->jugador.x - lab->fantasmas[i].x) > abs(lab->jugador.y - lab->fantasmas[i].y)){
            if(lab->jugador.x > lab->fantasmas[i].x){
                if(lab->tablero[lab->fantasmas[i].x + 1][lab->fantasmas[i].y] == '.'){
                    lab->tablero[lab->fantasmas[i].x][lab->fantasmas[i].y] = '.';
                    lab->fantasmas[i].x += 1;
                }
                else{
                    movimientoAleatorio(lab, i);
                }
            }
            else{
                if(lab->tablero[lab->fantasmas[i].x - 1][lab->fantasmas[i].y] == '.'){
                    lab->tablero[lab->fantasmas[i].x][lab->fantasmas[i].y] = '.';
                    lab->fantasmas[i].x -= 1;
                }
                else{
                    movimientoAleatorio(lab, i);
                }
            }
        }
        else{
            if(lab->jugador.y > lab->fantasmas[i].y){
                if(lab->tablero[lab->fantasmas[i].x][lab->fantasmas[i].y + 1] == '.'){
                    lab->tablero[lab->fantasmas[i].x][lab->fantasmas[i].y] = '.';
                    lab->fantasmas[i].y += 1;
                }
                else{
                    movimientoAleatorio(lab, i);
                }
            }
            else{
                if(lab->tablero[lab->fantasmas[i].x][lab->fantasmas[i].y - 1] == '.'){
                    lab->tablero[lab->fantasmas[i].x][lab->fantasmas[i].y] = '.';
                    lab->fantasmas[i].y -= 1;
                }
                else{
                    movimientoAleatorio(lab, i);
                }
            }
        }
        lab->tablero[lab->fantasmas[i].x][lab->fantasmas[i].y] = 'F';
        if (lab->fantasmas[i].x == lab->jugador.x && lab->fantasmas[i].y == lab->jugador.y) {
            estado->vidas--;
            printf("¡Fantasma te atrapó! -1 vida. Regresas a la entrada\n");
            getch();
            lab->jugador = lab->entrada;
            lab->tablero[lab->fantasmas[i].x][lab->fantasmas[i].y] = '.'; // Eliminar fantasma
            lab->fantasmas[i].x = -1; // Marcar como eliminado
        }
    }
}

int juegoTerminado(tEstadoJugador estado, tLaberinto lab) {
    // Verificar si el jugador llegó a la salida
    return (lab.jugador.x == lab.salida.x && lab.jugador.y == lab.salida.y);
}

void liberarMemoria(tLaberinto *lab) {
    int i;
    for (i = 0; i < lab->filas; i++) {
        free(lab->tablero[i]);
    }
    free(lab->tablero);
    free(lab->fantasmas);
    free(lab->premios);
    free(lab->vidasExtra);
}
void inicializarEstado(tLaberinto *lab, tEstadoJugador *estado, tConfiguracion config) {
    // Posicionar jugador en la entrada
    lab->jugador = lab->entrada;

    // Inicializar estado del jugador
    estado->vidas = config.vidasInicio;
    estado->puntos = 0;
    estado->premiosCapturados = 0;
}
void movimientoAleatorio(tLaberinto *lab, int numFantasma){
    int dir, nuevoX, nuevoY, direcciones[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Arriba, abajo, izquierda, derecha

    // Verificar movimiento válido
    do{
        dir = rand() % 4;
        nuevoX = lab->fantasmas[numFantasma].x + direcciones[dir][0];
        nuevoY = lab->fantasmas[numFantasma].y + direcciones[dir][1];
    }while((nuevoX < 0 || nuevoX > lab->filas || nuevoY < 0 || nuevoY > lab->columnas ||
        lab->tablero[nuevoX][nuevoY] != '.'));
    // Mover fantasma
    lab->tablero[lab->fantasmas[numFantasma].x][lab->fantasmas[numFantasma].y] = '.';
    lab->fantasmas[numFantasma].x = nuevoX;
    lab->fantasmas[numFantasma].y = nuevoY;
    lab->tablero[nuevoX][nuevoY] = 'F';
}
