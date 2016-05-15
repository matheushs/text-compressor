#include "huffman.h"

#include <cstdlib>
#include <cstring>
#include <algorithm>

bool Huffman::SortFrequencies(Alphabet a, Alphabet b)
{
	return a.freq < b.freq;
}

void Huffman::PrintTree(Node* tree)
{
	printf("%c %d\n", tree->value, tree->freq);
	if (tree->left)
		PrintTree(tree->left);
	if (tree->left)
		PrintTree(tree->right);
}

void Huffman::CreateHuffmanBits(Node* node, Alphabet* alphabet, char* bits, uint32_t size)
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

void Huffman::WriteTree(std::fstream* file, Node* node, Alphabet* alphabet)
{
	bool leaf = (node->left == nullptr) & (node->right == nullptr);
	if (leaf)
	{
		file->put(1);
		file->put(node->value);
	}
	else
		file->put(0);
	if (node->left)
		WriteTree(file, node->left, alphabet);
	if (node->right)
		WriteTree(file, node->right, alphabet);
}

void Huffman::ReadTree(std::istream* file, Node** node)
{
	char byteRead;
	file->get(byteRead);
	bool leaf = byteRead == 1;
	*node = (Node*)calloc(1, sizeof(Node));
	if (leaf)
	{
		file->get(byteRead);
		(*node)->value = byteRead;
	}
	else
	{
		ReadTree(file, &(*node)->left);
		ReadTree(file, &(*node)->right);
	}
}

void Huffman::CreateHuffman(Settings* settings, Node** root, Alphabet** alphabet, bool useAuxiliar)
{
	uint32_t size = 0;
	*alphabet = (Alphabet*)calloc(ALPHABET_SIZE, sizeof(Alphabet));
	while (true)
	{
		unsigned char byteRead;
		if (useAuxiliar)
		{

			settings->auxiliar->read((char*)&byteRead, 1);
			if (settings->auxiliar->eof())
				break;
		}
		else
		{
			settings->input->read((char*)&byteRead, 1);
			if (settings->input->eof())
				break;
		}
		if ((*alphabet)[(uint32_t)byteRead].freq == 0)
		{
			(*alphabet)[(uint32_t)byteRead].value = byteRead;
			size++;
		}
		(*alphabet)[(uint32_t)byteRead].freq++;
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
		node[i]->left = nullptr;
		node[i]->right = nullptr;
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

	if (useAuxiliar)
	{
		settings->auxiliar->clear();
		settings->auxiliar->seekg(0);
	}
	else
	{
		settings->input->clear();
		settings->input->seekg(0);
	}

	CreateHuffman(settings, &tree, &alphabet, useAuxiliar);
#ifdef DEBUG
	PrintTree(tree);
#endif // DEBUG


	std::fstream* auxiliar = new std::fstream("auxiliarHuffman.dat", std::ios::in | std::ios::out | std::ios::binary | std::ofstream::trunc);

	auxiliar->write(&settings->offset, 1);

	WriteTree(auxiliar, tree, alphabet);

	for (uint32_t i = 0; i < ALPHABET_SIZE; i++)
	{
		if (alphabet[i].freq > 0)
		{
#ifdef DEBUG
			printf("%c %s %d\n", alphabet[i].value, alphabet[i].bits, alphabet[i].freq);
#endif // DEBUG
			uint32_t temp = (strlen(alphabet[i].bits) * alphabet[i].freq) % 8;
			char tempChar = temp;
			settings->offset = (settings->offset + tempChar) % 8;
		}
	}
	auxiliar->seekp(0);
	auxiliar->write(&settings->offset, 1);
	auxiliar->seekp(0, std::ios_base::end);

	if (useAuxiliar)
	{
		settings->auxiliar->clear();
		settings->auxiliar->seekg(0);
	}
	else
	{
		settings->input->clear();
		settings->input->seekg(0);
	}

	uint32_t count = 0;
	unsigned char byteRead;
	unsigned char byteWrite = 0;
	while (true)
	{
		if (useAuxiliar)
		{

			settings->auxiliar->read((char*)&byteRead, 1);
			if (settings->auxiliar->eof())
				break;
		}
		else
		{
			settings->input->read((char*)&byteRead, 1);
			if (settings->input->eof())
				break;
		}
		char* bits = alphabet[(uint32_t)byteRead].bits;
		uint32_t bitsSize = strlen(bits);
		for (int i = 0; i < bitsSize; i++)
		{
			int index = 7 - count;
			if(bits[i] == '1')
				byteWrite += 1 << index;
			count++;
			if (count % 8 == 0)
			{
#ifdef DEBUG
				std::cout << byteWrite;
#endif // DEBUG
				auxiliar->put(byteWrite);
				byteWrite = 0;
				count = 0;
			}
		}
	}
	if(settings->offset > 0)
		auxiliar->write((char*)&byteWrite, 1);

	if (settings->auxiliar != nullptr)
	{
		settings->auxiliar->close();
		delete settings->auxiliar;
	}
	auxiliar->flush();
	settings->auxiliar = auxiliar;
}

void Huffman::Decode(Settings* settings, bool useAuxiliar)
{
	Node* tree;
	char byteRead;
	std::streampos inputSize, position;

	if (useAuxiliar)
	{
		settings->auxiliar->clear();
		settings->auxiliar->seekg(0, std::ios_base::end);
		inputSize = settings->auxiliar->tellg();
		settings->auxiliar->seekg(0);
		settings->auxiliar->read(&settings->offset, 1);
		ReadTree(settings->auxiliar, &tree);
	}
	else
	{
		settings->input->clear();
		settings->input->seekg(0, std::ios_base::end);
		inputSize = settings->input->tellg();
		settings->input->seekg(0);
		settings->input->read(&settings->offset, 1);
		ReadTree(settings->input, &tree);
	}

#ifdef DEBUG
	PrintTree(tree);
#endif

	std::fstream* auxiliar = new std::fstream("auxiliarHuffman.dat", std::ios::in | std::ios::out | std::ios::binary | std::ofstream::trunc);

	//auxiliar->write((char*)&settings->offset, sizeof(settings->offset));
	Node* currentNode = tree;
	uint32_t sizeRead = 0;
	while (true)
	{
		if (useAuxiliar)
		{
			settings->auxiliar->read(&byteRead, 1);
			if (settings->auxiliar->eof())
				break;
			position = settings->auxiliar->tellg();
		}
		else
		{
			settings->input->read(&byteRead, 1);
			if (settings->input->eof())
				break;
			position = settings->input->tellg();
		}

		for (int i = 7; i >= 0 && (position != inputSize || i > 7 - settings->offset); i--)
		{
			if (currentNode->left == nullptr && currentNode->right == nullptr)
			{
				auxiliar->put(currentNode->value);
				currentNode = tree;
			}
			bool bit = GETBIT(byteRead, i);
			if (bit)
				currentNode = currentNode->right;
			else
				currentNode = currentNode->left;
		}
		sizeRead += 8;
	}

	if (settings->auxiliar != nullptr)
	{
		settings->auxiliar->close();
		delete settings->auxiliar;
	}
	auxiliar->flush();
	settings->auxiliar = auxiliar;
}