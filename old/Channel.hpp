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
	vector<Post *>				_history;
    vector<Post *>::iterator	_itP;
	vector<Client *>			_kick;
	
	vector<Client *>			_admins;
public:
	Channel(string topic, Client *client);
	~Channel();
	void csv(void);

	vector<Post*>	history(){return _history;};
	void			sendHistory(Client *client);
	Client			*getClient(int key);
	void			addClient(Client *client);
	void			addAdmin(Client *client);
	void			rmClient(Client *client);
	void			kickClient(Client *client);
	void			trigger_mssg(Channel *channel, Client *client, enum post type);
	void			post(string messg, int id);
	string			topic(){return _topic;};
	vector<Client*>	clients() {return _clients;};
	int				size(){return _clients.size();};
	void			kick(Client *client) {for (_itC = _clients.begin(); _itC != _clients.end();) {if (*_itC == client){_kick.push_back(client); _clients.erase(_itC);} else _itC++;}};
	bool			rtnAdmins(Client *client);
};

std::ostream& operator<<(std::ostream& os, Channel& channel);

#endif
