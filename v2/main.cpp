#include "Headers.hpp"
#include "Socket.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"

int main()
{
	cout << "Up and Running\n";
	
	Socket	serv(PORT, "password");
	serv.runSocket();


	return 1;
}
