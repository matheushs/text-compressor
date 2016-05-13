#include "huffman.h"

#include <cstdlib>
#include <algorithm>

bool sortType(Alphabet a, Alphabet b)
{
	return a.value < b.value;
}

bool sortFrequenciesReverse(Alphabet a, Alphabet b)
{
	return a.freq > b.freq;
}

void Huffman::CreateHuffman(Settings* settings, Node** root, Alphabet** alphabet, int& size, bool useAuxiliar)
{
	*alphabet = new Alphabet[size];
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

	Alphabet* alphabetCopy = new Alphabet[ALPHABET_SIZE];

	std::memcpy(alphabetCopy, (*alphabet), ALPHABET_SIZE * sizeof(TYPE));

	std::sort(alphabetCopy, alphabetCopy + ALPHABET_SIZE, sortFrequenciesReverse);

	Node** node = (Node**)calloc(size, sizeof(Node*));

	Node* current;

	for (uint32_t i = ALPHABET_SIZE - size; i < ALPHABET_SIZE - 1; i++)
	{
		uint32_t index = i + size - ALPHABET_SIZE;
		current = new Node;
		if (node[i] == NULL)
		{
			node[i] = new Node;
			node[i]->freq = (*alphabet)[index].freq;
			node[i]->value = (*alphabet)[(uint32_t)alphabetCopy[index].value].value;
		}
		if (node[i + 1] == NULL)
		{
			node[i + 1] = new Node;
			node[i + 1]->freq = (*alphabet)[(uint32_t)alphabetCopy[index + 1].value].freq;
			node[i + 1]->value = (*alphabet)[(uint32_t)alphabetCopy[index + 1].value].value;
		}
		current->freq = node[i]->freq + node[i + 1]->freq;
		current->left = node[i];
		current->right = node[i + 1];
		node[i + 1] = current;
		alphabetCopy[i + 1].freq = current->freq;

		for (uint32_t j = i + 1; j < ALPHABET_SIZE - 1; j++)
		{
			if (alphabetCopy[j].freq > alphabetCopy[j + 1].freq)
			{
				Alphabet tempAlphabet;
				tempAlphabet.freq = alphabetCopy[j].freq;
				tempAlphabet.value = alphabetCopy[j].value;
				alphabetCopy[j].freq = alphabetCopy[j + 1].freq;
				alphabetCopy[j].value = alphabetCopy[j + 1].value;
				alphabetCopy[j + 1].freq = tempAlphabet.freq;
				alphabetCopy[j + 1].value = tempAlphabet.value;
				if (node[j + 1] != NULL)
				{
					Node tempNode;
					tempNode.freq = node[j]->freq;
					tempNode.value = node[j]->value;
					node[j]->freq = node[j + 1]->freq;
					node[j]->value = node[j + 1]->value;
					node[j + 1]->freq = tempNode.freq;
					node[j + 1]->value = tempNode.value;
				}
			}
			else
				break;
		}
	}
	delete alphabetCopy;
	(*root) = new Node;
	(*root)->left = node[size - 2];
	(*root)->right = node[size - 1];
}

void Huffman::Encode(Settings* settings, bool useAuxiliar)
{
}

void Huffman::Decode(Settings* settings, bool useAuxiliar)
{
}