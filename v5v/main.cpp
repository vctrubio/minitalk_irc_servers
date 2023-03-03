#include "Headers.hpp"
#include "Socket.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Post.hpp"

int main(int argc, char **argv)
{
	if (argc != 4)
	{
		cout << "Please provide proper information: \"./irc <host> <port> <password>\"\n";
		return -1;
	}
	int x;
	std::stringstream str(argv[2]);
	str >> x;
	cout << x << "IS X " << ": \n";
	Socket	serv(x, argv[3]);

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



