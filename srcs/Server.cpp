#include "../include/Server.hpp"

void	init_sockaddr(sockaddr_in *sockaddr)
{
	sockaddr->sin_family = AF_INET;	
	sockaddr->sin_addr.s_addr = INADDR_ANY;
	sockaddr->sin_port = htons(PORT);
}

int		init_socket(int sockfd)
{
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		cout << "Failed to create socket: ERROR" << ENDC << endl; 
		exit(EXIT_FAILURE);
	}
	return (sockfd);
}

bool	bind_n_listen_socket(int sockfd, sockaddr_in sockaddr)
{
	if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
	{
		cout << RED << "Failed to bind to port " << GREEN << PORT << RED << " RIP. ERROR: " << ENDC << endl;
		exit(EXIT_FAILURE);
	}

	if (listen(sockfd, MAX_CLIENTS) < 0)
	{
		cout << RED << "Failed to listen. ERROR: " << ENDC << endl;
		exit(EXIT_FAILURE);
	}
	return (true);
}

/*
This checks/listens for a connection, in a queu tho- if A connects before B, but B sends a mssg first, it will wait for A to send.
It also closes the connection, inside the loop, as it should? but that means u have to run telnet localhost PORT again ...
*/

void	while_socket_listen(int sockfd,sockaddr_in sockaddr)
{
	char		buffer[BUFF_SIZE];
	long int	bytesRead;
	int 		addrlen;
	int 		connection;

	string roger = "Gotcha mssg.\n";
	bzero(buffer, BUFF_SIZE);

	while(true)
	{
		cout << GREEN << "Listening..." << ENDC << endl;
		addrlen = sizeof(sockaddr);
		connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
		if (connection < 0)
		{
				cout << RED << "Failed to grab connection" << ENDC << endl;
				exit(EXIT_FAILURE);
		}
		cout << BLUE << "Connection established.\n" << "Ready to read.\n";

		bytesRead = read(connection, buffer, BUFF_SIZE);
		if (bytesRead == -1)
		{
			cout << RED << "Failed to read fd. ERROR: " << ENDC << endl;
			exit(EXIT_FAILURE);
		}
		else
			cout << GREEN << "bytesRead Completee\n" ENDC;
		buffer[BUFF_SIZE - 1] = 0;
		cout << "Message: " << buffer;
		
		send(connection, roger.c_str(), roger.size(), 0);
		bzero(buffer, BUFF_SIZE);
		close(connection);
	}
}

int initServer()
{
	int			sockfd;
	sockaddr_in	sockaddr;

	sockfd = init_socket(sockfd);
	init_sockaddr(&sockaddr);
	if (bind_n_listen_socket(sockfd, sockaddr))
	{
		std::cout << BLUE << "The server is up and runnig" << std::endl;
		std::cout << GREEN << "Server IP: " << sockaddr.sin_addr.s_addr << std::endl;
		std::cout << BLUE << "Listening on port " << GREEN << PORT << ENDC << std::endl;
	}
	while_socket_listen(sockfd, sockaddr);
	close(sockfd);
	return (1);
}

void	classInitServer()
{
	Server	server;

	cout << "classINITserver\n";
	

}
