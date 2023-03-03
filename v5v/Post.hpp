#ifndef POST_HPP
#define POST_HPP

#include "Headers.hpp"
#include "Client.hpp"

class Client;
class Channel;

class Post
{
	Client*		_client;
	string 		_mssg;
	Channel*	_channel;
	post		_type;
	time_t		_time;
	//timestamp
public:
	Post(Client *client, Channel* channel, string mssg, post action, time_t time);
	Post(Client *client, string mssg, post action); //just por heaven sake.
	// Post(Client *client, post action){};
	string 	mssg(){return _mssg;};
	~Post(){};
};

#endif
