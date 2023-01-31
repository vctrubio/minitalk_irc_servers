#include "Headers.hpp"

/*
 * socket() - returns a socket descriptor that represents the endpoint
 * bind() - after socket descriptor, bind unique name to the socket. Servers must bind a name to be accesible from the network
 * listen() - API indicates a willingness to accept connection requests
 * accept() - connect() to establish a connection, accept() to accept connection. 
 * send and receive functions - send() recv() read() write() ect.
 * close() - release any system resources acquired by the socket
 */

int	init_server_socket(sockaddr_in *serv_addr)
{
	int	fd;

	fd = socket(AF_UNIX, SOCK_STREAM, 0);	
	if (fd < 0)
			printf("ERROR tbd\n");
	else
	{
			serv_addr->sin_family = AF_INET;
			serv_addr->sin_port = htons(PORT);
	}
	return (fd);
}

int main(int ac, char **av)
{
	int	server_socket;
	struct sockaddr_in serv_addr;

	server_socket = init_server_socket(&serv_addr);

	if(inet_pton ( AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
	{
		printf ( "\nInvalid address ! This IP Address is not supported !\n" );
		return -1;
	}
	if ( connect(server_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
			printf ("Connection FAIL\n");
			return -1;
	}

	char *message = "A message from Client !";
	char buffer[1024] = {0};

	send(server_socket, message, strlen(message), 0);
	printf("Mssg has been sent\n");
	int reader = read(server_socket, buffer, 1024);
	printf("reading: %s\n", buffer);
	cout << "init.\n";
	
}
