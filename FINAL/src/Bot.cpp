#include "../inc/Bot.hpp"


/**
 * @brief Construct a new Bot object.
 * 
 * @param port: port to connect to
 * @param password: password to identify to the server
 * 
 * @note Initializes the Log class.
*/
Bot::Bot(int port, std::string password): Log("bot"), _port(port), _server("127.0.0.1"), _password(password), _nickname("Moulinette"), _username("Moulinette"), _realname("Moulientte") { }


/**
 * @brief Destroy the Bot object.
 * 
 * @note Sends a QUIT message to the server.
 * @note Closes the socket.
*/
Bot::~Bot()
{
    sendMsg("QUIT :Moulinette is leaving\n");
    close(_socket);
}


/**
 * @brief Initializes the bot.
 * 
 * @return true if the bot is initialized, false otherwise
 * 
 * @note Creates a socket.
 * @note Gets the port in the network byte order.
 * @note Gets the address of the server. 
*/
bool Bot::init()
{
    if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << RED << "Socket creation error" << std::endl;
        return false;
    }
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(_port);
	memset(&_hints, 0, sizeof(_hints));
	_hints.ai_family = AF_UNSPEC;
	_hints.ai_socktype = SOCK_STREAM;
	getaddrinfo( _server.c_str() , 0 , &_hints , &_servinfo);
	_addr.sin_addr = ((struct sockaddr_in *) _servinfo->ai_addr)->sin_addr;
    freeaddrinfo(_servinfo);
    return true;
}


/**
 * @brief Sends a message to the server.
 * 
 * @param msg: message to send
 * 
 * @note Writes the sent message to the log file.
*/
void Bot::sendMsg(std::string msg)
{
    if (msg.empty())
        return;
    std::stringstream ss;
    ss << getDate();
    if (send(_socket, msg.c_str(), msg.length(), 0) < 0)
        ss << "[Error sending message]" << std::endl;
    else
        ss << "[Message sent]" << std::endl;
    ss << msg << (msg[msg.size() - 1] == '\n' ? "" : "\n");
    std::string message = ss.str();
    writeLog(message);
}


/**
 * @brief Receives a message from the server.
 * 
 * @return std::string: message received
 * 
 * @note Writes the received message to the log file.
*/
std::string Bot::recvMsg()
{
    std::stringstream ss;
    char buffer[1024] = {0};
    int valread = read(_socket, buffer, 1024);
    if (valread <= 0)
        return "";
    ss << getDate();
    ss << "[Message received]" << std::endl;
    ss << buffer << (buffer[strlen(buffer) - 1] == '\n' ? "" : "\n");
    std::string message = ss.str();
    writeLog(message);
    return std::string(buffer);
}


/**
 * @brief Connects to the server.
 * 
 * @return true: connection successful
 * @return false: connection failed
 * 
 * @note If the connection fails, tries again after 1 second.
 * @note Sends the identification messages to the server.
*/
bool Bot::connectToServer()
{
    if (connect(_socket, (struct sockaddr *)&_addr, sizeof(_addr)) < 0)
        return false;
    memset(_buffer, 0, 1024);
    sprintf(_buffer, BOT_IDENTIFY, "ft_irc", _nickname.c_str());
    sendMsg(_buffer);
    memset(_buffer, 0, 1024);
    sprintf(_buffer, BOT_PASSWORD, _password.c_str());
    sendMsg(_buffer);
    memset(_buffer, 0, 1024);
    sprintf(_buffer, BOT_NICKNAME, _nickname.c_str());
    sendMsg(_buffer);
    memset(_buffer, 0, 1024);
    sprintf(_buffer, BOT_USERNAME, _username.c_str(), _realname.c_str());
    sendMsg(_buffer);
    return true;
}


/**
 * @brief Parses the message received from the server.
 * 
 * @param msg: message received
 * 
 * @note Sets the BotArgs struct.
 * @note Calls the findCommand function.
*/
void Bot::handleMsg(std::string msg)
{
    vector::string args = buildVector(msg);
    vector::string::iterator it = args.begin();
    if (it == args.end())
        return;
    _args.who = *it;
    if (_args.who.size() > 0 && _args.who[0] == ':')
        _args.who = _args.who.substr(1);
    if (_args.who == _nickname)
        return;
    it++;
    if (it == args.end())
        return;
    _args.cmd = *it;
    it++;
    if (it == args.end())
        return;
    _args.to = *it;
    it++;
    if (it != args.end())
    {
        _args.msg = *it;
        it++;
        while (it != args.end())
        {
            _args.msg += " ";
            _args.msg+= *it;
            it++;
        }
        if (_args.msg.size() > 0 && _args.msg[0] == ':')
            _args.msg = _args.msg.substr(1);
    }
    else
        _args.msg = "";
    _args.msg = std::toupper(_args.msg);
    if (_args.msg.find("\r") != std::string::npos)
        _args.msg.erase(_args.msg.find("\r"));
    if (_args.msg.find("\n") != std::string::npos)
        _args.msg.erase(_args.msg.find("\n"));
    findCommand();
}


