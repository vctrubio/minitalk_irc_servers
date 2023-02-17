#include "Headers.hpp"
#include "Socket.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"



int main()
{
	cout << "Up and Running\n";
	
	Socket	serv(PORT, "password");

	cout << GREEN << "RUN SOCKET()\n" << ENDC;
	serv.runSocket();
	
	return 1;
}




/*
	Client	victor("Victor", 7);
	Client	dictor("Dictor", 7);
	Client	gugu("gusta", 7);
	Channel lolita("treers");
	Channel lupita("luuuuupapapluuuuupapapa");

	lolita.addClient(&victor);
	lolita.addClient(&gugu);
	lolita.addClient(&dictor);

	//cout << "testing:\n" << victor;
	serv.addChannel(&lolita);
	serv.addChannel(&lupita);
//	serv.printChannels();
*/
