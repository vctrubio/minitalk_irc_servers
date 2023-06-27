#include "../inc/Commands.hpp"


// ---------------------------------------------------------------
// [ NICK ] - change nickname

Nick::Nick(CommandArgs args)
{
    vector::iterator::string it = args.buffer.begin();
    it++;
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
    sprintf(_tmp, MSG_NICK, args.user->getNickname().c_str(), (*it).c_str());
    args.user->setNickname(*it);
    sendMsg(args.user, _tmp);
}

// ---------------------------------------------------------------
// [ USER ] - user login

User::User(CommandArgs args)
{
    vector::iterator::string it = args.buffer.begin();
    it++;
    sprintf(_tmp, MSG_USER, args.user->getNickname().c_str(), (*it).c_str());
    sendMsg(args.user, _tmp);
}


// ---------------------------------------------------------------
// [ JOIN ] - join a channel

Join::Join(CommandArgs args)
{
    std::string _channel = args.buffer[1];
    Channel *channel = args.server->getChannel(_channel);
    if (channel == NULL)
    {
        channel = new Channel(_channel);
        args.server->addChannel(channel);
    }
    if (channel->isClient(args.user))
    {
        sprintf(_tmp, ERR_USERONCHANNEL, "ft_irc", args.user->getNickname().c_str(), _channel.c_str());
        sendMsg(args.user, _tmp);
        return;
    }
    if (channel->getClients().empty())
    {
        channel->addClient(args.user);
        channel->promoteClient(args.user);
        sprintf(_tmp, MSG_JOIN, args.user->getNickname().c_str(), channel->getTopic().c_str());
        sendMsg(args.user, _tmp);
        memset(_tmp, 0, 1024);
        sprintf(_tmp, MSG_MODE, "ft_irc", channel->getTopic().c_str(), "+o", args.user->getNickname().c_str());
        sendMsg(args.user, _tmp);
        std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
        std::cout << "New channel created: [" << args.server->getChannels().size() - 1 << "] " << GREEN << "+ 1" << ENDC << std::endl;
        std::cout << "Topic: " << GREEN << channel->getTopic() << ENDC << std::endl;
    }
    else
    {
        channel->addClient(args.user);
        sprintf(_tmp, MSG_JOIN, args.user->getNickname().c_str(), channel->getTopic().c_str());
        map::client clients = channel->getClients();
        for (map::iterator::client it = clients.begin(); it != clients.end(); ++it)
            sendMsg((*it).first, _tmp);
    }
}


// ---------------------------------------------------------------
// [ PART ] - leave a channel

Part::Part(CommandArgs args)
{
    vector::iterator::string it = args.buffer.begin();
    it++;
    std::string channel_ = *it;
    channel_ = channel_.substr(1, channel_.size() - 1);
    Channel *channel = args.server->getChannel(channel_);
    if (channel == NULL)
    {
        sprintf(_tmp, ERR_NOSUCHCHANNEL, "ft_irc", args.user->getNickname().c_str(), channel_.c_str());
        sendMsg(args.user, _tmp);
        return;
    }
    channel->rmClient(args.user);
    sprintf(_tmp, MSG_PART, args.user->getNickname().c_str(), channel->getTopic().c_str());
    sendMsg(args.user, _tmp);
    map::client clients = channel->getClients();
    for (map::iterator::client it = clients.begin(); it != clients.end(); ++it)
        sendMsg((*it).first, _tmp);
}

// ---------------------------------------------------------------
// [ NAMES ] - list users in a channel

Names::Names(CommandArgs args)
{
    std::string users = "";
    map::client clients = args.channel->getClients();
    for (map::iterator::client it = clients.begin(); it != clients.end(); ++it)
        users += (*it).first->getNickname() + " ";
    users = users.substr(0, users.size() - 1);
    sprintf(_tmp, MSG_NAMES, args.user->getNickname().c_str(), args.channel->getTopic().c_str(), users.c_str());
    sendMsg(args.user, _tmp);
}

// ---------------------------------------------------------------
// [ LIST ] - list all channels

List::List(CommandArgs args)
{
    vector::channel channels = args.server->getChannels();
    std::string tmp = "";
    for (vector::iterator::channel it = channels.begin(); it != channels.end(); ++it)
        tmp += (*it)->getTopic() + " ";
    tmp = tmp.substr(0, channels.size() - 1);
    sprintf(_tmp, MSG_LIST, args.user->getNickname().c_str(), "ft_irc", tmp.c_str());
    sendMsg(args.user, _tmp);
}

// ---------------------------------------------------------------
// [ PRIVMSG ] - send a message to a channel or user

