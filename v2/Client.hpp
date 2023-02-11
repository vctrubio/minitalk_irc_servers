#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Headers.hpp"

class Channel;

class Client
{
	string	_name;
	string	_mssg;
	bool	_refresh;

    list<Channel *>              _channels;

public:
	Client();
	Client(string name): _name(name), _refresh(false){};
	~Client();
	
	string	rtnName() {return _name;};
	
	void	putMssg(string str) {_mssg = str; _refresh = true;};
	string	rtnMssg() {_refresh = false; return _mssg;};
	void	refresh(){_refresh = false;};
	bool	status() {return _refresh;};
	list<Channel *>	 channels(){return _channels;};
};

#endif
