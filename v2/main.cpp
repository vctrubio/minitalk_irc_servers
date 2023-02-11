#include "Headers.hpp"
#include "Socket.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"

int main()
{
	cout << "Up and Running\n";
	
	Socket	serv(PORT); //works
	//Server	serv(PORT); //does not And I WANT TO KNOW WHYYYYYYY
	serv.runSocket();

	return 1;
}
