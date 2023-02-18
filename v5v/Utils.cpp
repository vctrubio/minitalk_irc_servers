#include "Utils.hpp"

vector<string>	buildVector(string str)
{
	vector<string>		tokens;
	string				t;
	std::stringstream 	ss(str);

	while (std::getline(ss, t, ' ')) {
		tokens.push_back(t);
	}

	//print DEBUG //does not take into considerations quotes "hi buddy"
	// cout << "Token Created:\n";
	// vector<string>::iterator it;
	// for (it = tokens.begin(); it != tokens.end(); it++)
	// 	cout << *it << endl;
	// cout << "--------------\n";
	return (tokens);
}