#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Headers.hpp"
#include "Channel.hpp"

class Channel;

class Client
{
	string	_name; //Full name
	string	_user; //Nickname
	char	_host[60]; //hostname
	string	_mssg;
	bool	_refresh;
	int		_id;

    vector<Channel *>			_channels; //first element is the one he is connected to .front()
    vector<Channel *>::iterator	_itC;
	
public:
	Client();
	Client(string name, int id);
	~Client();
	
	string	rtnName() {return _name;};
	char*	rtnHost() {return _host;};
	
	void	putMssg(string str, Client *it,  string channel_name);
	string	rtnMssg() {_refresh = false; return _mssg;};
	void	voidMssg() {_refresh = false; cout << _mssg;};
	void	refresh(){_refresh = false;};
	string	prompt();
	
	bool	status() {return _refresh;};
	int		id() {return _id;};
	string	name(){return _name;};
	string	user(){return _user;};
	string	host(){return _host;};
	string	mssg(){return _mssg;};
	void	subscribe(Channel *channel);
	void	desubscribe(Channel *channel);

	void	setName(string str);
	void	setUser(string str);
	void	setFront(Channel* ptr);

	vector<Channel *>	channels(){return _channels;};
	Channel*			rtnChannel(string topic);
	bool				hasChannel() { return _channels.empty() ? false : true;};
	bool				hasChannel(Channel *channel) {for(_itC = _channels.begin(); _itC != _channels.end(); _itC++) {if(*_itC == channel) return true;} return false;};
};
//maybe make friends with Channel to be able to add channel;



std::ostream& operator<<(std::ostream& os, Client& client);

#endif
