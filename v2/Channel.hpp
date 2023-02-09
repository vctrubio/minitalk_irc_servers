#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Headers.hpp"

//create a channel, that has many users and shows mssg between users

class Channel
{
public:
	Channel();
	Channel(const Channel &oldChannel);
	Channel& operator= (const Channel &oldChannel);
	~Channel();
};

#endif
