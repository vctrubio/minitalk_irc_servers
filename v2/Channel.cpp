#include "Channel.hpp"

Channel::Channel(string topic): _topic(topic)
{
	cout << "Channeld # " << _topic << " created \n";
}

Channel::~Channel()
{}
