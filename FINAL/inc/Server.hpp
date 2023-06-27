#ifndef SERVER_HPP
# define SERVER_HPP

# include "Headers.hpp"

class Server
{
	std::string			_password;
	Client*				_requestCall;
	vector::client		_clients;
	vector::channel		_channels;
	bool		_bot;

public:
	Server(std::string password);
	~Server() {};
	void	setRequestCall(Client *client) { _requestCall = client; }
	Client	*getRequestCall() const { return _requestCall; }
	const std::string		&getPassword() const { return _password; }
	vector::client	getClients() const { return _clients; }
	vector::channel	getChannels() const { return _channels; }
	void	setBot(bool bot);
	bool   getBot() const;
	void	cleanEmptyChannels();
	void	adjustOperators();
	void	addClient(Client *client);
	void	rmClient(Client *client);
	void	addChannel(Channel *channel);
	void	rmChannel(Channel *channel);
	void	findCommand(vector::string str);
	void	purge();
	Client	*getClient(int fd);
	Client	*getClient(const std::string &nickname);
	Channel	*getChannel(const std::string &name);
	bool	isNicknameAvailable(const std::string &nickname);
	bool	isChannelNameAvailable(const std::string &name);
	void	broadcast(const std::string &msg);
	vector::client getUsersWithoutChannels();
};

#endif
