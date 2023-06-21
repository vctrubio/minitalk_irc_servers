#include "Server.hpp"

Server::Server(string password) : _password(password)
{
}

void Server::addClient(Client *client)
{
	_clients.push_back(client);
}

void Server::removeClient(Client *client)
{
	itr_clients it;

	for (it = _clients.begin(); it != _clients.end();)
	{
		if (*it == client)
		{
			cout << RED << "- Client: " << (*it)->rtnName() << ENDC << endl;
			Client *ptr = *it;
			it = _clients.erase(it);
			delete ptr;
		}
		else
			it++;
	}
}

int Server::check_clients(string client)
{
	vector<Client *>::iterator it;

	it = _clients.begin();
	while (it != _clients.end())
	{
		if (client == (*it)->user())
			return 1;
		it++;
	}
	return 0;
}

void Server::printClients()
{
	if (_clients.empty())
	{
		cout << "No clients in list\n";
		return;
	}
}

void Server::printChannels()
{
	cout << "Servers Online: \n";
	for (itr_channels it = _channels.begin(); it != _channels.end(); ++it)
	{
		Channel *channel = *it;
		cout << (*channel).topic() << " ; ";
	}
}

Client *Server::getClient(int key)
{
	for (itr_clients it = _clients.begin(); it != _clients.end(); ++it)
	{
		Client *client = *it;
		if (client->id() == key)
			return client;
	}
	throw std::out_of_range("Key not found in map");
	return nullptr;
}

Client *Server::getClient(string nick)
{
	for (itr_clients it = _clients.begin(); it != _clients.end(); ++it)
	{
		Client *client = *it;
		if (client->user() == nick)
			return client;
	}
	throw std::out_of_range("Key not found in map");
	return nullptr;
}

Channel *Server::addChannel(string &topic)
{

	for (itr_channels it = _channels.begin(); it != _channels.end(); it++)
	{
		if ((*it)->topic() == topic)
			return *it;
	}

	_channels.push_back(new Channel(topic, _requestCall));
	return (_channels.back());
}

Channel *Server::addChannel(string &topic, Client *client)
{

	for (itr_channels it = _channels.begin(); it != _channels.end(); it++)
	{
		if ((*it)->topic() == topic)
			return *it;
	}

	_channels.push_back(new Channel(topic, _requestCall));
	_channels.back()->addAdmin(client);
	return (_channels.back());
}

void Server::rmChannel(Channel *channel)
{
	std::vector<Channel *>::iterator it = find(_channels.begin(), _channels.end(), channel);
	if ((*it) == channel)
	{
		_channels.erase(it);
		delete *it;
	}
}

string	Server::build_info(Client &client)
{
	string str;

	str = "Name: ";
	str += client.name();
	str += "\n";
	str += "User: ";
	str += client.user();
	str += "\n";
	str += "Host: ";
	str += client.host();
	str += "\n";
	return str;
}

