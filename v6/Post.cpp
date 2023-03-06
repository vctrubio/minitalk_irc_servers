#include "Post.hpp"

Post::Post(Client *client, Channel* channel, string mssg, post action, time_t time): _client(client), _channel(channel), _type(action), _time(time)
{
    if (action == JOIN)
    {
       _mssg = client->rtnName() +  ": joined";
    }
    else if (action == LEAVE)
    {
        _mssg = client->rtnName() + ": left";
    }
    else if (action == MSSG)
    {
        _mssg = "<" + client->rtnName() + ">  " +  mssg;
    }
};


Post::Post(Client *client, string mssg, post action): _client(client)
{
}


