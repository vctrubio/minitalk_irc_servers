#include "Server.hpp"


Server::Server(int port) :_port(port), _socket(port)
{
	cout << "INIT server\n";
}

Server::~Server()
{
	cout << "DE server\n";
}