void Server::find_cmd(vector<string> str)
{
	vector<string>::iterator it = str.begin();
	Channel *ptr;

	if (*it == "/help")
	{
		string mssg;
		mssg += "\n/help for CMD instructions.\n";
		mssg += "/doc for IRC documentation.\n";
		mssg += "/join channel to connect to #channels\n";
		mssg += "/nick [nickname] to change your nickname\n";
		mssg += "/name [name] to change your name\n";
		mssg += "/list to view all channels\n";
		mssg += "/channels to view your subscribed channels\n";
		mssg += "/online to see everyone online\n";
		mssg += "/who to view who is subscribed in the current channel or /who #name to see specific channel\n"; // BUGGING
		mssg += "/whois to stalk them\n";
		mssg += "/whoimi to stalk yourself\n";
		mssg += "/msg [nickname] to send a private mssg to a certain user\n";
		mssg += "/kick [nick] [channel] to kick someone (admin only)\n";
		mssg += "/invite [nick] to invite to current channel\n";
		// invite --
		send(_requestCall->id(), mssg.c_str(), mssg.size(), 0);
	}
	else if (*it == "/leave")
	{
		it++;
		if (it != str.end() && _requestCall->check_channels(*it) == 0)
			return;
		if (it == str.end())
		{
			if (_requestCall->hasChannel())
				ptr = _requestCall->channels().front();
			else
				return;
		}
		else
			ptr = _requestCall->rtnChannel(*it);
		if (ptr)
			ptr->rmClient(_requestCall);
		if (_requestCall->hasChannel())
			_requestCall->refreshChannel();
	}
	else if (*it == "/join")
	{
		it++;
		if (it == str.end())
			return;
		ptr = addChannel(*it);
		if (!_requestCall->hasChannel(ptr))
			ptr->addClient(_requestCall);
		else
			_requestCall->setFront(ptr);
		if (_requestCall->hasChannel())
			_requestCall->refreshChannel();
	}
	else if (*it == "/kick")
	{
		it++;
		if (it == str.end())
			return;
		string to_kick = *it;
		it++;
		if (it == str.end())
			return;
		if (getClient(to_kick)->rtnChannel(*it)->rtnAdmins(_requestCall) == false)
		{
			send(_requestCall->id(), "You do not have an administrator status\n", 40, 0);
			return;
		}
		if ((check_clients(to_kick) == 0 || getClient(to_kick)->check_channels(*it) == 0))
			return;
		if (it == str.end())
		{
			if (getClient(to_kick)->hasChannel())
				ptr = getClient(to_kick)->channels().front();
			else
				return;
		}
		else
			ptr = getClient(to_kick)->rtnChannel(*it);
		if (ptr)
			ptr->kickClient(getClient(to_kick));
		if (getClient(to_kick)->hasChannel())
			getClient(to_kick)->refreshChannel();
	}
	else if (*it == "/channels")
	{
		it++;
		if (it != str.end())
			return;
		if (_requestCall->hasChannel())
			_requestCall->refreshChannel();
	}
	else if (*it == "/nick")
	{
		it++;
		if (it == str.end() || (it + 1) != str.end())
			return;
		_requestCall->setUser(*it);
		sendNotice(_requestCall->channels(), _requestCall->rtnName() + " has changed his nickname.\n");
	}
	else if (*it == "/name")
	{
		it++;
		if (it == str.end())
			return;
		_requestCall->setName(*it);
		sendNotice(_requestCall->channels(), _requestCall->rtnName() + " has changed his name.\n");
	}
	else if (*it == "/list")
	{
		it++;
		if (it != str.end())
			return;
		string mssg;
		int i = 1;
		for (itr_channels ch = _channels.begin(); ch != _channels.end(); ch++)
		{
			mssg += to_string(i++);
			mssg += ": ";
			mssg += (*ch)->topic();
			mssg += " [";
			mssg += to_string((*ch)->size());
			mssg += "]\n";
		}
		send(_requestCall->id(), mssg.c_str(), mssg.size(), 0);
	}
	else if (*it == "/online")
	{
		vector<Client *>::iterator itC;
		string mssg;
		int i = 1;
		for (itC = _clients.begin(); itC != _clients.end(); itC++)
		{
			if (*itC == _requestCall)
				continue;
			mssg += to_string(i++);
			mssg += ": [";
			mssg += (*itC)->user();
			mssg += " : ";
			mssg += (*itC)->name();
			mssg += "]\n";
		}
		send(_requestCall->id(), mssg.c_str(), mssg.size(), 0);
	}
	else if (*it == "/whoimi")
	{
		stringstream mssg;
		mssg << *_requestCall;
		string hm = mssg.str();
		send(_requestCall->id(), hm.c_str(), hm.size(), 0);
	}
	else if (*it == "/who")
	{
		it++;
		if (it == str.end())
		{
			if (_requestCall->hasChannel())
				ptr = _requestCall->channels().front();
			else
				return;
		}
		else
			ptr = _requestCall->rtnChannel(*it);	
		int i = 1;
		if (ptr)
		{
			vector<Client *>::iterator itC;
			string mssg;
			for (itC = ptr->clients().begin(); itC != ptr->clients().end(); itC++)
			{
				// if (*itC == _requestCall)
				// 	continue;
				mssg += to_string(i++);
				mssg += ": [";
				// mssg += (*itC)->user(); //this segfault
				mssg += " : ";
				// mssg += (*itC)->name(); //this also segfault
				mssg += "]\n";
			}
			send(_requestCall->id(), mssg.c_str(), mssg.size(), 0);
		}
	}
	else if (*it == "/whois")
	{
		it++;
		if (it == str.end())
			return ;
		string who = *it;
		if (check_clients(who) == 0)
			return;
		string mssg = build_info(*(getClient(who)));
		send(_requestCall->id(), mssg.c_str(), mssg.size(), 0);
	}
	else if (*it == "/msg")
	{
		it++;
		string who = *it;
		if (check_clients(who) == 0)
			return;
		this->getClient(who);
		it++;
		string mssg = "[";
		mssg += BLUE;
		mssg += _requestCall->user();
		mssg += ENDC;
		mssg += "] ";
		mssg += *it;
		while (++it != str.end())
		{
			mssg += ' ';
			mssg += *it;
		}
		mssg += '\n';
		send(getClient(who)->id(), mssg.c_str(), mssg.size(), 0);
	}
	else if (*it == "/invite")
	{
		it++;
		if (_requestCall->channels().empty())
			return ;
		try
		{
			Client	*client = getClient(*it);
			Channel	*channel = _requestCall->channels().front();
			if (client)
			{
				channel->addClient(client);
				string mssg = _requestCall->name() + " has added you to the channel!\n";
				send(client->id(), mssg.c_str(), mssg.size(), 0);
			}
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
	else if (*it == "/history")
	{
		it++;
		vector<Post *>::iterator itP;
		Channel *channel;
		if (it == str.end())
			channel = _requestCall->channels().front();
		else
			channel = _requestCall->rtnChannel(*it);
		if (!channel)
			return;
		channel->sendHistory(_requestCall);
	}
}


//here.
void	sendNotice(vector<Channel*> channels, string mssg)
{
	for (vector<Channel*>::iterator channel = channels.begin(); channel != channels.end(); channel++)
	{
		// for (vector<Client>::iterator client = channel.clients().begin(); channel != channels.clients().end(); client++)
		// {

		// 		send(client->id(), mssg.c_str(), mssg.size(), 0);
		// }
	}
}