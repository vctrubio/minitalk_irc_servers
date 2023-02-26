#include "Server.hpp"

Server::Server(string password): _password(password)
{
}

Server::~Server()
{
}


void	Server::addClient(Client *client)
{
	_clients.push_back(client);
	//will this always work or do we need a try and catch?
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
	throw std::out_of_range("Key not found in map"); //needs a fucking try dummy
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
	vector<string>::iterator it;
	
	//1 arg ≠ working str.end is str.begin()
	//it++ needs validation
	
	//1 commands, like leave and quite
	//2 commands like join X nick X 
	//focusing on NOW- /msg NICK [msg]
	

	if (str.begin() == str.end() && str.begin()->compare("/leave") == 0) //this isnt working, but you get the point
	{
		cout << RED << "EASY LEEAVE\n";
		Channel *ptr = _requestCall->channels().front(); //
		cout << "CHANNEL TO LEAVE : " << ptr << " : " << ptr->topic() << endl;
		ptr->rmClient(_requestCall);
	}

	for (it = str.begin(); it != str.end(); it++)  
	{
		cout << "INIT find_cmd:"  << (*it) << endl;
		if (*it == "/join" && it == str.begin())
		{
			it++;
			Channel *ptr = addChannel((*it));
			if (!_requestCall->hasChannel(ptr))
				ptr->addClient(_requestCall);
			else
				_requestCall->setFront(ptr);
		}
		else if (*it == "/leave" && *it == str.front())
		{
			it++; // else throw front channel if no args passed //check if there is also .front()
			Channel *ptr = _requestCall->rtnChannel(*it); //we have segfaults here but watevr, 
			if (ptr)
			{
				cout << "CHANNEL TO LEAVE HARDWAY: " << ptr << " : " << ptr->topic() << endl;
				ptr->rmClient(_requestCall);
			}
			else
				cout << "CHANNEL TO LEAVE: SORRY NOT FOUND\n";
		}
		else if (*it == "/nick" && *it == str.front())
		{
			it++;
			_requestCall->setUser(*it);	
		}
		else if (*it == "/name" && *it == str.front())
		{
			it++;
			_requestCall->setName(*it);	
		}
	}
	}
