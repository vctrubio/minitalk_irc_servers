#include "Server.hpp"


Server::Server(int port)
{
	_port = port;
	_socket = Socket(port); //it is getting initialized twice though.
	cout << "INIT server\n";
}

Server::Server(const Server &oldServer)
{}

Server& Server::operator= (const Server &oldServer)
{
	return (*this);
}

Server::~Server()
{
	cout << "DE server\n";
}


//fts
void	Server::print()
{
	cout << "SERVER:\n" 
		<< "Port " << _port << endl 
//		<< "Socket " << _socket
		<< endl;
}

void	Server::test()
{
	_socket.print();
}
