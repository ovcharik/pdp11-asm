#include <iostream>
#include <fstream>
#include <string>
#include <regex>

#include "Compiler.h"

int main(int argc, char ** argv)
{
	if (argc != 3)
	{
		std::cerr << "Use as: ErshovAssembler <input file> <output file>" << std::endl;
		return 1;
	}

	std::ifstream ifs(argv[1]);
	std::ofstream ofs(argv[2]);
	if (ifs.is_open())
	{
		try {
			Compiler c(ifs);
			c.link();
			c.compile();
			c.write(ofs);
		} catch (std::string error) {
			std::cerr << error << std::endl;
			return 1;
		}
	}
	else
	{
		std::cerr << "Can't read input file: " << argv[1] << std::endl;
		return 1;
	}

	return 0;
}