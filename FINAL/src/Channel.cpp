#include "../inc/Channel.hpp"


/**
 * @brief Construct a new Channel object
 * 
 * @param name the name of the channel
 * 
 * @note Initializes the _topic and _name attributes.
 * @note Initializes the _hasPassword, _isTopicRestricted, _hasUserLimit and _isInviteOnly attributes to false.
*/
Channel::Channel(std::string name): _name(name), _topic(name.substr(1))  { _hasPassword = false; _isTopicRestricted = false; _hasUserLimit = false; _isInviteOnly = false; };


/**
 * @brief Sets the topic of the channel.
*/
void Channel::setTopic(const std::string &topic) { _topic = topic; }


/**
 * @brief Checks if the channel is invite only.
 * 
 * @return true if the channel is invite only, false otherwise.
*/
bool Channel::isInviteOnly() const { return _isInviteOnly; }


/**
 * @brief Checks if the channel is topic restricted.
 * 
 * @return true if the channel is topic restricted, false otherwise.
*/
bool Channel::isTopicRestricted() const { return _isTopicRestricted; }


/**
 * @brief Checks if the channel has a user limit.
 * 
 * @return true if the channel has a user limit, false otherwise.
*/
bool Channel::hasUserLimit() const { return _hasUserLimit; }


/**
 * @brief Checks if the channel has a password.
 * 
 * @return true if the channel has a password, false otherwise.
*/
bool Channel::hasPassword() const { return _hasPassword; }


/**
 * @brief Returns the map of clients in the channel.
 * 
 * @return the map of clients in the channel.
 * 
 * @note The map is a map of <Client*, bool> where the bool is true if the client is an operator, false otherwise.
*/
map::client	Channel::getClients() const { return _clients; }


/**
 * @brief Returns the user limit of the channel.
 * 
 * @return the user limit of the channel.
*/
unsigned int Channel::getUserLimit() const { return _userLimit; }


/**
 * @brief Returns the topic of the channel.
 * 
 * @return the topic of the channel.
*/
const std::string	&Channel::getTopic() const { return _topic; }


/**
 * @brief Returns the password of the channel.
 * 
 * @return the password of the channel.
*/
const std::string	&Channel::getPassword() const { return _password; }


/**
 * @brief Returns the name of the channel.
 * 
 * @return the name of the channel.
*/
const std::string	&Channel::getName() const { return _name; }


/**
 * @brief Set the channel as password protected or Remove the password protection
 * 
 * @param value true: set password, false: remove password
 * @param password the password to set, if value is false, this parameter is ignored
*/
void	Channel::setPassword(bool value, const std::string &password) { value == true ? _password = password : _password = ""; _hasPassword = value; }


/**
 * @brief Set the channel as user limit restricted or Remove the user limit restriction
 * 
 * @param value true: set user limit, false: remove user limit
 * @param userLimit the user limit to set, if value is false, this parameter is ignored
*/
void	Channel::setUserLimit(bool value, unsigned int userLimit) { value == true ? _userLimit = userLimit : _userLimit = 0; _hasUserLimit = value; }


/**
 * @brief Set the channel as topic restricted or Remove the topic restriction
 * 
 * @param value true: set topic restriction, false: remove topic restriction
*/
void	Channel::setTopicRestricted(bool value) { _isTopicRestricted = value; }


/**
 * @brief Set the channel as invite only or Remove the invite only restriction
 * 
 * @param value true: set invite only, false: remove invite only
*/
void	Channel::setInviteOnly(bool value) { _isInviteOnly = value; }


/**
 * @brief Sends a message to all clients in the channel.
 * 
 * @param message The message to send.
 */
void	Channel::broadcast(const std::string &message)
{
	if (!_clients.empty())
	{
		for (map::iterator::client it = _clients.begin(); it != _clients.end(); it++)
		{
			send((*it).first->getSocket(), message.c_str(), message.size(), 0);
			getLog()->CommandSent(message, (*it).first);
		}
	}
}


/**
 * @brief Adds a client, if it doesn't exist already, to the map _clients. The client will be added as a non-operator (bool = false)
 * 
 * @param client: The client that will be added.
*/
void	Channel::addClient(Client *client)
{
	if (!isClient(client))
		_clients.insert(map::pair::client(client, false));
}


/**
 * @brief Removes a client from the map _clients, if it exists.
 * 
 * @param client: The client that will be removed
*/
void	Channel::rmClient(Client *client)
{
	if (isClient(client))
	{
		for (map::iterator::client it = _clients.begin(); it != _clients.end(); it++)
		{
			if ((*it).first == client)
			{
				_clients.erase(it);
				break;
			}
		}
	}
}


/**
 * @brief Promotes a client to operator, if it exists and is not already an operator.
 * 
 * @param client: The client that will be promoted.
 * 
 * @note If the client is in the channel, the second value of the pair from the _clients map will be set to true.
*/
void	Channel::promoteClient(Client *client)
{
	if (isClient(client) && !isOperator(client))
		_clients[client] = true;
}


