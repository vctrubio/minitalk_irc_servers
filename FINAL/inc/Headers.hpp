#ifndef HEADERS_HPP
# define HEADERS_HPP

# include   <cstdlib>
# include   <ctime>
# include   <typeinfo>
# include   <memory>
# include   <limits>
# include   <cfloat>
# include   <cerrno>
# include   <string>
# include   <algorithm>
# include   <vector>
# include   <deque>
# include   <list>
# include   <map>
# include   <iterator>
# include   <fstream>
# include   <iostream>
# include   <iomanip>
# include   <dirent.h>
# include   <unistd.h>
# include   <signal.h>
# include   <sstream>
# include   <termios.h>
# include   <stdexcept>
# include   <exception>
# include   <sys/socket.h>
# include   <sys/select.h>
# include   <sys/un.h>
# include   <sys/time.h>
# include   <sys/types.h>
# include   <arpa/inet.h>
# include   <netdb.h>

# define    MAX_CLIENTS             8
# define    PORT                    8000
# define    BUFF_SIZE               1000
# define    WIDTH                   100
# define    HEIGHT                  20
# define    BLACK                  	"\033[0;30m"
# define    RED	                    "\033[0;31m"
# define    GREEN	                "\033[0;32m"
# define    YELLOW	                "\033[0;33m"
# define    BLUE	                "\033[0;34m"
# define    PURPLE	                "\033[0;35m"
# define    CYAN	                "\033[0;36m"
# define    WHITE	                "\033[0;37m"
# define    ENDC	                "\033[0m"
# define    BOLD	                "\033[1m"
# define    COMMANDS                { "NICK", "USER", "JOIN", "PART", "NAMES", "LIST", "TOPIC", "KICK", "INVITE", "PRIVMSG", "QUIT", "PING", "WHO", "WHOIS", "MODE" , NULL }
# define    COMMAND_COUNT           16
# define    CAP_REPLY_1             ":ft_irc CAP * LS :multi-prefix message-tags account-notify userhost-in-names join-notify mode-private mode-password mode-userlimit mode-topicrestrict privmsg channelmsg\r\n"
# define    CAP_REPLY_2             ":ft_irc CAP %s ACK :multi-prefix account-notify userhost-in-names\r\n"
# define    NOTICE_USER_JOINED      ":%s NOTICE %s :User %s has joined the server\r\n"
# define    BOT_IDENTIFY            "PRIVMSG %s :identify %s\r\n"
# define    BOT_PASSWORD            "PASS %s\r\n"
# define    BOT_NICKNAME            "NICK %s\r\n"
# define    BOT_USERNAME            "USER %s 0 * :%s\r\n"
# define    BOT_ANSWER              "%s %s :%s\r\n"
# define    MSG_JOIN                ":%s JOIN :%s\r\n"
# define    MSG_PART                ":%s PART :%s\r\n"
# define    MSG_NICK                ":ft_irc 001 %s : Your nickname is %s\r\n"
# define    MSG_USER                "%s USER %s\r\n"
# define    MSG_PRIVMSG             ":%s PRIVMSG %s :%s\r\n"
# define    MSG_PONG                ":ft_irc PONG :%s\r\n"
# define    MSG_PING                ":ft_irc PING :%s\r\n"
# define    MSG_TOPIC               ":%s TOPIC %s :%s\r\n"
# define    MSG_KICK                ":%s KICK %s %s :%s\r\n"
# define    MSG_INVITE              ":%s INVITE %s %s\r\n"
# define    MSG_NAMES               ":%s NAMES %s :%s\r\n"
# define    MSG_WHOIS               ":%s WHOIS %s :%s\r\n"
# define    MSG_MODE                ":%s MODE %s %s :%s\r\n"
# define    MSG_NOTICE              ":%s NOTICE %s :%s\r\n"
# define    MSG_WHO                 ":%s 352 %s %s %s %s %s H :0 %s\r\n"
# define    RPL_WELCOME             ":%s 001 %s :Welcome to the Internet Relay Network %s\r\n"
# define    RPL_YOURHOST            ":%s 002 %s :Your host is %s, running version %s\r\n"
# define    RPL_CREATED             ":%s 003 %s :This server was created %s\r\n"
# define    RPL_MYINFO              ":%s 004 %s %s %s %s %s\r\n"
# define    RPL_LUSERCLIENT         ":%s 251 %s :There are %d users and %d services on %d servers\r\n"
# define    RPL_LUSEROP             ":%s 252 %s %d :operator(s) online\r\n"
# define    RPL_LUSERUNKNOWN        ":%s 253 %s %d :unknown connection(s)\r\n"
# define    RPL_LUSERCHANNELS       ":%s 254 %s %d :channels formed\r\n"
# define    RPL_LUSERME             ":%s 255 %s :I have %d clients and %d servers\r\n"
# define    RPL_LISTSTART           ":%s 321 %s Channel :Users Name\r\n"
# define    RPL_LIST                ":%s 322 %s %s %d :%s\r\n"
# define    RPL_LISTEND             ":%s 323 %s :End of /LIST\r\n"
# define    RPL_CHANNELMODEIS       ":%s 324 %s %s %s\r\n"
# define    RPL_TOPIC               ":%s 332 %s %s :%s\r\n"
# define    RPL_NAMREPLY            ":%s 353 %s = %s :%s\r\n"
# define    RPL_ENDOFNAMES          ":%s 366 %s %s :End of /NAMES list\r\n"
# define    RPL_WHOISUSER           ":%s 311 %s %s %s %s * :%s\r\n"
# define    RPL_WHOISSERVER         ":%s 312 %s %s %s :%s\r\n"
# define    RPL_WHOISCHANNELS       ":%s 319 %s %s :%s\r\n"
# define    RPL_ENDOFWHOIS          ":%s 318 %s %s :End of /WHOIS list\r\n"
# define    ERR_NOSUCHNICK          ":%s 401 %s %s :No such nick\r\n"
# define    ERR_NOTCHNLOPER         ":%s 482 %s %s :You're not channel operator\r\n"
# define    ERR_UNKNOWNMODE         ":%s 472 %s %s :is unknown mode char to me\r\n"
# define    ERR_UNKNOWNCOMMAND      ":%s 421 %s %s :Unknown command\r\n"
# define    ERR_NICKNAMEINUSE       ":%s 433 %s %s :Nickname is already in use\r\n"
# define    ERR_CHANAMEINUSE        ":%s 400 %s %s :Channel name is already in use\r\n"
# define    ERR_USERONCHANNEL       ":%s 443 %s %s :is already on channel\r\n"
# define    ERR_CHANOPRIVSNEEDED    ":%s 482 %s %s :Change first the channel mode (%s)\r\n"
# define    ERR_NOSUCHCHANNEL       ":%s 403 %s %s :No such channel\r\n"
# define    ERR_NEEDMOREPARAMS      ":%s 461 %s %s :Not enough parameters\r\n"
# define    ERR_USERNOTINCHANNEL    ":%s 441 %s %s %s :They aren't on that channel\r\n"
# define    ERR_BANNEDFROMCHAN      ":%s 474 %s %s :Cannot join channel (+b)\r\n"
# define    ERR_INVITEONLYCHAN      ":%s 473 %s %s :Cannot join channel (+i)\r\n"
# define    ERR_BADCHANNELKEY       ":%s 475 %s %s :Cannot join channel (+k)\r\n"
# define    ERR_CHANNELISFULL       ":%s 471 %s %s :Cannot join channel (+l)\r\n"
# define    ERR_TOPICRESTRICTED     ":%s 480 %s %s :Channel is topic restricted\r\n"
# define    ERR_KEYSET              ":%s 467 %s %s :Channel key already set\r\n"
# define    ERR_CANTKILLSERVER      ":%s 483 %s %s :Dont kick yourself, leave the channel instead\r\n"
# define    ERR_BADCHANMASK         ":%s 476 %s :Bad Channel Mask. %s\r\n"

