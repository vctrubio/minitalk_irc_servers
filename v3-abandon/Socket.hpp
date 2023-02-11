#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "Headers.hpp"

class Channel;

class Socket
{
	int			_sockFd;
	sockaddr_in	_addr;
	// sockaddr_in	_client;
	// socklen_t	_cSize;

	//select()
	int			_activity;
	int			_clientSocket[MAX_CLIENTS];
	int			_sd, _max_sd;
	int			_newSocket;
	fd_set		_readFds;

public:
	Socket(int port);
	~Socket();
	
	void	print(){cout << "TEST FROM SOCKET: " << endl;};
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
