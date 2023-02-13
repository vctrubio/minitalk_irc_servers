#include "Socket.hpp"

Socket::Socket()
{
	cout << "INIT socket BAAAAAD\n";
}

Socket::Socket(int port, string password)
	: _port(port), Server(password)
{
	int opt = 1; // for setsockopt (geeksforgeeks)

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

Socket::~Socket()
{
}

//
void Socket::ft_add_user(int i)
{
	Client	*new_connection = new Client("Newbie", i); //adding client... NEEDS UI
	addClient(new_connection);
}

void Socket::runSocket()
{
	int addrlen = sizeof(_addr);
	int max_sd, sd, valread;
	int tmp_socket;
	char buffer[265];
	string welcome_mssg = "Welcome MSSG:::Instruction. /JOIN to join a channel\n";

	cout << "Waiting for connection...\n";
	Channel *channel = new Channel("TESTING CHANNEL");
	while (42)
	{
		FD_ZERO(&_readFds);
		FD_SET(_sockFd, &_readFds);
		max_sd = _sockFd;

		for (int i = 0; i < MAX_CLIENTS; i++)
		{
			sd = _clientSocket[i];
			if (sd > 0)
			{
				cout << BLUE << "SD: " << sd << ENDC << " on ClientSocket " << i << endl;
				FD_SET(sd, &_readFds);
//				cout << "MAX SD: " << max_sd << endl;
			}
			if (sd > max_sd) // MAX_SIZE so ignore connection? CANNOT will stall program
				max_sd = sd;
		}

		_activity = select(max_sd + 1, &_readFds, NULL, NULL, NULL);
		if ((_activity < 0) && (errno != EINTR))
			cout << RED << "EINTR _activity ERROR\n"
				 << ENDC;

		if (FD_ISSET(_sockFd, &_readFds))
		{
			if ((tmp_socket = accept(_sockFd, (struct sockaddr *)&_addr, (socklen_t *)&addrlen)) < 0)
				cout << RED << "Error: tmp_socket\n"
					 << ENDC;
			cout << GREEN << "New Connection Established: tmp_socket " << tmp_socket << " |ip & port tbd|" << ENDC << endl;
			ft_add_user(tmp_socket);
			send(tmp_socket, welcome_mssg.c_str(), welcome_mssg.length(), 0);

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
				if ((valread = read(sd, buffer, 254)) == 0)
				{
					getpeername(sd, (struct sockaddr *)&_addr, (socklen_t *)&addrlen);
					cout << "User " << RED << " Disconnected " << ENDC << endl; 
					removeClient(getClient(sd));
					close(sd);
					_clientSocket[i] = 0;
				}
				else
				{
					buffer[valread] = '\0';
					// this->sendMssg(buffer);
					if (buffer[0] == '/')
					{
						if (strncmp(buffer, "/join", 5) == 0)
						{
							// creeate a channel if not already existed.
							// join channel if not already
							channel->addClient(getClient(sd));
						}
						else
							cout << RED << "Command not found\n"
								 << ENDC;
					}
					else
					{
						/*
						cout << YELLOW << " FROM SD: " << sd << " RECV MSSG: " << buffer << ENDC;
						*/
						// IF CLIENT HAS CHANNEL, POST TO CHANNEL	
						if (getClient(sd)->hasChannel())
							channel->post(buffer, sd);
					}
				}
			}
		}
		for (vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
		{
			if ((*it)->status() == true)
				send((*it)->id(), (*it)->rtnMssg().c_str(), (*it)->rtnMssg().length(), 0); //needs to send to cinsike
			cout << (**it);
			cout << RED << "DEBUG PURPOSES^^\n" << ENDC;

		}
		cout << BLUE <<"LOOPED: " << ENDC << endl;
	}
}
