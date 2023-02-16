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
	string welcome_mssg = "Welcome MSSG:::Instruction. /JOIN to join a channel\n";

	Client	*new_connection = new Client("Newcumber", i); //adding client... NEEDS UI
	addClient(new_connection);


	send(i, welcome_mssg.c_str(), welcome_mssg.length(), 0);
}

void Socket::runSocket()
{
	int addrlen = sizeof(_addr);
	int max_sd, sd, valread;
	int tmp_socket;
	char buffer[265];

	cout << "Waiting for connection...\n";
	// string testChannel = "ChannelTest";
	// addChannel(testChannel);
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
			}
			if (sd > max_sd)
				max_sd = sd;
			//test if client > client_size
		}

		_activity = select(max_sd + 1, &_readFds, NULL, NULL, NULL);
		if ((_activity < 0) && (errno != EINTR))
			cout << RED << "EINTR _activity ERROR\n" << ENDC;

		if (FD_ISSET(_sockFd, &_readFds))
		{
			if ((tmp_socket = accept(_sockFd, (struct sockaddr *)&_addr, (socklen_t *)&addrlen)) < 0)
				cout << RED << "Error: tmp_socket\n" << ENDC;
			
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
					_requestCall = getClient(sd);
					init_cmd(buffer, sd);
				}
			}
		}
		cout << RED << "-----------PRINTING----------" << ENDC << "Buffer: " << YELLOW << buffer << ENDC << endl;
		for (vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
		{
			if ((*it)->status() == true)
				send((*it)->id(), (*it)->rtnMssg().c_str(), (*it)->rtnMssg().length(), 0); //needs to send to cinsike
			cout << (**it);
		}
		cout << endl << "----------CH------------" << endl;
		if (_channels.size() > 0)
		{
			Channel *ptr = _channels.front();
			cout << "Address:: " << (*ptr);
		}
		cout << endl << "----------------------" << endl;
		// cout << BLUE <<"LOOPED: " << ENDC << endl;
	}
}


vector<string>	buildVector(string str)
{
	vector<string>		tokens;
	string				t;
	std::stringstream 	ss(str);

	while (std::getline(ss, t, ' ')) {
        tokens.push_back(t);
    }

	//print DEBUG //does not take into considerations quotes "hi buddy"
	// cout << "Token Created:\n";
	// vector<string>::iterator it;
	// for (it = tokens.begin(); it != tokens.end(); it++)
	// 	cout << *it << endl;
	// cout << "--------------\n";
	return (tokens);
}


void	Socket::init_cmd(string buffer, int sd)
{
	vector<string>	ptr = buildVector(buffer);
	
	/* we have our vectors + sd
	now, join, leave
	*/
	if (buffer[0] == '/')
	{
		find_cmd(ptr);
	}
	/*
	else
	{
		 //cout << YELLOW << " FROM SD: " << sd << " RECV MSSG: " << buffer << ENDC;

		// IF CLIENT HAS CHANNEL, POST TO CHANNEL	
		if (getClient(sd)->hasChannel())
			channel->post(buffer, sd);
	}
	}
*/
}
