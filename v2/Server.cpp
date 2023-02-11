#include "Server.hpp"

Server::Server(string password): _password(password)
{
    cout << "Server up and running.\n";
}

Server::~Server()
{}


void	Server::addClient(int i, string name)
{
    Client  *client = new Client("Alexa");
    _clients[i] = client;
}

void	Server::removeClient(int i)
{
   _clients.erase(i);
}

void Server::printClients()
{
    if (_clients.empty())
    {
        cout << "No clients in list\n";
        return ;
    }
    for (map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
         cout << "Key: " << it->first << ", Value: " << it->second << endl;
    }
}
