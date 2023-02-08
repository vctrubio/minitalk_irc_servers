#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "Headers.hpp"

class Socket
{
	int			_port;
	int			_sockFd;
	
	sockaddr_in	_addr;
	sockaddr_in	_client;
	socklen_t	_cSize;

	//select()
	int			_activity;
	int			_clientSocket[MAX_CLIENTS];
	int			_sd, _max_sd, _valRead;
	int			_newSocket;
	fd_set		_readfds;

public:
	Socket();
	Socket(int port) :_port(port){cout << "Socket on " << port << endl;};
	Socket(const Socket &oldSocket);
	Socket& operator= (const Socket &oldSocket);
	~Socket();
	
	void	print(){cout << "SOCKET: " << _port << endl;};
	void	initAddr();
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
