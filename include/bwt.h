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
	static void RotateWord(std::vector<std::string>& rotate, std::string& word);
	static void GetIndex(std::vector<std::uint32_t>& index, bool* control, std::string& ori);
};
