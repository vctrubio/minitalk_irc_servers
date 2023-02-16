#include "Client.hpp"

Client::Client()
{
	_name = "Bobito";
	_refresh = false;
	_mssg = "";
}

Client::Client(string name, int id): _name(name), _id(id), _refresh(false)
{
	_user = name + "_username";
	gethostname(_host, 60);

	cout << RED << "New Client: " << _user << " : " << _id << ENDC << endl;
}

Client::~Client()
{}

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
			return ;
		}
	};
	
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
