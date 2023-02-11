#ifndef SERVER_HPP
#define SERVER_HPP

#include "Headers.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Server
{
	string	_password;
	
protected:
	map<int, Client *>		_clients;
	typedef map<int, Client *>::iterator itr_clients;

	vector<Channel *>		_channels;
	typedef vector<Channel *>::iterator itr_channels;
public:
	Server(){};
	Server(string password);
	~Server();

	void	addClient(int i, string name);
	void	removeClient(int i);
	void	printClients();
	Client	*getClient(int key);
};

#endif