/**
 * @brief Finds the command to execute.
 * 
 * @note If the command is not found, sends an error message.
 * @note If the command is found, calls the corresponding function.
*/
void    Bot::findCommand()
{
    if (_args.msg == "!COINFLIP" || _args.msg == "!FLIPACOIN" || _args.msg == "!FLIPCOIN" || _args.msg == "!COIN")
        CoinFlip();
    else if (_args.msg == "!DICE" || _args.msg == "!ROLLADICE" || _args.msg == "!ROLLADIE" || _args.msg == "!ROLLDICE")
        DiceRoll();
    else if (_args.msg == "!HELP")
        Help();
    else
    {
        memset(_buffer, 0, 1024);
        sprintf(_buffer, BOT_ANSWER, _args.cmd.c_str(), _args.to.c_str(), "Unknown command");
    }
}


/**
 * @brief Executes the Coin Flip Bot functionality.
 * 
 * @note Sends the result of the coin flip to the server.
 * @note Is called by the findCommand function.
*/
void    Bot::CoinFlip()
{
    srand(time(NULL));
    int random = rand() % 2;
    memset(_buffer, 0, 1024);
    if (random == 0)
        sprintf(_buffer, BOT_ANSWER, _args.cmd.c_str(), _args.to.c_str(), "Heads");
    else
        sprintf(_buffer, BOT_ANSWER, _args.cmd.c_str(), _args.to.c_str(), "Tails");
    std::cout << _buffer << std::endl;
    sendMsg(_buffer);
}


/**
 * @brief Executes the Dice Roll Bot functionality.
 * 
 * @note Sends the result of the dice roll to the server.
 * @note Is called by the findCommand function.
*/
void    Bot::DiceRoll()
{
    srand(time(NULL));
    int random = rand() % 6 + 1;
    memset(_buffer, 0, 1024);
    sprintf(_buffer, BOT_ANSWER, _args.cmd.c_str(), _args.to.c_str(), std::to_string(random).c_str());
    sendMsg(_buffer);
}


/**
 * @brief Executes the Help Bot functionality.
 * 
 * @note Sends the help message to the server.
 * @note Is called by the findCommand function.
*/
void    Bot::Help()
{
    memset(_buffer, 0, 1024);
    sprintf(_buffer, BOT_ANSWER, _args.cmd.c_str(), _args.to.c_str(), "I can flip a coin or roll a dice. Just ask me to do it!");
    sendMsg(_buffer);
}


/**
 * @brief Checks if there is activity on the socket.
 * 
 * @return true: if there is activity on the socket.
 * @return false: if there is no activity on the socket.
*/
bool Bot::checkActivity()
{
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    int activity = select(_socket + 1, &_readfds, NULL, NULL, &tv);
    if (activity < 0)
    {
        std::cout << RED << "Select error" << ENDC << std::endl;
        return false;
    }
    return true;
}


/**
 * @brief Joins a channel.
 * 
 * @param room: channel to join.
 * 
 * @note Sets the topic of the channel.
 * @note Restricts the change of topic.
*/
void Bot::joinChannel(const std::string &room)
{
    sendMsg("JOIN " + room);
    memset(_buffer, 0, 1024);
    sprintf(_buffer, MSG_TOPIC, _nickname.c_str(), room.c_str(), "Moulinette Parlor");
    sendMsg(_buffer);
    memset(_buffer, 0, 1024);
    sprintf(_buffer, MSG_MODE, "Moulinette", _nickname.c_str(), room.c_str(), "+t");
    sendMsg(_buffer);
}


/**
 *  @brief Calls all the functions needed to run the bot.
 * 
 * @return true: if the bot ran successfully.
 * @return false: if the bot failed to run.
*/
bool Bot::_run()
{
    if (!init())
        return false;
    if (!connectToServer())
        return false;
    int confirm = 0;
    while (confirm < 4)
    {
        if (!checkActivity() || confirm == 4)
            return false;
        std::string msg = recvMsg();
        if (msg.empty())
            continue;
        if (msg.find("Password accepted") != std::string::npos)
            confirm++;
        if (msg.find("Nickname accepted") != std::string::npos)
            confirm++;
        if (msg.find("Username accepted") != std::string::npos)
            confirm++;
        if (msg.find("Realname accepted") != std::string::npos)
            confirm++;
    }
    if (confirm < 4)
    {
        std::cout << RED << "Moulinettte failed to connect to server" << ENDC << std::endl;
        return false;
    }
    std::cout << GREEN << "Moulinette connected to server" << ENDC << std::endl;
    joinChannel("#42");
    while (true)
    {
        if (!checkActivity())
            return false;
        std::string msg = recvMsg();
        if (msg.empty())
            return false;
        handleMsg(msg);
    }
}

/**
 * @brief The main function of the bot.
 * 
 * @note Tries to run the bot 5 times.
 * @note If the bot fails to run 5 times, it stops.
*/
void Bot::run()
{
    int tries = 0;
    while (tries < 5)
    {
        if (_run())
            return;
        tries++;
        close(_socket);
        std::cout << RED << "Moulinette failed to connect to server. Trying again..." << ENDC << std::endl;
        sleep(5);
    }
    std::cout << RED << "Moulinette failed to connect to server. Stopping..." << ENDC << std::endl;
}


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
		std::cout << "PORT ≠ Digits 1-65535\n" << std::endl;
		return -1;
	}
	if (std::to_string(argv[2]).compare(0, 5, "PASS ") == 0)
	{
		std::cout << "Password cannot start with \'PASS \'" << std::endl;
		return -1;
	}
	Bot	bot(x, argv[2]);
	try 
	{
        bot.run();
	}
	catch (std::exception &e)
	{
        std::cout << std::endl << RED << "Bot stopped" << ENDC << std::endl;
	}
    return (0);
}