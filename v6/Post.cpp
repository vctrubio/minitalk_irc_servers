#include "Post.hpp"

Post::Post(Client *client, Channel* channel, string mssg, post action, time_t time): _client(client), _channel(channel), _type(action), _time(time)
{
    std::strftime(_strTime, sizeof(_strTime), "%Y-%m-%d %H:%M| ", std::localtime(&_time));
    if (action == JOIN)
    {
        _mssg = string(_strTime);
        _mssg += client->rtnName() +  ": joined" + "\n";
    }
    else if (action == LEAVE)
    {
        _mssg = string(_strTime);
        _mssg += client->rtnName() + ": left" + "\n";
    }
    else if (action == MSSG)
    {
        _mssg = string(_strTime);
        _mssg += mssg + "\n";
    }
};

Post::Post(Client *client, Channel* channel, post action, time_t time): _client(client), _channel(channel), _type(action), _time(time)
{
    std::strftime(_strTime, sizeof(_strTime), "%Y-%m-%d %H:%M| ", std::localtime(&_time));
    if (action == JOIN)
    {
        _mssg = string(_strTime);
        _mssg += client->rtnName() +  ": joined" + "\n";
    }
    else if (action == LEAVE)
    {
        _mssg = string(_strTime);
        _mssg += client->rtnName() + ": left" + "\n";
    }
};

void	Post::print(int sd)
{
	send(sd, _mssg.c_str(), _mssg.size(), 0);
}
