#pragma once

#include <iostream>
#include <stdlib.h>
#include <string>
#include <climits>
#include "settings.h"

class RunLength
{
public:
	static void Encode(std::string text, std::string& out);
	static void Decode(std::string text, std::string& out);
	static void EncodeFromFile(Settings& settings, std::string& out);
	static void DecodeFromFile(Settings& settings, std::string& out);
};
