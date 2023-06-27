#ifndef LOG_HPP
# define LOG_HPP

# include "Headers.hpp"

class Client;


class Log
{
    std::string		_fileName;
    std::ofstream	_file;
    void    writeLog(std::string message);
    std::string    getDate(void);

public:
    Log();
    ~Log();
    void    CommandReceived(std::string command, Client *client);
    void    CommandReceived(vector::string command, Client *client);
    void    CommandSent(std::string command, Client *client);
};

Log  *getLog(void);

# include "Client.hpp"


#endif