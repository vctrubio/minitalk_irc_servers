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
{
	cout << "Client Deconstructor.\n";
}



char*	Client::prompt()
{
	if (Channel *ptr = _channels.front())
	{

		char* topic = strdup(GREEN);
		strcat(topic, ptr->topic().c_str());
		strcat(topic, ENDC);
		strcat(topic, "#");
		return (topic);
	} //not really working man , atleast not how i want it to
	return(strdup(""));
}



void	Client::setUser(string str)
{
	_user = str;
	char *mssg = strdup("Nickname changed to: ");
	strcat(mssg, GREEN);
	strcat(mssg, str.c_str());
	strcat(mssg, ENDC);
	strcat(mssg, "\n");
	send(_id, mssg, strlen(mssg), 0);
}

void	Client::setName(string str)
{
	_name = str;
	char *mssg = strdup("Name changed to: ");
	strcat(mssg, GREEN);
	strcat(mssg, str.c_str());
	strcat(mssg, ENDC);
	strcat(mssg, "\n");
	send(_id, mssg, strlen(mssg), 0);
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
			char *mssg = strdup("Leaving Channel# ");
			strcat(mssg, RED);
			//strcat(mssg, channel->topic()); Doesnt fucking work cuase they are not friends and you can't add the header Channel in Clients because of fucking CPP stupid compilations
			strcat(mssg, ENDC);
			strcat(mssg, "\n");
			send(_id, mssg, strlen(mssg), 0);
			return ;
		}
	};
	
}

void	Client::putMssg(string mssg, Client *it, string channel_name)
{
	// _mssg = "[" + channel_name + "]"+ it->user() + ": " + mssg; //but we have a newline so its prerrty gay
	_mssg = " -incoming from- " + channel_name + it->user() + " : " + mssg;
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
