#ifndef SERVER_HPP
# define SERVER_HPP

# include "Headers.hpp"
# include "Commands.hpp"

class Client;
class Channel;
class Log;
Log  *getLog(void);

class Server
{
	std::string			_password;
	Client*				_requestCall;
	vector::client		_clients;
	vector::channel		_channels;

public:
	Server(std::string password) : _password(password) { }
	~Server() {};
	void	setRequestCall(Client *client) { _requestCall = client; }
	Client	*getRequestCall() const { return _requestCall; }
	const std::string		&getPassword() const { return _password; }
	vector::client	getClients() const { return _clients; }
	vector::channel	getChannels() const { return _channels; }
	void	cleanEmptyChannels();
	void	addClient(Client *client);
	void	rmClient(Client *client);
	void	addChannel(Channel *channel);
	void	rmChannel(Channel *channel);
	void	findCommand(vector::string str);
	void	purge();
	bool	isNicknameAvailable(const std::string &nickname);
	bool	isChannelNameAvailable(const std::string &name);
	Client	*getClient(int fd);
	Client	*getClient(const std::string &nickname);
	Channel	*getChannel(const std::string &name);
};

# include "Channel.hpp"
# include "Client.hpp"
# include "Log.hpp"

#endif
