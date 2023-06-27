# Todo

## Allways run `make valgrind` - no leaks allowed

---

- [x] ERROR message for when the user tries to send message and its not on the channel
- [ ] Error message for when the user tries to join a "banned" channel
- [ ] JOin message for all the channel members
- [ ] Quit message for all the channel members
- [ ] Operator replacement in case the user is the last operator
- [ ] nick not changing when the user is already connected

# Subject

- Errors
    - [x] Your reference client must be able to connect to your server without encountering any error
    - [ ] Verify absolutely every possible error and issue (receiving partial data, low bandwidth, and so forth)
- Authenticate
    - [x] `PASS <password>` -> recieves the password, if correct connect and send 001, else send 464
    - [x] `NICK <nickname>` -> recieves the nickname, if its taken, send 433 (it might try to change the nickname and reconnect)
    - [x] `USER <username> <hostname> <servername> <realname>` -> sets the username and realname; host and server are garbage values
- Join a channel
    - [x] `JOIN <#channel>` -> joins the channel, if it doesnt exist, create it and set the user as operator
        - [x] If the client is already in the channel, send error message
        - [x] If the channel doesnt exist, create it and set the user as operator
        - [x] If the channel exists, add the user to the channel
        - [x] If the client is banned from the channel, send error message
    - [x] Channel names have to start with a ’#’ character
        - [x] If the channel name does not start with ’#’, send error message
        - [x] If the channel name is too long, send error message
        - [x] If the channel name contains invalid characters, send error message
        - [x] If the channel name is already taken, send error message
    - [x] Handle password protected channels
        - [x] If the channel is password protected, and the user does not provide the password, send error message
        - [x] If the password is incorrect, send error message
        - [x] If the password is correct, add the user to the channel
    - [x] Handle user limit
        - [x] If the channel has a user limit, and the limit is reached, send error message
        - [x] If the channel has a user limit, and the limit is not reached, add the user to the channel
        - [x] If the channel does not have a user limit, add the user to the channel
    - [x] Handle invite-only channels
        - [x] If the channel is invite-only, and the user is not invited, send error message
        - [x] If the channel is invite-only, and the user is invited, add the user to the channel
        - [x] If the channel is not invite-only, add the user to the channel
        - [x] If the user has invite, remove it and add the user to the channel
- Send and receive private messages
    - [x] `PRIVMSG <nickname> <message>` -> sends a message to the destination user
    - [x] If the user is not connected, send error message
    - [x] If the user is connected, send the message to the destination user
- Send message to channel
    - [x] `PRIVMSG <#channel> <message>` -> sends a message to the channel
    - [x] All the messages sent from one client to a channel have to be forwarded to every other client that joined the channel.
    - [x] If the channel does not exist, send error message
    - [x] If the channel exists, send the message to the channel, all the clients in the channel will receive the message, even the client that sent the message
- Channel Operator
    - [x] You must have operators and regular users.
    - Operations only done by operators:
        - [x] `KICK` - Eject a client from the channel
            - [x] If the client is an operator, he can kick other operators
            - [x] If the operator kicks himself, an error message is sent
            - [x] If the client is not an operator, an error message is sent
        - [x] `INVITE` - Invite a client to a channel
            - [x] If the client is an operator, he can invite other clients
            - [x] If the client is not an operator, an error message is sent
            - [x] If the client is not connected, an error message is sent
            - [x] If the client is connected, an invite message is sent to the client and the client can join the channel
            - [x] If the client is already in the channel, an error message is sent
            - [x] Banned clients can be invited
        - [x] `TOPIC` - Change or view the channel topic
            - [x] to see the topic, send `TOPIC <#channel>`
            - [x] to change the topic, send `TOPIC <#channel> <topic>`
            - [x] If the client is an operator, he can change the topic
            - [x] If the client is not an operator, an error message is sent
            - [x] If the channel does not exist, an error message is sent
        - [x] `MODE` - Change the channel’s mode:
            - [x] If the client is an operator, he can change the channel’s mode
            - [x] If the client is not an operator, an error message is sent
            - [x] If the channel does not exist, an error message is sent
            - [x] If the channel exists, the mode is changed
                - [x] `i`: Set/remove Invite-only channel
                - [x] `t`: Set/remove the restrictions of the TOPIC command to channel operators
                - [x] `k`: Set/remove the channel key (password)
                - [x] `o`: Give/take channel operator privilege
                - [x] `l`: Set/remove the user limit to channel
        - [x] `BAN` - Set/remove ban mask to keep users out
            - [x] If the client is an operator, he can set/remove ban mask
            - [x] If the client is not an operator, an error message is sent
            - [x] If the channel does not exist, an error message is sent
            - [x] If the channel exists, the ban mask is set/removed
        - [x] `UNBAN` - Set/remove ban mask to keep users out
            - [x] If the client is an operator, he can set/remove ban mask
            - [x] If the client is not an operator, an error message is sent
            - [x] If the channel does not exist, an error message is sent
            - [x] If the channel exists, the ban mask is set/removed
            - [x] If the client is not banned, an error message is sent
