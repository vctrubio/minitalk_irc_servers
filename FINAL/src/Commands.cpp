#include "../inc/Commands.hpp"

void    Command::sendMsg(Client *user, const std::string &msg) { send(user->getSocket(), msg.c_str(), msg.length(), 0); getLog()->CommandSent(msg, user); }



//----------------------------------------------------------------------------------------------------
// NICK

Nick::Nick(CommandArgs args)
{
	vector::iterator::string it = args.buffer.begin();
	it++;
	if (it == args.buffer.end())
	{
		sprintf(_tmp, ERR_NEEDMOREPARAMS, "ft_irc", args.user->getNickname().c_str(), "NICK");
		sendMsg(args.user, _tmp);
		return;
	}
	vector::client clients = args.server->getClients();
	for (vector::iterator::client it2 = clients.begin(); it2 != clients.end(); it2++)
	{
		if (*it == (*it2)->getNickname() && args.user != (*it2))
		{
			sprintf(_tmp, ERR_NICKNAMEINUSE, "ft_irc", args.user->getNickname().c_str(), (*it).c_str());
			sendMsg(args.user, _tmp);
			return;
		}
	}
	if ((*it) == "Moulinette")
	{
		sprintf(_tmp, ERR_NICKNAMEINUSE, "ft_irc", args.user->getNickname().c_str(), (*it).c_str());
		sendMsg(args.user, _tmp);
		memset(_tmp, 0, 1024);
		sprintf(_tmp, MSG_NOTICE, "ft_irc", args.user->getNickname().c_str(), "bot is a reserved nickname");
		sendMsg(args.user, _tmp);
		return;
	}
	sprintf(_tmp, MSG_NICK, (*it).c_str(), (*it).c_str());
	args.user->setNickname(*it);
	sendMsg(args.user, _tmp);
}



//----------------------------------------------------------------------------------------------------
// USER

User::User(CommandArgs args)
{
	vector::iterator::string it = args.buffer.begin();
	it++;
	if (it == args.buffer.end())
	{
		sprintf(_tmp, ERR_NEEDMOREPARAMS, "ft_irc", args.user->getNickname().c_str(), "USER");
		sendMsg(args.user, _tmp);
		return;
	}
	if (*it == "Moulinette")
	{
		sprintf(_tmp, ERR_NICKNAMEINUSE, "ft_irc", args.user->getNickname().c_str(), (*it).c_str());
		sendMsg(args.user, _tmp);
		memset(_tmp, 0, 1024);
		sprintf(_tmp, MSG_NOTICE, "ft_irc", args.user->getNickname().c_str(), "bot is a reserved username");
		sendMsg(args.user, _tmp);
		return;
	}
	sprintf(_tmp, MSG_USER, args.user->getNickname().c_str(), (*it).c_str());
	sendMsg(args.user, _tmp);
}



//----------------------------------------------------------------------------------------------------
// JOIN

Join::Join(CommandArgs args)
{
	vector::iterator::string it = args.buffer.begin();
	it++;
	if (it == args.buffer.end())
	{
		sprintf(_tmp, ERR_NEEDMOREPARAMS, "ft_irc", args.user->getNickname().c_str(), "JOIN");
		sendMsg(args.user, _tmp);
		return;
	}
	std::string channelName = *it;
	// If the channel name is invalid, send an error
	if (isChannelNameValid(channelName) == false)
	{
		sendMsg(args.user, _tmp);
		return;
	}
	Channel *channel = args.server->getChannel(channelName);
	// If the channel doesn't exist, create it
	// add the user to it
	// make him operator
	if (channel == NULL)
	{
		// If the channel name is already taken, send an error
		if (args.server->isChannelNameAvailable(channelName) == false)
		{
			sprintf(_tmp, ERR_CHANAMEINUSE, "ft_irc", args.user->getNickname().c_str(), channelName.c_str());
			sendMsg(args.user, _tmp);
			return;
		}
		channel = new Channel(channelName);
		args.server->addChannel(channel);
		channel->addClient(args.user);
		Names::broadcast(channel);
		sprintf(_tmp, MSG_JOIN, args.user->getNickname().c_str(), channel->getName().c_str());
		sendMsg(args.user, _tmp);
		std::cout << "New channel created: [" << args.server->getChannels().size() - 1 << "] " << GREEN << "+ 1" << ENDC << std::endl;
		std::cout << "Name: " << GREEN << channel->getName() << ENDC << std::endl;
		std::cout << "Topic: " << GREEN << channel->getTopic() << ENDC << std::endl << std::endl;
		return;
	}
	// If the channel exists and the user is already in it, send an error
	else if (channel->isClient(args.user))
	{
		sprintf(_tmp, ERR_USERONCHANNEL, "ft_irc", args.user->getNickname().c_str(), (channelName).c_str());
		sendMsg(args.user, _tmp);
		return;
	}
	// If the channel exists and the user is banned from it and is not invited, send an error
	else if (channel->isBanned(args.user))
	{
		if (channel->isInvited(args.user))
		{
			channel->uninvite(args.user);
			channel->addClient(args.user);
			sprintf(_tmp, MSG_JOIN, args.user->getNickname().c_str(), channel->getName().c_str());
			sendMsg(args.user, _tmp);
			return;
		}
		sprintf(_tmp, ERR_BANNEDFROMCHAN, "ft_irc", args.user->getNickname().c_str(), (channelName).c_str());
		sendMsg(args.user, _tmp);
		return;
	}
	// If the channel exists and the user is not invited, send an error
	// if the user is invited, remove him from the invited list and let him join
	if (channel->isInviteOnly())
	{
		if (channel->isInvited(args.user) == false)
		{
			sprintf(_tmp, ERR_INVITEONLYCHAN, "ft_irc", args.user->getNickname().c_str(), (channelName).c_str());
			sendMsg(args.user, _tmp);
			return;
		}
		else
			channel->uninvite(args.user);
			// it will add at the end of the function
	}
	// If the channel exists and has a user limit, check if the limit is reached
	// if the limit is reached, send an error
	if (channel->hasUserLimit())
	{
		if (channel->getClients().size() >= channel->getUserLimit())
		{
			sprintf(_tmp, ERR_CHANNELISFULL, "ft_irc", args.user->getNickname().c_str(), (channelName).c_str());
			sendMsg(args.user, _tmp);
			return;
		}
	}
	// If the channel exists and has a password, check if the password is correct
	// if the password is incorrect, send an error
	// If the user didn't provide a password, send an error
	if (channel->hasPassword())
	{
		it++;
		if (it == args.buffer.end())
		{
			sprintf(_tmp, ERR_NEEDMOREPARAMS, "ft_irc", args.user->getNickname().c_str(), (channelName).c_str());
			sendMsg(args.user, _tmp);
			return;
		}
		std::string password = *it;
		if (password != channel->getPassword())
		{
			sprintf(_tmp, ERR_BADCHANNELKEY, "ft_irc", args.user->getNickname().c_str(), (channelName).c_str());
			sendMsg(args.user, _tmp);
			return;
		}
	}
	channel->addClient(args.user);
	memset(_tmp, 0, 1024);
	sprintf(_tmp, MSG_JOIN, args.user->getNickname().c_str(), channel->getName().c_str());
	channel->broadcast(_tmp);
	Names::broadcast(channel);
}

