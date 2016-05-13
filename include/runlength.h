#pragma once

#include <iostream>
#include <stdlib.h>
#include <string>
#include <climits>
#include "settings.h"

class RunLength
{
public:
	static void Encode(Settings* settings, uint64_t size, bool useAuxiliar = false);
	static void Decode(Settings* settings, uint64_t& size, bool useAuxiliar = false);
};
