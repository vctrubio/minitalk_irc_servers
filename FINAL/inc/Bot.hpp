#ifndef BOT_HPP
# define BOT_HPP

# include "Headers.hpp"

class Bot: public Log
{
    int                 _socket;
    char                _buffer[1024];
    fd_set              _readfds;
    BotArgs             _args;
    const int           _port;
    struct addrinfo     _hints;
    struct addrinfo     *_servinfo;
    struct sockaddr_in  _addr;
    const std::string   _server;
    const std::string   _password;
    const std::string   _nickname;
    const std::string   _username;
    const std::string   _realname;

    protected:
        void            Help();
        void            CoinFlip();
        void            DiceRoll();
        void            findCommand();
        void            sendMsg(std::string msg);
        void            handleMsg(std::string msg);
        void            joinChannel(const std::string &room);
        bool            _run();
        bool            init();
        bool            checkActivity();
        bool            connectToServer();
        std::string     recvMsg();

    public:
        Bot(int port, std::string password);
        ~Bot();
        void            run();
};

#endif