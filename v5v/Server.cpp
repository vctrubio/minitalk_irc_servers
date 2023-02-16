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
		cout << (*channel).topic() << " ; ";
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
	std::vector<Channel *>::iterator it = std::find(_channels.begin(), _channels.end(), channel);
	if (it != _channels.end())
	{
		_channels.erase(it); //How TO CALL descontructor on Channel?¿
	}

}


void	Server::find_cmd(vector<string> str)
{

	vector<string>::iterator it;
	for (it = str.begin(); it != str.end(); it++)
	{
		cout << "INIT find_cmd:"  << (*it) << endl;
		//if its just /join or leave /leave as one arguments, its not working
		if (*it == "/join" && it == str.begin())
		{
			cout << "DO THE JOIN\n";
			it++; //needs validatiion for args
			Channel *ptr = addChannel((*it)); //always returns a channel, so need to throw if alreraedy exist;
			if (!_requestCall->hasChannel(ptr))
			{
				cout << "ADDING NOT SEEN BEFORE\n";
				ptr->addClient(_requestCall);
			}
		}
		else if (*it == "/leave" && *it == str.front())
		{
			it++; //needs validatiion for args if (it)
			// else throw front channel if no args passed //check if there is also .front()
			Channel *ptr = _requestCall->channels().front(); //we have segfaults here but watevr, 
			cout << "CHANNEL TO LEAVE : " << ptr << " : " << ptr->topic() << endl;
			ptr->rmClient(_requestCall);
		}
		// else 
		// {
		// 	cout << YELLOW << "IT : " << (*it) << " vs: " << str.front() << ENDC << endl;
		// }


	}

}