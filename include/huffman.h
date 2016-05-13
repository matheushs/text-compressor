#pragma once

#include <iostream>
#include <stdlib.h>
#include <string>
#include <climits>
#include "settings.h"

#define ALPHABET_SIZE 256
#define TYPE unsigned char

struct Node
{
	struct Node* left;
	struct Node* right;
	uint32_t freq;
	TYPE value;
};

struct Alphabet
{
	char* bits;
	TYPE value;
	uint32_t freq;
};

class Huffman
{
	static void CreateHuffman(Settings* settings, Node** root, Alphabet** alphabet, int& size, bool useAuxiliar);
public:
	static void Encode(Settings* settings, bool useAuxiliar = false);
	static void Decode(Settings* settings, bool useAuxiliar = false);
};
