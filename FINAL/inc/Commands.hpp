#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "Headers.hpp"

class Command
{
    public:
        Command() { memset(_tmp, 0, 1024); }
        virtual ~Command() {}
        void    sendMsg(Client *user, const std::string &msg);
    protected:
        char            _tmp[1024];

};

class Nick : public Command
{
    public:
        Nick(CommandArgs args);
        ~Nick() {}
};

class User : public Command
{
    public:
        User(CommandArgs args);
        ~User() {}
};

class Join : public Command
{
    public:
        Join(CommandArgs args);
        ~Join() {}
        bool isChannelNameValid(const std::string &name);
};

class Part : public Command
{
    public:
        Part(CommandArgs args);
        ~Part() {}
};

class Names : public Command
{
    public:
        Names(CommandArgs args);
        ~Names() {}
        static void broadcast(Channel *channel);
};

class List : public Command
{
    public:
        List(CommandArgs args);
        ~List() {}
};

class Topic : public Command
{
    public:
        Topic(CommandArgs args);
        ~Topic() {}

};

class Kick : public Command
{
    public:
        Kick(CommandArgs args);
        ~Kick() {}
};

class Invite : public Command
{
    public:
        Invite(CommandArgs args);
        ~Invite() {}

};

class Privmsg : public Command
{
    public:
        Privmsg(CommandArgs args);
        ~Privmsg() {}
};

class Ping : public Command
{
    public:
        Ping(CommandArgs args);
        ~Ping() {}
};

class Who : public Command
{
    public:
        Who(CommandArgs args);
        ~Who() {}
};


class Mode : public Command
{
    public:
        Mode(CommandArgs args);
        ~Mode() {}
};

class Quit : public Command
{
    public:
        Quit(CommandArgs args);
        ~Quit() {}
};


class CAP : public Command
{
    public:
        CAP(CommandArgs args);
        ~CAP() {}
};

class Whois : public Command
{
    public:
        Whois(CommandArgs args);
        ~Whois() {}
};

class Lusers : public Command
{
    public:
        Lusers(CommandArgs args);
        ~Lusers() {}
};

class Notice : public Command
{
    public:
        Notice(CommandArgs args);
        ~Notice() {}
};

#endif