#include "Headers.hpp"
#include "Socket.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"




void	showme(list<Client*> cl)
{
	cout << "Test Size = " << cl.size() << endl;
	Client *ptr;

	ptr = cl.front();
	cout << ptr->rtnName() << endl;

	Channel *ch;
	ch = ptr->_channels.front();
	cout << "Connected TO " <<  ch->rtnName() << endl;
}




int main()
{
	cout << "Up and Running\n";
	
	Server	server(PORT);


	// Channel	one(PORT, "Lupita");
	// Channel	two(PORT, "Gupito");
	// Client	bob("Bob");
	// Client	billy("billy");
	// list<Client*> cl = one.rtnList();

	// one.join(&bob);
	// one.join(&bob); //no need to add again, validation working.
	// one.join(&billy);
	// two.join(&bob);

	// server.addChannel(&one);
	// server.addChannel(&two);
	// showme(one.rtnList());

	server.runSocket();
	return 1;
}
