#include "serverlib.h"

///Funciones auxiliares
void login(char * text, tArbol *iUsuario, FILE *fUsuario){
    char usuario[16], contrasenia[16];
    int aux, intentos = 5;
    if(sscanf(text, "%[^|]|%s",usuario, contrasenia)!=2){
        strcpy(text, "E");/// Error
        return;
    }
    /*  Usuario no encontrado = -1
        Usuario encontrado, contrasenia incorrecta  = 0
        Usuario encontrado, contrasenia correcta = 1
    */
    aux = buscarUsuario(iUsuario, fUsuario, usuario, contrasenia);
    switch (aux) {
        case -1:
            while(darDeAlta(usuario, contrasenia, iUsuario,fUsuario) == 0 && intentos > 0){
                intentos--;
            }
            strcpy(text, "R");
            break;
        case 0:
            strcpy(text, "F");
            break;
        case 1:
            strcpy(text, "I");
    }
}
void getRank(char * text, tArbol *p, FILE *pf){
    if(!*p){
        sprintf(text, "No hay partidas registradas\n");
        return;
    }

    __getRank(text, p, pf);
}
void __getRank(char * text, tArbol *p, FILE *pf){
    tIndicePartida idx;
    tPartida part;
    char linea[50];
    if(!*p) return;

    __getRank(text, &(*p)->izq, pf);

    verNodo(p, &idx, sizeof(tIndicePartida));
    fseek(pf, idx.offset, SEEK_SET);
    fread(&part, sizeof(tPartida), 1, pf);
    sprintf(linea, "%8d|%-16s|%8d|%8d\n", part.id, part.usuario, part.puntaje, part.movimientos);
    strcat(text, linea);

    __getRank(text, &(*p)->der, pf);
}
void getStats(char * text, tArbol *p, FILE *pf){
    tUsuario usr;
    buscarEnArbolIndice(p, &usr, text, pf, leerDatosArchivoUsuarioConIdx, compararIndUsuClave);
    sprintf(text, "Usuario: %s, ID: %d, Cantidad de partidas jugadas: %d, Cantidad de Puntos: %d",
            usr.usuario, usr.id, usr.cantPartidas, usr.puntosTotales);
}
void postGame(char * text, tArbol *pArbolIdxPart, FILE *fRank, tArbol *pArbolIdxUsua, FILE *fUsuario){
    tPartida part, aux;
    tIndicePartida menorIdx, nueIdx;
    tUsuario usr;
    tIndiceUsuarioNombre idxUsu;
    sscanf(text, "%[^|]|%d|%d", part.usuario, &part.puntaje, &part.movimientos);

    fseek(fRank, -(long int)sizeof(tPartida), SEEK_END);
    ///Para la primer partida
    if(fread(&aux, sizeof(tPartida), 1, fRank) < sizeof(tPartida))
        part.id = 0;
    else
        part.id = aux.id + 1;

    fseek(fRank, 0L, SEEK_END);
    fwrite(&part, sizeof(tPartida), 1, fRank);

    ///Me fijo si la partida actual es mayor a la menor partida
    if(!verMenorNodo(pArbolIdxPart, &menorIdx, sizeof(tIndicePartida))){
        nueIdx.id = part.id;
        nueIdx.puntaje = part.puntaje;
        nueIdx.offset = ftell(fRank) - sizeof(tPartida);
        insertarEnArbolBRec(pArbolIdxPart, &nueIdx, sizeof(tIndicePartida), compararIndPar);
    }
    else {
        if(menorIdx.puntaje < part.puntaje){
            nueIdx.id = part.id;
            nueIdx.puntaje = part.puntaje;
            nueIdx.offset = ftell(fRank) - sizeof(tPartida);
            insertarEnArbolBRec(pArbolIdxPart, &nueIdx, sizeof(tIndicePartida), compararIndPar);
            ///El ranking muestra las 10 partidas con mayores puntajes
            if(contarNodos(pArbolIdxPart) > 10){
                eliminarNodo(pArbolIdxPart, &menorIdx.puntaje, &menorIdx, sizeof(tIndicePartida), compararIndPar);
            }
        }
    }
    ///Actualizo el archivo de usuarios.dat
    if(!buscarNodo2(pArbolIdxUsua, &idxUsu, part.usuario, sizeof(tIndiceUsuarioNombre), compararIndUsuClave)){
        ///Por si se cargan datos guardados en local y el usuario no existia.
        darDeAlta(part.usuario, "12345", pArbolIdxUsua,fUsuario);
        buscarNodo2(pArbolIdxUsua, &idxUsu, part.usuario, sizeof(tIndiceUsuarioNombre), compararIndUsuClave);
    }
    fseek(fUsuario, idxUsu.offset, SEEK_SET);
    fread(&usr, sizeof(tUsuario), 1, fUsuario);
    usr.cantPartidas++;
    usr.puntosTotales += part.puntaje;
    fseek(fUsuario, -(long int)sizeof(tUsuario), SEEK_CUR);
    fwrite(&usr, sizeof(tUsuario), 1, fUsuario);
}

