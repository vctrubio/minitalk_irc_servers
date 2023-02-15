#include "Channel.hpp"

Channel::Channel(string topic): _topic(topic)
{
	cout << "Channel #" << _topic << " created \n";
}

Channel::~Channel()
{
	cout << "Channel #" << _topic << " destroyed \n";
}


void	Channel::addClient(Client *client)
{
	_clients.push_back(client);
	client->subscribe(this);
}

void	Channel::rmClient(Client *client)
{

	for (int i; i < _clients.size(); i++)
	{
		if (_clients[i] == client)
		{
			vector<Client *>::iterator it = _clients.begin() + i;
			_clients.erase(it);
			cout << RED << client->user() << "Has Left" << Channel::_topic << ENDC << endl;
			break;
		}
	}
	for (int i; i < _clients.size(); i++)
	{

	}
	client->unsubscribe(this);
}



void	Channel::post(string mssg, int id)
{
	string full = "[" + _clients[id]->name() + "]: ";
	for (vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if ((*it)->id() == id)
			continue;
		(*it)->putMssg(full + mssg);
    }
}

void	Channel::postToSelf(string mssg, int id)
{
	for (vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if ((*it)->id() != id)
			continue;
		(*it)->putMssg(mssg);
    }
}

std::ostream& operator<<(std::ostream& os, Channel& channel)
{
	vector<Client *> _clients = channel.clients();

	os << GREEN << "Channel: " << channel.topic() << " [" << channel.size() << "]" << ENDC << endl;
	for (int i = 0; i < _clients.size(); i++)
	{
		Client *client = _clients[i];
		os << "Client " << i << ": " << *client << std::endl;
	}	

	return os;

}
