#pragma once

#include <iostream>
#include <stdlib.h>
#include <string>
#include <climits>
#include "settings.h"

class BWT
{
public:
	BWT(std::string text);
	BWT(Settings& settings, bool encode);

	void Encode(std::string& out, Settings* settings = nullptr);
	void Decode(std::string& out, Settings* settings = nullptr);
};
