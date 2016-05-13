#pragma once

#include <iostream>
#include <stdlib.h>
#include <string>
#include <climits>
#include "settings.h"

class RunLength
{
public:
	static void Encode(Settings* settings, bool useAuxiliar = false);
	static void Decode(Settings* settings, bool useAuxiliar = false);
};
