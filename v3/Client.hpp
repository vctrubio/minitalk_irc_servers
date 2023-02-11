#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Headers.hpp"

class Client
{
	int		_fd;
	int		_port; //?

	string _username;
	string _nickname;

//buffer send() recv()
	string	_sendmssg;
	string	_recvmssg;
public:
	Client();
	Client(const Client &oldClient);
	Client& operator= (const Client &oldClient);
	~Client();
};

#endif
