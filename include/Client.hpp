#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Headers.hpp"

class	Client
{
	int		_fd;
	string	_nickname;
	string	_username;
public:
	Client(){};
	~Client(){};

	int		rtnFd() {return (_fd);};
	//int	&getFd() {return (_fd)};

	string	&rtnNick() {return (_nickname);};
	string	&rtnUser() {return (_username);}; 




};


#endif
