#include "Server.hpp"

Server::Server(string password): _password(password)
{
}

Server::~Server()
{
}


void	Server::addClient(Client *client)
{
	_clients.push_back(client); //will this always work or do we need a try and catch?
}

void	Server::removeClient(Client *client)
{
   itr_clients it;

    for (it = _clients.begin(); it != _clients.end();) 
    {
        if (*it == client)
        {
			cout << "User " << (*it)->rtnName() << RED << " Disconnected " << ENDC << endl; 
            Client *ptr = *it;
			it = _clients.erase(it);
			delete ptr;
        }
		else
			it++;
    }	
}

void Server::printClients()
{
    if (_clients.empty())
    {
        cout << "No clients in list\n";
        return ;
    }	
}

void	Server::printChannels()
{
	cout << "Servers Online: \n";	
	for (itr_channels it = _channels.begin(); it != _channels.end(); ++it) {
		Channel *channel = *it;	
		cout << (*channel).topic() << " ; ";
	}
}

Client	*Server::getClient(int key) 
{
	for (itr_clients it = _clients.begin(); it != _clients.end(); ++it)
	{
		Client *client= *it;
		if (client->id() == key)
			return client;
	}
	throw std::out_of_range("Key not found in map"); //needs a fucking try/catch dummy
}

Channel*	Server::addChannel(string &topic)
{

	for (itr_channels it = _channels.begin(); it != _channels.end(); it++)
	{
		if ((*it)->topic() == topic)
			return *it;
	}
	_channels.push_back(new Channel(topic));
	return (_channels.back());
}

void	Server::rmChannel(Channel *channel)
{
	std::vector<Channel *>::iterator it = find(_channels.begin(), _channels.end(), channel);
	if ((*it) == channel)
	{
		_channels.erase(it);
		delete *it;
	}
}

void	Server::find_cmd(vector<string> str)
{
	vector<string>::iterator 	it = str.begin();
	Channel 					*ptr;
	
	if (*it == "/leave")
	{
		it++;
		if (it == str.end())
			ptr = _requestCall->channels().front();
		else
			ptr = _requestCall->rtnChannel(*it);
		if (ptr)
			ptr->rmClient(_requestCall);
		if (_requestCall->hasChannel())
			_requestCall->refreshChannel();	
	}
	if (*it == "/join")
	{
		it++;
		if (it == str.end())
			return ;
		ptr = addChannel(*it);
		if (!_requestCall->hasChannel(ptr))
			ptr->addClient(_requestCall);
		else
			_requestCall->setFront(ptr);
		if (_requestCall->hasChannel())
			_requestCall->refreshChannel();		
	}
	else if (*it == "/nick")
	{
		it++;
		_requestCall->setUser(*it);
	}
	else if (*it == "/name")
	{
		it++;
		_requestCall->setName(*it);	
	}
	//......
}
