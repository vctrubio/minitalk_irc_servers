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

	try 
	{
		if (ac != 2)
			throw "invalid arguments";
	}
	catch (const char* e) {
        std::cout << e << std::endl;
    }

	rtn["test"] = "noshow";
	return (rtn);
}


int main(int ac, char **av)
{
	cout << "init.\n";

	map<string, string>	clients;

	clients["AMiguel"] = "Homosapien";
	clients["Dico"] = "Girrafe";
	clients["Alex"] = "Poolio";

	cout << "clients size: " << clients.size() << endl;
	//cout << "1: " << clients["Miguel"] << endl;
	map<string, string>::iterator itr;

	clients.insert(pair<string, string>("Zimba", "The Whale"));
	
	if (clients["Dico"] <= clients["asdasAlex"])
			cout << "YESMAN.\n";


	itr = clients.begin();
	while (itr != clients.end())
	{
		cout << (*itr).first << " : " << itr->second << endl;
		itr++;
	}
}
