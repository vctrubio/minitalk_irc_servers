#ifndef SERVER_HPP
#define SERVER_HPP

#include "Headers.hpp"
#include "Socket.hpp"

class Server
{
	Socket	_socket;
	int		_port;
public:
    Server(){};
    
    Socket rtnSocket(){return (_socket);};
};

int 	initServer();
void	classInitServer();

#endif
