#include "Socket.hpp"

Socket::Socket()
{
	cout << "INIT socket\n";
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
