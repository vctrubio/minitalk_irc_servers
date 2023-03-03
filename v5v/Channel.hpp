#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Headers.hpp"
#include "Client.hpp"
#include "Post.hpp"

class Client;
class Post;

class Channel
{
	string						_topic;
	vector<Client *>			_clients; 
    vector<Client *>::iterator	_itC;

	Client*				_admin; //later
	vector<Post *>		_history;
public:
	// Channel(string topic, Client *admin); //later
	Channel(string topic);
	~Channel();

	vector<Post*>	history; //to add, display, messages, now posting directly here and not user;	
	Client			*getClient(int key);


	void			addClient(Client *client);
	void			rmClient(Client *client);
	void			trigger_mssg(Channel *channel, Client *client, enum post type);
	void			post(string messg, int id);
	string			topic(){return _topic;};
	vector<Client*>	clients() {return _clients;};
	int				size(){return _clients.size();};
	void			kick(Client *client) {for (_itC = _clients.begin(); _itC != _clients.end();) {if (*_itC == client){_clients.erase(_itC);} else _itC++;}};
};

std::ostream& operator<<(std::ostream& os, Channel& channel);

#endif
