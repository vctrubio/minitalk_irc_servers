#ifndef SERVER_HPP
#define SERVER_HPP

#include "Headers.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Server
{
	string	_password;
	
protected:
	vector<Client *>		_clients;
	typedef vector<Client *>::iterator itr_clients;

	vector<Channel *>		_channels;
	typedef vector<Channel *>::iterator itr_channels;
public:
	Server(){};
	Server(string password);
	~Server();

	Client	*getClient(int key);
	void	addClient(Client *);
	void	removeClient(Client *client);
	void	printClients();
	void	printChannels();
	
	void	rmChannel(Channel *channel);
	void	addChannel(Channel *channel);
};

#endif