Privmsg::Privmsg(CommandArgs args)
{
    vector::iterator::string it = args.buffer.begin();
    it++;
    std::string to = *it;
    it++;
    std::string message = *it;
    message = message.substr(1, message.size() - 1);
    if (to[0] == '#')
    {
        to = to.substr(1, to.size() - 1);
        Channel *channel = args.server->getChannel(to);
        if (channel == NULL)
        {
            sprintf(_tmp, ERR_NOSUCHNICK, "ft_irc", args.user->getNickname().c_str(), to.c_str());
            sendMsg(args.user, _tmp);
        }
        else
        {
            map::client clients = channel->getClients();
            for (map::iterator::client it = clients.begin(); it != clients.end(); ++it)
            {
                if ((*it).first == args.user)
                    continue;
                memset(_tmp, 0, 1024);
                sprintf(_tmp, MSG_PRIVMSG, args.user->getNickname().c_str(), ("#" + channel->getTopic()).c_str(), message.c_str());
                sendMsg((*it).first, _tmp);
            }
        }
    }
    else
    {
        Client *client = args.server->getClient(to);
        if (client == NULL)
        {
            sprintf(_tmp, ERR_NOSUCHNICK, "ft_irc", args.user->getNickname().c_str(), to.c_str());
            sendMsg(args.user, _tmp);
        }
        else
        {
            sprintf(_tmp, MSG_PRIVMSG, args.user->getNickname().c_str(), to.c_str(), message.c_str());
            sendMsg(client, _tmp);
        }
    }
}

// ---------------------------------------------------------------
// [ WHO ] - list user and channel information

Who::Who(CommandArgs args)
{
    map::client clients = args.channel->getClients();
    for (map::iterator::client it = clients.begin(); it != clients.end(); ++it)
    {
        memset(_tmp, 0, 1024);
        sprintf(_tmp, MSG_WHO, args.user->getNickname().c_str(), args.channel->getTopic().c_str(), (*it).first->getUsername().c_str(), (*it).first->getHost().c_str(), "ft_irc", (*it).first->getNickname().c_str(), buildChannelPermissions(args.channel, (*it).first).c_str(), (*it).first->getRealname().c_str());
        sendMsg(args.user, _tmp);
    }
        
}


// ---------------------------------------------------------------
// [ MODE ] - change user mode

