#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Headers.hpp"
#include "Client.hpp"

//create a channel, that has many users and shows mssg between users


class Channel
{
	int				_port;

	list<Client *> 	_clients;
	string			_name;
	string			_mssg;
public:
	Channel(int port, string name);
	Channel(string name): _name(name){};
	Channel(const Channel &oldChannel);
	Channel& operator= (const Channel &oldChannel);
	~Channel();

	list<Client*>	rtnList(){return _clients;}; 
	void			join(Client *client) {_clients.push_front(client);};
	void			putMssg(Client &client);
	string			rtnMssg() {return _mssg;};
	string			rtnName() {return _name;};
	void			sendMssg();
}; 


#endif