/**
 * @brief Check if the channel name is valid
 * 
 * @details A Channel name is valid if it starts with a #, is less than 50 characters long and inly contains alphanumeric characters + #
 * 
 * @param name the channel name
*/
bool Join::isChannelNameValid(const std::string &name)
{
	if (name[0] != '#')
	{
		sprintf(_tmp, ERR_BADCHANMASK, "ft_irc", name.c_str(), "Name must start with a #");
		return false;
	}
	else if (name.size() > 50)
	{
		sprintf(_tmp, ERR_BADCHANMASK, "ft_irc", name.c_str(), "Name must be less than 50 characters long");
		return false;
	}
	else if (name.size() < 2)
	{
		sprintf(_tmp, ERR_BADCHANMASK, "ft_irc", name.c_str(), "Name must be more than 1 character long");
		return false;
	}
	else
	{
		for (std::string::const_iterator it = name.begin(); it != name.end(); it++)
		{
			if (isalnum(*it) == false && *it != '#')
			{
				
				return false;
			}
		}
	}
	return true;
}



//----------------------------------------------------------------------------------------------------
// PART

Part::Part(CommandArgs args)
{
	vector::iterator::string it = args.buffer.begin();
	it++;
	if (it == args.buffer.end())
	{
		sprintf(_tmp, ERR_NEEDMOREPARAMS, "ft_irc", args.user->getNickname().c_str(), "PART");
		sendMsg(args.user, _tmp);
		return;
	}
	std::string channel_ = *it;
	Channel *channel = args.server->getChannel(channel_);
	if (channel == NULL)
	{
		sprintf(_tmp, ERR_NOSUCHCHANNEL, "ft_irc", args.user->getNickname().c_str(), channel_.c_str());
		sendMsg(args.user, _tmp);
		return;
	}
	channel->rmClient(args.user);
	sprintf(_tmp, MSG_PART, args.user->getNickname().c_str(), channel->getName().c_str());
	sendMsg(args.user, _tmp);
	map::client clients = channel->getClients();
	for (map::iterator::client it = clients.begin(); it != clients.end(); it++)
		sendMsg((*it).first, _tmp);
}



//----------------------------------------------------------------------------------------------------
// NAMES

