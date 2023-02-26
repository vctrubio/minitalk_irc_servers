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
	string mssg = "Welcome: ";

	mssg +=  GREEN;
	mssg +=  host;
	mssg +=  ENDC;
	mssg +=  "\n nick [nickname] to change your nickname\nname [name] to change your name\nhelp for more help.\n";
	send(i, mssg.c_str(), mssg.size(), 0);
}

void	Socket::init_cmd(string buffer, int sd)
{
	vector<string>	ptr = buildVector(buffer);

	if (buffer[0] == '/')
		find_cmd(ptr);
	else
	{
		//cout << YELLOW << " FROM SD: " << sd << " RECV MSSG: " << buffer << ENDC;
		if (getClient(sd)->hasChannel())
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
			
		//	cout << GREEN << "New Connection Established: tmp_socket " << tmp_socket << " |ip & port tbd|" << ENDC << endl;
			ft_add_user(tmp_socket);

			for (int i = 0; i < MAX_CLIENTS; i++)
			{
				if (_clientSocket[i] == 0)
				{
					_clientSocket[i] = tmp_socket;
		//			cout << "ºAdding to list of _clientrSocket as " << i <<  "SD: " << sd << endl;
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
					// FIRST: CHECK TO SEE IF 2 CHARS at teh END EXIST (PROTOCOL IN IRC) Not always granted
					// cout << "BUFFER: " << buffer << ":"<< strlen(buffer) << "| valread " << valread << endl;
					buffer[valread] = '\0'; 
					string trimBuffer = string(buffer);
					trimBuffer.resize(valread - 2);
					// cout << "BUFFER: " << trimBuffer << ":"<< trimBuffer.length() << "| valread " << valread << endl;
					_requestCall = getClient(sd);
					init_cmd(trimBuffer, sd);
				}
			}
		}


		//all this extra loop must be converted into an actual function...
		if (!_clients.empty())
		{
			for (vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
			{
				if ((*it)->status() == true)
				{
					send((*it)->id(), (*it)->rtnMssg().c_str(), (*it)->rtnMssg().length(), 0); //needs to send to cinsike
				}
				(*it)->refreshChannel(); //for teh sake of the next line
				if ((*it)->hasChannel() && (*it)->isRefreshChannel())
					send((*it)->id(), (*it)->prompt().c_str(), (*it)->prompt().size(), 0);
			}
		}
		cout << RED << "-----------PRINTING----------" << ENDC << "Buffer: " << YELLOW << buffer << ENDC << endl;
		for (vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
			cout << (**it);
		cout << endl << "----------CH------------" << endl;
		if (!_channels.empty())
		{
			for (vector<Channel *>::iterator it = _channels.begin(); it != _channels.end();) 
			{
				if (!(*it)->size())
				{   
				//	cout << RED << "CHANNEL EMPTY NEEDS TO BE DELETED\n" << ENDC;
					Channel *ptr = *it;
					it = _channels.erase(it);
					delete ptr;
				}
				else
					it++;
			}
		}
		printChannels();
		cout << endl << "----------------------" << endl;
	}
}
