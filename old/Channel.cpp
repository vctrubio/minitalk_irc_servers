#include "Channel.hpp"

Channel::Channel(string topic, Client *client)
{
	string tmp = topic;
	_topic = tmp;
	cout << GREEN << "Channel #" << _topic << " created \n" << ENDC;
	_admins.push_back(client);
}

void	Channel::csv()
{
	if (_history.empty())
		return ;
	string fileTopic = "#" + topic() + " : " + _history.front()->time() + "ChannelHistory.csv";
	ofstream file(fileTopic);
	if (!file)
	{
		cerr << "Error in creating CSV file.\n";
		return ;
	}
	file << "Date Time       | User: Message\n";
		for (_itP = _history.begin(); _itP != _history.end(); _itP++)
		file << (*_itP)->mssg();
}

Channel::~Channel()
{
	csv();
	cout << RED << "Channel #" << _topic << " destroyed \n" << ENDC;
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
	if (type == KICK)
	{
		mssg = "#" + channel->topic() + "[";
		mssg += RED;
		mssg += "-1";
		mssg += ENDC; 
		mssg += "][";
		mssg += to_string(channel->size());
		mssg += "] ";
		mssg += RED + client->rtnName();
		mssg += " has been kicked\n";
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

void	Channel::addAdmin(Client *client)
{
	_admins.push_back(client);
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
	_history.push_back(new Post(client, this, JOIN, time(NULL)));
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
	_history.push_back(new Post(client, this, LEAVE, time(NULL)));
}

void	Channel::kickClient(Client *client)
{
	for (_itC = _clients.begin(); _itC != _clients.end(); _itC++)
	{
		if ((*_itC) == client)
		{
			_clients.erase(_itC);
			client->desubscribe(this);
			trigger_mssg(this, client, KICK);
			break;
		}
	}
}

void	Channel::post(string mssg, int id)
{
	_history.push_back(new Post(getClient(id), this, mssg, MSSG, time(NULL)));
	for (vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if ((*it)->id() == id)
			continue;
		(*it)->putMssg(mssg, topic());
    }
}

void	Channel::sendHistory(Client *client)
{
	for (_itP = _history.begin(); _itP != _history.end(); _itP++)
	{
		(*_itP)->print(client->id());
	}
}



std::ostream& operator<<(std::ostream& os, Channel& channel)
{
	vector<Client *> _clients = channel.clients();

	os << GREEN << "Channel: " << channel.topic() << " [" << channel.size() << "]" << ENDC << endl;
	for (unsigned int i = 0; i < _clients.size(); i++)
	{
		Client *client = _clients[i];
		os << "Client " << i << ": " << (*client).rtnName() << std::endl;
	}
	return os;
}

bool	Channel::rtnAdmins(Client *client)
{
	vector<Client *>::iterator it;
	for (it = _admins.begin(); it != _admins.end(); it++){
		if ((*it)->rtnName() == client->rtnName())
			return true;
	}
	return (false); 
}


