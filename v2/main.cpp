#include "Headers.hpp"
#include "Socket.hpp"
#include "Client.hpp"
#include "Channel.hpp"

int main()
{
	cout << "Up and Running\n";
	
	Socket	socket(PORT);

	socket.runSocket();
	return 1;
}
