#include "Headers.hpp"
#include "Socket.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"

int main()
{
	cout << "Up and Running\n";
	
	Socket	serv(PORT, "password");
	Client	victor("Victor", 7);
	Client	dictor("Dictor", 7);
	Client	gugu("gusta", 7);
	Channel lolita("treers");

	lolita.addClient(&victor);
	lolita.addClient(&gugu);
	lolita.addClient(&dictor);
	//serv.runSocket();

	//cout << "testing:\n" << victor;
	cout << lolita << endl;
	return 1;
}
