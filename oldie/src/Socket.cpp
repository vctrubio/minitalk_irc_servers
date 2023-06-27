#include "../inc/Socket.hpp"

/**
 * @brief Starts the socket
 * 
 * @details Creates a socket, binds it to the address and port, and starts listening
*/
void	Socket::init()
{
	int opt = 1; 

	for (int i = 0; i < MAX_CLIENTS; i++)
		_clientSocket[i] = 0;
	_sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockFd < 0)
		throw std::runtime_error("Failed to set create socket");
	else
	{
		if (setsockopt(_sockFd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
			throw std::runtime_error("Failed to set socket options");
	}
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = INADDR_ANY;
	_addr.sin_port = htons(_port);

	if (bind(_sockFd, (struct sockaddr *)&_addr, sizeof(_addr)) < 0)
		throw std::runtime_error("Failed to bind socket");
	if (listen(_sockFd, MAX_CLIENTS) < 0)
		throw std::runtime_error("Failed to listen");

	std::cout << GREEN << "The server is up and runnig" << std::endl;
	std::cout << BLUE << "Server Size: " << GREEN << MAX_CLIENTS << ENDC << std::endl;
	std::cout << BLUE << "Address: " << GREEN << inet_ntoa(_addr.sin_addr) << ENDC << std::endl;
	std::cout << BLUE << "Port: " << GREEN << _port << ENDC << std::endl;
}

/**
 * @brief Adds a new user to the server
 * 
 * @param fd: the file descriptor of the user
*/
bool Socket::addUser(int fd)
{
	vector::string	buffer;
	std::string nick = "";
	std::string user = "";
	std::string real = "";
	std::string password = "";
	while (nick.empty() || user.empty())
	{
		buffer = readBuffer(fd);
		getLog()->CommandReceived(buffer, NULL);
		for (vector::iterator::string it = buffer.begin(); it != buffer.end(); it++)
		{
			if (it->find("CAP LS") != std::string::npos)
			{
				send(fd, CAP_REPLY_1, strlen(CAP_REPLY_1), 0);
				getLog()->CommandSent(CAP_REPLY_1, NULL);
			}
			else if (it->find("NICK") != std::string::npos)
			{
				nick = getXWord(removeSpaces(extractLine("NICK", buffer)), 0);
				if (_server->isNicknameAvailable(nick) == false)
				{
					std::string errNick = ":localhost 433 * " + nick + " :Nickname is already in use\r\n";
					send(fd, errNick.c_str(), errNick.size(), 0);
					getLog()->CommandSent(errNick, NULL);
					nick = "";
				}
			}
			else if (it->find("USER") != std::string::npos)
			{
				user = getXWord(removeSpaces(extractLine("USER", buffer)), 0);
				real = getXWord(removeSpaces(extractLine("USER", buffer)), 3);
				real = real.substr(1);
			}
			else if (it->find("PASS") != std::string::npos)
			{
				password = getXWord(removeSpaces(extractLine("PASS", buffer)), 0);
				if (_password.compare(password) != 0)
				{
					std::cout << "Password received: [" << password << "]" <<std::endl;
					std::cout << "Password expected: [" << _password << "]" << std::endl;
					std::string errPass = ":localhost 464 * :Password incorrect\r\n";
					send(fd, errPass.c_str(), errPass.size(), 0);
					close(fd);
					getLog()->CommandSent(errPass, NULL);
					return false;
				}
			}
		}
	}
	std::cout << "----------------------------------------" << std::endl;
	std::cout << "New client connected " << "[" << _server->getClients().size() << "] " << GREEN << "+ 1" << ENDC << std::endl;
	std::cout << "Nickname: " << GREEN << nick << ENDC << std::endl;
	std::cout << "Username: " << GREEN << user << ENDC << std::endl;
	std::cout << "Realname: " << GREEN << real << ENDC << std::endl;
	_server->addClient(new Client(user, nick, real, fd));
	std::string welcome = ":ft_irc 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + "\r\n";
	send(fd, welcome.c_str(), welcome.size(), 0);
	getLog()->CommandSent(welcome, _server->getClient(fd));
	return true;
}

/**
 * @brief Reads the buffer and calls the function to handle the command
 * 
 * @param buffer: the buffer to read
 * @param sd: the socket descriptor
*/
void	Socket::init_cmd(std::string buffer, int sd)
{
	_server->setRequestCall(_server->getClient(sd));
	vector::string newBuffer = buildVector(buffer);
	getLog()->CommandReceived(buffer, _server->getClient(sd));
	_server->findCommand(newBuffer);
}

/**
 * @brief Handles the incoming connection
 * 
 * @param addrlen: the length of the address
 * @param tmp_socket: the socket descriptor
 * 
 * @return true if the connection is accepted, false otherwise
*/
bool	Socket::incomingConnection(const int &addrlen, int &tmp_socket)
{
	std::string	password;

	if ((tmp_socket = accept(_sockFd, (struct sockaddr *)&_addr, (socklen_t *)&addrlen)) < 0)
		throw std::runtime_error ("Tmp socket failed");
	if (_server->getClients().size() >= MAX_CLIENTS)
	{
		std::string errMax = ":localhost 421 * :Too many connections (max: " + std::to_string(MAX_CLIENTS) + ")" + "\r\n";
		send(tmp_socket, errMax.c_str(), errMax.size(), 0);
		close(tmp_socket);
		getLog()->CommandSent(errMax, NULL);
		return false;
	}
	if (!addUser(tmp_socket))
		return false;
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (_clientSocket[i] == 0)
		{
			_clientSocket[i] = tmp_socket;
			break;
		}
	}
	return true;
}