// Implementacin de funciones pblicas
int initWinsock()
{
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2, 2), &wsa);
}

SOCKET createServerSocket()
{
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) return INVALID_SOCKET;

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
    {
        closesocket(s);
        return INVALID_SOCKET;
    }

    if (listen(s, 1) == SOCKET_ERROR)
    {
        closesocket(s);
        return INVALID_SOCKET;
    }

    return s;
}

void procesarRequest(tArbol *indiceUsuario, tArbol *indiceRanked, const char *request, char *response, FILE *fRank, FILE *fUsuarios)
{
    char operation[16], text[BUFFER_SIZE];
    sscanf(request, "%5s %s", operation, text);

    if (strcmp(operation, "LOGIN") == 0)
    {
        login(text, indiceUsuario, fUsuarios);
        sprintf(response, "%s", text);
    }
    else if (strcmp(operation, "RANKS") == 0)
    {
        strcpy(text, "");
        getRank(text, indiceRanked, fRank);
        sprintf(response, "%s", text);
    }
    else if (strcmp(operation, "STATS") == 0)
    {
        getStats(text, indiceUsuario, fUsuarios);
        sprintf(response, "%s", text);
    }
    else if (strcmp(operation, "POST") == 0)
    {
        postGame(text, indiceRanked, fRank, indiceUsuario, fUsuarios);
        sprintf(response,"Posteado con exito");
    }
    else if (strcmp(operation, "FINAL") == 0)
    {
        strcpy(response, "EXIT");
    }
    else
    {
        strcpy(response, "Operacion no valida");
    }
}

