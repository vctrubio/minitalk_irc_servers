#include "Socket.hpp"

Socket::Socket(int port, string password)
	: _port(port), Server(password)
{
	int opt = 1; 

	_sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockFd < 0)
		cout << RED << "Failed to create socket\n"
			 << ENDC;
	else
	{
		if (setsockopt(_sockFd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
			cout << RED << "Failed to setSockOpt\n"
				 << ENDC;
	}

	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = INADDR_ANY; // IP = 0...?
	_addr.sin_port = htons(PORT);

	if (bind(_sockFd, (struct sockaddr *)&_addr, sizeof(_addr)) < 0)
		cout << RED << "Failed to bind\n"
			 << ENDC;
	if (listen(_sockFd, MAX_CLIENTS) < 0)
		cout << RED << "Failed to listen\n"
			 << ENDC;

	cout << BLUE << "The server is up and runnig" << endl;
	cout << GREEN << "Server IP: " << _addr.sin_addr.s_addr << endl;
	cout << BLUE << "Listening on port " << GREEN << PORT << ENDC << endl;
}

//
void Socket::ft_add_user(int i)
{
	char*	name = getenv("USER");
	Client	*new_connection = new Client(name, i);
	addClient(new_connection);

	string	host = new_connection->rtnHost();
	char	*mssg = strdup("Welcome: ");
	strcat(mssg, GREEN);
	strcat(mssg, host.c_str());
	strcat(mssg, ENDC);
	strcat(mssg, "\n\
	/nick [nickname] to change your nickname\n\
	/name [name] to change your name\n\
	/help for more help.\n");
	send(i, mssg, strlen(mssg), 0);
}

void	Socket::init_cmd(string buffer, int sd)
{
	vector<string>	ptr = buildVector(buffer);

	if (buffer[0] == '/')
		find_cmd(ptr);
	else
	{
		//cout << YELLOW << " FROM SD: " << sd << " RECV MSSG: " << buffer << ENDC;
		if (getClient(sd)->hasChannel()) // IF CLIENT HAS CHANNEL, POST TO CHANNEL	
			getClient(sd)->channels().front()->post(buffer, sd);
	}
}

void Socket::runSocket()
{
	int addrlen = sizeof(_addr);
	int max_sd, sd, valread;
	int tmp_socket;
	char buffer[265];

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
			cout << RED << "EINTR _activity ERROR\n" << ENDC; //throw

		if (FD_ISSET(_sockFd, &_readFds))
		{
			if ((tmp_socket = accept(_sockFd, (struct sockaddr *)&_addr, (socklen_t *)&addrlen)) < 0)
				cout << RED << "Error: tmp_socket\n" << ENDC; //throw
			
			cout << GREEN << "New Connection Established: tmp_socket " << tmp_socket << " |ip & port tbd|" << ENDC << endl;
			ft_add_user(tmp_socket);

			for (int i = 0; i < MAX_CLIENTS; i++)
			{
				if (_clientSocket[i] == 0)
				{
					_clientSocket[i] = tmp_socket;
					cout << "ºAdding to list of _clientrSocket as " << i << endl;
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
					cout << "BUFFER: " << buffer << "| valread " << valread << endl;
					buffer[valread] = '\0'; //this F is giving me afucking new LINE 
					_requestCall = getClient(sd);
					init_cmd(buffer, sd);
				}
			}
		}
		for (vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
		{
			if ((*it)->status() == true)
			{
				send((*it)->id(), (*it)->rtnMssg().c_str(), (*it)->rtnMssg().length(), 0); //needs to send to cinsike
			}
			if ((*it)->hasChannel())
				send((*it)->id(), (*it)->prompt(), strlen((*it)->prompt()), 0);
		}
		///*
		cout << RED << "-----------PRINTING----------" << ENDC << "Buffer: " << YELLOW << buffer << ENDC << endl;
		for (vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
			cout << (**it);
		cout << endl << "----------CH------------" << endl;
		if (_channels.size() > 0)
		{
			for (vector<Channel *>::iterator it = _channels.begin(); it != _channels.end();)
			{
				cout << "Address:: " << (**it) << endl;
				if (!(*it)->size())
				{   
					cout << RED << "CHANNEL EMPTY NEEDS TO BE DELETED\n" << ENDC;
					it = _channels.erase(it);
					delete *it;
					//i believe this may be working
				}
				else
				{
					++it;
				}
			}
		}
		cout << endl << "----------------------" << endl;
		//*/
	}
}

/* some segfaults,
some bus erros
some Heap corruption detected, free list is damaged at 0x600002ba0040
some life problems....
*/
