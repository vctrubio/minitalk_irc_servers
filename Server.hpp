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
	Client*					_requestCall;
	vector<Client *>		_clients;
	typedef vector<Client *>::iterator itr_clients;

	vector<Channel *>		_channels;
	typedef vector<Channel *>::iterator itr_channels;
public:
	Server(){};
	Server(string password);
	~Server(){};

	Client	*getClient(int key); //HOW TO MAKE THIS GLOBAL so also channel can use it
	Client	*getClient(string nick);
	void	addClient(Client *);
	void	removeClient(Client *client);
	void	printClients();
	void	printChannels();

	string	build_info(Client& client);
	void	rmChannel(Channel *channel);
	Channel	*addChannel(string &topic);
	Channel	*addChannel(string &topic, Client *client);
	int		size(){ if (!_channels.empty()) return _clients.size(); return 0;};
	void	find_cmd(vector<string> str);
	int		check_clients(string client);
	void	sendNotice(vector<Channel*> channels, string mssg);
};

#endif
