#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Headers.hpp"
#include "Client.hpp"


class Channel
{
	string				_topic;
	vector<Client *>	_clients; 
	Client*				_admin; //later
public:
	Channel(string topic);
	~Channel();

	void			addClient(Client *client);
	void			rmClient(Client *client);
	void			post(string messg, int id);
	void			postToSelf(string messg, int id);
	string			topic(){return _topic;};
	vector<Client*>		clients() {return _clients;};
	int				size(){return _clients.size();};
};

std::ostream& operator<<(std::ostream& os, Channel& channel);

#endif
