#ifndef HEADERS_HPP
#define HEADERS_HPP

#include <cstdlib>
#include <ctime>
#include <typeinfo>
#include <memory>
#include <limits>
#include <cfloat>
#include <cerrno>
#include <string>
#include <algorithm>
#include <array>
#include <vector>
#include <tuple>
#include <deque>
#include <list>
#include <map>
#include <iterator>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <dirent.h>
#include <unistd.h>
#include <sstream>

#include <stdexcept>
#include <exception>

#include <sys/socket.h>
#include <sys/select.h>
#include <sys/un.h>
#include <sys/time.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 5
#define PORT 8000
#define BUFF_SIZE 1000
#define WIDTH 100
#define HEIGHT 20

# define BLACK	"\033[0;30m"
# define RED	"\033[0;31m"
# define GREEN	"\033[0;32m"
# define YELLOW	"\033[0;33m"
# define BLUE	"\033[0;34m"
# define WHITE	"\033[0;37m"
# define ENDC	"\033[0m"

using namespace std;

enum post
{
    JOIN,
    MSSG,
    PRIVATE,
    LEAVE,
    KICK,
};

#endif
