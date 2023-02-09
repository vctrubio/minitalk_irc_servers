#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "Headers.hpp"

class Socket
{
	sockaddr_in	_hint;
	int			_port;
	int			_sockedFd;

	int			_clientSocket[MAX_CLIENTS];
};


#endif
