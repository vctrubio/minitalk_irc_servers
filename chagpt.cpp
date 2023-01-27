#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main_gpt(int argc, char* argv[]) {
    int listen_sock, client_sock, server_sock;
    struct sockaddr_in listen_addr, client_addr, server_addr;
    socklen_t client_addr_len;
    char buffer[1024];

    // Create the listening socket
    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_addr.s_addr = INADDR_ANY;
    listen_addr.sin_port = htons(8080);
    bind(listen_sock, (struct sockaddr*) &listen_addr, sizeof(listen_addr));
    listen(listen_sock, 5);

    // Wait for incoming connections
    while (1) {
        client_addr_len = sizeof(client_addr);
        client_sock = accept(listen_sock, (struct sockaddr*) &client_addr, &client_addr_len);

        // Connect to the destination server
        server_sock = socket(AF_INET, SOCK_STREAM, 0);
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr("192.168.1.100");
        server_addr.sin_port = htons(80);
        connect(server_sock, (struct sockaddr*) &server_addr, sizeof(server_addr));

        // Forward data between the client and server
        while (1) {
            int n = recv(client_sock, buffer, sizeof(buffer), 0);
            if (n <= 0) break;
            send(server_sock, buffer, n, 0);
            n = recv(server_sock, buffer, sizeof(buffer), 0);
            if (n <= 0) break;
            send(client_sock, buffer, n, 0);
        }

        // Close the client and server sockets
        close(client_sock);
        close(server_sock);
    }
    close(listen_sock);
    return 0;
}
