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
	char		_strTime[100];
public:
	Post(Client *client, Channel* channel, string mssg, post action, time_t time);
	Post(Client *client, Channel* channel, post action, time_t time);
	string 	mssg(){return _mssg;};
	~Post(){};

	string	time(){return string(_strTime);};
	void	print(int);
};


#endif
