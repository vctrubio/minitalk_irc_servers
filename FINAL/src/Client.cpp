#include "../inc/Client.hpp"


/**
 * @brief Construct a new Client object
 * 
 * @param username the username of the client
 * @param nickname the nickname of the client
 * @param realname the realname of the client
 */
Client::Client(std::string username, std::string nickname, std::string realname, int fd): _fd(fd), _host("localhost"), _username(username), _nickname(nickname), _realname(realname) { }


/**
 * @brief Returns the nickname of the client.
 * 
 * @return const std::string& the nickname of the client.
 */
const std::string	&Client::getNickname() const { return _nickname; };


/**
 * @brief Returns the username of the client.
 * 
 * @return const std::string& the username of the client.
 */
const std::string	&Client::getUsername() const { return _username; };


/**
 * @brief Returns the realname of the client.
 * 
 * @return const std::string& the realname of the client.
 */
const std::string	&Client::getRealname() const { return _realname; };


/**
 * @brief Returns the host of the client.
 * 
 * @return const std::string& the host of the client.
 */
const std::string	&Client::getHost() const { return _host; };


/**
 * @brief Returns the socket of the client.
 * 
 * @return const int& the socket of the client.
 */
const int			&Client::getSocket() const { return _fd; };


/**
 * @brief Sets the nickname of the client.
 * 
 * @param nickname the nickname of the client.
 */
void		Client::setNickname(const std::string &nickname) { _nickname = nickname; }


/**
 * @brief Sets the username of the client.
 * 
 * @param username the username of the client.
 */
void		Client::setUsername(const std::string &username) { _username = username; }