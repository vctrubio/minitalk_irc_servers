#include "Channel.hpp"

// Channel::Channel(string topic, Client *admin): _topic(topic), _admin(admin)
Channel::Channel(string topic)
{
	string tmp = topic;
	_topic = tmp;
	cout << "Channeld # " << _topic << " created \n";
}

Channel::~Channel()
{
	cout << "Channeld # " << _topic << " destroyed \n";
}


void	Channel::trigger_mssg(Channel *channel, Client *client, enum post type)
{

	string	mssg;

	_itC = _clients.begin();
	if (type == JOIN)
	{
		mssg = "#" + channel->topic() + "[";
		mssg += GREEN;
		mssg += "+1";
		mssg += ENDC; 
		mssg += "][";
		mssg += to_string(channel->size());
		mssg += "] ";
		mssg += GREEN + client->rtnName();
		mssg += " joined\n";
		mssg += ENDC;
		while (_itC != _clients.end())
		{
			if ((*_itC) != client)
				send((*_itC)->id(), mssg.c_str(), mssg.size(), 0);
			_itC++;
		}
	}
	if (type == LEAVE)
	{
		mssg = "#" + channel->topic() + "[";
		mssg += RED;
		mssg += "-1";
		mssg += ENDC; 
		mssg += "][";
		mssg += to_string(channel->size());
		mssg += "] ";
		mssg += RED + client->rtnName();
		mssg += " left\n";
		mssg += ENDC;
		while (_itC != _clients.end())
		{
			if ((*_itC) != client)
				send((*_itC)->id(), mssg.c_str(), mssg.size(), 0);
			_itC++;
		}
	}

}

void	Channel::addClient(Client *client)
{
	_clients.push_back(client);
	client->subscribe(this);

	string welcome = ("Joined ");
	welcome +=  GREEN;
	welcome += "#";
	welcome +=  topic();

	if (_clients.size() > 0)
	{
		cout << to_string(_clients.size()) << " SIZE IS\n";
		welcome += ENDC;
		welcome += " |Online: ";
		welcome += YELLOW;
		string	name;
		for (_itC = _clients.begin(); _itC != _clients.end(); _itC++)
		{
			name += "[";
			name += (*_itC)->user();
			name += " : ";
			name += (*_itC)->name();
			name += "]";
		}
		welcome += name;
	}
	welcome += ENDC;
	welcome += "\n";
	send(client->id(), welcome.c_str(), welcome.size(), 0);
	trigger_mssg(this, client, JOIN);
}

Client	*Channel::getClient(int key) 
{
	vector<Client *>::iterator it;

	for (it = _clients.begin(); it != _clients.end(); ++it)
	{
		Client *client= *it;
		if (client->id() == key)
			return client;
	}
	throw std::out_of_range("Key not found in map"); //THIS is the same as Server.cpp
}

void	Channel::rmClient(Client *client)
{
	for (_itC = _clients.begin(); _itC != _clients.end(); _itC++)
	{
		if ((*_itC) == client)
		{
			_clients.erase(_itC);
			client->desubscribe(this);
			trigger_mssg(this, client, LEAVE);
			break;
		}
	}
}

void	Channel::post(string mssg, int id)
{
	Client	*client = getClient(id);
	_history.push_back(new Post(client, this, mssg, MSSG));
	for (vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if ((*it)->id() == id)
			continue;
		(*it)->putMssg(mssg, (*it), topic());
    }
}

std::ostream& operator<<(std::ostream& os, Channel& channel)
{
	vector<Client *> _clients = channel.clients();

	os << GREEN << "Channel: " << channel.topic() << " [" << channel.size() << "]" << ENDC << endl;
	for (int i = 0; i < _clients.size(); i++)
	{
		Client *client = _clients[i];
		os << "Client " << i << ": " << (*client).rtnName() << std::endl;
	}
	return os;

}
