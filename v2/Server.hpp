#ifndef SERVER_HPP
#define SERVER_HPP

#include "Headers.hpp"
#include "Socket.hpp"

class Client;
class Channel;

class Server
{
	//this can be a list or vector - Client	**clients;
	int		_port;
	Socket	_socket;

protected:
	map<int, Client *>		_clients;
	typedef map<int, Client *>::iterator itr_clients;

	vector<Channel *>		_channels;
	typedef vector<Channel *>::iterator itr_channels;

public:
	Server(int port);
	~Server();

//
	void	runSocket() {_socket.runSocket();};
	// void	addChannel(Channel *channel) {_socket.setChannel(channel);};  
};

#endif
