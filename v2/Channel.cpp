#include "Channel.hpp"

Channel::Channel(string topic): _topic(topic)
{
	cout << "Channeld # " << _topic << " created \n";
}

Channel::~Channel()
{
	cout << "Channeld # " << _topic << " destroyed \n";
}


void	Channel::addClient(Client *client)
{
	_clients.push_back(client);
}

void	Channel::sendMssg(string mssg)
{
	for (vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		(*it)->putMssg(mssg);
    }
}