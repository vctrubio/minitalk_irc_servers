#include "Headers.hpp"

int createMap(int ac, char **av)
{
	cout << "init.\n";

	map<string, string>	clients;

	clients["AMiguel"] = "Homosapien";
	clients["Dico"] = "Girrafe";
	clients["Alex"] = "Poolio";

	cout << "clients size: " << clients.size() << endl;
	//cout << "1: " << clients["Miguel"] << endl;
	map<string, string>::iterator itr;

	clients.insert(pair<string, string>("Zimba", "The Whale"));
	
	if (clients["Dico"] <= clients["Alex"])
			cout << "YESMAN.\n";
	

	itr = clients.begin();
	while (itr != clients.end())
	{
		cout << (*itr).first << " : " << itr->second << endl;
		itr++;
	}
	return (1);
}
