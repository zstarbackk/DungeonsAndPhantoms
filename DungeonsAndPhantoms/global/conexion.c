#include "conexion.h"

int iniciarConexion(){
    if (init_winsock() != 0) {
        return WSK_ERR;
    }

    SOCKET sock = connect_to_server(SERVER_IP, PORT);
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        return CNX_ERR;
    }
    return 1;
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
    if (send(sock, request, strlen(request), 0) < 0) {
        return -1;
    }

    int bytes_received = recv(sock, response, BUFFER_SIZE - 1, 0);
    if (bytes_received <= 0) {
        return -1;
    }

    response[bytes_received] = '\0';
    return 0;
}

void close_connection(SOCKET sock) {
    closesocket(sock);
    WSACleanup();
}
