#include "Client.hpp"

Client::Client()
{
	_name = "Unamed";
	_refresh = false;
}


Client::Client(const Client &oldClient)
{}

Client& Client::operator= (const Client &oldClient)
{
	return (*this);
}

Client::~Client()
{}
