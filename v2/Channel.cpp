#include "Channel.hpp"

Channel::Channel(int port, string name) : _port(port), _name(name)
{
	cout << GREEN << "Channel Created on Port: " << _port << endl;
}

Channel::Channel(const Channel &oldChannel)
{
}

Channel &Channel::operator=(const Channel &oldChannel)
{
	return (*this);
}

Channel::~Channel()
{
}

void Channel::putMssg(Client &client)
{
	list<Client *> ptr = rtnList();
	for (list<Client *>::iterator it = ptr.begin(); it != ptr.end(); ++it)
	{
		if (*it == &client && ((it) != ptr.begin()))
		{
			cout << "FOUND " << (*it)->rtnName() << endl;
			ptr.splice(ptr.begin(), ptr, it);
			break;
		}
	}
	_mssg = "<" + client.rtnName() + "> " + client.rtnMssg();
}

void	Channel::sendMssg()
{
	Client	*ptr;
	for (list<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it != _clients.begin())
		{
			ptr = *it;
			(*it)->putMssg(_mssg);
			(*it)->refresh();
			cout << RED << "MSSG PUT\n" << ENDC;
		}
	}
}