class Client;
class Channel;
class Server;

enum ModeType { setInviteOnly, removeInviteOnly, setTopicRestricted, removeTopicRestricted, setPassword, removePassword, setUserLimit, removeUserLimit, setUserOperator, removeUserOperator };

namespace vector
{
    namespace iterator
    {
        typedef std::vector<std::string>::iterator    string;
        typedef std::vector<Client *>::iterator       client;
        typedef std::vector<Channel *>::iterator      channel;
    };
    typedef std::vector<std::string>    string;
    typedef std::vector<Client *>       client;
    typedef std::vector<Channel *>      channel;
};

namespace map
{
    typedef std::map<Client *, bool> client;
    namespace pair { typedef std::pair<Client *, bool> client; };
    namespace iterator { typedef std::map<Client *, bool>::iterator client; };
};

typedef struct CommandArgs
{
    vector::string      buffer;
    Client              *user;
    Server              *server;
} CommandArgs;

typedef struct BotArgs
{
    std::string         who;
    std::string         cmd;
    std::string         to;
    std::string         msg;
} BotArgs;

namespace std
{
    template <typename T>
    std::string to_string(T value)
    {
        std::ostringstream os;
        os << value;
        return os.str();
    }
};

template <typename T>
bool isNull(T container, typename T::iterator it)
{
    if (it == container.end())
        return true;
    return false;
};

# include   "Client.hpp"
# include   "Channel.hpp"
# include   "Utils.hpp"
# include   "Server.hpp"
# include   "Log.hpp"
# include   "Socket.hpp"
# include   "Commands.hpp"

#endif
