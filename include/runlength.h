#pragma once

#include <iostream>
#include <stdlib.h>
#include <string>
#include <climits>
#include "settings.h"

class RunLength {
public:
	static std::string Encode(std::string text);
	static std::string Decode(std::string text);
	static void EncodeFromFile(Settings& settings, std::string& out, bool writeFile);
	static void DecodeFromFile(Settings& settings, std::string& out, bool writeFile);
};