void runServer(tArbol *iUsuario, tArbol *iRanked)
{
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    int bytes_received;

    struct sockaddr_in client_addr;
    int client_addr_size;
    int err, cerrar = 0;
    char peticion[BUFFER_SIZE];
    tArbol indiceUsuario, indiceRanked;
    tCola cola;
    FILE *fUsuarioIdx, *fRankedIdx, *fRankedDat, *fUsuarioDat;

    ///Se crean los indices
    fUsuarioIdx = fopen("usuariosNombre.idx","a+b");
    if(!fUsuarioIdx)
        return;

    fRankedIdx = fopen("partidas.idx","a+b");
    if(!fRankedIdx){
        fclose(fUsuarioIdx);
        return;
    }

    crearArbolB(&indiceRanked);
    crearArbolB(&indiceUsuario);

    cargarArbolDesdeArchivoOrdenado(&indiceUsuario,fUsuarioIdx,sizeof(tIndiceUsuarioNombre),leerDatosIdxUsuario);
    cargarArbolDesdeArchivoOrdenado(&indiceRanked, fRankedIdx, sizeof(tIndicePartida), leerDatosIdxPartida);

    ///Archivos con los datos
    fUsuarioDat = fopen("usuarios.dat", "a+b");
    if(!fUsuarioDat){
        fclose(fUsuarioIdx);
        fclose(fRankedIdx);
        vaciarArbol(&indiceRanked);
        vaciarArbol(&indiceUsuario);
        return;
    }

    fRankedDat = fopen("partidas.dat", "a+b");
    if(!fRankedDat){
        fclose(fUsuarioIdx);
        fclose(fRankedIdx);
        fclose(fUsuarioDat);
        vaciarArbol(&indiceRanked);
        vaciarArbol(&indiceUsuario);
        return;
    }

    if (initWinsock() != 0)
    {
        printf("Error al inicializar Winsock\n");
        return;
    }

    SOCKET server_socket = createServerSocket();
    if (server_socket == INVALID_SOCKET)
    {
        printf("Error al crear socket del servidor\n");
        WSACleanup();
        return;
    }

    printf("Servidor escuchando en puerto %d...\n", PORT);
    crearCola(&cola);

    while (!cerrar)
    {
        client_addr_size = sizeof(client_addr);
        SOCKET client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size);

        if (client_socket == INVALID_SOCKET)
        {
            printf("Error en accept(): %d\n", WSAGetLastError());
            continue; // Sigue escuchando aunque un accept falle
        }

        printf("Cliente conectado.\n");


        while (1)
        {
            bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);

            if (bytes_received > 0)
            {
                buffer[bytes_received] = '\0';
                printf("[RX] Recibido: %s\n", buffer);
                ponerEnCola(&cola, buffer, BUFFER_SIZE);
            }
            else if (bytes_received == 0)
            {
                printf("Cliente desconectado.\n");
                break; // Sale del bucle de cliente, vuelve a esperar otro
            }
            else
            {
                err = WSAGetLastError();
                if (err != WSAEWOULDBLOCK)
                {
                    printf("Error en recv(): %d\n", err);
                    break;
                }
            }

            // Procesar mensajes pendientes en la cola
            while (!colaVacia(&cola))
            {
                sacarDeCola(&cola, peticion, BUFFER_SIZE);
                printf("[PROC] Procesando: %s\n", peticion);
                procesarRequest(&indiceUsuario, &indiceRanked, peticion, response, fRankedDat, fUsuarioDat);
                send(client_socket, response, strlen(response), 0);
                printf("[TX] Enviado: %s\n", response);
            }
            if(strcmp(response,"EXIT")==0){
                cerrar = 1;
                break;
            }///Sale del bucle y Guarda todo
            Sleep(100);
        }
        closesocket(client_socket);
        printf("Esperando nuevo cliente...\n");
    }

    ///Se guardan los nuevos indices
    cargarArchivoDesdeArbol(&indiceRanked, fRankedIdx);
    cargarArchivoDesdeArbol(&indiceUsuario, fUsuarioIdx);
    avisarSeCerroBien();

    ///Se libera toda la memoria
    vaciarArbol(&indiceRanked);
    vaciarArbol(&indiceUsuario);
    vaciarCola(&cola);
    ///Se cierra el server
    closesocket(server_socket);
    WSACleanup();
    ///Se cierran los archivos
    fclose(fUsuarioIdx);
    fclose(fRankedIdx);
    fclose(fRankedDat);
    fclose(fUsuarioDat);
}

void avisarSeCerroBien(){
    FILE * pf = fopen("state.txt","wt");
    if(pf == NULL)
        return;
    fprintf(pf, "1");
    fclose(pf);
}
int chequearEstado(){
    FILE *pf = fopen("state.txt", "rt");
    if(!pf){
        return 0;
    }
    ///Se pudo abrir, existe. Se borra el archivo para despues generarlo de nuevo
    fclose(pf);
    remove("state.txt");
    return 1;
}
