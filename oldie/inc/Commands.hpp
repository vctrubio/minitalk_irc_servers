#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "Headers.hpp"
# include "Utils.hpp"

# define    CAP_REPLY_1  ":ft_irc CAP * LS :multi-prefix message-tags account-notify userhost-in-names join-notify mode-private mode-password mode-userlimit mode-topicrestrict privmsg channelmsg\r\n"
# define    CAP_REPLY_2  ":ft_irc CAP %s ACK :multi-prefix account-notify userhost-in-names\r\n"


# define    MSG_JOIN    ":%s JOIN #%s\r\n"
# define    MSG_PART    ":%s PART #%s\r\n"
# define    MSG_NICK    ":%s NICK %s\r\n"
# define    MSG_USER    "%s USER %s\r\n"
# define    MSG_PRIVMSG ":%s PRIVMSG %s :%s\r\n"
# define    MSG_PONG    ":%s PONG %s\r\n"
# define    MSG_PING    ":%s PING %s\r\n"
# define    MSG_TOPIC   ":%s TOPIC %s :%s\r\n"
# define    MSG_KICK    ":%s KICK %s %s :%s\r\n"
# define    MSG_INVITE  ":%s INVITE %s %s\r\n"
# define    MSG_LIST    ":%s LIST %s :%s\r\n"
# define    MSG_NAMES   ":%s NAMES %s :%s\r\n"
# define    MSG_WHOIS   ":%s WHOIS %s :%s\r\n"
# define    MSG_MODE    ":%s MODE #%s %s %s\r\n"
# define    MSG_WHO     ":%s 352 %s %s %s %s %s %s :%s\r\n"

# define    RPL_CHANNELMODEIS   ":%s 324 %s %s %s\r\n"

# define    ERR_NOSUCHNICK      ":%s 401 %s %s :No such nick/channel\r\n"
# define    ERR_NOTCHNLOPER     ":%s 482 %s #%s :You're not channel operator\r\n"
# define    ERR_UNKNOWNMODE     ":%s 472 %s %s :is unknown mode char to me\r\n"
# define    ERR_UNKNOWNCOMMAND  ":%s 421 %s %s :Unknown command\r\n"
# define    ERR_NICKNAMEINUSE   ":%s 433 %s %s :Nickname is already in use\r\n"
# define    ERR_CHANAMEINUSE    ":%s 400 %s %s :Channel name is already in use\r\n"
# define    ERR_USERONCHANNEL   ":%s 443 %s %s :is already on channel\r\n"
# define    ERR_CHANOPRIVSNEEDED ":%s 482 %s %s :Change first the channel mode (%s)\r\n"
# define    ERR_NOSUCHCHANNEL    ":%s 403 %s %s :No such channel\r\n"
# define    ERR_NEEDMOREPARAMS  ":%s 461 %s %s :Not enough parameters\r\n"
# define    ERR_USERNOTINCHANNEL ":%s 441 %s %s %s :They aren't on that channel\r\n"

class Client;
class Channel;
class Server;


typedef struct CommandArgs
{
    vector::string  buffer;
    Client          *user;
    Channel         *channel;
    Server          *server;
} CommandArgs;


class Command
{
    public:
        Command() { memset(_tmp, 0, 1024); }
        virtual ~Command() {}
        void    sendMsg(Client *user, const std::string &msg) { send(user->getSocket(), msg.c_str(), msg.length(), 0); getLog()->CommandSent(msg, user); }

    protected:
        char            _tmp[1024];

};

class Nick : public Command
{
    public:
        Nick(CommandArgs args);
        ~Nick() {}
};

class User : public Command
{
    public:
        User(CommandArgs args);
        ~User() {}
};

class Join : public Command
{
    public:
        Join(CommandArgs args);
        ~Join() {}
};

class Part : public Command
{
    public:
        Part(CommandArgs args);
        ~Part() {}
};

class Names : public Command
{
    public:
        Names(CommandArgs args);
        ~Names() {}
};

class List : public Command
{
    public:
        List(CommandArgs args);
        ~List() {}
};

class Topic : public Command
{
    public:
        Topic(CommandArgs args);
        ~Topic() {}

};

class Kick : public Command
{
    public:
        Kick(CommandArgs args);
        ~Kick() {}
};

class Invite : public Command
{
    public:
        Invite(CommandArgs args);
        ~Invite() {}

};

class Privmsg : public Command
{
    public:
        Privmsg(CommandArgs args);
        ~Privmsg() {}
};

class Ping : public Command
{
    public:
        Ping(CommandArgs args);
        ~Ping() {}
};

class Who : public Command
{
    public:
        Who(CommandArgs args);
        ~Who() {}
};


class Mode : public Command
{
    public:
        Mode(CommandArgs args);
        ~Mode() {}
};

class Quit : public Command
{
    public:
        Quit(CommandArgs args);
        ~Quit() {}
};


class CAP : public Command
{
    public:
        CAP(CommandArgs args);
        ~CAP() {}
};

# include "Client.hpp"
# include "Channel.hpp"
# include "Server.hpp"
# include "Log.hpp"

#endif