/**
 * @brief Parses the buffer of the clients
 * 
 * @param addrlen: the length of the address
 * 
 * @return true if the buffer is parsed, false if the client is disconnected
*/
bool Socket::parseClientsBuffer(const int &addrlen, int &sd)
{
	int valread;
	char buffer[265];

	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		sd = _clientSocket[i];
		if (FD_ISSET(sd, &_readFds))
		{
			if ((valread = read(sd, buffer, 254)) == 0 || strncmp(buffer, "/exit", 5) == 0)
			{
				getpeername(sd, (struct sockaddr *)&_addr, (socklen_t *)&addrlen);
				_server->rmClient(_server->getClient(sd));
				close(sd);
				_clientSocket[i] = 0;
			}
			else
			{
				if (valread <= 0)
					return false;
				buffer[valread] = '\0';
				std::string trimBuffer = buffer;
				for (int x = 0; trimBuffer[x] != '\0'; x++)
				{
					if (trimBuffer[x] == '\r')
					{
						trimBuffer.resize(valread - 2);
						break ;
					}
					if (trimBuffer[x] == '\n')
					{
						trimBuffer.resize(valread - 1);
						break ;
					}

				}
				init_cmd(trimBuffer, sd);
			}
		}
	}
	return true;
}


/**
 * @brief Runs the server
 * 
 * @details The server runs in an infinite loop and waits for incoming connections
*/
void Socket::run()
{
	int max_sd, sd, tmp_socket, addrlen = sizeof(_addr);

	while (42)
	{
		FD_ZERO(&_readFds);
		FD_SET(_sockFd, &_readFds);
		max_sd = _sockFd;
		for (int i = 0; i < MAX_CLIENTS; i++)
		{
			sd = _clientSocket[i];
			if (sd > 0 && sd < FD_SETSIZE)
				FD_SET(sd, &_readFds);
			if (sd > max_sd)
				max_sd = sd;
		}
		_activity = select(max_sd + 1, &_readFds, NULL, NULL, NULL);
		if ((_activity < 0)) // && (errno!=EINTR))
			throw std::runtime_error ("Select error");
		if (FD_ISSET(_sockFd, &_readFds))
		{
			if (!incomingConnection(addrlen, tmp_socket))
				continue;
		}
		if (!parseClientsBuffer(addrlen, sd))
			continue;
		_server->cleanEmptyChannels();
	}
}