#include "Socket.hpp"

Socket::Socket()
{
	cout << "INIT socket\n";
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
	_addr.sin_addr.s_addr = INADDR_ANY;
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

void	Socket::initAddr()
{
	cout << "initAddr_ft\n";
}
