#include "../inc/Server.hpp"


void	Server::addClient(Client *client)
{
	if (client == NULL)
		return ;
	for (vector::iterator::client it = _clients.begin(); it != _clients.end(); ++it)
	{
		if ((*it)->getSocket() == client->getSocket())
		{
			delete client;
			return ;
		}
	}
	_clients.push_back(client);
}

void	Server::rmClient(Client *client)
{
	if (client == NULL)
		return ;
	for (vector::iterator::client it = _clients.begin(); it != _clients.end(); ++it)
	{
		if ((*it)->getSocket() == client->getSocket())
		{
			Client *tmp = *it;
			_clients.erase(it);
			delete tmp;
			return ;
		}
	}
}

void	Server::addChannel(Channel *channel)
{
	if (channel == NULL)
		return ;
	for (vector::iterator::channel it = _channels.begin(); it != _channels.end(); ++it)
	{
		if ((*it)->getTopic() == channel->getTopic())
		{
			delete channel;
			return ;
		}
	}
	_channels.push_back(channel);
}

void	Server::rmChannel(Channel *channel)
{
	if (channel == NULL)
		return ;
	for (vector::iterator::channel it = _channels.begin(); it != _channels.end(); ++it)
	{
		if ((*it)->getTopic() == channel->getTopic())
		{
			Channel *tmp = *it;
			_channels.erase(it);
			delete tmp;
			return ;
		}
	}
}

/**
 * @brief Free all the memory allocated for the server. 
*/
void	Server::purge()
{
	if (!_clients.empty())
	{
		for (vector::iterator::client it = _clients.begin(); it != _clients.end(); ++it)
			delete *it;
	}
	if (!_channels.empty())
	{
		for (vector::iterator::channel it = _channels.begin(); it != _channels.end(); ++it)
			delete *it;
	}
}


/**
 * @brief Returns a pointer to the client with the fd passed as argument, if it exists.
 * 
 * @param fd: The fd of the client to return.
 * @return Client*: A pointer to the client with the fd passed as argument, if it exists. Otherwise, returns NULL.
*/
Client	*Server::getClient(int fd)
{
	if (!_clients.empty())
	{
		for (vector::iterator::client it = _clients.begin(); it != _clients.end(); it++)
			if (fd == (*it)->getSocket())
				return (*it);
	}
	return NULL;

}

Client	*Server::getClient(const std::string &nickname)
{
	if (!_clients.empty())
	{
		for (vector::iterator::client it = _clients.begin(); it != _clients.end(); it++)
			if (nickname == (*it)->getNickname())
				return (*it);
	}
	return NULL;
}

/**
 * @brief Returns a pointer to the channel with the name passed as argument, if it exists.
 * 
 * @param name: The name of the channel to return.
 * 
 * @return Channel*: A pointer to the channel with the name passed as argument, if it exists. Otherwise, returns NULL.
*/
Channel	*Server::getChannel(const std::string &name)
{
	if (!_channels.empty())
	{
		for (vector::iterator::channel it = _channels.begin(); it != _channels.end(); it++)
			if (name == (*it)->getTopic())
				return (*it);
	}
	return NULL;
}

/**
 * @brief Cleans the channels that have no clients.
*/
void	Server::cleanEmptyChannels()
{
	if (!_channels.empty())
	{
		for (vector::iterator::channel it = _channels.begin(); it != _channels.end(); ++it)
		{
			if (_channels.empty())
				return ;
			if ((*it)->getClients().empty())
			{
				Channel *tmp = *it;
				std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
				std::cout << "Channel destroyed: [" << getChannels().size() << "] " << RED << "- 1" << ENDC << std::endl;
				std::cout << "Topic: " << RED << tmp->getTopic() << ENDC << std::endl;
				_channels.erase(it);
				delete tmp;
				it = _channels.begin();
			}
		}
	}
}

/*
 * Handles the commands that the client sends to the server.
*/
void Server::findCommand(vector::string str)
{
	if (str.empty())
		return ;
	CommandArgs args;
	vector::iterator::string it = str.begin();
	args.user = _requestCall;
	args.channel = this->getChannels().empty() ? NULL : this->getChannels().front();
	args.server = this;
	args.buffer = str;
	if (*it == "NICK")
		Nick cmd(args);
	else if (*it == "USER")
		User cmd(args);
	else if (*it == "JOIN")
		Join cmd(args);
	else if (*it == "PART")
		Part cmd(args);
	else if (*it == "NAMES")
		Names cmd(args);
	else if (*it == "LIST")
		List cmd(args);
	else if (*it == "TOPIC")
		Topic cmd(args);
	else if (*it == "INVITE")
		Invite cmd(args);
	else if (*it == "KICK")
		Kick cmd(args);
	else if (*it == "PRIVMSG")
		Privmsg cmd(args);
	else if (*it == "PING")
		Ping cmd(args);
	else if (*it == "WHO")
		Who cmd(args);
	else if (*it == "MODE")
		Mode cmd(args);
	else if (*it == "QUIT")
		Quit cmd(args);
	else if (*it == "CAP" && it + 1 != str.end() && *(it + 1) != "END")
		CAP cmd(args);
	else if (*it == "CAP")
		return ;
	else
	{
		char _tmp[1024];
		sprintf(_tmp, ERR_UNKNOWNCOMMAND, _requestCall->getNickname().c_str(), _requestCall->getNickname().c_str(), (*it).c_str());
		send(_requestCall->getSocket(), _tmp, strlen(_tmp), 0);
	}
}

bool	Server::isNicknameAvailable(const std::string &nickname)
{
	for (vector::iterator::client it = _clients.begin(); it != _clients.end(); ++it)
		if ((*it)->getNickname() == nickname)
			return (false);
	return (true);
}

bool	Server::isChannelNameAvailable(const std::string &name)
{
	for (vector::iterator::channel it = _channels.begin(); it != _channels.end(); ++it)
		if ((*it)->getTopic() == name)
			return (false);
	return (true);
}