- Users Commands
    - [X] `LIST` - List all channels
    - [x] `NAMES` - List all users in the channel
        - [x] `NAMES <#channel>` - List all users in the channel
        - [x] `NAMES` - List all users in the front channel
        - [x] `NAMES <nickname>` - List all users in the channel where the user is
        - [x] `NAMES <#channel1,#channel2>` - List all users in the channel1 and channel2
    - [x] `PART` - Leave the channel
        - [x] `PART <#channel>` - Leave the channel
        - [x] `PART` - Leave the front channel
        - [x] If the client is not in the channel, an error message is sent
        - [x] If the client is in the channel, he leaves the channel
        - [x] If the client is the last one in the channel, the channel is destroyed
        - [x] If the client is the last operator, the server must give the operator status to another client
    - [x] `QUIT` - Disconnect from the server
- Server Commands
    - [x] `LUSERS` - List all users and channels
        - It will list in this format:
            - [x] `:server_name 251 <nickname> :There are <number> users and <number> channels on the server`
    - [x] `WHO` - List all users
        - It can be sent in two ways:
            - [x] `WHO <#channel>` - List all users in the channel
    - [x] `WHOIS` - List all information about a user
        - It will list in this format:
            - [x] `:server_name 311 <nickname> <username> <hostname> * :<realname>`
            - [x] `:server_name 312 <nickname> <servername> :<server_info>`
            - [x] `:server_name 313 <nickname> :is an IRC operator`
            - [x] `:server_name 317 <nickname> <integer> :seconds idle`
            - [x] `:server_name 318 <nickname> :End of WHOIS list`
    - [x] `PING` -> `PONG` - Ping the server (might be a user request or the client checking if the server is still alive)
- Signals
    - [x] Use ctrl+D to send the command in several parts: ’com’, then ’man’, then ’d\n’. `SIGQUIT`
    - [x] `SIGINT` & `SIGTERM` & `SIGKILL` & `SIGSTOP` -> exit the program

&nbsp;

## Bonus

- [ ] Handle file transfer.
- [ ] A bot.

&nbsp;

---

&nbsp;

# Evaluation

- [x] Executable file name: `ircserv`
- [x] Only one `pool` equivalent in the code.
    - `Socket.cpp`
        - `void Socket::run()`
            - `_activity = select(max_sd + 1, &_readFds, NULL, NULL, NULL);`
- [x] `pool` equivalent being called everytime before `read`/`send`/`accept`
- [x] `errno` is not being called
- [x] `fcntl` is not being called
- [x] The server listens to all interfaces on the port specified as a parameter
- [x] Using `nc` tool, we can connect to the server and the server answers to our commands
- [x] Using the irc client (`hexchat`), we can connect to the server
- [x] The server can handle multiple clients at the same time
- [x] We can join to a channel, send a message and all the clients in the channel receive the message
- [x] With a wrong command sent on one connection, other connections are not affected
- [x] If a client gets unexpectedly disconnected, the server continues running
- [x] If we unexpectedly kill a instance of `nc` with just half of a command sent, the server continues running and not in a odd state
- [ ] When a client connected to a channel is stopped `^-Z` and we flood the channel with messages/commands, all pending messages/operations are still executed when the client comes back
- [ ] Using `nc` and `hexchat`, we are able to set a nickname, a username and join a channel.
- [ ] `NOTICE` and `PRIVMSG` are handled correctly
- [ ] A normal user does not have access to the operator commands, only operators do

