#ifndef UTILS_HPP
#define UTILS_HPP

#include "Headers.hpp"

class Channel;
class Client;

vector::string	buildVector(std::string str);

class SignalException : public std::exception
{
	public:
        SignalException(int signum);
		virtual const char* what() const throw();
    
    private:
        int _signum;
};

std::string buildChannelPermissions(Channel *channel, Client *client);
std::string removeSpaces(std::string str);
std::string getXWord(std::string str, int x);
std::vector<std::string> readBuffer(int socketFd);
std::string	extractLine(std::string token, std::vector<std::string> buffer);
bool	isCommand(const std::string &buffer);

//FOR UI Interface
void	welcome();
void	putUnderScore(std::string &out);
void	putLine(std::string &out, char c);
void	putRight(std::string in, std::string &out, char c);
void	putChannel(Channel *channel, std::string &out, int i, int p);

#include "Channel.hpp"
#include "Client.hpp"


#endif
