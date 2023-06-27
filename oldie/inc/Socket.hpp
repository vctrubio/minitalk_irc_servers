#ifndef SOCKET_HPP
# define SOCKET_HPP

# include "Headers.hpp"
# include "Server.hpp"
# include "Channel.hpp"
# include "Utils.hpp"
# include "Commands.hpp"

class Client;
class Log;

Log  *getLog(void);

class Socket
{
	int			_port;
	int			_sockFd;
	int			_activity;
	int			_clientSocket[MAX_CLIENTS];
	fd_set		_readFds;
	sockaddr_in	_addr;
	std::string	_password;
	Server		*_server;


public:
	Socket(int port, std::string password) : _port(port), _password(password) { _server = new Server(password); };
	~Socket() { purge(); };
	vector::client	getClients() const { return _server->getClients(); }
	vector::channel	getChannels() const { return _server->getChannels(); }
	void	purge() { _server->purge(); delete _server; }
	void	init();
	bool 	addUser(int fd);
	void	init_cmd(std::string buffer, int sd);
	void	run();
	bool 	incomingConnection(const int &addrlen, int &tmp_socket);
	bool	parseClientsBuffer(const int &addrlen, int &sd);
};


//example of expection
class ExError : public std::exception
{
	public:
		virtual const char *what() const throw()
		{
			return "Error: ExampleError";
		}
};

# include "Log.hpp"


#endif
