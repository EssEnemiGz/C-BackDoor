#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 7777 
#define MAX_CONNECTIONS 1

int command_mode(socket, server_socket){
    puts("You're in command mode");

    while (1){
        char input[128] = {0};
        char buff[1024] = {0};
        printf("\n$ ");
        fgets(input, 128, stdin);
        if (!strcmp(input, "exit")) {
            close(socket);
            close(server_socket);
            puts("Desconexión exitosa");
            break;
        }
        send(socket, input, sizeof(input), 0);

        read(socket, buff, 1024);
        if (!strcmp(buff, "error")){
            printf("ERROR: %s", buff);
            break;
        }


        if (!strcmp(buff, "Server status: listen")){
            while (1){
                if (!strcmp(buff, "Server status: stop-listen")) {
                    break;
                }
                for (int i = 0; i < 1024; i++) buff[i] = 0;
                read(socket, buff, 1024);
                printf("%s", buff);
            }
        }
    }
}

int main(){
    int client_socket, server_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_length;
    char *welldone = "Connection!";

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1){
        perror("\033[33 Error al crear socket del servidor \033[0m\n");
        exit(EXIT_FAILURE);
    }

    // Configurando direccion del servidor
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1){
        perror("\033[33 Error al vincular el socket al puerto \033[0m\n");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, MAX_CONNECTIONS) == -1){
        perror("\033[33 Error al escuchar conexiones entrantes \033[0m\n");
        exit(EXIT_FAILURE);
    }

    printf("Servidor lanzado en 0.0.0.0, 7777...\n");

    // Aceptar conexion entrante
    client_address_length = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_length);
    if (client_socket == -1){
        perror("\033[33 Error al crear socket del cliente \033[0m\n");
        exit(EXIT_FAILURE);
    }

    // Send msg
    send(client_socket, welldone, strlen(welldone), 0);
    printf("Mensaje de bienvenida enviado al cliente.\n");

    // Default mode
    command_mode(client_socket, server_socket);

    // Cerrar sockets
    close(server_socket);
    close(client_socket);
    
    return 0;
}