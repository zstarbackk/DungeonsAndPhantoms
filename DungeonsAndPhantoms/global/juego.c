#include "laberinto.h"
void menuPrincipal(){
    char nomUsuario[25], buff[BUFFER_SIZE];
    int opc;
    SOCKET sock = iniciarConexion();
    if(sock != INVALID_SOCKET){
        //cargarArchivoLocal(sock);
        iniciarSesion(sock, nomUsuario);
        do{
            do{
                system("cls");
                printf("Bienvenido a Dungeons and Phantoms\n");
                printf("    1. Iniciar Partida\n    2. Ver Ranking\n    3. Ver Estadisticas Del Jugador\n 4.Salir\n>");
                scanf("%d", &opc);
                if(opc < 1 || opc > 3){
                    printf("Opcion no Valida\n");
                    system("pause");
                }
            }while(opc < 1 || opc > 4);
            switch(opc){
                case 1:
                    iniciarJuego(sock, nomUsuario);
                    break;
                case 2:
                    if(send_request(sock, "RANKS", buff)){
                        printf("Ranking de Jugadores de Dungeons And Phantoms\n");
                        printf("%s", buff);
                        system("pause");
                    }
                    else{
                        printf("No hay ranking\n");
                        system("pause");
                    }
                    break;
                case 3:
                    if(send_request(sock, "STATS", buff)){
                        printf("Estadisticas del jugador %s", nomUsuario);
                        printf("%s", buff);
                        system("pause");                    }
                    else{
                        printf("El jugador no tiene estadisticas registradas\n");
                        system("pause");
                    }
                    break;
            }

        }while(opc != 4);
    }
    else{
        printf("No se pudo generar una conexion con el servidor, los datos seran guardados en forma local\n");
        system("pause");
        printf("Ingrese el nombre de usuario\n>");
        fgets(nomUsuario, sizeof(nomUsuario) -1, stdin);
        do{
            do{
                system("cls");
                printf("Bienvenido a Dungeons and Phantoms\n");
                printf("    1. Iniciar Partida\n    2. Salir\n>");
                scanf("%d", &opc);
                if(opc != 1 && opc != 2){
                    printf("Opcion no Valida\n");
                    system("pause");
                }
            }while(opc != 1 && opc != 2);
            if(opc == 1)
                iniciarJuego(INVALID_SOCKET, nomUsuario);
        }while(opc != 2);
    }

    close_connection(sock);
}
int iniciarJuego(SOCKET sock, const char *nomUsu){
    tCola regMovJugador, movJuego;
    tConfiguracion config;
    tLaberinto laberinto;
    tEstadoJugador estado;
    ///Nombre, puntos, cant mov,
    // Bucle principal del juego
    char movimiento;
    int finJuego = 0;

    // Leer configuración
    leerConfiguracion(&config);
    crearCola(&movJuego);
    crearCola(&regMovJugador);

    estado.cantMov = 0;

    // Generar laberinto
    if(!generarLaberinto(&laberinto, config))
        return LAB_ERR;
    guardarLaberinto(laberinto, "laberinto.txt");

    // Inicializar juego
    inicializarEstado(&laberinto, &estado, config);


    while (!finJuego && estado.vidas > 0) {
        // Mover jugador
        movimiento = moverJugador(&laberinto, &estado);
        if (movimiento == 'q') {
            vaciarCola(&movJuego);
            vaciarCola(&regMovJugador);
            liberarMemoria(&laberinto);
            return 0;
        }
        // Mover fantasmas
        ponerEnCola(&regMovJugador, &movimiento, sizeof(char));
        ponerEnCola(&movJuego, &movimiento, sizeof(char));
        moverFantasmas(&laberinto, &movJuego);
        efectuarMovimientos(&laberinto, &movJuego, &estado);
        // Verificar si el juego terminó
        finJuego = juegoTerminado(estado, laberinto);
    }

    // Mostrar resultado final
    mostrarTablero(&laberinto, &estado);
    if (estado.vidas <= 0) {
        printf("\n¡PERDISTE! Te quedaste sin vidas.\n");
    } else if (finJuego) {
        printf("\n¡GANASTE! Llegaste a la salida con %d puntos.\n", estado.puntos);
    }

    printf("Puntos finales: %d\n", estado.puntos);
    printf("Premios capturados: %d\n", estado.premiosCapturados);
    printf("Movimientos realizados por el jugador: ");
    while(sacarDeCola(&regMovJugador, &movimiento, sizeof(char))){
        switch (movimiento) {
            case 'w': printf("arriba "); break;
            case 's': printf("abajo "); break;
            case 'a': printf("izquierda "); break;
            case 'd': printf("derecha "); break;
        }
    }

    if(sock != INVALID_SOCKET){
        //cargarResultados(sock, nomUsu, estado.puntos, estado.cantMov);
    }
    else{
        //guardarResultadosLocal(nomUsu, estado.puntos, estado.cantMov);
    }

    vaciarCola(&movJuego);
    vaciarCola(&regMovJugador);
    liberarMemoria(&laberinto);
    system("pause");
    return 1;
}
char moverJugador(tLaberinto *lab, tEstadoJugador *estado) {
    int nuevoX, nuevoY;
    char movimiento;
    do{
        mostrarTablero(lab, estado);
        fflush(stdin);
        printf("Movimiento: ");
        movimiento = getch();
        if (movimiento == 'q') {
            return movimiento;
        }
        nuevoX = lab->jugador.x;
        nuevoY = lab->jugador.y;
        switch (movimiento) {
            case 'w': nuevoX--; break;
            case 's': nuevoX++; break;
            case 'a': nuevoY--; break;
            case 'd': nuevoY++; break;
            default:
                printf("Movimiento no valido\n");
                ///getch();
                system("pause");
        }
        // Verificar límites
        if (nuevoX < 0 || nuevoX >= lab->filas || nuevoY < 0 || nuevoY >= lab->columnas) {
            printf("Movimiento fuera de limites\n");
            system("pause");
            system("cls");
        }
        // Verificar colisión con pared
        else if (lab->tablero[nuevoX][nuevoY] == '#') {
            printf("¡Choque con pared!\n");
            system("pause");
            system("cls");
        }

    }while(nuevoX < 0 || nuevoX >= lab->filas || nuevoY < 0 || nuevoY >= lab->columnas || lab->tablero[nuevoX][nuevoY] == '#');

    estado->cantMov++;
    return movimiento;
}
void moverFantasmas(tLaberinto *lab, tCola *movimientos) {
    int i;
    char mov;

    for (i = 0; i < lab->numFantasmas; i++) {
        if (lab->fantasmas[i].x == -1) continue; // Fantasma eliminado
        //Se fija si el fantasma esta mas cerca vertical u horizontalmente
        if(abs(lab->jugador.x - lab->fantasmas[i].x) > abs(lab->jugador.y - lab->fantasmas[i].y)){
            //Si esta mas cerca horizontalmente se fija si para la derecha o para la izquierda
            if(lab->jugador.x > lab->fantasmas[i].x){
                //Se fija si puede ir para la derecha
                if(lab->tablero[lab->fantasmas[i].x + 1][lab->fantasmas[i].y] == '.'){
                    mov = 's';
                }
                //Si no puede se mueve aleatoriamente
                else{
                    mov = movimientoAleatorio(lab, i);
                }
            }
            //Se fija si puede ir para la izquierda
            else{
                if(lab->tablero[lab->fantasmas[i].x - 1][lab->fantasmas[i].y] == '.'){
                    mov = 'w';
                }
                //Si no se puede se mueve aleatoriamente
                else{
                    mov = movimientoAleatorio(lab, i);
                }
            }
        }
        else{
            //Si esta mas cerca verticalmente se fija si para arriba o para abajo
            if(lab->jugador.y > lab->fantasmas[i].y){
                //Se fija si puede ir para arriba
                if(lab->tablero[lab->fantasmas[i].x][lab->fantasmas[i].y + 1] == '.'){
                    mov = 'd';
                }
                //Si no se mueve aleatoriamente
                else{
                    mov = movimientoAleatorio(lab, i);
                }
            }
            else{
                //Se fija si puede ir para abajo
                if(lab->tablero[lab->fantasmas[i].x][lab->fantasmas[i].y - 1] == '.'){
                    mov = 'a';
                }
                //Si no se mueve aleatoriamente
                else{
                    mov = movimientoAleatorio(lab, i);
                }
            }
        }
        ponerEnCola(movimientos, &mov, sizeof(char));
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
char movimientoAleatorio(tLaberinto *lab, int numFantasma){
    int dir, nuevoX, nuevoY, direcciones[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Arriba, abajo, izquierda, derecha
    char mov[] = {'w', 's', 'i', 'd'};

    //Verificar que el fantasma se pueda mover
    if(lab->tablero[lab->fantasmas[numFantasma].x + 1][lab->fantasmas[numFantasma].y] != '.' &&
       lab->tablero[lab->fantasmas[numFantasma].x -1 ][lab->fantasmas[numFantasma].y] != '.' &&
       lab->tablero[lab->fantasmas[numFantasma].x][lab->fantasmas[numFantasma].y + 1] != '.' &&
       lab->tablero[lab->fantasmas[numFantasma].x][lab->fantasmas[numFantasma].y - 1] != '.')
        return 'q';
    // Verificar movimiento válido
    do{
        dir = rand() % 4;
        nuevoX = lab->fantasmas[numFantasma].x + direcciones[dir][0];
        nuevoY = lab->fantasmas[numFantasma].y + direcciones[dir][1];
    }while((nuevoX < 0 || nuevoX > lab->filas || nuevoY < 0 || nuevoY > lab->columnas ||
        lab->tablero[nuevoX][nuevoY] != '.'));

    return *(mov + dir);
}

void efectuarMovimientos(tLaberinto *lab, tCola *movs, tEstadoJugador *estado){
    int i, nuevoX, nuevoY;
    char elemento, dir;
    //El primero de la cola siempre va a ser el movimiento del jugador
    sacarDeCola(movs, &dir, sizeof(char));
    // Mover jugador
    switch(dir){
        case 'w': lab->jugador.x--; break;
        case 's': lab->jugador.x++; break;
        case 'a': lab->jugador.y--; break;
        case 'd': lab->jugador.y++; break;
    }
    // Verificar colisión con elementos
    elemento = lab->tablero[lab->jugador.x][lab->jugador.y];
    switch(elemento){
    case 'P':
        estado->puntos += 10;
        estado->premiosCapturados++;
        printf("¡Premio capturado! +10 puntos\n");
        lab->tablero[lab->jugador.x][lab->jugador.y] = '.'; // Remover premio
        break;
    case 'V':
        estado->vidas++;
        printf("¡Vida capturada! +1 vida\n");
        lab->tablero[lab->jugador.x][lab->jugador.y] = '.'; // Remover premio
        break;
    case 'F':
        estado->vidas--;
        printf("¡Fantasma! -1 vida. Regresas a la entrada\n");
        getch();
        lab->jugador = lab->entrada; // Regresar a entrada
        break;
    }
    //Los siguientes van a ser los movimientos de los fantasmas en orden
    for(i=0;i<lab->numFantasmas;i++){
        if(lab->fantasmas[i].x == -1) continue; //Fantasma eliminado
        sacarDeCola(movs, &dir, sizeof(char));
        nuevoX = lab->fantasmas[i].x;
        nuevoY = lab->fantasmas[i].y;
        switch(dir){
            case 'w': nuevoX--; break;
            case 's': nuevoX++; break;
            case 'a': nuevoY--; break;
            case 'd': nuevoY++; break;
        }
        if(lab->tablero[nuevoX][nuevoY] == '.' || lab->tablero[nuevoX][nuevoY] == 'J'){
            lab->tablero[lab->fantasmas[i].x][lab->fantasmas[i].y] = '.';
            lab->fantasmas[i].x = nuevoX;
            lab->fantasmas[i].y = nuevoY;
            lab->tablero[lab->fantasmas[i].x][lab->fantasmas[i].y] = 'F';
        }

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
void iniciarSesion(SOCKET sock, char *nomUsu){
    char nom[16], contr[16], req[50], res[10];

    do{
        printf("Ingrese el nombre de usuario\n>");
        fgets(nom, sizeof(nom) -1, stdin);
        sacarSalto(nom);
        printf("Ingrese la contraseña\n>");
        fgets(contr, sizeof(nom) -1, stdin);
        sacarSalto(contr);
        sprintf(req, "LOGIN %s|%s", nom, contr);
        send_request(sock, req, res);
    }while(strcmp(res, "E") == 0 || strcmp(res, "F") == 0);

    strcpy(nomUsu, nom);

    if(strcmp(res, "R"))
        printf("Se ha registrado el usuario correctamente\n");
    else
        printf("Se ha iniciado sesion correctamente\n");
    system("pause");
}













