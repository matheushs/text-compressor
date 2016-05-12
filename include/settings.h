#pragma once

#include <iostream>
#include <fstream>
#include <cstdlib>

struct Settings
{
	bool bwt = false;
	bool huffman = false;
	bool runLength = false;

	int textBlockSize = 0;

	std::string inputFilename;
	std::string outputFilename;

	std::ifstream input;
	std::ofstream output;
};