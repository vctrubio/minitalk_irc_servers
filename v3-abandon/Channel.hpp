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
	Channel();
	Channel(const Channel &oldChannel);
	Channel& operator= (const Channel &oldChannel);
	~Channel();
};

#endif
