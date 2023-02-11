#include "Server.hpp"

Server::Server()
	:_running(1), _port(PORT), _password("papaya")
{
	_sock = newSocket();
	cout << "Init Server: ";
	cout << _port << " : " << _password << endl;
	cout << "sockFD: " << _sock << endl;
}

Server::Server(int port, string &password)
	:_running(1), _port(port), _password(password)
{
	_sock = newSocket();
	cout << "Init Server: ";
	cout << _port << " : " << _password << endl;
	cout << "sockFD: " << _sock << endl;
}

Server::~Server()
{
	cout << "Deconstruct Server\n";
}


int Server::newSocket()
{
	int sockFd;
	int opt = 1; //for setsockopt (geeksforgeeks)
	
	sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockFd < 0)
		cout << RED << "Failed to create socket\n" << ENDC;
	else //ALL this needs to be throw...
	{
		if (setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
			cout << RED << "Failed to setSockOpt\n" << ENDC;
	}

	/*
	 * As requested from subject we set the socket to NON-BLOCKING mode
	 * allowing it to return any data that the system has in it's read buffer
	 * for that socket, but, it won't wait for that data.
	 */
	// if (fcntl(sockFd, F_SETFL, O_NONBLOCK) == -1) {
	// 	throw std::runtime_error("Error while setting socket to NON-BLOCKING.");
	// }

	struct sockaddr_in addr = {};
	bzero((char *) &addr, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY; //IP = 0...?
	addr.sin_port = htons(PORT); //change to _port later

	if (bind(sockFd, (struct sockaddr*)&addr, sizeof(addr)) < 0 )
		cout << RED << "Failed to bind\n" << ENDC;
	if (listen(sockFd, MAX_CLIENTS) < 0)
		cout << RED << "Failed to listen\n" << ENDC;
	//again throw and expection needed...

	_addrlen = sizeof(addr);
	return (sockFd);
}

void	Server::runServer()
{
	int			max_sd, sd, valread;
	int			tmp_socket;
	char		buffer[265];
	string		welcome_mssg = "Welcome MSSG:::\n";

	//HERE DILEMA between -pollfd and fd_set
	pollfd server_fd = {_sock, POLLIN, 0};
	_pollfds.push_back(server_fd);

	cout << BLUE << "The server is up and runnig" << endl;
	cout << GREEN << "Server IP: ..." << endl; //IP TBD
	cout << BLUE << "Listening on port " << GREEN << PORT << ENDC << endl;

	while(_running)
	{
		// Loop waiting for incoming connects or for incoming data on any of the connected sockets.
		if (poll(_pollfds.begin().base(), _pollfds.size(), -1) < 0)
			throw std::runtime_error("Error while polling from fd.");
		for (itr_poll it = _pollfds.begin(); it != _pollfds.end(); it++) 
		{
			cout << "POLL = " << it->fd << endl;
		}
	}
}

