#include "Channel.hpp"

// Channel::Channel(string topic, Client *admin): _topic(topic), _admin(admin)
Channel::Channel(string topic)
{
	string tmp = topic;
	_topic = tmp; //FUCKKING NEW LINEp
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

	string welcome = ("Joined# ");
	welcome +=  GREEN;
	welcome +=  topic();

	if (_clients.size() > 0)
	{
		cout << _clients.size() << " SIZE IS\n"; //not working 
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
		os << "Client " << i << ": " << (*client).rtnName() << std::endl;
	}
	return os;

}
