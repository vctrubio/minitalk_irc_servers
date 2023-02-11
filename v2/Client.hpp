#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Headers.hpp"

class Channel;

class Client
{
	string	_name;
	string	_mssg;
	bool	_refresh;

    std::list<Channel *>              _channels;

public:
	Client();
	Client(string name): _name(name), _refresh(false){};
	Client(const Client &oldClient);
	Client& operator= (const Client &oldClient);
	~Client();
	
	string	rtnName() {return _name;};
	void	putMssg(string str) {_mssg = str; _refresh = true;};
	string	rtnMssg() {return _mssg;};
	void	refresh(){_refresh = false;};
};

#endif
