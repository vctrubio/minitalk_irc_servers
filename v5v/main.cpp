#include "Headers.hpp"
#include "Socket.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Post.hpp"


void	TestInterface()
{
	string			buffer;
	string			topic;
	string			name = "GustaLola";
	int				size;
	int				y; //min height to display
	vector<string>	msg;
	vector<Channel*>	_channels;

	string ptr = YELLOW;
	if (name.length() > WIDTH)
		name = name.substr(0, WIDTH);

	ptr += "|";
	ptr += name;
	ptr += ENDC;
	ptr += "\n";

	for (size = 0; size < WIDTH + 1; ++size) //name
	{
		if (size == 0 || size == WIDTH)
			ptr += "|";
		else
			ptr += "-";
	}
	ptr += "\n";
		
	Channel c("helloworld");
	Channel b("helloback");
	Channel a("hellolast");

	_channels.push_back(&c);
	_channels.push_back(&a);
	_channels.push_back(&b);

	Client peter("PeterPop", 21);

	Post p2(&peter, "what", JOIN);
	a.history.push_back(&p2);

	Post p3(&peter, "hello world", MSSG);
	a.history.push_back(&p3);

	Post p4(&peter, "hello world", LEAVE);
	a.history.push_back(&p4);

	if (!_channels.empty())
	{
		vector<Channel*>::iterator it;;
		it = _channels.begin();
		int p = 0; //to find position, if P = 0; make channel GREEN because thats the one connected to
		while (*it)
		{
			putChannel(*it, ptr, (*it)->size(), p);
			putLine(ptr, '_');
			p++;
			it++;
		}
	}
	else
	{
		putRight("No Channels Found:", ptr, ' ');	
		for (int y = 0; y < HEIGHT; y++)
		{
			if (y % 2 == 0)
				putLine(ptr, '-');
			else
				putLine(ptr, ' ');
		}
		putLine(ptr, '-');
	}
	buffer += ptr;
	cout << "BUFFER: \n" << buffer << endl;
}


int main()
{
	//What is would like to connect as a USER, change cout to send() for it to work with sockfds
	welcome("Papito");	//send welcome mss
	TestInterface();		//a loop that is send everytime to the user

	//
	Socket	serv(PORT, "password");

	
	// try 
	// {
	// 	serv.runSocket();
	// }
	// catch (exception &e)
	// {
	// 	cerr << "Error Cought: " << e.what() << endl;
	// }

	return 1;
}
