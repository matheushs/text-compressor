#pragma once

#include <iostream>
#include <stdlib.h>
#include <string>
#include <climits>
#include "settings.h"

#define ALPHABET_SIZE 256

struct Node
{
	struct Node* left;
	struct Node* right;
	uint32_t freq;
	unsigned char value;
};

struct Alphabet
{
	char* bits;
	unsigned char value;
	uint32_t freq;
};

class Huffman
{
	static bool SortFrequencies(Alphabet a, Alphabet b);
	static void PrintTree(Node* tree);
	static void CreateHuffmanBits(Node* node, Alphabet* alphabet, char* bits, uint32_t size);
	static void WriteTree(std::fstream* file, Node* node, Alphabet* alphabet);
	static void ReadTree(std::istream* file, Node** node);

	static void CreateHuffman(Settings* settings, Node** root, Alphabet** alphabet, bool useAuxiliar);
public:
	static void Encode(Settings* settings, bool useAuxiliar = false);
	static void Decode(Settings* settings, bool useAuxiliar = false);
};
