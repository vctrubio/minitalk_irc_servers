#include "Headers.hpp"
#include "Socket.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Post.hpp"

int	check_port(char *arg)
{
	for (int i = 0; i < strlen(arg); i++)
	{
		if (!isdigit(arg[i])) 
		{
			return -1;
		}
	}

	std::stringstream str(arg);
	int x;
	str >> x;
	if (x > 0 && x < 10000)
		return x;
	return -1; 
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		cout << "Please provide proper information: \"./irc <port> <password>\"\n";
		return -1;
	}

	int x = check_port(argv[1]);
	if (x < 1)
	{	
		cout << "PORT ≠ Digits 1-9999\n" << endl;
		return -1;
	}

	Socket	serv(x, argv[2]);
	try 
	{
		serv.runSocket();
	}
	catch (exception &e)
	{
		cerr << "Error Cought: " << e.what() << endl;
	}

	return 1;
}



