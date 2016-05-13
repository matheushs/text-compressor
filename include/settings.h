#pragma once

#include <iostream>
#include <fstream>
#include <cstdlib>

#define GETBIT(data, index) ((bool)((data & (1 << index)) >> index))

struct Settings
{
	bool bwt = false;
	bool huffman = false;
	bool runLength = false;

	uint32_t textBlockSize = 0;
	uint32_t size = UINT32_MAX;

	std::string inputFilename;
	std::string outputFilename;

	std::ifstream* input;
	std::ofstream* output;
	std::fstream* auxiliar = nullptr;
};