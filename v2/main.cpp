#include "Headers.hpp"
#include "Socket.hpp"
#include "Client.hpp"
#include "Server.hpp"


int main()
{
	cout << "Up and Running\n";
	
	Server	server(PORT);

	server.runSocket();

	return 1;
}
