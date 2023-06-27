#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Headers.hpp"

class Client
{
	int				_fd;
	std::string		_host;
	std::string		_username;
	std::string		_nickname;
	std::string		_realname;
	
public:
	Client(std::string username, std::string nickname, std::string realname, int fd);
	~Client() {};
	void				setNickname(const std::string &nickname);
	void				setUsername(const std::string &username);
	const int			&getSocket() const;
	const std::string	&getNickname() const;
	const std::string	&getUsername() const;
	const std::string	&getRealname() const;
	const std::string	&getHost() const;
};


#endif
