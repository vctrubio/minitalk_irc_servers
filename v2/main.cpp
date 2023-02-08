#include "Headers.hpp"
#include "Socket.hpp"
#include "Client.hpp"
#include "Server.hpp"


int main()
{
	cout << "Up and Running\n";
	
	Server	server(PORT);

	server.print();
	server.test();

	return 1;
}
