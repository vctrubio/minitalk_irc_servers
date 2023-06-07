#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>
#include <iterator>
#include <algorithm>

/** An easy g++ iHeader.cpp && ./a.out "Test" 
 ** creating Test.cpp and Test.hpp oxthodoxstyle
***/

using namespace std;

struct	iFile
{
	string	name;
	string	nameCpp;
	string	nameHpp;
	string	_nameHpp;
};

//Function List
void	createHeader();
void	createHpp(iFile *file);
void	createCpp(iFile *file);
void	createFileStruct(string name, iFile *file); //feature: want to add private members too
bool	checkFileExist(string filename);
void	run(char **av);

//Program
int main(int ac, char **av)
{	
	if (ac == 1)
		cout << "Name your cpp files: ./a.out one two three\n";
	else
			run(av);
	return (1);
};

void	run(char **av)
{
	iFile	file;
	int		i;

	i = 0;
	while (av[++i])
	{
		if (checkFileExist(av[i]))
		{
				//open file;
				createFileStruct(av[i], &file);	
				cout << "\033[32mCreating\033[0m " << file.nameCpp << " & "<< file.nameHpp << endl;

				//write to file
				createHpp(&file);
				createCpp(&file);
		}
	};

};

//FileStruct
void	toUpper(char &c)
{
	if (c >= 'a' && c <= 'z')
			c -= 32; 
};

string	strUpper(string	&str)
{
	for_each(str.begin(), str.end(), toUpper);
	return (str);
};

void	createFileStruct(string name, iFile *file)
{
	if (!file->name.empty())
	{
		file->nameCpp.clear();
		file->nameHpp.clear();
		file->name.clear();
		file->_nameHpp.clear();
	}
	file->nameCpp = name + ".cpp";
	file->nameHpp = name + ".hpp";
	file->name = name;
	file->_nameHpp = strUpper(name) + "_HPP";
};

//VALIDATION
bool	checkFileExist(string filename)
{
	DIR		*directory;
	struct	dirent *x;
	string	tmpC = filename + ".cpp";
	string	tmpH = filename + ".hpp";
	char	c;

	if (( directory = opendir(".")) != NULL)
	{
			while ((x = readdir(directory)) != NULL)
			{
					if ((x->d_name == tmpC) || (x->d_name == tmpH))
					{
						cout << "File \"" << filename << "\" already exist, do you want to override it (y/n)\n";
						cin >> c;
						if (c != 'y')
								return (false);
						else
								break;
					}
			}
	}
	closedir (directory);
	return (true);
}

//CREATE FILES
void	createHeader()
{
	ofstream	f;

	f.open("Headers.hpp");

	f << "#ifndef " << "HEADERS_HPP" << endl;
	f << "#define " << "HEADERS_HPP" << endl << endl;
	//Utilities library
	f << "#include " << "<cstdlib>\n";
	f << "#include " << "<typeinfo>\n";
	//Dynamic memory management
	f << "#include " << "<memory>\n";
	//Numeric limits
	f << "#include " << "<limits>\n";
	f << "#include " << "<cfloat>\n";
	//Error handling
	f << "#include " << "<cerrno>\n";
	//Strings library
	f << "#include " << "<string>\n";
	//Math Library
	f << "#include " << "<algorithm>\n";
	//C++
	f << "#include " << "<array>\n";
	f << "#include " << "<vector>\n";
	f << "#include " << "<tuple>\n";
	f << "#include " << "<deque>\n";
	f << "#include " << "<list>\n";
	f << "#include " << "<map>\n";
	f << "#include " << "<iterator>\n";
	//Input/output library
	f << "#include " << "<fstream>\n";
	f << "#include " << "<iostream>\n";
	f << "#include " << "<iomanip>\n";
	//Dir Library
	f << "#include " << "<dirent.h>\n";

	//COLORS
	f << "# define BLACK	\"\033[0;30m\"\n";
	f << "# define RED	\"\033[0;31m\"\n";
	f << "# define GREEN	\"\033[0;32m\"\n";
	f << "# define YELLOW	\"\033[0;33m\"\n";
	f << "# define BLUE	\"\033[0;34m\"\n";
	f << "# define WHITE	\"\033[0;37m\"\n";
	f << "# define ENDC	\"\033[0m\"\n";
	f << "#endif\n";

	f.close();
};

void	createHpp(iFile *file)
{
	ofstream f;

	f.open(file->nameHpp);

	f << "#ifndef " << file->_nameHpp << endl;
	f << "#define " << file->_nameHpp << endl << endl;
	
	//other libraries can go here
	f << "#include \"Headers.hpp\"\n\n";

	f << "class " << file->name << endl << "{\n";
	f << "public:\n";
	f << "\t" << file->name << "(){};\n";
	f << "\t" << "~" << file->name << "(){};\n};\n\n";

	f << "#endif\n";
	f.close();
};

void	createCpp(iFile *file)
{
	ofstream 	f;
	string		name = file->name;

	f.open(file->nameCpp);

	f << "#include \"" << file->nameHpp << "\"\n\n"; 
	
	f.close();
};

