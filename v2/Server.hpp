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

public:
	Server(int port);
	Server(const Server &oldServer);
	Server& operator= (const Server &oldServer);
	~Server();

//
	void	print();
	void	test();
	void	runSocket() {_socket.runSocket();};
	void	addChannel(Channel *channel) {_socket.setChannel(channel);};  
};

#endif
