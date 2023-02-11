#ifndef SERVER_HPP
#define SERVER_HPP

#include "Headers.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Socket.hpp"



class Server : public Socket
{
	string	_password;
	map<int, Client *>		_clients;
	typedef map<int, Client *>::iterator itr_clients;

	vector<Channel *>		_channels;
	typedef vector<Channel *>::iterator itr_channels;
public:
	Server(int port);
	~Server();
};

#endif
