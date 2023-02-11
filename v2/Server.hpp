#ifndef SERVER_HPP
#define SERVER_HPP

#include "Headers.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Server
{
	string	_password;
	map<int, Client *>		_clients;
	typedef map<int, Client *>::iterator itr_clients;

	vector<Channel *>		_channels;
	typedef vector<Channel *>::iterator itr_channels;
public:
	Server(){};
	Server(string password);
	~Server();
};

#endif
