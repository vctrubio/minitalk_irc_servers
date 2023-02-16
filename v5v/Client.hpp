#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Headers.hpp"

class Channel;

class Client
{
	string	_name; //Full name
	string	_user; //Nickname
	char	_host[60]; //hostname
	string	_mssg;
	bool	_refresh;
	int		_id;

    list<Channel *>	_channels;
	
public:
	Client();
	Client(string name, int id);
	~Client();
	
	string	rtnName() {return _name;};
	
	void	putMssg(string str) {_mssg = str; _refresh = true;};
	string	rtnMssg() {_refresh = false; return _mssg;};
	void	voidMssg() {_refresh = false; cout << _mssg;};
	void	refresh(){_refresh = false;};
	
	bool	status() {return _refresh;};
	int		id() {return _id;};
	string	name(){return _name;};
	string	user(){return _user;};
	string	host(){return _host;};
	string	mssg(){return _mssg;};
	void	subscribe(Channel *channel) {_channels.push_front(channel);};

	list<Channel *>	 channels(){return _channels;};
	bool			hasChannel() { return _channels.empty() ? false : true;};
	void			addChannel(Channel *channel);

};

std::ostream& operator<<(std::ostream& os, Client& client);

#endif
