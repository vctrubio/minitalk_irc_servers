#include "Client.hpp"

Client::Client()
{
}

Client::Client(string name, int id): _name(name), _id(id), _refresh(false), _refreshChannel(false)
{
	_user = name + "_username" + to_string
	(rand() % 100);
	gethostname(_host, 60);
	_mssg = "";
	cout << RED << "New Client: " << _user << " : " << _id << ENDC << endl;
}

Client::~Client()
{
	for (_itC = _channels.begin(); _itC != _channels.end(); _itC++)
		(*_itC)->kick(this);
}

void	Client::setFront(Channel *ptr)
{
  std::vector<Channel*>::iterator it = std::find(_channels.begin(), _channels.end(), ptr);
  if (it != _channels.end()) {
    std::rotate(_channels.begin(), it, it + 1);
  }
}

Channel*	Client::rtnChannel(string topic)
{
	for (_itC = _channels.begin(); _itC != _channels.end(); _itC++)
	{
		if ((*_itC)->topic() == topic)
			return (*_itC);
	}
	return nullptr;
};

string	Client::prompt()
{
	string mssg;
	int i = 0;

	_itC = _channels.begin();
	while (_itC != _channels.end())
	{
		if (i == 0)
			mssg += GREEN;
		mssg += "#" + (*_itC)->topic();
		if (i == 0)
			mssg += ENDC;
		mssg += "[";
		mssg += to_string((*_itC)->size());
		mssg += "] ";
		_itC++;
		i++;
	}
	if (i > 0)
		mssg += "\n";
	return mssg;
}

void	Client::setUser(string str)
{
	_user = str;
	string mssg = "Nickname changed to: ";
	mssg += GREEN;
	mssg += str;
	mssg += ENDC;
	mssg += "\n";
	send(_id, mssg.c_str(), mssg.size(), 0);
}

void	Client::setName(string str)
{
	_name = str;
	string mssg = "Name changed to: ";
	mssg += GREEN;
	mssg += str;
	mssg += ENDC;
	mssg += "\n";
	send(_id, mssg.c_str(), mssg.size(), 0);
}


void Client::subscribe(Channel *channel)
{
	_channels.insert(_channels.begin(), channel);
}

void Client::desubscribe(Channel *channel)
{

	if (_channels.size() == 0)
		return ;

	for (_itC = _channels.begin(); _itC != _channels.end(); _itC++) 
	{
		if (*_itC == channel)
		{
			_channels.erase(_itC); 
			string mssg = "Leaving ";
			mssg += RED;
			mssg += "#";
			mssg +=  channel->topic() + "\n";
			mssg += ENDC;
			send(_id, mssg.c_str(), mssg.size(), 0);
			return ;
		}
	};
	
}

void	Client::putMssg(string mssg, Client *it, string channel_name)
{
	if (_channels.front()->topic() != channel_name)
		_mssg = "[#" + channel_name + "]" + mssg + "\n";
	else
		_mssg = mssg + "\n";
	_refresh = true;
}

std::ostream& operator<<(std::ostream& os, Client& client)
{
	os << "Client information:" << std::endl;
	os << "Name: " << client.name() << std::endl;
	os << "User: " << client.user() << std::endl;
	os << "Host: " << client.host() << std::endl;
	os << "Message: " << client.mssg() << std::endl;
	os << "Refresh status: " << client.status() << std::endl;
	os << "ID: " << client.id() << std::endl;

	os << "Channel:" ;
	if (client.channels().size() > 0)
	{
		vector<Channel*>::iterator 	it;
		vector<Channel*>			ptr = client.channels();
		int i = -1;
		while(++i < ptr.size())
		{
			os << i << " : " << ptr[i] << "; ";
		}
	}
	os << endl;
	return os;
}

int		Client::check_channels(string chnl)

{
	vector<Channel *>::iterator	it;

	it = _channels.begin();
	while (it != _channels.end())
	{
		if (chnl == (*it)->topic())
			return 1;
		it++;
	}
	return 0;
}