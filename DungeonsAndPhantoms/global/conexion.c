#include "conexion.h"

SOCKET iniciarConexion(){
    if (init_winsock() != 0) {
        return INVALID_SOCKET;
    }

    SOCKET sock = connect_to_server(SERVER_IP, PORT);
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        return INVALID_SOCKET;
    }
    return sock;
}

int init_winsock() {
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2, 2), &wsa);
}

SOCKET connect_to_server(const char *server_ip, int port) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) return INVALID_SOCKET;

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        closesocket(sock);
        return INVALID_SOCKET;
    }

    return sock;
}

int send_request(SOCKET sock, const char *request, char *response) {
    int bytes_received;
    if (send(sock, request, strlen(request), 0) < 0) {
        return 0;
    }

    bytes_received = recv(sock, response, BUFFER_SIZE - 1, 0);
    if (bytes_received <= 0) {
        return 0;
    }

    response[bytes_received] = '\0';
    return 1;
}

void close_connection(SOCKET sock) {
    closesocket(sock);
    WSACleanup();
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

int cargarResultados(SOCKET sock, const char* nomUsu, int puntaje, int cantMov){
    char req[50], res[10];
    sprintf(req, "POST %s|%d|%d", nomUsu, puntaje, cantMov);
    send_request(sock, req, res);
    if(strcmp(res, "OK") == 0){
        return 1;
    }
    else{
        return 0;
    }
}

int guardarResultadosLocal(const char* nomUsu, int puntaje, int cantMov){
    FILE *pf;
    pf = fopen("archivoLocal.txt", "at");
    if(!pf) return 0;
    fprintf(pf, "%s|%d|%d\n", nomUsu, puntaje, cantMov);
    fclose(pf);
    return 1;
}

int cargarArchivoLocal(SOCKET sock){
    char buff[20], req[50], res[10];
    FILE *pf;
    pf = fopen("archivoLocal.txt", "rt");
    if(!pf) return 0;
    while(fgets(buff, sizeof(buff), pf)){
        sacarSalto(buff)
        sprintf(req, "POST %s", buff);
        send_request(sock, req, res);
    }
    fclose(pf);
    remove("archivoLocal.txt");
    return 1;
}