Names::Names(CommandArgs args)
{
	std::string channelName = "";
	vector::iterator::string it = args.buffer.begin();
	it++;
	if (it != args.buffer.end() && *it != "ft_irc")
	{
		channelName = *it;
		Channel *channel = args.server->getChannel(channelName);
		if (channel == NULL)
		{
			sprintf(_tmp, ERR_NOSUCHCHANNEL, "ft_irc", args.user->getNickname().c_str(), channelName.c_str());
			sendMsg(args.user, _tmp);
			return;
		}
		else
		{
			std::string users = "";
			map::client clients = channel->getClients();
			for (map::iterator::client it = clients.begin(); it != clients.end(); it++)
			{
				if (channel->isOperator(it->first))
					users += "@";
				users += it->first->getNickname() + " ";
			}
			memset(_tmp, 0, 1024);
			sprintf(_tmp, RPL_NAMREPLY, "ft_irc", args.user->getNickname().c_str(), channel->getName().c_str(), users.c_str());
			sendMsg(args.user, _tmp);
			memset(_tmp, 0, 1024);
			sprintf(_tmp, RPL_ENDOFNAMES, "ft_irc", args.user->getNickname().c_str(), channel->getName().c_str());
			sendMsg(args.user, _tmp);
			return;
		}
	}
	else
	{
		vector::channel channels = args.server->getChannels();
		for (vector::iterator::channel it = channels.begin(); it != channels.end(); it++)
		{
			Channel *channel = *it;
			std::string users = "";
			map::client clients = channel->getClients();
			for (map::iterator::client it = clients.begin(); it != clients.end(); it++)
			{
				if (channel->isOperator(it->first))
					users += "@";
				users += it->first->getNickname() + " ";
			}
			memset(_tmp, 0, 1024);
			sprintf(_tmp, RPL_NAMREPLY, "ft_irc", args.user->getNickname().c_str(), channel->getName().c_str(), users.c_str());
			sendMsg(args.user, _tmp);
		}
		memset(_tmp, 0, 1024);
		sprintf(_tmp, RPL_ENDOFNAMES, "ft_irc", args.user->getNickname().c_str(), "*");
		sendMsg(args.user, _tmp);
		vector::client clients = args.server->getUsersWithoutChannels();
		std::string users = "";
		for (vector::iterator::client it = clients.begin(); it != clients.end(); it++)
		{
			users += (*it)->getNickname() + " ";
		}
		memset(_tmp, 0, 1024);
		sprintf(_tmp, RPL_NAMREPLY, "ft_irc", args.user->getNickname().c_str(), "*", users.c_str());
		sendMsg(args.user, _tmp);
		memset(_tmp, 0, 1024);
		sprintf(_tmp, RPL_ENDOFNAMES, "ft_irc", args.user->getNickname().c_str(), "*");
		sendMsg(args.user, _tmp);
	
	}
}

void Names::broadcast(Channel *channel)
{
	char _tmp[1024];
	std::string users = "";
	map::client clients = channel->getClients();
	for (map::iterator::client it = clients.begin(); it != clients.end(); it++)
	{
		if (channel->isOperator(it->first))
			users += "@";
		users += it->first->getNickname() + " ";
	}
	for (map::iterator::client it = clients.begin(); it != clients.end(); it++)
	{
		memset(_tmp, 0, 1024);
		sprintf(_tmp, RPL_NAMREPLY, "ft_irc", it->first->getNickname().c_str(), channel->getName().c_str(), users.c_str());
		send(it->first->getSocket(), _tmp, strlen(_tmp), 0);
		getLog()->CommandSent(_tmp, it->first);
		memset(_tmp, 0, 1024);
		sprintf(_tmp, RPL_ENDOFNAMES, "ft_irc", it->first->getNickname().c_str(), channel->getName().c_str());
		send(it->first->getSocket(), _tmp, strlen(_tmp), 0);
		getLog()->CommandSent(_tmp, it->first);
	}
}


//----------------------------------------------------------------------------------------------------
// LIST

List::List(CommandArgs args)
{
	vector::channel channels = args.server->getChannels();
	sprintf(_tmp, RPL_LISTSTART, "ft_irc", args.user->getNickname().c_str());
	sendMsg(args.user, _tmp);
	for (vector::iterator::channel it = channels.begin(); it != channels.end(); it++)
	{
		Channel *channel = *it;
		memset(_tmp, 0, 1024);
		sprintf(_tmp, RPL_LIST, "ft_irc", args.user->getNickname().c_str(), channel->getName().c_str(), static_cast<int>(channel->getClients().size()), channel->getTopic().c_str());
		sendMsg(args.user, _tmp);

	}
	memset(_tmp, 0, 1024);
	sprintf(_tmp, RPL_LISTEND, "ft_irc", args.user->getNickname().c_str());
	sendMsg(args.user, _tmp);
}



//----------------------------------------------------------------------------------------------------
// PRIVMSG

Privmsg::Privmsg(CommandArgs args)
{
	vector::iterator::string it = args.buffer.begin();
	it++;
	if (it == args.buffer.end())
	{
		sprintf(_tmp, ERR_NEEDMOREPARAMS, "ft_irc", args.user->getNickname().c_str(), "PRIVMSG");
		sendMsg(args.user, _tmp);
		return;
	}
	std::string to = *it;
	it++;
	if (it == args.buffer.end())
	{
		sprintf(_tmp, ERR_NEEDMOREPARAMS, "ft_irc", args.user->getNickname().c_str(), "PRIVMSG");
		sendMsg(args.user, _tmp);
		return;
	}
	std::string message = *it;
	if (message.size() > 1)
		message = message.substr(1, message.size() - 1);
	it++;
	// Append the rest of the message
	while (it != args.buffer.end())
	{
		message += " " + *it;
		it++;
	}
	// If the target starts with a #, it's a channel
	if (to[0] == '#')
	{
		Channel *channel = args.server->getChannel(to);
		// If the channel doesn't exist, send an error
		if (channel == NULL)
		{
			sprintf(_tmp, ERR_NOSUCHCHANNEL, "ft_irc", args.user->getNickname().c_str(), to.c_str());
			sendMsg(args.user, _tmp);
			return;
		}
		// If it exists, send the message to all the clients in the channel, except the sender
		map::client clients = channel->getClients();
		for (map::iterator::client it = clients.begin(); it != clients.end(); it++)
		{
			if ((*it).first == args.user)
				continue;
			memset(_tmp, 0, 1024);
			sprintf(_tmp, MSG_PRIVMSG, args.user->getNickname().c_str(), channel->getName().c_str(), message.c_str());
			sendMsg((*it).first, _tmp);
		}
	}
	else
	{
		// If it doesn't start with a #, it's a user
		Client *client = args.server->getClient(to);
		// If the user doesn't exist, send an error
		if (client == NULL)
		{
			sprintf(_tmp, ERR_NOSUCHNICK, "ft_irc", args.user->getNickname().c_str(), to.c_str());
			sendMsg(args.user, _tmp);
			return;
		}
		// If it exists, send the message to the user
		sprintf(_tmp, MSG_PRIVMSG, args.user->getNickname().c_str(), to.c_str(), message.c_str());
		sendMsg(client, _tmp);
	}
}



