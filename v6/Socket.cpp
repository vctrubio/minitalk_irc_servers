#include "Socket.hpp"

Socket::Socket(int port, string password)
	: _port(port), Server(password), _password(password)
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
	_addr.sin_addr.s_addr = INADDR_ANY; // IP = 0...?
	_addr.sin_port = htons(_port);

	if (bind(_sockFd, (struct sockaddr *)&_addr, sizeof(_addr)) < 0)
		throw runtime_error ("Failed to bind");
	if (listen(_sockFd, MAX_CLIENTS) < 0)
		throw runtime_error ("Failed to listen");

	cout << BLUE << "The server is up and runnig" << endl;
	cout << GREEN << "Server IP: " << _addr.sin_addr.s_addr << endl;
	cout << BLUE << "Listening on port " << GREEN << _port << ENDC << endl;
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
	mssg += "/doc for IRC documentation.\n";
	mssg += "/join channel to connect to #channels\n";
	mssg += "/nick [nickname] to change your nickname\n/name [name] to change your name\n"; //are we allowed to change Name tho?
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
			// TO TEST if client > client_size
		}

		_activity = select(max_sd + 1, &_readFds, NULL, NULL, NULL);
		if ((_activity < 0) && (errno != EINTR))
			throw runtime_error ("Activity Errno");

		if (FD_ISSET(_sockFd, &_readFds))
		{
			if ((tmp_socket = accept(_sockFd, (struct sockaddr *)&_addr, (socklen_t *)&addrlen)) < 0)
				throw runtime_error ("Tmp socket failed");
			
			send(tmp_socket, "Please enter the server's password: ", strlen("Please enter the server's password: "), 0);
			valread = read(tmp_socket, password, 1024);
			if (valread < 0) {
				cerr << "Read error\n";
				close(tmp_socket);
				continue;
			}
			for (int i = 0; i < strlen(password); i++) 
			{        
				if (password[i] == '\r' || password[i] == '\n') 
				{
					password[i] = '\0';
					break;
				}
			}
			if (_password.compare(password) == 0)
			{
				send(tmp_socket, "Connection Successful\n", strlen("Connection Successful\n"), 0);	
				// Connection accepted, do something with the client // ...
			}
			else 
			{
				cout << "\nPASSWORD: |" << _password << "| VS INPUT: |" << password << "|\nResult: " << _password.compare(password) << std::endl;	
				send(tmp_socket, "Failed to connect\n", strlen("Failed to connect\n"), 0);
				close(tmp_socket);
				continue;
			}
			//	cout << GREEN << "New Connection Established: tmp_socket " << tmp_socket << " |ip & port tbd|" << ENDC << endl;
			ft_add_user(tmp_socket);
			for (int i = 0; i < MAX_CLIENTS; i++)
			{
				if (_clientSocket[i] == 0)
				{
					_clientSocket[i] = tmp_socket;
					//cout << "ºAdding to list of _clientrSocket as " << i <<  "SD: " << sd << endl;
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
					trimBuffer.resize(valread - 2);
					_requestCall = getClient(sd);
					init_cmd(trimBuffer, sd);
				}
			}
		}
		loop_mssg();

		//Debug on console purposes
		cout << RED << "-----------PRINTING----------" << ENDC << "Buffer: " << YELLOW << buffer << ENDC << endl;
		debug();
		printChannels();
		cout << endl << "----------------------" << endl;
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
}

void	Socket::debug()
{
	for (vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
		cout << (**it); //show users connected
	cout << endl << "----------CH------------" << endl;
	if (!_channels.empty())
	{
		for (vector<Channel *>::iterator it = _channels.begin(); it != _channels.end();) 
		{
			if (!(*it)->size())
			{   
				Channel *ptr = *it;
				it = _channels.erase(it);
				delete ptr;
			}
			else
				it++;
		}
	}
}