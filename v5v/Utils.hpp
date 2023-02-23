#ifndef UTILS_HPP
#define UTILS_HPP

#include "Headers.hpp"
#include "Channel.hpp"

vector<string>	buildVector(string str);


//FOR UI
void	welcome(string name);
void	putUnderScore(string &out);
void	putLine(string &out, char c);
void	putRight(string in, string &out, char c);

void	putChannel(Channel *channel, string &out, int i, int p);

#endif