//----------------------------------------------------------------------------------------------------
// WHO

Who::Who(CommandArgs args)
{
	vector::iterator::string it = args.buffer.begin();
	it++;
	if (it == args.buffer.end())
	{
		sprintf(_tmp, ERR_NEEDMOREPARAMS, "ft_irc", args.user->getNickname().c_str(), "WHO");
		sendMsg(args.user, _tmp);
		return;
	}
	Channel *channel = args.server->getChannel(*it);
	if (channel == NULL)
	{
		sprintf(_tmp, ERR_NOSUCHCHANNEL, "ft_irc", args.user->getNickname().c_str(), (*args.buffer.begin()).c_str());
		sendMsg(args.user, _tmp);
		return;
	}
	map::client clients = channel->getClients();
	for (map::iterator::client it = clients.begin(); it != clients.end(); it++)
	{
		memset(_tmp, 0, 1024);
		sprintf(_tmp, MSG_WHO, "ft_irc", args.user->getNickname().c_str(), channel->getName().c_str(), (*it).first->getNickname().c_str(), (*it).first->getUsername().c_str(), (*it).first->getHost().c_str(), (*it).first->getRealname().c_str());
		sendMsg(args.user, _tmp);
	}
		
}



//----------------------------------------------------------------------------------------------------
// MODE

