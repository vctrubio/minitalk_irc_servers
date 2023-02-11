#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Headers.hpp"

class Client;

class Channel
{
	string				_topic;
	vector<Client *>	_clients;
	Client*				_admin;
public:
	Channel(string topic);
	Channel& operator= (const Channel &oldChannel);
	~Channel();
};

#endif
