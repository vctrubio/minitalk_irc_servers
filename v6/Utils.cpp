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

void	welcome(string name)
{

	string	mssg = GREEN;
	mssg += "Connected\n";
	mssg += ENDC;
	mssg += "UI: Please provide a big enough screen; you should see this message in a single line (80CHARS_MIN)\n";
	mssg += "/help for CMD instructions.\n";
	mssg += "/doc for IRC documentation.\n";
	mssg += "/join channel to connect to #channels\n";

	cout << mssg;	
//	send(sockfd, mssg.c_str(), mssg.size(), 0);
}

void	putUnderScore(string &out)
{
	int i = 0;
	for (int size = 0; size < WIDTH + 1;) //topic_underline
	{
		if (size == 0 || size == WIDTH)
			out += "|";
		else
		{
			if (i % 2 == 0)
				out += '-';
			else
				out += ' ';
			i++;
		}
		++size;
	}
	out += "\n";
}

void	putLine(string &out, char c)
{
	for (int size = 0; size < WIDTH + 1;) //topic
	{
		if (size == 0 || size == WIDTH)
			out += "|";
		else
			out += c;
		++size;
	}
	out += "\n";
}

void	putRight(string in, string &out, char c)
{
	int size; 

	for (size = 0; size < WIDTH + 2;) //topic
	{
		if (size == 0 || size == WIDTH + 1)
			out += "|";
		else if (size <= in.length() - 1)
		{
			out += in;
			size += in.length();
		}
		else
			out += c;
		++size;
	}
	out += "\n";

} 


void	putHistory(vector<Post*> history, string &out)
{
	vector<Post*>::iterator it = history.begin();

	int size;
	//need to trunc if too long into two lines
	while (*it)
	{
		for (size = 0; size < WIDTH + 2;)
		{
			if (size == 0 || size == WIDTH + 1)
				out += "|";
			else if (size <= (*it)->mssg().length() - 1)
			{
				out += (*it)->mssg();
				size += (*it)->mssg().length();
			}
			else
				out += ' ';
			++size;
		}
		out += "\n";
		it++;
	}
}

void	putChannel(Channel *channel, string &out, int i, int p)
{
	string	iSize;
	iSize = "[";
	iSize += to_string(i);
	iSize += "]";


	int marginUnit = WIDTH - iSize.length();
	int size = 0;
	while (size <= WIDTH)
	{
		if (size == 0 || size == WIDTH)
		{
			out += "|";
			size++;
		}
		else if (size <= channel->topic().length() - 1)
		{
			if (p == 0)
				out += GREEN;
			out += channel->topic();
			size += channel->topic().length();
		}
		else if (size >= marginUnit)
		{
			out += iSize;
			if (p == 0)
				out += ENDC;
			size += iSize.length();
		}
		else 
		{
			out += " ";
			++size;
		}
	}
	out += "\n";
	putUnderScore(out);

}
