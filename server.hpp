#ifndef SERVER_HPP
#define SERVER_HPP

#include "Headers.hpp"

class server
{
public:
	server();
	server(const server &oldserver);
	server& operator= (const server &oldserver);
	~server();
};

#endif
