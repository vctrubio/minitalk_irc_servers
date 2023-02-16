#include "Channel.hpp"

// Channel::Channel(string topic, Client *admin): _topic(topic), _admin(admin)
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
	client->subscribe(this);
}

void	Channel::post(string mssg, int id)
{
	for (vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if ((*it)->id() == id)
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
