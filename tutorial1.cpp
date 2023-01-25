#include <string.h>
#include "server.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>


/* STEPS for TCP : https://www.youtube.com/watch?v=cNdlrbZSkyQ&list=PLUJCSGGiox1Q-QvHfMMydtoyEa1IEzeLe&index=2
 * create a socket 
 * bind a socket to a IP / port
 * mark the socket for listening in
 * accept a call
 * close the listening socket
 * while receiving - display message, echo message
 * close socket
 */

//videotutorial not working tho. just the general idea i would say()
int main2()
{
	std::cout << "init.\n";
	
	int	listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening <= 0)
	{
			std::cout << "listening error\n";
			return (2);
	}
	
	sockaddr_in	hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000); //htons for converting, not necessary i dont think
	inet_pton(AF_INET, "0.0.0.0", (sockaddr*)&hint.sin_addr);

	//a socket is a number we use internally to represent connection
	if (bind(listening, (sockaddr *)&hint, sizeof(hint) == -1))
	{
			std::cerr << "Cannot bind to IP Port\n";
			return (2);
	}

	if (listen(listening, SOMAXCONN) == -1)
	{
			std::cerr << "Cannot listening\n";
			return (2);
	}

	sockaddr_in	client;
	socklen_t	clientsize = sizeof(client);
	char		host[NI_MAXHOST];
	char		svc[NI_MAXSERV];
	//accept an incomming connection
	int			clientsocket = accept(listening, (sockaddr*)&client, &clientsize);

	if (clientsocket == -1)
	{
			std::cerr << "Cannot connect client\n";
			return (2);
	}
	close(listening);
	memset(host, 0, NI_MAXHOST);
	memset(svc, 0, NI_MAXHOST);

	int result = getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);
	if (result)
	{
			std::cout << "HOST connected on " << std::endl;
	}
	else
	{
			inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
			std::cout << "HoST connected on " << ntohs(client.sin_port) << std::endl;
	}


	char buffer[4096];
	while (1)
	{
		memset(buffer, 0, 4096);
		int bytesRecv = recv(clientsocket, buffer, 4096, 0);
		if (bytesRecv == -1)
		{
				std::cerr << "Connection Issue Error\n";
				break ;
		}
		else if (bytesRecv == 0)
		{
				std::cout << "Connection Client Disconected\n";
				break ;
		}
		std::cout << "RECIEVED: " << std::endl;
		send(clientsocket, buffer, bytesRecv + 1, 0);
	}
	close(clientsocket);
	std::cout << "exit.\n";

	return (1);

}

