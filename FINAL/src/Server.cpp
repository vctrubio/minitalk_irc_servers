#include "../inc/Server.hpp"

Server::Server(std::string password) : _password(password), _bot(false)
{
}

void	Server::setBot(bool bot)
{
	_bot = bot;
}

bool   Server::getBot() const
{
	return _bot;
}

void	Server::addClient(Client *client)
{
	if (client == NULL)
		return ;
	for (vector::iterator::client it = _clients.begin(); it != _clients.end(); it++)
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
	for (vector::iterator::client it = _clients.begin(); it != _clients.end(); it++)
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
	for (vector::iterator::channel it = _channels.begin(); it != _channels.end(); it++)
	{
		if ((*it)->getName() == channel->getName())
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
	for (vector::iterator::channel it = _channels.begin(); it != _channels.end(); it++)
	{
		if ((*it)->getName() == channel->getName())
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
		for (vector::iterator::client it = _clients.begin(); it != _clients.end(); it++)
			delete *it;
	}
	if (!_channels.empty())
	{
		for (vector::iterator::channel it = _channels.begin(); it != _channels.end(); it++)
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
			if (name == (*it)->getName())
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
		for (vector::iterator::channel it = _channels.begin(); it != _channels.end(); it++)
		{
			if (_channels.empty())
				return ;
			if ((*it)->getClients().empty())
			{
				Channel *tmp = *it;
				std::cout << "Channel destroyed: [" << getChannels().size() << "] " << RED << "- 1" << ENDC << std::endl;
				std::cout << "Name: " << RED << tmp->getName() << ENDC << std::endl;
				std::cout << "Topic: " << RED << tmp->getTopic() << ENDC << std::endl << std::endl;
				_channels.erase(it);
				delete tmp;
				it = _channels.begin();
			}
		}
	}
}

void	Server::adjustOperators()
{
	if (!_channels.empty())
	{
		for (vector::iterator::channel it = _channels.begin(); it != _channels.end(); it++)
		{
			if (!(*it)->hasOperator() && (*it)->getClients().size() > 0) 
			{
				char tmp[1024];
				memset(tmp, 0, 1024);
				map::client clients = (*it)->getClients();
				map::iterator::client it2 = clients.begin();
				Client *client = it2->first;
				sprintf(tmp, MSG_MODE, "ft_irc", (*it)->getName().c_str(), "+o", client->getNickname().c_str());
				(*it)->promoteClient(client);
				(*it)->broadcast(tmp);
				Names::broadcast((*it));
				break ;
			}
		
		} 
	}
}

void	Server::broadcast(const std::string &msg)
{
	if (!_clients.empty())
	{
		for (vector::iterator::client it = _clients.begin(); it != _clients.end(); it++)
		{
			send((*it)->getSocket(), msg.c_str(), msg.size(), 0);
			getLog()->CommandSent(msg, (*it));
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
	it = str.begin();
	args.user = _requestCall;
	args.server = this;
	args.buffer = str;
	*it = std::toupper(*it);
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
	else if (*it == "WHOIS")
		Whois cmd(args);
	else if (*it == "LUSERS")
		Lusers cmd(args);
	else if (*it == "NOTICE")
		Notice cmd(args);
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
	for (vector::iterator::client it = _clients.begin(); it != _clients.end(); it++)
		if ((*it)->getNickname() == nickname)
			return (false);
	return (true);
}

bool	Server::isChannelNameAvailable(const std::string &name)
{
	for (vector::iterator::channel it = _channels.begin(); it != _channels.end(); it++)
		if ((*it)->getName() == name)
			return (false);
	return (true);
}

vector::client Server::getUsersWithoutChannels()
{
	vector::client users;
	for (vector::iterator::client it = _clients.begin(); it != _clients.end(); it++)
	{
		bool found = false;
		for (vector::iterator::channel it2 = _channels.begin(); it2 != _channels.end(); it2++)
		{
			if ((*it2)->getClients().find(*it) != (*it2)->getClients().end())
			{
				found = true;
				break ;
			}
		}
		if (!found)
			users.push_back(*it);
	}
	return (users);
}