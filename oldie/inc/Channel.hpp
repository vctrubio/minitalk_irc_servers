#ifndef CHANNEL_HPP
#define CHANNEL_HPP

# include "Headers.hpp"

class Client;
class Log;
Log  *getLog(void);

// Map of clients
// first:	client pointer
// second:	permission level -> false: client, true: operator

class Channel
{
	std::string			_topic;
	map::client			_clients;
	std::string			_password;
	unsigned int		_userLimit;
	bool				_hasPassword;
	bool				_isTopicRestricted;
	bool				_hasUserLimit;
	bool				_isInviteOnly;

public:
	Channel(std::string topic): _topic(topic) { _hasPassword = false; _isTopicRestricted = false; _hasUserLimit = false; _isInviteOnly = false; };
	~Channel() {};
	const	std::string		&getTopic() const { return _topic; }
	const std::string	&getPassword() const { return _password; }
	unsigned int		getUserLimit() const { return _userLimit; }
	bool	isInviteOnly() const { return _isInviteOnly; }
	bool	isTopicRestricted() const { return _isTopicRestricted; }
	bool	hasUserLimit() const { return _hasUserLimit; }
	bool	hasPassword() const { return _hasPassword; }
	map::client	getClients() const { return _clients; }
	void	setTopic(const std::string &topic) { _topic = topic; }
	void	broadcast(const std::string &message);
	void	addClient(Client *client);
	void	rmClient(Client *client);
	void	promoteClient(Client *client);
	void	demoteClient(Client *client);
	bool	isClient(Client *client);
	bool	isOperator(Client *client);
	Client	*getClient(std::string nickname);
	Client	*getClient(int fd);

	/**
	 * @brief Set the channel as password protected or Remove the password protection
	 * 
	 * @param value true: set password, false: remove password
	 * @param password the password to set, if value is false, this parameter is ignored
	*/
	void	setPassword(bool value, const std::string &password) { value == true ? _password = password : _password = ""; _hasPassword = value; }
	
	/**
	 * @brief Set the channel as user limit restricted or Remove the user limit restriction
	 * 
	 * @param value true: set user limit, false: remove user limit
	 * @param userLimit the user limit to set, if value is false, this parameter is ignored
	*/
	void	setUserLimit(bool value, unsigned int userLimit) { value == true ? _userLimit = userLimit : _userLimit = 0; _hasUserLimit = value; }
	
	/**
	 * @brief Set the channel as topic restricted or Remove the topic restriction
	 * 
	 * @param value true: set topic restriction, false: remove topic restriction
	*/
	void	setTopicRestricted(bool value) { _isTopicRestricted = value; }

	/**
	 * @brief Set the channel as invite only or Remove the invite only restriction
	 * 
	 * @param value true: set invite only, false: remove invite only
	*/
	void	setInviteOnly(bool value) { _isInviteOnly = value; }
};

#include "Client.hpp"
#include "Log.hpp"

#endif
