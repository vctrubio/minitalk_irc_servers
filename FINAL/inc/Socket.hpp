#ifndef SOCKET_HPP
# define SOCKET_HPP

# include "Headers.hpp"

class Socket
{
	int			_port;
	int			_sockFd;
	int			_activity;
	int			_clientSocket[MAX_CLIENTS];
	fd_set		_readFds;
	sockaddr_in	_addr;
	std::string	_password;
	std::string	_creationTime;
	Server		*_server;
	bool		_bot;


public:
	Socket(int port, std::string password);
	~Socket();
	void	purge();
	void	init();
	bool 	addUser(int fd);
	void	init_cmd(std::string buffer, int sd);
	void	run();
	bool 	incomingConnection(const int &addrlen, int &tmp_socket);
	bool	parseClientsBuffer(const int &addrlen, int &sd);
	void	welcomeMessage(Client *user);
	void	getCreationTime();
};

#endif
