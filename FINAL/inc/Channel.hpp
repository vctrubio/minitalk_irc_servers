#ifndef CHANNEL_HPP
#define CHANNEL_HPP

# include "Headers.hpp"

class Channel
{
	bool				_hasPassword;
	bool				_hasUserLimit;
	bool				_isInviteOnly;
	bool				_isTopicRestricted;
	std::string			_name;
	std::string			_topic;
	map::client			_clients;
	std::string			_password;
	unsigned int		_userLimit;
	vector::client		_invited;
	vector::client		_banned;

	public:
		Channel(std::string name);
		~Channel() {};
		void				setUserLimit(bool value, unsigned int userLimit);
		void				setTopicRestricted(bool value);
		void				setInviteOnly(bool value);
		void				ban(Client *client);
		void				setTopic(const std::string &topic);
		void				broadcast(const std::string &message);
		void				addClient(Client *client);
		void				rmClient(Client *client);
		void				promoteClient(Client *client);
		void				setPassword(bool value, const std::string &password);
		void				unban(Client *client);
		void				invite(Client *client);
		void				uninvite(Client *client);
		void				demoteClient(Client *client);
		bool				isInviteOnly() const;
		bool				isTopicRestricted() const;
		bool				hasUserLimit() const;
		bool				hasPassword() const;
		bool				hasOperator();
		bool				isClient(Client *client);
		bool				isOperator(Client *client);
		bool				isBanned(Client *client);
		bool				isInvited(Client *client);
		Client				*getClient(std::string nickname);
		Client				*getClient(int fd);
		map::client			getClients() const;
		unsigned int		getUserLimit() const;
		const std::string	&getTopic() const;
		const std::string	&getPassword() const;
		const std::string	&getName() const;
};

#endif
