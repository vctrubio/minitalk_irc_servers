#include "Post.hpp"


Post::Post(Client *client, string mssg, post action): _client(client)
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
        _mssg = "<" + client->rtnName() + "> git " +  mssg;
    }
}