Mode::Mode(CommandArgs args)
{
	vector::iterator::string it = args.buffer.begin();
	it++;
	if (it == args.buffer.end())
	{
		sprintf(_tmp, ERR_NEEDMOREPARAMS, "ft_irc", args.user->getNickname().c_str(), "MODE");
		sendMsg(args.user, _tmp);
		return;
	}
	Channel *channel = args.server->getChannel(*it);
	if (channel == NULL)
	{
		sprintf(_tmp, ERR_NOSUCHCHANNEL, "ft_irc", args.user->getNickname().c_str(), (*it).c_str());
		sendMsg(args.user, _tmp);
		return;
	}
	it++;
	if (it == args.buffer.end())
	{
		sprintf(_tmp, RPL_CHANNELMODEIS, "ft_irc", args.user->getNickname().c_str(), channel->getName().c_str(), "+n");
		sendMsg(args.user, _tmp);
		return;
	}
	else if (channel->isOperator(args.user) == false)
	{
		sprintf(_tmp, ERR_NOTCHNLOPER, "ft_irc", args.user->getNickname().c_str(), channel->getName().c_str());
		sendMsg(args.user, _tmp);
		return;
	}
	std::string mode = *it;
	Client *client;
	if (mode == "+i")
	{
		if (channel->isInviteOnly() == true)
		{
			
			sendMsg(args.user, _tmp);
			return;
		}
		channel->setInviteOnly(true);
		sprintf(_tmp, MSG_MODE, args.user->getNickname().c_str(), channel->getName().c_str(), "+i", "");
		channel->broadcast(_tmp);
	}
	else if (mode == "-i")
	{
		if (channel->isInviteOnly() == false)
		{
			sprintf(_tmp, ERR_KEYSET, "ft_irc", args.user->getNickname().c_str(), channel->getName().c_str());
			sendMsg(args.user, _tmp);
			return;
		}
		channel->setInviteOnly(false);
		sprintf(_tmp, MSG_MODE, args.user->getNickname().c_str(), channel->getName().c_str(), "-i", "");
		channel->broadcast(_tmp);
	}
	else if (mode == "+t")
	{
		if (channel->isTopicRestricted() == true)
		{
			sprintf(_tmp, ERR_KEYSET, "ft_irc", args.user->getNickname().c_str(), channel->getName().c_str());
			sendMsg(args.user, _tmp);
			return;
		}
		channel->setTopicRestricted(true);
		sprintf(_tmp, MSG_MODE, args.user->getNickname().c_str(), channel->getName().c_str(), "+t", "");
		channel->broadcast(_tmp);
	}
	else if (mode == "-t")
	{
		if (channel->isTopicRestricted() == false)
		{
			sprintf(_tmp, ERR_KEYSET, "ft_irc", args.user->getNickname().c_str(), channel->getName().c_str());
			sendMsg(args.user, _tmp);
			return;
		}
		channel->setTopicRestricted(false);
		sprintf(_tmp, MSG_MODE, args.user->getNickname().c_str(), channel->getName().c_str(), "-t", "");
		channel->broadcast(_tmp);
	}
	else if (mode == "+k")
	{
		it++;
		if (it == args.buffer.end())
		{
			sprintf(_tmp, ERR_NEEDMOREPARAMS, "ft_irc", args.user->getNickname().c_str(), channel->getName().c_str());
			sendMsg(args.user, _tmp);
			return;
		}
		channel->setPassword(true, *it);
		sprintf(_tmp, MSG_MODE, args.user->getNickname().c_str(), channel->getName().c_str(), "+k", (*it).c_str());
		channel->broadcast(_tmp);
	}
	else if (mode == "-k")
	{
		if (channel->hasPassword() == false)
		{
			sprintf(_tmp, ERR_KEYSET, "ft_irc", args.user->getNickname().c_str(), channel->getName().c_str());
			sendMsg(args.user, _tmp);
			return;
		}
		channel->setPassword(false, "");
		sprintf(_tmp, MSG_MODE, args.user->getNickname().c_str(), channel->getName().c_str(), "-k", "");
		channel->broadcast(_tmp);
	}
	else if (mode == "+l")
	{
		it++;
		if (it == args.buffer.end())
		{
			sprintf(_tmp, ERR_NEEDMOREPARAMS, "ft_irc", args.user->getNickname().c_str(), channel->getName().c_str());
			sendMsg(args.user, _tmp);
			return;
		}
		channel->setUserLimit(true, atoi((*it).c_str()));
		sprintf(_tmp, MSG_MODE, args.user->getNickname().c_str(), channel->getName().c_str(), "+l", std::to_string(channel->getUserLimit()).c_str());
		channel->broadcast(_tmp);
	}
	else if (mode == "-l")
	{
		if (channel->hasUserLimit() == false)
		{
			sprintf(_tmp, ERR_KEYSET, "ft_irc", args.user->getNickname().c_str(), channel->getName().c_str());
			sendMsg(args.user, _tmp);
			return;
		}
		channel->setUserLimit(false, 0);
		sprintf(_tmp, MSG_MODE, args.user->getNickname().c_str(), channel->getName().c_str(), "-l", args.user->getNickname().c_str());
		channel->broadcast(_tmp);
	}
	else if (mode == "+o")
	{
		it++;
		if (it == args.buffer.end())
		{
			sprintf(_tmp, ERR_NEEDMOREPARAMS, "ft_irc", args.user->getNickname().c_str(), channel->getName().c_str());
			sendMsg(args.user, _tmp);
			return;
		}
		client = args.server->getClient(*it);
		if (client == NULL)
		{
			sprintf(_tmp, ERR_NOSUCHNICK, "ft_irc", args.user->getNickname().c_str(), (*it).c_str());
			sendMsg(args.user, _tmp);
			return;
		}
		else if (channel->isClient(client) == false)
		{
			sprintf(_tmp, ERR_USERNOTINCHANNEL, "ft_irc", args.user->getNickname().c_str(), (*it).c_str(), channel->getName().c_str());
			sendMsg(args.user, _tmp);
			return;
		}
		else if (channel->isOperator(client) == true)
		{
			// the user that is a channel does not recieve this message
			sprintf(_tmp, ERR_KEYSET, "ft_irc", args.user->getNickname().c_str(), channel->getName().c_str());
			return;
		}
		channel->promoteClient(client);
		sprintf(_tmp, MSG_MODE, args.user->getNickname().c_str(), channel->getName().c_str(), "+o", client->getNickname().c_str());
		channel->broadcast(_tmp);
	}
	else if (mode == "-o")
	{
		it++;
		if (it == args.buffer.end())
		{
			sprintf(_tmp, ERR_NEEDMOREPARAMS, "ft_irc", args.user->getNickname().c_str(), channel->getName().c_str());
			sendMsg(args.user, _tmp);
			return;
		}
		client = args.server->getClient(*it);
		if (client == NULL)
		{
			sprintf(_tmp, ERR_NOSUCHNICK, "ft_irc", args.user->getNickname().c_str(), (*it).c_str());
			sendMsg(args.user, _tmp);
			return;
		}
		else if (channel->isClient(client) == false)
		{
			sprintf(_tmp, ERR_USERNOTINCHANNEL, "ft_irc", args.user->getNickname().c_str(), (*it).c_str(), channel->getName().c_str());
			sendMsg(args.user, _tmp);
			return;
		}
		else if (channel->isOperator(client) == false)
		{
			
			sendMsg(args.user, _tmp);
			return;
		}
		channel->demoteClient(client);
		sprintf(_tmp, MSG_MODE, args.user->getNickname().c_str(), channel->getName().c_str(), "-o", client->getNickname().c_str());
		channel->broadcast(_tmp);
	}
	else if (mode == "+b")
	{
		it++;
		if (it == args.buffer.end())
		{
			sprintf(_tmp, ERR_NEEDMOREPARAMS, "ft_irc", args.user->getNickname().c_str(), channel->getName().c_str());
			sendMsg(args.user, _tmp);
			return;
		}
		client = args.server->getClient(*it);
		if (client == NULL)
		{
			sprintf(_tmp, ERR_NOSUCHNICK, "ft_irc", args.user->getNickname().c_str(), (*it).c_str());
			sendMsg(args.user, _tmp);
			return;
		}
		else if (channel->isClient(client) == false)
		{
			sprintf(_tmp, ERR_USERNOTINCHANNEL, "ft_irc", args.user->getNickname().c_str(), (*it).c_str(), channel->getName().c_str());
			sendMsg(args.user, _tmp);
			return;
		}
		else if (channel->isBanned(client) == true)
		{
			sprintf(_tmp, ERR_KEYSET, "ft_irc", args.user->getNickname().c_str(), channel->getName().c_str());
			return;
		}
		sprintf(_tmp, MSG_MODE, args.user->getNickname().c_str(), channel->getName().c_str(), "+b", client->getNickname().c_str());
		channel->broadcast(_tmp);
		memset(_tmp, 0, 1024);
		sprintf(_tmp, MSG_KICK, args.user->getNickname().c_str(), channel->getName().c_str(), client->getNickname().c_str(), "Banned from channel");
		channel->broadcast(_tmp);
		memset(_tmp, 0, 1024);
		sprintf(_tmp, ":%s QUIT :%s", client->getNickname().c_str(), "Banned from channel");
		sendMsg(client, _tmp);
		Names::broadcast(channel);
		channel->ban(client);
		channel->rmClient(client);
		
	}
	else if (mode == "-b")
	{
		it++;
		if (it == args.buffer.end())
		{
			sprintf(_tmp, ERR_NEEDMOREPARAMS, "ft_irc", args.user->getNickname().c_str(), channel->getName().c_str());
			sendMsg(args.user, _tmp);
			return;
		}
		client = args.server->getClient(*it);
		if (client == NULL)
		{
			sprintf(_tmp, ERR_NOSUCHNICK, "ft_irc", args.user->getNickname().c_str(), (*it).c_str());
			sendMsg(args.user, _tmp);
			return;
		}
		else if (channel->isBanned(client) == false)
		{
			
			sendMsg(args.user, _tmp);
			return;
		}
		sprintf(_tmp, MSG_MODE, args.user->getNickname().c_str(), channel->getName().c_str(), "-b", client->getNickname().c_str());
		channel->broadcast(_tmp);
		channel->unban(client);
	}
	else
	{
		sprintf(_tmp, ERR_UNKNOWNMODE, "ft_irc", args.user->getNickname().c_str(), mode.c_str());
		sendMsg(args.user, _tmp);
	}
}



