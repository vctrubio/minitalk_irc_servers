#include "Headers.hpp"
#include "Socket.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"


void	welcome(string name)
{

	string	mssg = GREEN;
	mssg += "Connected\n";
	mssg += ENDC;
	mssg += "UI: Please provide a big enough screen; you should see this message in a single line (80CHARS_MIN)\n";
	mssg += "/help for CMD instructions.\n";
	mssg += "/doc for IRC documentation.\n";
	mssg += "/join channel to connect to #channels\n";

	cout << mssg;	
//	send(sockfd, mssg.c_str(), mssg.size(), 0);
}

void	putUnderScore(string &out)
{
	int i = 0;
	for (int size = 0; size < WIDTH + 1;) //topic_underline
	{
		if (size == 0 || size == WIDTH)
			out += "|";
		else
		{
			if (i % 2 == 0)
				out += '-';
			else
				out += ' ';
			i++;
		}
		++size;
	}
	out += "\n";
}




void	putLine(string &out, char c)
{
	for (int size = 0; size < WIDTH + 1;) //topic
	{
		if (size == 0 || size == WIDTH)
			out += "|";
		else
			out += c;
		++size;
	}
	out += "\n";
}



void	putChannel(Channel *channel, string &out, int i, int p)
{
	string	iSize;
	iSize = "[";
	iSize += to_string(i);
	iSize += "]";

	//if p do GREEN

	int marginUnit = WIDTH - iSize.length();
	cout << "MU: " << marginUnit << endl;

	int size = 0;
	while (size <= WIDTH)
	{
		if (size == 0 || size == WIDTH)
		{
			out += "|";
			size++;
		}
		else if (size <= channel->topic().length() - 1)
		{
			if (p == 0)
				out += GREEN;
			out += channel->topic();
			size += channel->topic().length();
		}
		else if (size >= marginUnit)
		{
			out += iSize;
			if (p == 0)
				out += ENDC;
			size += iSize.length();
		}
		else 
		{
			out += " ";
			++size;
		}
	}
	out += "\n";
}

void	putRight(string in, string &out, char c)
{
	int size; 

	cout << "in leght = "<< in.length() << endl;

	for (size = 0; size < WIDTH + 2;) //topic
	{
		if (size == 0 || size == WIDTH + 1)
			out += "|";
		else if (size <= in.length() - 1)
		{
			out += in;
			size += in.length();
		}
		else
			out += c;
		++size;
	}
	out += "\n";

} 

void	interface()
{
	string			buffer;
	string			topic;
	string			name = "GustaLola";
	int				size;
	int				y; //min height to display
	vector<string>	msg;
	vector<Channel*>	_channels;

	cout << "TEST\n";
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

	if (!_channels.empty())
	{
		vector<Channel*>::iterator it;;
		it = _channels.begin();
		int p = 0; //to find position, if P = 0; make channel green because thats the one connected to
		while (*it)
		{
			putChannel(*it, ptr, (*it)->size(), p);
			putUnderScore(ptr);
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
	//welcome("Papito");	//send welcome mss
	interface();		//a loop that is send everytime to the user



	//


	//Socket	serv(PORT, "password");

/*	try 
	{
		serv.runSocket();
	}
	catch (exception &e)
	{
		cerr << "Error Cought: " << e.what() << endl;
	}
*/

	return 1;
}
