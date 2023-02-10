#include "Client.hpp"

Client::Client()
{
	_name = "Unamed";
	_refresh = false;
}


Client::Client(const Client &oldClient)
{}

Client& Client::operator= (const Client &oldClient)
{
	return (*this);
}

Client::~Client()
{}


void	Client::addChannel(Channel *channel)
{
	if (_channels.empty())
	{
		cout << ": " << rtnName() <<  " Connecterd to NO Channels, now belongs to-  " << channel->rtnName() << endl;
		_channels.push_back(channel);
		return ;
	}

										//≠_channel we need a list of *channels
	for (list<Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		cout << "EVAL :" << rtnName()  << " SEAERCHING FOR " << (*it)->rtnName() << endl;
		if (*it == channel)
		{
			cout << ": " << rtnName() <<" :Channel already exist no NEED to add\n";
			break;
		}
		if (it == _channels.end())
		{
			_channels.push_back(channel);
			cout << "Channel name:" << channel->rtnName() << " ADDED TO USER: " << this->rtnName() << endl;
		}
		cout << "END AND NO SHOW\n";
	}
	//ite through to see if channel exist,
	//if not, add channel
}