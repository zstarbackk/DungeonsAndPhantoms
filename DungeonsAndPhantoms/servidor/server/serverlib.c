#include "serverlib.h"
int buscarUsuario(char * nombre, char * text){
    return 1;
}
int darDeAlta(char * usuario, char * contrasenia){
    tUsuario user;
    FILE * pf = fopen("usuarios.dat", "ab");
    if(pf==NULL){
        return 0;
    }
    strcpy(user.usuario, usuario);
    strcpy(user.contrasenia, contrasenia);
    user.puntosTotales = 0;
    user.cantPartidas = 0;
    return 1;
}
///Funciones auxiliares
void login(char * text){
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
    aux = buscarUsuario(usuario, contrasenia);
    switch (aux) {
        case -1:
            while(darDeAlta(usuario, contrasenia) == 0&& intentos >0){
                intentos++;
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
void getRank(char * text){

}
void getStats(char * text){

}
void postGame(char * text){}

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

void procesarRequest(const char *request, char *response)
{
    char operation[16], text[BUFFER_SIZE];
    sscanf(request, "%5s %s", operation, text);

    if (strcmp(operation, "LOGIN") == 0)
    {
        login(text);
        snprintf(response, BUFFER_SIZE, "%s", text);
    }
    else if (strcmp(operation, "RANKS") == 0)
    {
        getRank(text);
        snprintf(response, BUFFER_SIZE, "%s", text);
    }
    else if (strcmp(operation, "STATS") == 0)
    {
        getStats(text);
        snprintf(response, BUFFER_SIZE, "%s", text);
    }
    else if (strcmp(operation, "POSTS") == 0)
    {
        postGame(text);
        snprintf(response, BUFFER_SIZE, "%s", text);
    }
    else if (strcmp(operation, "FINAL") == 0)
    {
        strcpy(response, "EXIT");
        snprintf(response, BUFFER_SIZE, "%s", text);
    }
    else
    {
        snprintf(response, BUFFER_SIZE, "Operacion no valida");
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
    ///cargarIndiceUsario(&indiceUsuario);
    ///cargarIndiceRanked(&indiceRanked);
    tCola cola;
    int CantidadPeticionesBackup = 0;

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

    while (cerrar==0)
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
                ponerEnCola(&colaBackup, buffer, BUFFER_SIZE);
                CantidadPeticionesBackup++;
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
                procesarRequest(peticion, response, iUsuario, iRanked);
                send(client_socket, response, strlen(response), 0);
                printf("[TX] Enviado: %s\n", response);
            }
            if(strcmp(response,"EXIT")==0){
                cerrar = 0;
                break;
            }///Sale del bucle y Guarda todo
            Sleep(100);
        }
        closesocket(client_socket);
        printf("Esperando nuevo cliente...\n");
    }
    avisarSeCerroBien();
    vaciarCola(&cola);
    closesocket(server_socket);
    WSACleanup();
}

void avisarSeCerroBien(){
    FILE * pf = fopen("state.txt","wt");
    if(pf == NULL)
        return;
    fprintf(pf, "1");
    fclose(pf);
}
int chequearEstado(){
    FILE * pf = fopen("state.txt", "rt");
    if(pf==NULL){
        return 0;
    }
    ///Se pudo abrir, existe. Se borra el archivo para despues generarlo de nuevo
    fclose(pf);
    remove("state.txt");
    return 1;
}
