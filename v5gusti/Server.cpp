#include "Server.hpp"

Server::Server(string password): _password(password)
{
    cout << "Server up and running.\n";
}

Server::~Server()
{}


void	Server::addClient(Client *client)
{
	_clients.push_back(client);
}

void	Server::removeClient(Client *client)
{
   itr_clients it;

    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (*it == client)
        {
            _clients.erase(it);
            break;
        }
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
	cout << "Server: Printing\n";	
	for (itr_channels it = _channels.begin(); it != _channels.end(); ++it) {
		Channel *channel = *it;	
		cout << (*channel);
	}


}

Client	*Server::getClient(int key) 
{

		for (itr_clients it = _clients.begin(); it != _clients.end(); ++it) {
		Client *client= *it;
		if (client->id() == key)
			return client;
	}
	
	throw std::out_of_range("Key not found in map");
}

void	Server::addChannel(Channel *channel)
{
	_channels.push_back(channel);
}

void	Server::rmChannel(Channel *channel)
{
	std::vector<Channel *>::iterator it = std::find(_channels.begin(), _channels.end(), channel);
	if (it != _channels.end())
	{
		_channels.erase(it);
		//would be nice to call descontructor on Channel & Gucci
	}

}
