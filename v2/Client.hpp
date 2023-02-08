#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Headers.hpp"

class Client
{

	string	username;
public:
	Client();
	Client(const Client &oldClient);
	Client& operator= (const Client &oldClient);
	~Client();
};

#endif
