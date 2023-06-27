#include "../inc/Utils.hpp"

SignalException::SignalException(int signum) : _signum(signum) { sprintf(_msg, "Signal %d", _signum); }
const char* SignalException::what() const throw() {return _msg;}

std::string buildChannelPermissions(Channel *channel, Client *client)
{
	std::string	permissions = "+";
	if (channel->isInviteOnly())
		permissions += "i";
	if (channel->isTopicRestricted())
		permissions += "t";
	if (channel->hasUserLimit())
		permissions += "l";
	if (channel->hasPassword())
		permissions += "k";
	if (channel->isOperator(client))
		permissions += "o";
	if (channel->isBanned(client))
		permissions += "b";
	if (channel->isInvited(client))
		permissions += "I";
	if (permissions == "+")
		permissions = "";
	return permissions;
}

bool	isCommand(const std::string &buffer)
{
	const char *commands[] = COMMANDS;
	for (int i = 0; commands[i]; i++)
		if (buffer == commands[i])
			return true;
	return false;
}

vector::string readBuffer(int socketFd)
{
	char buffer[1024];
	int	valread = read(socketFd, buffer, 1024);
	vector::string	ptr;
	std::string		tmp;
	int				i = 0;
	if (valread < 0)
	{
		close(socketFd);
		throw std::runtime_error("Read error");
	}
	while (i < valread)
	{
		if (buffer[i] == '\r' && buffer[i + 1] == '\n')
		{
			if (buffer[i] == '\r' && buffer[i + 1] == '\n')
				i++;
			ptr.push_back(tmp);
			tmp.clear();
		}
		else
			tmp += buffer[i];
		i++;
	}
	ptr.push_back(tmp);
	return ptr;
}

std::string	extractLine(std::string token, std::vector<std::string> buffer)
{
	std::string line;
	for (vector::iterator::string it = buffer.begin(); it != buffer.end(); it++)
	{
		if (it->find(token) != std::string::npos)
		{
			if (token.length() <= it->length())
				line = it->substr(token.length());
			return line;
		}
	}
	return "";
}

/**
 * @brief removes the spaces from a string (beginning and end)
 * 
 * @param str: the string to trim
*/
std::string removeSpaces(std::string str)
{
	std::string::iterator it;
	for (it = str.begin(); it != str.end() && *it == ' '; it++);
	str.erase(str.begin(), it);
	for (it = str.end() - 1; it != str.begin() && *it == ' '; it--);
	str.erase(it + 1, str.end());
	return str;
}


/**
 * @brief returns the Xth word of a string, starting from 0, 0 is the first word
 * 
 * @param str: the string to trim
*/
std::string getXWord(std::string str, int x)
{
	std::string::iterator startOfTheWord = str.begin();
	std::string::iterator endOfTheWord = str.begin();
	int i = -1;
	while (i < x)
	{
		startOfTheWord = endOfTheWord;
		while (startOfTheWord != str.end() && (*startOfTheWord == ' ' || *startOfTheWord == '\t' || *startOfTheWord == '\r' || *startOfTheWord == '\n'))
			startOfTheWord++;
		endOfTheWord = startOfTheWord;
		while (endOfTheWord != str.end() && *endOfTheWord != ' ' && *endOfTheWord != '\t' && *endOfTheWord != '\r' && *endOfTheWord != '\n')
			endOfTheWord++;
		i++;
	}
	return std::string(startOfTheWord, endOfTheWord);
}

/*
 * Split a string into a vector of strings by the delimiter ' '.
*/
vector::string	buildVector(std::string buffer)
{
	vector::string		tokens;
	std::string			tmp;
	std::stringstream	ss(buffer);
	while (std::getline(ss, tmp, ' '))
		tokens.push_back(tmp);
	return tokens;
}

std::string std::toupper(const std::string &str)
{
	std::string tmp = const_cast<std::string &>(str);
	for (std::string::iterator it = tmp.begin(); it != tmp.end(); it++)
		*it = std::toupper(*it);
	return tmp;
}

/*
 * Signal handler for Signals
*/
void	sighandler(int signum)
{
	throw SignalException(signum);
}

/*
 * Checker for port number
*/
int	check_port(char *arg)
{
	for (size_t i = 0; i < strlen(arg); i++)
	{
		if (!isdigit(arg[i])) 
		{
			return -1;
		}
	}

	std::stringstream str(arg);
	int x;
	str >> x;
	if (x > 0 && x < 65536)
		return x;
	return -1; 
}