#include "Post.hpp"


Post::Post(Client *client, Channel* channel, string mssg, post action): _client(client), _channel(channel)
{
    //add timestamp
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
    //add timestamp
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
}


