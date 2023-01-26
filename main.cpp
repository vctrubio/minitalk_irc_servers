#include "Headers.hpp"



/*
 * socket() - returns a socket descriptor that represents the endpoint
 * bind() - after socket descriptor, bind unique name to the socket. Servers must bind a name to be accesible from the network
 * listen() - API indicates a willingness to accept connection requests
 * accept() - connect() to establish a connection, accept() to accept connection. 
 * send and receive functions - send() recv() read() write() ect.
 * close() - release any system resources acquired by the socket
 */

map<string, string>	parser(int ac, char **av)
{
	map<string, string>	rtn;

	if (ac != 1)
	{
		cerr << "Invalid args" << endl;
	}
	rtn["test"] = "noshow";
	return (rtn);
}

int main(int ac, char **av)
{
	map<string, string>	args;

	args = parser(ac, av);

	map<string, string>::iterator it = args.begin();
	if (it != args.end()) {
		std::cout << it->first << std::endl;
}
	std::cout << "init.\n";

}
