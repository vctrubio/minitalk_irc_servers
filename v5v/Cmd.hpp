#ifndef CMD_HPP
#define CMD_HPP

#include "Headers.hpp"

class Cmd
{
    map<string, void (*)(vector<string>)>	_cmd;

public:
	Cmd(){};
	~Cmd(){};

	void	find_cmd(vector<string>);
};

#endif
