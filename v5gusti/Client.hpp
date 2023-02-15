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
	Channel *_currchannel;

    list<Channel *>	_channels;
public:
	Client();
	Client(string name, int id);
	~Client();
	
	void	setName(string name) {_name = name;};
	void	setUser(string user) {_user = user;};
	string	rtnName() {return _name;};
	
	void	putMssg(string str) {_mssg = str; _refresh = true;};
	string	rtnMssg() {_refresh = false; return _mssg;};
	void	voidMssg() {_refresh = false; cout << _mssg;};
	void	refresh(){_refresh = false;};
	void	travel(Channel *channel){_currchannel = channel;};
	
	bool	status() {return _refresh;};
	int		id() {return _id;};
	string	name(){return _name;};
	string	user(){return _user;};
	string	host(){return _host;};
	string	mssg(){return _mssg;};
	void	subscribe(Channel *channel) {_channels.push_front(channel);};
	void	unsubscribe(Channel *channel) {_channels.remove(channel);};
	list<Channel *>	 channels(){return _channels;};
	Channel *	 currentChannel(){return _currchannel;};
	bool	hasChannel() { return _channels.empty() ? false : true;};
};

std::ostream& operator<<(std::ostream& os, Client& client);
template <class T, class Alloc>  bool operator== (const list<T,Alloc>& lhs, const list<T,Alloc>& rhs);

#endif
