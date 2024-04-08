#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 7777
#define SERVER_ADDR "127.0.0.1"

int main(){
    int sock;
    struct sockaddr_in server_address;
    char buffer[1024] = {0};

    // Crear un socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("Error creando el toyo e socket\n");
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

    return 0;
}