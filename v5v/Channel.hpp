#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Headers.hpp"
#include "Client.hpp"

class Client;

class Channel
{
	string						_topic;
	vector<Client *>			_clients; 
    vector<Client *>::iterator	_itC;

	Client*				_admin; //later
public:
	// Channel(string topic, Client *admin); //later
	Channel(string topic);
	~Channel();

	void			addClient(Client *client);
	void			rmClient(Client *client);

	void			post(string messg, int id);
	string			topic(){return _topic;};
	vector<Client*>	clients() {return _clients;};
	int				size(){return _clients.size();};
	void			kick(Client *client) {for (_itC = _clients.begin(); _itC != _clients.end();) {if (*_itC == client){_clients.erase(_itC);} else _itC++;}};
};

std::ostream& operator<<(std::ostream& os, Channel& channel);

#endif
