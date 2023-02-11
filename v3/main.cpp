#include "Headers.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"


int main()
{
	cout << "Up and Running\n";
	
	Server	server;

	server.runServer();
	return 1;
}