&nbsp;

## Bonus

- [ ] File Transfer works with the reference client (`hexchat`)
- [ ] There is an IRC bot in the server

---

| Sources |
| :-----: |
| [IRC GOD \| Posts](https://ircgod.com/posts/) |
| [Basic IRC Commands](https://www.mirc.com/help/html/index.html?basic_irc_commands.html) |
| [Internet Relay Chat](http://chi.cs.uchicago.edu/chirc/irc.html) |
| [Socket Programming in C/C++](https://www.geeksforgeeks.org/socket-programming-cc/) |
| [Tutorial: How To Make An IRC Server Connection](https://oramind.com/tutorial-how-to-make-an-irc-server-connection/) |
| [Video - Example of what the server should look like](https://www.youtube.com/watch?v=mccMiP9qr-s) |
| [Socket Programming in C/C++: Handling multiple clients on server without multi threading](https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/) |


| References |
| :--------: |
| [ft_irc by Alexdelia](https://github.com/Alexdelia/42-ft_irc) |
| [ft_irc by raccoman](https://github.com/raccoman/ft_irc) |
| [ \[Resources at the end\] ft_irc by MCCiupek](https://github.com/MCCiupek/ft_irc) |
| [ft_irc by avan-pra](https://github.com/avan-pra/ft_irc) |

# Internet Relay Chat Protocol

&nbsp;
&nbsp;

---

# Architecture

&nbsp;

## Components

### Servers

The server is a single process that forms the backbone of the IRC network. It is the only component of the protocol which is able ti link all the other components together. Provides:
- A point to which clients can connect, to talk to each other.
- A point to which other servers can connect, to relay messages between clients connected to different servers.
- A point to which services can connect, to provide services to clients.

### Clients

A client is anything that is connected to the server, except another server. There are two type of clients:
- **User clients**: These are the clients that are used by people to connect to the IRC network. They are the ones that are able to send and receive messages. They are usually called *users*.
- **Service clients**: These are the clients that are used by services to connect to the IRC network. Unlike user clients, they are not able to send and receive messages. They are usually called *services*.

### Architecture

An IRC network is defined by a group of servers connected to each other. A single server is what forms the simplest IRC network. The only configuration allowed is a tree-like structure, where each server can have any number of servers connected to it, but each server can only be connected to one other server. This means that there is always a single path between any two servers in the network.

```Example
                1--\
                    A        D---4
                2--/ \      /
                        B----C
                    /      \
                    3        E

Servers: A, B, C, D, E         Clients: 1, 2, 3, 4
```

The IRC protocol provides no mean for two clients communicate directly. All comunication between client is relayed by the server(s). This means that if a client wants to send a message to another client, it will be sent to the server, which will then relay it to the other client.

&nbsp;


## IRC Protocol Services

### Client Locator

The client locator service is used to find the server that a client is connected to. It is used by servers to find the server that a client is connected to, so that they can relay messages to it.

### Message Relaying

The message relaying service is used to relay messages between clients connected to different servers. It is used by servers to relay messages between clients connected to different servers.

### Channel Hosting and Management

A Channel is a group of clients that are able to communicate with each other. It's characterized by its name, current memebers and the set of properties that can be manipulated by the channel operators. The server is responsible for the channel hosting and management service, that is used to host channels and manage them.

&nbsp;

## IRC Concepts

### One-to-One Communication

One-to-one communication is the simplest form of communication. It is the communication between two clients. It is used to send messages between two clients. It is **required** that the server relays the message to the other client along the shortest path of the spanning tree.

```Example
Example 1: A message between clients 1 and 2 is only seen by server A, which sends it straight to client 2.

Example 2: A message between clients 1 and 3 is seen by servers A & B, and client 3. No other clients or servers are allowed see the message.

Example 3: A message between clients 2 and 4 is seen by servers A, B, C & D and client 4 only.
```

### One-to-Many Communication

The main goal of IRC is to provide a forum wich allows easy communication between a large number of clients. IRC offers severals means to achieve this, each serving its own purpose.

- **To A Channel**: In IRC the channel has a role equivalent to that of a room in a chat application. Their existance is dynamic and the actual conversation carried out on a channel must only be sent to servers wich are supporting users on that channel. The message should only be sent once to every local link as each sever is responsible for relaying the message to the other servers.

```Example
Example 4: Any channel with 1 client in it. Messages to the channel go to the server and then nowhere else.

Example 5: 2 clients in a channel. All messages traverse a path as if they were private messages between the two clients outside a channel.

Example 6: Clients 1, 2 and 3 in a channel.  All messages to the channel are sent to all clients and only those servers which must be traversed by the message if it were a private message to a single client.  If client 1 sends a message, it goes back to client 2 and then via server B to client 3.
```

- **To A Host/server Mask**: To provide with some mechanism to send messages to a large body of users, host and server masks are available. The server is responsible for relaying the message to all the clients that match the mask. This messages are only sent to locations where users are, similar to the channel messages.

- **To A List**: The least efficiant style of one-to-many conversation is through the use of a list of targets (client, channel, mask). This is not efficient as using a channel since the destination list may be broken up and the dispatch sent without checking to make sure duplicates aren't being sent.

### One-To-All

The one-to-all communication is used to send a message to all clients on the server(s). In a large network this can be a very expensive operation, being sent over the network in an effort to reach all of the desired destinations. For some class of messages, there is no option but to send the message to all clients. For example, the message to shut down the server must be sent to all clients. This message is also known as a broadcast message.

- **Client-to-Client**:  There is no class of message which, from a single message, results in a message being sent to every other client.

- **Client-to-Server**: Most commands wich result in a change of state information must be sent to all servers by default. This distribution should not be changed by the client.

- **Server-to-Server**: While most messages between servers are distributed to all ’other’ servers, this is only required for any message that affects a user, channel or server.  Since these are the basic items found in IRC, nearly all messages originating from a server are broadcast to all other connected servers.

&nbsp;

---

&nbsp;

# Channel Management

&nbsp;

## Channel Characteristics

### Namespace

Channels names ares strings that start with `#`, `+`, `&` or `!`, of lenght up to **50** characters. Channel names are case insensitive. Apart from the channel start with one of the four characters, the channel name can contain any character except for `space`, `comma (,)`, `bell (\a)`, `null (\0)`, `carriage return (\r)`, `line feed (\n)`, `colon (:)` and `control-G (\007)`. The use of different prefixes allows for different types of channels. This is important because of the protocol limitations regarding namespaces.

### Channel Scope

A Channel entety is known by one or more servers on the IRC network. A User can only become a member of a channel that is known by the server that the user is connected to. The channels must be a contiguos part of the IRC network. This means that there must be a path between any two servers that are hosting the channel, in order for the messages addressed to the channel to be sent to all the channel members.

Channels with `&` as prefix are local to the server where they are created. This means that they are not known by any other server. This is useful for channels that are only used by the users on the server where they are created.

Other channels are known to one or more servers connected to the network, depending on the channel mask.

If there is no channel mask, then the channel is known to all the servers.

If there is a channel mask, then the channel must only be known to servers wich has a local user in that channel, and to neighbouring servers if the mask matched both the local and the neighbouring server names.

### Channel Properties

Each channel has a set of properties that can be manipulated by the channel members. These properties affect the way server manage a given channel.

### Privileged Channel Members

In order for the channel members to keep some control over a channel, some channel members are given special privileges. These privileges are given to the channel members by the server, and can be revoked at any time. Those privileges allow the memeber to execute the follwong actions:
- `INVITE`: Invite a client to an invite-only channel (mode +i)
- `KICK`: Eject a client from the channel
- `MODE`: Change the channel’s mode, as well as members’ privileges
- `PRIVMSG`: Sending messages to the channel (mode +n, +m, +v)
- `TOPIC`: Change the channel topic in a mode +t channel

### Channel Operators

Channel operators are a subset of the privileged channel members. They are given the `MODE` privilege, and are able to change the channel mode. They are also able to change the privileges of the other channel members. The channel operators are identified by the `@` prefix in front of their nickname. Since channels starting with the character `+` as a prefix do not support channel modes, no member can therefore be a channel operator on such a channel.

### Channel Creator

The user who creates a channel with a prefix `!` is automatically a operator on that channel In recognition of this status, the channel creators are endowed with the ability to toggle certain modes of the channel which channel operators may not manipulate. A "channel creator" can be distinguished from a channel operator by issuing the proper `MODE` command.

&nbsp;

## Channel Lifetime

In regards to the lifetime of a channel, there are typically two groups of channels:
- **Standard Channels** wich prefix is either `&`, `#` or `+`.
- **Safe Channels** wich prefix is `!`.

### Standard Channels

These channels are created implicitly when the first user joins it, and cease to exist when the last user leaves it.  While the channel exists, any client can reference the channel using the name of the channel. The user that creates the channel is automatically a channel operator on that channel, with the exception of channels with the `+` prefix. In order to avoid the creation of duplicate channels, the server must check if the channel already exists before creating it. If a channel operator left the channel because of a network split, the channel name is temporatily unavailable for other users. This prevents local users from creating a channel using the same name, but does not prevent the channel to be recreated by a remote user. This mechanism is called **channel delay**.

### Safe Channels

Unlike other channels, `safe channels` are not implicitly created. A user wishing to create such channel must request the creation by sending a special `JOIN` command in which the channel name starts with the `!` prefix. The creation of this server is strictly controlled. The user only chooses part of the channel name (short name), the server automatically prepends the user provided name with a channel identifier consisting of 5 characters. The channel name resulting from the combination is unique, making the channel safe from abuse based on network splits. The user who creates the channel automatically becomes `channel creator`. A Server must not allow the creation of a new channel if there is another channel with the same short name or if any other channel that had the same shortname exited recently and no other member recently left the channel because of a network split. Unlike `channel delay`, channel names do not become unavailable. These channels continue to exist even if all the users leave the channel. ONly the user who created the channel, becomes `channel creator`, user joining an existing empty channel, will not become `channel creator` or `channel operator`.

&nbsp;


## Channel Modes

The various modes available for channels are as follows:

```
O - give "channel creator" status;
o - give/take channel operator privilege;
v - give/take the voice privilege;
a - toggle the anonymous channel flag;
i - toggle the invite-only channel flag;
m - toggle the moderated channel;
n - toggle the no messages to channel from clients on the outside;
q - toggle the quiet channel flag;
p - toggle the private channel flag;
s - toggle the secret channel flag;
r - toggle the server reop channel flag;
t - toggle the topic settable by channel operator only flag;
k - set/remove the channel key (password);
l - set/remove the user limit to channel;
b - set/remove ban mask to keep users out;
e - set/remove an exception mask to override a ban mask;
I - set/remove an invitation mask to automatically override the invite-only flag;
```

### `Channel Creator` Status

The mode `O` is only used in conjunction with `safe channels` and SHALL NOT be manipulated by users.  Servers use it to give the user creating the channel the status of "channel creator"

### `Channel Operator` Status

The mode `o` is used to toggle the operator status of a channel member.

### `Voice` Privilege

The mode `v` is used to give a channel member the privilege to speak on a moderated channel.

### `Anonymous` Channel Flag

The mode `a` is used to toggle the anonymous channel flag. This means that when a message sent to the channel is sent by the server to users and the origin is a user, then it must be masked. To maks the message, the server replaces the origin with the nickname of the channel creator.
To mask the message, the origin is changed to `anonymous!anonymous@anonymous.` (`nickname`!`username`@`host`). For this reason the server does not allow the nickname `anonymous` to be used by a user. Servers must also don't send `QUIT` messages to the channel members when a user leaves this kind of channel. It should generate a `PART` message instead.
On channels with the character `&` as prefix, this flag may be toogled by channel operator, but on channels with the character `!` as prefix, if this flag is set, it cannot be unset. The flag must not be available on channels with the character `#` or `+` as prefix.
Replies to the `WHO`, `WHOIS` and `NAMES` commands must not reveal information and presence of other users on this channel.

### Invite Only Flag

When the channel flag `i` is set, new members are only accepted if the mask matched Invite-list or they have been invited by a channel operator. This flag also restricts the use of the `INVITE` command.

### Moderated Channel

When the channel flag `m` is set, only channel operators and members with the `voice` privilege are allowed to send messages to the channel.

### No Messages to Channel from Clients on the Outside

When the channel flag `n` is set, only channel members are allowed to send messages to the channel.

### Quiet Channel Flag

The flag `q` can only be set by the server. It restricts the type of data sent to users about the channel operations like when another user joints, parts and nick changes. From a user perspective, the channel only contains one user, the user itself.

### Private and Secret Channel Flags

The channel flag `p` is used to mark a channel as private, while the flag `s` is to mark the channel as secret.  Both properties are similar and conceal the existence of the channel from other users. This means that there is no way of getting the channel's name from the server without being member. IN other words, these channels must be omitted from replies to queries like the WHOIS command.
When a channel is secret, in addition to the restricion mentioned above, the channel is also not listed in replies to the `LIST`, `TOPIC` and `NAMES` commands. Finally, this channels are not included in the reply to the `LUSERS` command.
The flags `p` and `s` must not both be set at the same time. If the `p` flag is enabled and a user tries to enable the flag `s` it will be secretly ignored by the server.

### Server Reop Flag

The channel flag `r` is only available on channels which name begins with the character `!` and MAY only be toggled by the `channel creator`.
This flag is used to prevent a channel from having no channel operators for a long period of time. WHen this flag is set, any channel that has lost all its channel operator for longer than the time specified by the `REOP` command, will have the `channel creator` automatically become a channel operator. This mechanism is also known as `channel reop mechanism`.

### Topic

The channel flag `t` is to restrict the usage of the command `TOPIC` to channel operators only.

### User Limit

A user limit may be sent on channels by using the channel flag `l`. When a limit is reached, the server must forbid  their local users to join the channel. The value of the limit must only be made available to the channel members in the reply sent by the server to a `MODE` command.

### Channel Key

When a channel key is set, servers must reject their local users request to join the channel unless the user provides the correct key. The key is set by using the channel flag `k`. The key must only be made available to the channel members in the reply sent by the server to a `MODE` command.

### Channel Access Control

- **Channel Ban and Exception**: When a user requests to join a channel, the server must check if the user's address matches any of the ban masks set for the channel. If it does, the server must check if the user's address matches any of the exception masks set for the channel. If it does, the user is allowed to join the channel. Otherwise, the user is not allowed to join the channel. Server must not allow a channel member who is banned to send messages to the channel, unless they are channel operator or have the `voice` privilege. The ban and exception masks are set by using the channel flags `b` and `e` respectively. The user who is banned from the channel and who carries an invitation from a channel operator, must be allowed to join the channel.
- **Channel Invitation**: For channels wich have the invite-only flag set, users whose address matches an invitation mask set for the channel are allowed to join the channel without any invitation.

&nbsp;


## Current Implementation

### Tracking Recenlty Used Channels

This mechanish is commonly know as `Channel Delay` and generally applies to channels with the character `#` as prefix. The server keeps track of the channels that lost a channel operator as result of a break. These chennels enter in a special state wich lasts for a certain period of time. In this period, the channels cannot cease to exist. If all the channels members leave the channel, the channel becomes unavailable. It will only become available again either because a remote user has joined the channel, or because the delay period has expired (in this case, the channel will be destroyed, and a new channel will be created).

### Safe Channels

This channels identifier is a fucntion of time. The current time is converted in a string of 5 characters using the follwoing base:
`ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890`
The channel identifier therefore has a preidiocity of 36 ^ 5 seconds (~700 days).
These channels must be subject of an adapted `channel delay` mechanism. Servers must keep track of all such channels wich loose members as a resul of a network split, despite the user being operator or not. If this happens, this channels do not become unavailable, so its possible to join them even if they are empty. To avoid channel colision, the server must keep a list of all the safe channels that have been created for a certain period of time. Eventually a server may choose to extend this procedure to forbid creation of channels with the same shortname as a safe channel.
When a channel has been opless for a longer than the `repo delay` and the channel has the flag `r` set, the server is responsible to giving channel operator status to some of the members. The server should use the logic described in the `RFC 2811` protocol of the Reop Mechanism, though it is not mandatory but recommended.

&nbsp;

---
