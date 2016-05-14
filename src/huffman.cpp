#include "huffman.h"

#include <cstdlib>
#include <algorithm>

bool SortFrequencies(Alphabet a, Alphabet b)
{
	return a.freq < b.freq;
}

void PrintTree(Node* tree)
{
	printf("%c %d\n", tree->value, tree->freq);
	if (tree->left)
		PrintTree(tree->left);
	if (tree->left)
		PrintTree(tree->right);
}

void CreateHuffmanBits(Node* node, Alphabet* alphabet, char* bits, uint32_t size)
{
	if (!node->left && !node->right)
	{
		alphabet[(uint32_t)node->value].bits = (char*)calloc(size + 1, 1);
		std::memcpy(alphabet[(uint32_t)node->value].bits, bits, size);
	}
	if (node->left)
	{
		bits[size] = '0';
		CreateHuffmanBits(node->left, alphabet, bits, size + 1);
	}
	if (node->right)
	{
		bits[size] = '1';
		CreateHuffmanBits(node->right, alphabet, bits, size + 1);
	}

}

void Huffman::CreateHuffman(Settings* settings, Node** root, Alphabet** alphabet, uint32_t& size, bool useAuxiliar)
{
	*alphabet = (Alphabet*)calloc(ALPHABET_SIZE, sizeof(Alphabet));
	while (true)
	{
		TYPE readType;
		if (useAuxiliar)
		{

			settings->auxiliar->read((char*)&readType, sizeof(TYPE));
			if (settings->auxiliar->eof())
				break;
		}
		else
		{
			settings->input->read((char*)&readType, sizeof(TYPE));
			if (settings->input->eof())
				break;
		}
		if ((*alphabet)[(uint32_t)readType].freq == 0)
		{
			(*alphabet)[(uint32_t)readType].value = readType;
			size++;
		}
		(*alphabet)[(uint32_t)readType].freq++;
	}

	Alphabet* alphabetCopy = (Alphabet*)calloc(ALPHABET_SIZE, sizeof(Alphabet));

	std::memcpy(alphabetCopy, (*alphabet), ALPHABET_SIZE * sizeof(Alphabet));

	std::sort(alphabetCopy, alphabetCopy + ALPHABET_SIZE, SortFrequencies);

	Node** node = (Node**)calloc(size, sizeof(Node*));

	for (uint32_t i = 0; i < size; i++)
	{
		node[i] = new Node;
		node[i]->freq =  (*alphabet)[(uint32_t)alphabetCopy[i + ALPHABET_SIZE - size].value].freq;
		node[i]->value = (*alphabet)[(uint32_t)alphabetCopy[i + ALPHABET_SIZE - size].value].value;
		node[i]->left = NULL;
		node[i]->right = NULL;
	}

	Node* current;

	for (uint32_t i = 0; i < size - 1; i++)
	{
		current = new Node;
		current->freq = node[i]->freq + node[i + 1]->freq;
		current->right = node[i];
		current->left = node[i + 1];
		node[i + 1] = current;

		for (uint32_t j = i + 1; j < size - 1; j++)
		{
			if (node[j]->freq > node[j + 1]->freq)
			{
				Node tempNode;
				tempNode.freq = node[j]->freq;
				tempNode.value = node[j]->value;
				tempNode.right = node[j]->right;
				tempNode.left = node[j]->left;

				node[j]->freq = node[j + 1]->freq;
				node[j]->value = node[j + 1]->value;
				node[j]->right = node[j + 1]->right;
				node[j]->left = node[j + 1]->left;

				node[j + 1]->freq = tempNode.freq;
				node[j + 1]->value = tempNode.value;
				node[j + 1]->right = tempNode.right;
				node[j + 1]->left = tempNode.left;
			}
			else
				break;
		}
	}
	delete alphabetCopy;
	(*root) = node[size - 1];
	char* str = (char*)calloc(size, 1);
	CreateHuffmanBits(*root, *alphabet, str, 0);
	delete str;
}

void Huffman::Encode(Settings* settings, bool useAuxiliar)
{
	Node* tree;
	Alphabet* alphabet;
	settings->size = 0;
	CreateHuffman(settings, &tree, &alphabet, settings->size, useAuxiliar);
#ifdef DEBUG
	PrintTree(tree);
	for (uint32_t i = 0; i < ALPHABET_SIZE; i++)
		printf("%c %s %d\n", alphabet[i].value, alphabet[i].bits, alphabet[i].freq);
#endif // DEBUG


	std::fstream* auxiliar = new std::fstream("auxiliarHuffman.dat", std::ios::in | std::ios::out | std::ios::binary | std::ofstream::trunc);

	while (true)
	{
		TYPE readType;
		if (useAuxiliar)
		{

			settings->auxiliar->read((char*)&readType, sizeof(TYPE));
			if (settings->auxiliar->eof())
				break;
		}
		else
		{
			settings->input->read((char*)&readType, sizeof(TYPE));
			if (settings->input->eof())
				break;
		}
		if ((*alphabet)[(uint32_t)readType].freq == 0)
		{
			(*alphabet)[(uint32_t)readType].value = readType;
			size++;
		}
		(*alphabet)[(uint32_t)readType].freq++;
	}
}

void Huffman::Decode(Settings* settings, bool useAuxiliar)
{
}