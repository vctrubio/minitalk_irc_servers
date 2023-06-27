#include "../inc/Headers.hpp"

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Please provide proper information: \"./ircserv <port> <password>\"\n";
		return -1;
	}

    signal(SIGINT, sighandler);
	signal(SIGQUIT, sighandler);
	signal(SIGTERM, sighandler);
	signal(SIGKILL, sighandler);
	signal(SIGSTOP, sighandler);
	signal(SIGTSTP, sighandler);
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
	try 
	{
		Socket	serv(x, argv[2]);
		serv.init();
		serv.run();
	}
	catch (std::exception &e)
	{
		std::cout << std::endl << RED << "Server stopped" << ENDC << std::endl;
	}
	return 0;
}



