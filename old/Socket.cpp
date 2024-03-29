#include "Socket.hpp"

Socket::Socket(int port, string password)
	:Server(password), _port(port), _password(password)
{
	int opt = 1; 

	_sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockFd < 0)
		throw runtime_error("Failed to set create socket");
	else
	{
		if (setsockopt(_sockFd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
			throw runtime_error("Failed to set socket options");
	}

	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = INADDR_ANY;
	_addr.sin_port = htons(_port);

	if (bind(_sockFd, (struct sockaddr *)&_addr, sizeof(_addr)) < 0)
		throw runtime_error ("Failed to bind");
	if (listen(_sockFd, MAX_CLIENTS) < 0)
		throw runtime_error ("Failed to listen");

	std::cout << BLUE << "The server is up and runnig" << std::endl;
	std::cout << GREEN << "Server Size: " << MAX_CLIENTS << std::endl;
	std::cout << BLUE << "Listening on port " << GREEN << _port << ENDC << std::endl;
}

void Socket::ft_add_user(int i)
{
	char*	name = getenv("USER");
	Client	*new_connection = new Client(name, i);

	addClient(new_connection);
	
	string	host = new_connection->rtnHost();
	string mssg = "Connnected: ";
	mssg += GREEN;
	mssg += host;
	mssg += ENDC;
	mssg += "\n/help for CMD instructions.\n";
	mssg += "/join channel to connect to #channels\n";
	mssg += "/nick [nickname] to change your nickname\n/name [name] to change your name\n"; //are we allowed to change Name or NICKNAME tho?
	send(i, mssg.c_str(), mssg.size(), 0);
}

void	Socket::init_cmd(string buffer, int sd)
{
	vector<string>	ptr = buildVector(buffer);

	if (buffer[0] == '/')
		find_cmd(ptr);
	else
	{
		if (getClient(sd)->hasChannel())
		{
			string	mssg = getClient(sd)->rtnName() + ": ";
			mssg += buffer;
			getClient(sd)->channels().front()->post(mssg, sd);
		}
	}
	
}

void Socket::runSocket()
{
	int addrlen = sizeof(_addr);
	int max_sd, sd, valread;
	int tmp_socket;
	char buffer[265];
	char password[1024];
	std::string	errmsgCapacity = "\033[0;31mMax Cap\n\033[0m";
	std::string	errmsgPassword = "\033[0;31mWrong Password\n\033[0m";

	while (42)
	{
		FD_ZERO(&_readFds);
		FD_SET(_sockFd, &_readFds);
		max_sd = _sockFd;

		for (int i = 0; i < MAX_CLIENTS; i++)
		{
			sd = _clientSocket[i];
			if (sd > 0)
				FD_SET(sd, &_readFds);
			if (sd > max_sd)
				max_sd = sd;
		}

		_activity = select(max_sd + 1, &_readFds, NULL, NULL, NULL);
		if ((_activity < 0) && (errno != EINTR))
			throw runtime_error ("Activity Errno");

		if (FD_ISSET(_sockFd, &_readFds))
		{

			if ((tmp_socket = accept(_sockFd, (struct sockaddr *)&_addr, (socklen_t *)&addrlen)) < 0)
				throw runtime_error ("Tmp socket failed");

			if (_clients.size() >= MAX_CLIENTS)
			{
				send(tmp_socket, errmsgCapacity.c_str(), errmsgCapacity.length(), 0);
				close(tmp_socket);
				continue;
			}
			
			send(tmp_socket, "Please enter the server's password: ", strlen("Please enter the server's password: "), 0);
			valread = read(tmp_socket, password, 1024);
			if (valread < 0) {
				std::cout << "Error in valread\n";
				close(tmp_socket);
				continue;
			}

			for (int i = 0; i < (int)strlen(password); i++) 
			{        
				if (password[i] == '\r' || password[i] == '\n') 
				{
					password[i] = '\0';
					break;
				}
			}
			if (_password.compare(password) == 0)
				send(tmp_socket, "Connection Successful\n", strlen("Connection Successful\n"), 0);	
			else 
			{
				send(tmp_socket, errmsgPassword.c_str(), errmsgPassword.length() , 0);
				close(tmp_socket);
				continue;
			}
			ft_add_user(tmp_socket);
			for (int i = 0; i < MAX_CLIENTS; i++)
			{
				if (_clientSocket[i] == 0)
				{
					_clientSocket[i] = tmp_socket;
					break;
				}
			}
		}
		for (int i = 0; i < MAX_CLIENTS; i++)
		{
			sd = _clientSocket[i];
			if (FD_ISSET(sd, &_readFds))
			{
				if ((valread = read(sd, buffer, 254)) == 0 || strncmp(buffer, "/exit", 5) == 0)
				{
					getpeername(sd, (struct sockaddr *)&_addr, (socklen_t *)&addrlen);
					removeClient(getClient(sd));
					close(sd);
					_clientSocket[i] = 0;
				}
				else
				{
					buffer[valread] = '\0'; 
					string trimBuffer = string(buffer);
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
					_requestCall = getClient(sd);
					init_cmd(trimBuffer, sd);
				}
			}
		}
		loop_mssg();
		// std::cout << RED << "-----------PRINTING----------" << ENDC << "Buffer: " << YELLOW << buffer << ENDC << endl;
		// debug();
	}
}

void	Socket::loop_mssg()
{
	for (vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if ((*it)->status() == true)
			send((*it)->id(), (*it)->rtnMssg().c_str(), (*it)->rtnMssg().length(), 0);
		if ((*it)->isRefreshChannel())
			send((*it)->id(), (*it)->prompt().c_str(), (*it)->prompt().size(), 0);
	}
	for (vector<Channel *>::iterator it = _channels.begin(); it != _channels.end();) 
	{
		if (!(*it)->size())
		{   
			Channel *ptr = *it;
			delete ptr;
			it = _channels.erase(it);
		}
		else
			it++;
	}
}

void	Socket::debug()
{
	for (vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
		std::cout << (**it);
	std::cout << endl << "----------CH------------" << endl;
	printChannels();
	std::cout << endl << "----------------------" << endl;
}