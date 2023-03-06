#include "Post.hpp"

Post::Post(Client *client, Channel* channel, string mssg, post action, time_t time): _client(client), _channel(channel), _type(action), _time(time)
{
    if (action == JOIN)
    {
       _mssg = client->rtnName() +  ": joined" + "\n";
    }
    else if (action == LEAVE)
    {
        _mssg = client->rtnName() + ": left" + "\n";
    }
    else if (action == MSSG)
    {
		//add timestamp of message
        _mssg = mssg + "\n";
    }
	cout << "COPIED	ºn " << _mssg << endl;
	print(client->id());
};


Post::Post(Client *client, string mssg, post action): _client(client)
{
}

void	Post::print(int sd)
{
	cout << "ID : " << sd << endl;
	send(sd, _mssg.c_str(), _mssg.size(), 0);
}
