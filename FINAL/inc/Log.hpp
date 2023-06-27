#ifndef LOG_HPP
# define LOG_HPP

# include "Headers.hpp"

class Log
{
    std::string		_fileName;
    std::ofstream	_file;

    protected:
        std::string    getDate(void);
        void    writeLog(std::string message);

    public:
        Log(const std::string &type); 
        ~Log();
        void    setFileName(std::string fileName);
        void    CommandReceived(std::string command, Client *client);
        void    CommandReceived(vector::string command, Client *client);
        void    CommandSent(std::string command, Client *client);
};

Log  *getLog(void);


#endif