//----------------------------------------------------------------------------------------------------
// PING

Ping::Ping(CommandArgs args)
{
	if (args.buffer.size() < 2)
	{
		sprintf(_tmp, ERR_NEEDMOREPARAMS, "ft_irc", args.user->getNickname().c_str(), "PING");
		sendMsg(args.user, _tmp);
		return;
	}
	sprintf(_tmp, MSG_PONG, args.buffer[1].c_str());
	sendMsg(args.user, _tmp);
}



//----------------------------------------------------------------------------------------------------
// KICK

Kick::Kick(CommandArgs args)
{
	vector::iterator::string it = args.buffer.begin();
	it++;
	if (it == args.buffer.end())
	{
		sprintf(_tmp, ERR_NEEDMOREPARAMS, "ft_irc", args.user->getNickname().c_str(), "KICK");
		sendMsg(args.user, _tmp);
		return;
	}
	std::string channelName = *it;
	it++;
	if (it == args.buffer.end())
	{
		sprintf(_tmp, ERR_NEEDMOREPARAMS, "ft_irc", args.user->getNickname().c_str(), "KICK");
		sendMsg(args.user, _tmp);
		return;
	}
	std::string toKick = *it;
	std::string reason = "";
	if (it != args.buffer.end())
		reason = *it;
	Client *client = args.server->getClient(toKick);
	if (client == NULL)
	{
		sprintf(_tmp, ERR_NOSUCHNICK, "ft_irc", args.user->getNickname().c_str(), toKick.c_str());
		sendMsg(args.user, _tmp);
		return;
	}
	else if (client == args.user)
	{
		sprintf(_tmp, ERR_CANTKILLSERVER, "ft_irc", args.user->getNickname().c_str(), toKick.c_str());
		sendMsg(args.user, _tmp);
		return;
	}
	Channel *channel = args.server->getChannel(channelName);
	if (channel->isOperator(args.user) == false)
	{
		sprintf(_tmp, ERR_NOTCHNLOPER, "ft_irc", args.user->getNickname().c_str(), channel->getName().c_str());
		sendMsg(args.user, _tmp);
		return;
	}
	if (channel == NULL)
	{
		sprintf(_tmp, ERR_NOSUCHCHANNEL, "ft_irc", args.user->getNickname().c_str(), channelName.c_str());
		sendMsg(args.user, _tmp);
		return;
	}
	else if (channel->isClient(client) == false)
	{
		sprintf(_tmp, ERR_USERNOTINCHANNEL, "ft_irc", args.user->getNickname().c_str(), toKick.c_str(), channel->getName().c_str());
		sendMsg(args.user, _tmp);
		return;
	}
	sprintf(_tmp, MSG_KICK, args.user->getNickname().c_str(), channel->getName().c_str(), toKick.c_str(), reason.c_str());
	channel->broadcast(_tmp);
	channel->rmClient(client);
}



