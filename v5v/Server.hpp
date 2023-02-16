#ifndef SERVER_HPP
#define SERVER_HPP

#include "Headers.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Server
{
	string	_password;
	friend class Cmd;
protected:
	Client*					_requestCall; //who is making the SD
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
	Channel	*addChannel(string &topic);
	int		size(){return _clients.size();};
	void	find_cmd(vector<string> str);

};

#endif