Mode::Mode(CommandArgs args)
{
    vector::iterator::string it = args.buffer.begin();
    it++;
    Channel *channel = args.server->getChannel((*it).substr(1, (*it).size() - 1));
    if (channel != NULL)
    {
        it++;
        if (it == args.buffer.end())
        {
            char tmp[1024];
            sprintf(tmp, RPL_CHANNELMODEIS, "ft_irc", args.user->getNickname().c_str(), channel->getTopic().c_str(), buildChannelPermissions(channel, args.user).c_str());
            sendMsg(args.user, tmp);
            return;
        }
        std::string mode = *it;
        if (mode == "+o")
        {
            it++;
            if (args.channel->isOperator(args.user) == false)
                sprintf(_tmp, ERR_NOTCHNLOPER, "ft_irc", args.user->getNickname().c_str(), channel->getTopic().c_str());
            else if (it == args.buffer.end())
                sprintf(_tmp, ERR_NEEDMOREPARAMS, "ft_irc", args.user->getNickname().c_str(), channel->getTopic().c_str());
            else
            {
                std::string user = *it;
                Client *client = args.server->getClient(user);
                if (channel->isClient(client) == false)
                    sprintf(_tmp, ERR_USERNOTINCHANNEL, "ft_irc", args.user->getNickname().c_str(), user.c_str(), channel->getTopic().c_str());
                else
                {
                    channel->promoteClient(args.user);
                    sprintf(_tmp, MSG_MODE, args.user->getNickname().c_str(), channel->getTopic().c_str(), "+o", user.c_str());
                }
            }
        }
        else if (mode == "-o")
        {
            it++;
            if (channel->isOperator(args.user) == false)
                sprintf(_tmp, ERR_NOTCHNLOPER, "ft_irc", args.user->getNickname().c_str(), channel->getTopic().c_str());
            else if (it == args.buffer.end())
                sprintf(_tmp, ERR_NEEDMOREPARAMS, "ft_irc", args.user->getNickname().c_str(), channel->getTopic().c_str());
            else
            {
                std::string user = *it;
                Client *client = args.server->getClient(user);
                if (channel->isClient(client) == false)
                    sprintf(_tmp, ERR_USERNOTINCHANNEL, "ft_irc", args.user->getNickname().c_str(), user.c_str(), channel->getTopic().c_str());
                else
                {
                    channel->demoteClient(args.user);
                    sprintf(_tmp, MSG_MODE, args.user->getNickname().c_str(), channel->getTopic().c_str(), "-o", user.c_str());
                }
            }
        }
        else if (mode == "+i" || mode == "-i" || mode == "+t" || mode == "-t" || mode == "+l" || mode == "-l" || mode == "+k" || mode == "-k")
        {
            if (channel->isOperator(args.user) == false)
                sprintf(_tmp, ERR_NOTCHNLOPER, "ft_irc", args.user->getNickname().c_str(), channel->getTopic().c_str());
            else
            {
                if (mode == "+i")
                {
                    channel->setInviteOnly(true);
                    sprintf(_tmp, MSG_MODE, args.user->getNickname().c_str(), channel->getTopic().c_str(), "+i", "");
                }
                else if (mode == "-i")
                {
                    channel->setInviteOnly(false);
                    sprintf(_tmp, MSG_MODE, args.user->getNickname().c_str(), channel->getTopic().c_str(), "-i", "");
                }
                else if (mode == "+t")
                {
                    channel->setTopicRestricted(true);
                    sprintf(_tmp, MSG_MODE, args.user->getNickname().c_str(), channel->getTopic().c_str(), "+t", "");
                }
                else if (mode == "-t")
                {
                    channel->setTopicRestricted(false);
                    sprintf(_tmp, MSG_MODE, args.user->getNickname().c_str(), channel->getTopic().c_str(), "-t", "");
                }
                else if (mode == "+l")
                {
                    it++;
                    if (it == args.buffer.end())
                        sprintf(_tmp, ERR_NEEDMOREPARAMS, "ft_irc", args.user->getNickname().c_str(), mode.c_str());
                    else
                    {
                        channel->setUserLimit(true, atoi((*it).c_str()));
                        sprintf(_tmp, MSG_MODE, args.user->getNickname().c_str(), channel->getTopic().c_str(), "+l", (*it).c_str());
                    }
                }
                else if (mode == "-l")
                {
                    channel->setUserLimit(false, 0);
                    sprintf(_tmp, MSG_MODE, args.user->getNickname().c_str(), channel->getTopic().c_str(), "-l", "");
                }
                else if (mode == "+k")
                {
                    it++;
                    if (it == args.buffer.end())
                        sprintf(_tmp, ERR_NEEDMOREPARAMS, "ft_irc", args.user->getNickname().c_str(), mode.c_str());
                    else
                    {
                        channel->setPassword(true, *it);
                        sprintf(_tmp, MSG_MODE, args.user->getNickname().c_str(), channel->getTopic().c_str(), "+k", (*it).c_str());
                    }
                }
                else if (mode == "-k")
                {
                    channel->setPassword(false, "");
                    sprintf(_tmp, MSG_MODE, args.user->getNickname().c_str(), channel->getTopic().c_str(), "-k", "");
                }
            }
        }
        else
            sprintf(_tmp, ERR_UNKNOWNMODE, "ft_irc", args.user->getNickname().c_str(), mode.c_str());
    }
    else
        sprintf(_tmp, ERR_NOSUCHCHANNEL, "ft_irc", args.user->getNickname().c_str(), (*it).c_str());
    sendMsg(args.user, _tmp);
}

// ---------------------------------------------------------------
// [ PING ] - answer to ping

Ping::Ping(CommandArgs args)
{
    sprintf(_tmp, MSG_PONG, args.user->getNickname().c_str(), args.buffer[1].c_str());
    sendMsg(args.user, _tmp);
}

// ---------------------------------------------------------------
// [ Kick ] - kick a user from a channel

Kick::Kick(CommandArgs args)
{
    vector::iterator::string it = args.buffer.begin();
    it++;
    std::string channelName = *it;
    channelName = channelName.substr(1);
    it++;
    std::string toKick = *it;
    std::string reason = "";
    if (it != args.buffer.end())
        reason = *it;
    if (args.channel->isOperator(args.user) == false)
        sprintf(_tmp, ERR_NOTCHNLOPER, "ft_irc", args.user->getNickname().c_str(), args.channel->getTopic().c_str());
    else
    {
        Client *client = args.server->getClient(toKick);
        std::cout << "channel" << channelName << std::endl;
        Channel *channel = args.server->getChannel(channelName);
        if (client == NULL)
            sprintf(_tmp, ERR_NOSUCHNICK, "ft_irc", args.user->getNickname().c_str(), toKick.c_str());
        else if (channel == NULL)
            sprintf(_tmp, ERR_NOSUCHCHANNEL, "ft_irc", args.user->getNickname().c_str(), ("#" +channel->getTopic()).c_str());
        else if (channel->isClient(client) == false)
            sprintf(_tmp, ERR_USERNOTINCHANNEL, "ft_irc", args.user->getNickname().c_str(), toKick.c_str(), ("#" +channel->getTopic()).c_str());
        else
        {
            channel->rmClient(client);
            sprintf(_tmp, MSG_KICK, args.user->getNickname().c_str(), ("#" +channel->getTopic()).c_str(), toKick.c_str(), reason.c_str());
            map::client::iterator it = channel->getClients().begin();
            while (it != channel->getClients().end())
            {
                sendMsg((*it).first, _tmp);
                it++;
            }
            return ;
        }
    }
    sendMsg(args.user, _tmp);
}