//----------------------------------------------------------------------------------------------------
// INVITE

Invite::Invite(CommandArgs args)
{
	vector::iterator::string it = args.buffer.begin();
	it++;
	if (it == args.buffer.end())
	{
		sprintf(_tmp, ERR_NEEDMOREPARAMS, "ft_irc", args.user->getNickname().c_str(), "INVITE");
		sendMsg(args.user, _tmp);
		return;
	}
	Client *client = args.server->getClient(*it);
	if (client == NULL)
	{
		sprintf(_tmp, ERR_NOSUCHNICK, "ft_irc", args.user->getNickname().c_str(), (*it).c_str());
		sendMsg(args.user, _tmp);
		return;
	}
	it++;
	if (it == args.buffer.end())
	{
		sprintf(_tmp, ERR_NEEDMOREPARAMS, "ft_irc", args.user->getNickname().c_str(), "INVITE");
		sendMsg(args.user, _tmp);
		return;
	}
	Channel *channel = args.server->getChannel(*it);
	if (channel == NULL)
	{
		sprintf(_tmp, ERR_NOSUCHCHANNEL, "ft_irc", args.user->getNickname().c_str(), (*it).c_str());
		sendMsg(args.user, _tmp);
		return;
	}
	else if (channel->isClient(args.user) == false)
	{
		sprintf(_tmp, ERR_USERNOTINCHANNEL, "ft_irc", args.user->getNickname().c_str(), (*it).c_str(), channel->getName().c_str());
		sendMsg(args.user, _tmp);
		return;
	}
	else if (channel->isOperator(args.user) == false)
	{
		sprintf(_tmp, ERR_NOTCHNLOPER, "ft_irc", args.user->getNickname().c_str(), channel->getName().c_str());
		sendMsg(args.user, _tmp);
		return;
	}
	channel->invite(client);
	sprintf(_tmp, MSG_INVITE, args.user->getNickname().c_str(), client->getNickname().c_str(), channel->getName().c_str());
	sendMsg(args.user, _tmp);
	sendMsg(client, _tmp);
}



//----------------------------------------------------------------------------------------------------
// TOPIC

Topic::Topic(CommandArgs args)
{
	vector::iterator::string it = args.buffer.begin();
	it++;
	if (it == args.buffer.end())
	{
		sprintf(_tmp, ERR_NEEDMOREPARAMS, "ft_irc", args.user->getNickname().c_str(), "TOPIC");
		sendMsg(args.user, _tmp);
		return;
	}
	Channel *channel = args.server->getChannel(*it);
	if (channel == NULL)
	{
		sprintf(_tmp, ERR_NOSUCHCHANNEL, "ft_irc", args.user->getNickname().c_str(), (*it).c_str());
		sendMsg(args.user, _tmp);
		return;
	}
	it++;
	if (it == args.buffer.end())
	{
		sprintf(_tmp, RPL_TOPIC, "ft_irc", args.user->getNickname().c_str(), channel->getName().c_str(), channel->getName().c_str());
		sendMsg(args.user, _tmp);
		return;
	}
	if (channel->isTopicRestricted())
	{
		if (channel->isOperator(args.user))
			sprintf(_tmp, ERR_CHANOPRIVSNEEDED, "ft_irc", args.user->getNickname().c_str(), channel->getName().c_str(), "+t");
		else
			sprintf(_tmp, ERR_TOPICRESTRICTED, "ft_irc", args.user->getNickname().c_str(), channel->getName().c_str());
		sendMsg(args.user, _tmp);
		return;
	}
	std::string topic = "";
	while (it != args.buffer.end())
	{
		topic += *it + " ";
		it++;
	}
	if (topic.size() > 2)
		topic = topic.substr(1, topic.size() - 2);
	channel->setTopic(topic);
	sprintf(_tmp, MSG_TOPIC, args.user->getNickname().c_str(), channel->getName().c_str(), topic.c_str());
	sendMsg(args.user, _tmp);
}



//----------------------------------------------------------------------------------------------------
// QUIT

Quit::Quit(CommandArgs args)
{
	vector::iterator::string it = args.buffer.begin();
	it++;
	if (it == args.buffer.end())
	{
		sprintf(_tmp, ":%s QUIT :%s\r\n", args.user->getNickname().c_str(), "Client Quit");
		sendMsg(args.user, _tmp);
		args.server->rmClient(args.user);
		return;
	}
	std::string reason = *it;
	if (it != args.buffer.end())
	{
		if(reason.size() > 1)
			reason = reason.substr(1, reason.size() - 1);
		it++;
		while (it != args.buffer.end())
		{
			reason += " " + *it;
			it++;
		}
	}
	if (reason.find("Bot is leaving") == std::string::npos)
	{
		args.server->broadcast("Bot left the server\n\r");
		args.server->setBot(false);
		return;
	}
	sprintf(_tmp, ":%s QUIT :%s\r\n", args.user->getNickname().c_str(), reason.c_str());
	sendMsg(args.user, _tmp);
}



//----------------------------------------------------------------------------------------------------
// CAP

CAP::CAP(CommandArgs args)
{
	sprintf(_tmp, CAP_REPLY_2, args.user->getNickname().c_str());
	sendMsg(args.user, _tmp);
}



