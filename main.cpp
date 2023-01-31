#include "Headers.hpp"

/*
 * socket() - returns a socket descriptor that represents the endpoint
 * bind() - after socket descriptor, bind unique name to the socket. Servers must bind a name to be accesible from the network
 * listen() - API indicates a willingness to accept connection requests
 * accept() - connect() to establish a connection, accept() to accept connection. 
 * send and receive functions - send() recv() read() write() ect.
 * close() - release any system resources acquired by the socket
 */


int main() {
  int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc == -1) {
    std::cerr << "Could not create socket" << std::endl;
    return 1;
  }

  printf("Init server\n");

  sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(8080);

  if (bind(socket_desc, (sockaddr*) &server, sizeof(server)) < 0) {
    std::cerr << "Bind failed" << std::endl;
    return 1;
  }

  listen(socket_desc, 3);

  sockaddr_in client;
  socklen_t client_len = sizeof(client);

  int new_socket = accept(socket_desc, (sockaddr*) &client, &client_len);
  if (new_socket < 0) {
    std::cerr << "Accept failed" << std::endl;
    return 1;
  }

  char* message = "Hello client, I am the server";
  send(new_socket, message, strlen(message), 0);

  close(socket_desc);
  close(new_socket);

  return 0;
}
