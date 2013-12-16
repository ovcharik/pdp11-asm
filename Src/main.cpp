#include <iostream>
#include <fstream>
#include <string>

#include "Compiler.h"

int main(int argc, char ** argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: pdp11-asm <input file> <output file>" << std::endl;
		return 1;
	}

	std::ifstream ifs(argv[1]);
	if (ifs.is_open())
	{
		try {
			Compiler c(ifs);
			c.link();
			c.compile();
			
			std::ofstream ofs(argv[2]);
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
