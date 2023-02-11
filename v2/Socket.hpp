#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "Headers.hpp"
#include "Server.hpp"

class Socket : public Server
{
	int			_port;
	int			_sockFd;

	sockaddr_in	_addr;
	// sockaddr_in	_client;
	// socklen_t	_cSize;

	int			_activity;
	int			_clientSocket[MAX_CLIENTS];
	int			_sd, _max_sd;
	int			_newSocket;
	fd_set		_readFds;


public:
	Socket();
	Socket(int port, string password);
	~Socket();
	
	void	runSocket();
};


//example of expection

class ExError : public std::exception
{
	public:
		virtual const char *what() const throw()
		{
			return "Error: ExampleError";
		}
};


#endif
