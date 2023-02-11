#ifndef CHANNELOPS_HPP
#define CHANNELOPS_HPP

#include "Headers.hpp"

class ChannelOps
{
public:
	ChannelOps();
	ChannelOps(const ChannelOps &oldChannelOps);
	ChannelOps& operator= (const ChannelOps &oldChannelOps);
	~ChannelOps();
};

#endif
