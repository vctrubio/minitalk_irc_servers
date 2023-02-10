#include "Socket.hpp"

Socket::Socket()
{
	cout << "INIT socket BAAAAAD\n";
}

Socket::Socket(int port)
	:_port(port)
{
	int opt = 1; //for setsockopt (geeksforgeeks)

	_sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockFd < 0)
		cout << RED << "Failed to create socket\n" << ENDC;
	else
	{
		if (setsockopt(_sockFd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
			cout << RED << "Failed to setSockOpt\n" << ENDC;
	}

	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = INADDR_ANY; //IP = 0...?
	_addr.sin_port = htons(PORT);
	
	if (bind(_sockFd, (struct sockaddr*)&_addr, sizeof(_addr)) < 0 )
		cout << RED << "Failed to bind\n" << ENDC;
	if (listen(_sockFd, MAX_CLIENTS) < 0)
		cout << RED << "Failed to listen\n" << ENDC;

	cout << BLUE << "The server is up and runnig" << endl;
	cout << GREEN << "Server IP: " << _addr.sin_addr.s_addr << endl;
	cout << BLUE << "Listening on port " << GREEN << PORT << ENDC << endl;
	cout << GREEN << "Welcome USEME\n" << ENDC;
}

Socket::Socket(const Socket &oldSocket)
{}

Socket& Socket::operator= (const Socket &oldSocket)
{
	return (*this);
}

Socket::~Socket()
{}

//
void	Socket::sendMssg(string mmsg)
{
	
}



//
void	Socket::runSocket()
{
	int	addrlen = sizeof(_addr);
	int			max_sd, sd, valread;
	int			tmp_socket;
	char		buffer[265];
	string		welcome_mssg = "Welcome MSSG:::\n";


	cout << "Waiting for connection...\n";
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
			cout << RED << "EINTR _activity ERROR\n" << ENDC;

		if (FD_ISSET(_sockFd, &_readFds))
		{
			if ((tmp_socket = accept(_sockFd, (struct sockaddr *)&_addr, (socklen_t *)&addrlen)) < 0)
				cout << RED << "Error: tmp_socket\n" << ENDC;
			cout << GREEN << "New Connection Established: tmp_socket " << tmp_socket << " |ip & port tbd|" << ENDC << endl;
			send(tmp_socket, welcome_mssg.c_str(), welcome_mssg.length(), 0);
			cout << "Welcome MSSG SENT!\n";

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
					//user disconnected...
					getpeername(sd, (struct sockaddr *)&_addr, (socklen_t *)&addrlen);
					cout << RED << "User nickname Disconnected" << ENDC << endl;
					close(sd);
					_clientSocket[i] = 0;
				}
				else
				{
					buffer[valread] = '\0';
					// this->sendMssg(buffer);
					cout << YELLOW << " FROM SD: " << sd << "RECV MSSG: " << buffer <<  ENDC;
				}
			}
		}
		//Display mssg in USERS
	}
}
