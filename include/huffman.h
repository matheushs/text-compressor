#pragma once

#include <iostream>
#include <stdlib.h>
#include <string>
#include <climits>
#include "settings.h"

class Huffman
{

public:
	Huffman(std::string text);
	Huffman(Settings& settings, bool encode);

	void Encode(std::string& out);
	void Decode(std::string& out);
};
