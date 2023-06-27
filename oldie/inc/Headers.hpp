#ifndef HEADERS_HPP
# define HEADERS_HPP

#include <cstdlib>
#include <ctime>
#include <typeinfo>
#include <memory>
#include <limits>
#include <cfloat>
#include <cerrno>
#include <string>
#include <algorithm>
#include <vector>
#include <deque>
#include <list>
#include <map>
#include <iterator>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <dirent.h>
#include <unistd.h>
#include <signal.h>
#include <sstream>

#include <stdexcept>
#include <exception>

#include <sys/socket.h>
#include <sys/select.h>
#include <sys/un.h>
#include <sys/time.h>
#include <sys/types.h>
#include <arpa/inet.h>

# define MAX_CLIENTS 8
# define PORT 8000
# define BUFF_SIZE 1000
# define WIDTH 100
# define HEIGHT 20

# define BLACK	"\033[0;30m"
# define RED	"\033[0;31m"
# define GREEN	"\033[0;32m"
# define YELLOW	"\033[0;33m"
# define BLUE	"\033[0;34m"
# define PURPLE	"\033[0;35m"
# define CYAN	"\033[0;36m"
# define WHITE	"\033[0;37m"
# define ENDC	"\033[0m"


# define    COMMANDS        { "NICK", "USER", "JOIN", "PART", "NAMES", "LIST", "TOPIC", "KICK", "INVITE", "PRIVMSG", "QUIT", "PING", "WHO", "WHOIS", "MODE" , NULL }
# define    COMMAND_COUNT   16

namespace std
{
    // to replace std::to_string() for C++ versions < C++11
    template <typename T>
    std::string to_string(T value)
    {
        std::ostringstream os;
        os << value;
        return os.str();
    }
}

class Client;
class Channel;


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
    namespace iterator
    {
        typedef std::map<Client *, bool>::iterator    client;
    };
    namespace pair
    {
        typedef std::pair<Client *, bool>    client;
    };
    typedef std::map<Client *, bool>    client;
};

#endif
