// Main server source code.
#include "utils.h"


/*
    Creates new socket for the server and configures its port, ip address
    and protocol (IPv4).
*/
void configureServer(int *network_socket, struct sockaddr_in *server_config, int port) {
    // Create socket
    *network_socket = socket(AF_INET, SOCK_STREAM, 0);
    // Configure server
    server_config->sin_family = AF_INET;
    server_config->sin_port = htons(port);
    server_config->sin_addr.s_addr = INADDR_ANY; // On localhost
}


/*
    Binds the server to pre-configured socket and listens to the socket.
*/
void startServer(int network_socket, struct sockaddr_in server_config) {
    // Bind socket to server
    bind(network_socket, (struct sockaddr *) &server_config, sizeof(server_config));
    // Listen on the socket
    listen(network_socket, 50);
}


int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_config;
    pthread_t thread_id;
    struct sockaddr_storage server_storage;
    socklen_t addr_size = sizeof(server_storage);

    configureServer(&server_socket, &server_config, 1100);
    startServer(server_socket, server_config);

    printf("Up and running!\n");

    // Process all connections to the socket
    while(1) {
        client_socket = accept(server_socket, (struct sockaddr *) &server_storage, &addr_size);
        if( pthread_create(&thread_id, NULL, handleClient, &client_socket) != 0 ) printf("Failed to create thread!\n");
        pthread_detach(thread_id);
    }

    close(server_socket);
    return 0;
}
