#ifndef UTILS_HPP
# define UTILS_HPP

# include "Headers.hpp"

vector::string	buildVector(std::string str);
void	sighandler(int signum);

namespace std
{
  std::string toupper(const std::string &str);
};

class SignalException : public std::exception
{
	public:
        SignalException(int signum);
		virtual const char* what() const throw();
    
    private:
        int _signum;
        char _msg[20];
};

std::string buildChannelPermissions(Channel *channel, Client *client);
std::string removeSpaces(std::string str);
std::string getXWord(std::string str, int x);
std::vector<std::string> readBuffer(int socketFd);
std::string	extractLine(std::string token, std::vector<std::string> buffer);
bool	isCommand(const std::string &buffer);
int	check_port(char *arg);



#endif
