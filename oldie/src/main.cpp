#include "../inc/Headers.hpp"
#include "../inc/Client.hpp"
#include "../inc/Channel.hpp"
#include "../inc/Server.hpp"
#include "../inc/Socket.hpp"



/*
 * Checker for port number
*/
int	check_port(char *arg)
{
	for (size_t i = 0; i < strlen(arg); i++)
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

/*
 * Signal handler for CTRL+C
*/
void	sighandler(int signum)
{
	throw SignalException(signum);
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Please provide proper information: \"./irc <port> <password>\"\n";
		return -1;
	}

    signal(SIGINT, sighandler); 
	int x = check_port(argv[1]);
	if (x < 1)
	{	
		std::cout << "PORT ≠ Digits 1-9999\n" << std::endl;
		return -1;
	}
	if (std::to_string(argv[2]).compare(0, 5, "PASS ") == 0)
	{
		std::cout << "Password cannot start with \'PASS \'" << std::endl;
		return -1;
	}
		
	Socket	serv(x, argv[2]);
	try 
	{
		serv.init();
		serv.run();
	}
	catch (std::exception &e)
	{
		if (e.what() != std::string("SIGINT"))
			std::cerr << "Error: " << e.what() << std::endl;
		std::cerr << "Server turned off" << std::endl;
	}

	return 1;
}

