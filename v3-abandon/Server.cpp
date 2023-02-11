#include "Server.hpp"

Server::Server()
	:_running(1), _port(PORT), _password("papaya"), _socket(PORT)
{
	cout << "Init Server: ";
	cout << _port << " : " << _password << endl;
	// cout << "sockFD: " << _sock << endl;
}

Server::Server(int port, string &password)
	:_running(1), _port(port), _password(password), _socket(PORT)
{
	cout << "Init Server: ";
	cout << _port << " : " << _password << endl;
	// cout << "sockFD: " << _sock << endl;
}

Server::~Server()
{
	cout << "Deconstruct Server\n";
}




/* Poll() Example
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
			.......
		}
	}
*/

/*
	* As requested from subject we set the socket to NON-BLOCKING mode
	* allowing it to return any data that the system has in it's read buffer
	* for that socket, but, it won't wait for that data.
	*/
// if (fcntl(sockFd, F_SETFL, O_NONBLOCK) == -1) {
// 	throw std::runtime_error("Error while setting socket to NON-BLOCKING.");
// }