#include "Headers.hpp"
#define BUFFER_SIZE 204
/*
 * socket() - returns a socket descriptor that represents the endpoint
 * bind() - after socket descriptor, bind unique name to the socket. Servers must bind a name to be accesible from the network
 * listen() - API indicates a willingness to accept connection requests
 * accept() - connect() to establish a connection, accept() to accept connection. 
 * send and receive functions - send() recv() read() write() ect.
 * close() - release any system resources acquired by the socket
 */

class Server
{
public:
    sockaddr_in addr;
    int         port;
    int         socketFd;
    int         result;
};

int main()
{
  Server  server;

  cout << "init\n";

  //socket
  server.port = PORT;
  server.socketFd = socket(AF_INET, SOCK_STREAM, 0);

  //bind
  server.addr.sin_family = AF_INET;
  server.addr.sin_port = htons(PORT);
  server.addr.sin_addr.s_addr = INADDR_ANY;
  server.result = bind(server.socketFd, reinterpret_cast<sockaddr*>(&server.addr), sizeof(server.addr));
  printf("server.result = %d\n", server.result);

  //listen
  listen(server.socketFd, 30);
  printf("Listening on port %d - address: %d\n", PORT, (int)server.addr);

  //accept
  while (true)
  {
    sockaddr_in client_address;
    socklen_t   client_address_len = sizeof(client_address);
    int         client_socket = accept(server.socketFd, reinterpret_cast<sockaddr*>(&client_address), &client_address_len);
    if (client_socket == -1) {
      std::cerr << "Failed to accept incoming connection" << std::endl;
      continue;
    }
    std::cout << "Accepted incoming connection from " << inet_ntoa(client_address.sin_addr) << ":" << ntohs(client_address.sin_port) << std::endl;
    // ft read_n_write
    char buffer[BUFFER_SIZE];
    int bytes_read;
    while ((bytes_read = read(client_socket, buffer, BUFFER_SIZE)) > 0)
    {
      int bytes_written = write(client_socket, buffer, bytes_read);
      if (bytes_written == -1) {
        std::cerr << "Failed to write data to client" << std::endl;
        break;
      }
    }
    close(client_socket);
  }

  cout << "exit\n";
  close(server.socketFd);
  return (42);
}
