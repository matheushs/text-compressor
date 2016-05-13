#pragma once

#include <iostream>
#include <stdlib.h>
#include <string>
#include <climits>
#include "settings.h"

struct Node
{
	struct Node* left;
	struct Node* right;

	char value;
};

class Huffman
{

public:
	static void Encode(Settings* settings, bool useAuxiliar = false);
	static void Decode(Settings* settings, bool useAuxiliar = false);
};
