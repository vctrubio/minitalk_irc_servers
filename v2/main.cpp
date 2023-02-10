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
	ptr->
}




int main()
{
	cout << "Up and Running\n";
	
	Server	server(PORT);


	Channel	one(PORT, "Lupita");
	Client	bob("Bob");
	Client	billy("billy");
	list<Client*> cl = one.rtnList();

	one.join(&bob);
	one.join(&billy);
	cout << "SIZE = " << one.rtnList().size() << endl;

	server.addChannel(&one);
	showme(one.rtnList());

	// server.runSocket();
	return 1;
}