// ---------------------------------------------------------------
// [ Invite ] - invite a user to a channel

Invite::Invite(CommandArgs args)
{
    vector::iterator::string it = args.buffer.begin();
    it++;
    std::string toInvite = *it;
    it++;
    std::string channelName = *it;
    if (args.channel->isOperator(args.user) == false)
        sprintf(_tmp, ERR_NOTCHNLOPER, "ft_irc", args.user->getNickname().c_str(), args.channel->getTopic().c_str());
    else
    {
        Client *client = args.server->getClient(toInvite);
        if (client == NULL)
            sprintf(_tmp, ERR_NOSUCHNICK, "ft_irc", args.user->getNickname().c_str(), toInvite.c_str());
        else
        {
            Channel *channel = args.server->getChannel(channelName);
            if (channel == NULL)
            {
                channel = new Channel(channelName);
                args.server->addChannel(channel);
            }
            channel->addClient(client);
            sprintf(_tmp, MSG_INVITE, args.user->getNickname().c_str(), toInvite.c_str(), channelName.c_str());
        }
    }
    sendMsg(args.user, _tmp);
}

// ---------------------------------------------------------------
// [ Topic ] - change channel topic

Topic::Topic(CommandArgs args)
{
    vector::iterator::string it = args.buffer.begin();
    it++;
    std::string topic = *it;
    if (args.channel->isOperator(args.user) == false)
        sprintf(_tmp, ERR_NOTCHNLOPER, "ft_irc", args.user->getNickname().c_str(), args.channel->getTopic().c_str());
    else if (args.channel->isTopicRestricted() == true)
        sprintf(_tmp, ERR_CHANOPRIVSNEEDED, "ft_irc", args.user->getNickname().c_str(), args.channel->getTopic().c_str(), "Topic Restricted");
    else if (args.server->isChannelNameAvailable(topic) == false)
        sprintf(_tmp, ERR_CHANAMEINUSE, "ft_irc", args.user->getNickname().c_str(), topic.c_str());
    else
    {
        args.channel->setTopic(topic);
        sprintf(_tmp, MSG_TOPIC, args.user->getNickname().c_str(), args.channel->getTopic().c_str(), topic.c_str());
    }
    sendMsg(args.user, _tmp);
}

Quit::Quit(CommandArgs args)
{
    vector::iterator::string it = args.buffer.begin();
    it++;
    std::string reason = *it;
    sprintf(_tmp, ":%s QUIT :%s\r\n", args.user->getNickname().c_str(), reason.c_str());
    sendMsg(args.user, _tmp);
	std::cout << "----------------------------------------" << std::endl;
	std::cout << "Client disconnected " << "[" << args.server->getClients().size() << "] " << RED << "- 1" << ENDC << std::endl;
	std::cout << "Nickname: " << RED << args.user->getNickname() << ENDC << std::endl;
	std::cout << "Username: " << RED << args.user->getUsername() << ENDC << std::endl;
    std::cout << "Realname: " << RED << args.user->getRealname() << ENDC << std::endl;
    vector::channel channels = args.server->getChannels();
    vector::iterator::channel itChannel = channels.begin();
    while (itChannel != channels.end())
    {
        if ((*itChannel)->isClient(args.user) == true)
            (*itChannel)->rmClient(args.user);
    }
    vector::client clients = args.server->getClients();
    vector::iterator::client itClient = clients.begin();
    while (itClient != clients.end())
    {
        if ((*itClient)->getNickname() != args.user->getNickname())
        {
            sprintf(_tmp, ":%s QUIT :%s\r\n", args.user->getNickname().c_str(), reason.c_str());
            sendMsg((*itClient), _tmp);
        }
        itClient++;
    }
    args.server->rmClient(args.user);
}

CAP::CAP(CommandArgs args)
{
    sprintf(_tmp, CAP_REPLY_2, args.user->getNickname().c_str());
    sendMsg(args.user, _tmp);
}