/**
 * @brief Demotes a client to regular client, if it exists and is an operator.
 * 
 * @param client: The client that will be demoted.
 * 
 * @note If the the client is in the channel, the second value of the pair from the _clients map will be set to false.
*/
void	Channel::demoteClient(Client *client)
{
	if (isClient(client) && isOperator(client))
		_clients[client] = false;
}


/**
 * @brief Checks if the client passed as argument is on the channel.
 * 
 * @param client: The client to check.
 * 
 * @return bool: True if the client is in the _clients map, operator or not. Otherwise, returns false.
*/
bool	Channel::isClient(Client *client)
{
	if (!_clients.empty())
	{
		for (map::iterator::client it = _clients.begin(); it != _clients.end(); it++)
			if ((*it).first == client)
				return true;
	}
	return false;
}


/**
 * @brief Checks if the client passed as argument is an operator on the channel.
 * 
 * @param client: The client to check.
 * 
 * @return bool: True if the client is in the _clients map and is an operator. Otherwise, returns false.
*/
bool	Channel::isOperator(Client *client)
{
	if (isClient(client))
	{
		for (map::iterator::client it = _clients.begin(); it != _clients.end(); it++)
			if ((*it).first == client && (*it).second == true)
				return true;
	}
	return false;
}


/**
 * @brief Retuns a pointer to the client with the nickname passed as argument, if it exists.
 * 
 * @param nickname: The nickname of the client to return.
 * 
 * @return Client*: A pointer to the client with the nickname passed as argument, if it exists. Otherwise, returns NULL.
*/
Client	*Channel::getClient(std::string nickname)
{
	if (!_clients.empty())
	{
		for (map::iterator::client it = _clients.begin(); it != _clients.end(); it++)
		{
			if ((*it).first->getNickname() == nickname)
				return (*it).first;
		}
	}
	return NULL;
}


/**
 * @brief Returns a pointer to the client with the fd passed as argument, if it exists.
 * 
 * @param fd: The fd of the client to return.
 * 
 * @return Client*: A pointer to the client with the fd passed as argument, if it exists. Otherwise, returns NULL.
*/
Client	*Channel::getClient(int fd)
{
	if (!_clients.empty())
	{
		for (map::iterator::client it = _clients.begin(); it != _clients.end(); it++)
		{
			if ((*it).first->getSocket() == fd)
				return (*it).first;
		}
	}
	return NULL;
}


/**
 * @brief Adds a client to the ban list, if it doesn't exist already.
 * 
 * @param client: The client to ban.
*/
void	Channel::ban(Client *client)
{
	if (!isBanned(client))
		_banned.push_back(client);
}


/**
 * @brief Removes a client from the ban list, if it exists.
 * 
 * @param client: The client to unban.
*/
void	Channel::unban(Client *client)
{
	if (isBanned(client))
	{
		for (vector::iterator::client it = _banned.begin(); it != _banned.end(); it++)
		{
			if ((*it) == client)
			{
				_banned.erase(it);
				break;
			}
		}
	}
}


/**
 * @brief Adds a client to the invite list, if it doesn't exist already.
 * 
 * @param client: The client to invite.
*/
void	Channel::invite(Client *client)
{
	if (!isInvited(client))
		_invited.push_back(client);
}


/**
 * @brief Removes a client from the invite list, if it exists.
 * 
 * @param client: The client to uninvite.
*/
void	Channel::uninvite(Client *client)
{
	if (isInvited(client))
	{
		for (vector::iterator::client it = _invited.begin(); it != _invited.end(); it++)
		{
			if ((*it) == client)
			{
				_invited.erase(it);
				break;
			}
		}
	}
}


/**
 * @brief Checks if the client passed as argument is banned from the channel.
 * 
 * @param client: The client to check.
 * 
 * @return bool: True if the client is in the _banned vector. Otherwise, returns false.
*/
bool	Channel::isBanned(Client *client)
{
	for (vector::iterator::client it = _banned.begin(); it != _banned.end(); it++)
	{
		if ((*it) == client)
			return true;
	}
	return false;
}


/**
 * @brief Checks if the client passed as argument is invited to the channel.
 * 
 * @param client: The client to check.
 * 
 * @return bool: True if the client is in the _invited vector. Otherwise, returns false.
*/
bool	Channel::isInvited(Client *client)
{
	if (!_invited.empty())
	{
		for (vector::iterator::client it = _invited.begin(); it != _invited.end(); it++)
		{
			if ((*it) == client)
				return true;
		}
	}
	return false;
}


/**
 * @brief Checks if channel has an operator.
 * 
 * @return bool: True if the channel has an operator. Otherwise, returns false.
*/
bool	Channel::hasOperator()
{
	if (!_clients.empty())
	{
		for (map::iterator::client it = _clients.begin(); it != _clients.end(); it++)
		{
			if ((*it).second == true)
				return true;
		}
	}
	return false;
}