//----------------------------------------------------------------------------------------------------
// WHOIS

Whois::Whois(CommandArgs args)
{
	vector::iterator::string it = args.buffer.begin();
	it++;
	if (it == args.buffer.end())
	{
		sprintf(_tmp, ERR_NEEDMOREPARAMS, "ft_irc", args.user->getNickname().c_str(), "WHOIS");
		sendMsg(args.user, _tmp);
		return;
	}
	Client *client = args.server->getClient(*it);
	if (client == NULL)
	{
		sprintf(_tmp, ERR_NOSUCHNICK, "ft_irc", args.user->getNickname().c_str(), (*it).c_str());
		sendMsg(args.user, _tmp);
		return;
	}
	sprintf(_tmp, RPL_WHOISUSER, "ft_irc", args.user->getNickname().c_str(), client->getNickname().c_str(), client->getUsername().c_str(), client->getHost().c_str(), client->getRealname().c_str());
	sendMsg(args.user, _tmp);
	sprintf(_tmp, RPL_WHOISSERVER, "ft_irc", args.user->getNickname().c_str(), client->getNickname().c_str(), "ft_irc", "ft_irc");
	sendMsg(args.user, _tmp);
	std::string channels = "";
	vector::channel chans = args.server->getChannels();
	for (vector::iterator::channel it = chans.begin(); it != chans.end(); it++)
	{
		if ((*it)->isClient(client))
			channels += (*it)->getName() + " ";
	}
	if (channels.size() > 0)
		channels = channels.substr(0, channels.size() - 1);
	sprintf(_tmp, RPL_WHOISCHANNELS, "ft_irc", args.user->getNickname().c_str(), client->getNickname().c_str(), channels.c_str());
	sendMsg(args.user, _tmp);
	sprintf(_tmp, RPL_ENDOFWHOIS, "ft_irc", args.user->getNickname().c_str(), client->getNickname().c_str());
	sendMsg(args.user, _tmp);

}



//----------------------------------------------------------------------------------------------------
// LUSERS

Lusers::Lusers(CommandArgs args)
{
	sprintf(_tmp, RPL_LUSERCLIENT, "ft_irc", args.user->getNickname().c_str(), static_cast<int>(args.server->getClients().size()), static_cast<int>(args.server->getClients().size()), 1);
	sendMsg(args.user, _tmp);
	sprintf(_tmp, RPL_LUSEROP, "ft_irc", args.user->getNickname().c_str(), 0);
	sendMsg(args.user, _tmp);
	sprintf(_tmp, RPL_LUSERUNKNOWN, "ft_irc", args.user->getNickname().c_str(), 0);
	sendMsg(args.user, _tmp);
	sprintf(_tmp, RPL_LUSERCHANNELS, "ft_irc", args.user->getNickname().c_str(), static_cast<int>(args.server->getChannels().size()));
	sendMsg(args.user, _tmp);
	sprintf(_tmp, RPL_LUSERME, "ft_irc", args.user->getNickname().c_str(), static_cast<int>(args.server->getClients().size()), 1);
	sendMsg(args.user, _tmp);
}



//----------------------------------------------------------------------------------------------------
// NOTICE

Notice::Notice(CommandArgs args)
{
	vector::iterator::string it = args.buffer.begin();
	it++;
	if (it == args.buffer.end())
	{
		sprintf(_tmp, ERR_NEEDMOREPARAMS, "ft_irc", args.user->getNickname().c_str(), "NOTICE");
		sendMsg(args.user, _tmp);
		return;
	}
	std::string to = *it;
	it++;
	if (it == args.buffer.end())
	{
		sprintf(_tmp, ERR_NEEDMOREPARAMS, "ft_irc", args.user->getNickname().c_str(), "NOTICE");
		sendMsg(args.user, _tmp);
		return;
	}
	std::string message = *it;
	if (message.size() > 1)
		message = message.substr(1, message.size() - 1);
	it++;
	// Append the rest of the message
	while (it != args.buffer.end())
	{
		message += " " + *it;
		it++;
	}
	// If the target starts with a #, it's a channel
	if (to[0] == '#')
	{
		Channel *channel = args.server->getChannel(to);
		// If the channel doesn't exist, send an error
		if (channel == NULL)
		{
			sprintf(_tmp, ERR_NOSUCHCHANNEL, "ft_irc", args.user->getNickname().c_str(), to.c_str());
			sendMsg(args.user, _tmp);
			return;
		}
		sprintf(_tmp, MSG_NOTICE, args.user->getNickname().c_str(), to.c_str(), message.c_str());
		channel->broadcast(_tmp);
	}
	else
	{
		// If it doesn't start with a #, it's a user
		Client *client = args.server->getClient(to);
		// If the user doesn't exist, send an error
		if (client == NULL)
		{
			sprintf(_tmp, ERR_NOSUCHNICK, "ft_irc", args.user->getNickname().c_str(), to.c_str());
			sendMsg(args.user, _tmp);
			return;
		}
		// If it exists, send the message to the user
		sprintf(_tmp, MSG_NOTICE, args.user->getNickname().c_str(), to.c_str(), message.c_str());
		sendMsg(client, _tmp);
	}
}