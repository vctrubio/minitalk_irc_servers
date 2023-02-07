#ifndef SERVER_HPP
#define SERVER_HPP

#include "Headers.hpp"

class Server
{
    int         _socket;
    sockaddr_in sockaddr;
public:
    Server(){};
    Server(int socket) :_socket(socket){};
    
    int rtnSocket(){return (_socket);};
};

int initServer();

#endif