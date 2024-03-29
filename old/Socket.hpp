#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "Headers.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Utils.hpp"

class Client;

class Socket : public Server
{
	int			_port;
	int			_sockFd;
	string		_password;
	sockaddr_in	_addr;

	int			_activity;
	int			_clientSocket[MAX_CLIENTS];
	fd_set		_readFds;

public:
	Socket(){};
	Socket(int port, string password);
	~Socket(){};


	void 	ft_add_user(int i);
	void	init_cmd(string, int);
	void	runSocket();
	void	loop_mssg();
	void	debug();
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
