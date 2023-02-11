#ifndef SERVER_HPP
#define SERVER_HPP

#include "Headers.hpp"

class Client;
class Channel;

class Server
{
	string	_password;
	int		_port;
	int		_sock;
	int		_running;
	int		_addrlen;
	//what is a host used for: string	_host;  _host("127.0.0.1") AKA localhost?

	vector<pollfd>			_pollfds;
	typedef vector<pollfd>::iterator itr_poll;

	map<int, Client *>		_clients;
	typedef map<int, Client *>::iterator itr_clients;

	vector<Channel *>		_channels;
	typedef vector<Channel *>::iterator itr_channels;
	
public:
	Server();
	Server(int port, string &password);
	~Server();


	int 	newSocket();
	void	runServer();
};

#endif