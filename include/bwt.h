#pragma once

#include <iostream>
#include <stdlib.h>
#include <string>
#include <climits>
#include <vector>
#include <algorithm>
#include "settings.h"

class BWT
{
public:
	static void Encode(Settings* settings, bool useAuxiliar = false);
	static void Decode(Settings* settings, bool useAuxiliar = false);
private:
	static std::vector<std::string> RotateWord(std::string word);
	static std::vector<uint32_t> GetIndex(std::string ori);
};
