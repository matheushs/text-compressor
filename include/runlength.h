#pragma once

#include <iostream>
#include <stdlib.h>
#include <string>
#include <climits>
#include "settings.h"

class RunLength
{
public:
	static void EncodeByte(Settings* settings, bool useAuxiliar = false);
	static void DecodeByte(Settings* settings, bool useAuxiliar = false);
	static void EncodeBit(Settings* settings, bool useAuxiliar = false);
	static void DecodeBit(Settings* settings, bool useAuxiliar = false);
};
