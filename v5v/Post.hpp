#ifndef POST_HPP
#define POST_HPP

#include "Headers.hpp"
#include "Client.hpp"

class Client;

class Post
{
	Client *	_client;
	string 		_mssg;
public:
	Post(Client *client, string mssg, post action);
	// Post(Client *client, post action){};
	string 	mssg(){return _mssg;};
	~Post(){};
};

#endif
