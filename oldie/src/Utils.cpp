#include "../inc/Utils.hpp"

SignalException::SignalException(int signum) : _signum(signum) {}
const char* SignalException::what() const throw() {return (_signum == SIGINT) ? "SIGINT" : (_signum == SIGTSTP) ? "SIGTSTP" : (_signum == SIGQUIT) ? "SIGQUIT" : "Unknown signal received"; }

std::string buildChannelPermissions(Channel *channel, Client *client)
{
	std::string	permissions;
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

/*
 * Sends the welcome message to the client.
*/
void	welcome()
{

	std::string	mssg = GREEN;
	mssg += "Connected\n";
	mssg += ENDC;
	mssg += "UI: Please provide a big enough screen; you should see this message in a single line (80CHARS_MIN)\n";
	mssg += "/help for CMD instructions.\n";
	mssg += "/doc for IRC documentation.\n";
	mssg += "/join channel to connect to #channels\n";

	std::cout << mssg;	
//	send(sockfd, mssg.c_str(), mssg.size(), 0);
}

/*
 * Builds a decorated string.
*/
void	putUnderScore(std::string &out)
{
	int i = 0;
	for (int size = 0; size < WIDTH + 1;) //topic_underline
	{
		if (size == 0 || size == WIDTH)
			out += "|";
		else
		{
			if (i % 2 == 0)
				out += '-';
			else
				out += ' ';
			i++;
		}
		++size;
	}
	out += "\n";
}

/*
 * Builds a decorated string.
*/
void	putLine(std::string &out, char c)
{
	for (int size = 0; size < WIDTH + 1;) //topic
	{
		if (size == 0 || size == WIDTH)
			out += "|";
		else
			out += c;
		++size;
	}
	out += "\n";
}


/*
 * Builds a decorated string.
*/
void	putRight(std::string in, std::string &out, char c)
{
	int size; 

	for (size = 0; size < WIDTH + 2;) //topic
	{
		if (size == 0 || size == WIDTH + 1)
			out += "|";
		else if (size <= (int)in.length() - 1)
		{
			out += in;
			size += in.length();
		}
		else
			out += c;
		++size;
	}
	out += "\n";

} 

