#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Headers.hpp"
#include "Channel.hpp"

class Channel;

class Client
{
	std::string		_username;
	std::string		_nickname;
	std::string		_realname;
	std::string		_host;
	int				_fd;
	
public:
	Client(std::string username, std::string nickname, std::string realname, int fd): _username(username), _nickname(nickname), _realname(realname), _fd(fd) { char host[60]; gethostname(host, 60); _host = host; };
	~Client() {};

	const std::string		&getNickname() const { return _nickname; };
	const std::string		&getUsername() const { return _username; };
	const std::string		&getRealname() const { return _realname; };
	const std::string		&getHost() const { return _host; };
	const int				&getSocket() const { return _fd; };
	void		setNickname(const std::string &nickname) { _nickname = nickname; }
	void		setUsername(const std::string &username) { _username = username; }
};


#endif
