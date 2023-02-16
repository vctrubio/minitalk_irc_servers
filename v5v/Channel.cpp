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

	char *welcome = strdup("Joined# ");
	strcat(welcome, GREEN);
	strcat(welcome, topic().c_str());

	if (_clients.size() > 0)
	{
		cout << _clients.size() << " SIZE IS\n";
		strcat(welcome, ENDC);
		strcat(welcome, "Online: ");
		strcat(welcome, BLUE);
		string	name;
		for (_itC = _clients.begin(); _itC != _clients.end(); _itC++)
		{
			name += "[";
			name += (*_itC)->user();
			name += " : ";
			name += (*_itC)->name();
			name += "]";
		}
		strcat(welcome, name.c_str());
	}
	strcat(welcome, ENDC);
	strcat(welcome, "\n");
	send(client->id(), welcome, strlen(welcome), 0);
}

void	Channel::rmClient(Client *client)
{
	for (_itC = _clients.begin(); _itC != _clients.end(); _itC++)
	{
		if ((*_itC) == client)
		{
			_clients.erase(_itC);
			client->desubscribe(this);
			break;
		}
	}
	cout << RED << "RMCLIENT " << client->rtnName() << " :FROM Channel: " << topic() << ENDC << endl;
}

void	Channel::post(string mssg, int id)
{
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
		os << "Client " << i << ": " << *client << std::endl;
	}
	return os;

}
