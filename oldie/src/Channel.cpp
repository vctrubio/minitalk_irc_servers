#include "../inc/Channel.hpp"

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
 * @details If the client is in the channel, the second value of the pair from the _clients map will be set to true.
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
 * @details If the the client is in the channel, the second value of the pair from the _clients map will be set to false.
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
