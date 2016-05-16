#include "huffman.h"

#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <bitset>

// Ordena o alfabeto pela frequencia
bool Huffman::SortFrequencies(Alphabet a, Alphabet b)
{
	return a.freq < b.freq;
}

// Imprime arvore
void Huffman::PrintTree(Node* node)
{
	bool leaf = (node->left == nullptr) & (node->right == nullptr);

	if (leaf)
	{
		std::cout << 1;
		std::cout << node->value;
	}
	else
	{
		std::cout << 0;
		PrintTree(node->left);
		PrintTree(node->right);
	}
}

// Cria os bits de cada alfabeto
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

// Escreve arvore no arquivo
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

// Le arvore do arquivo
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

// Libera da memoria arvore
void Huffman::DeleteTree(Node** node)
{
	if ((*node)->right != nullptr)
		DeleteTree(&(*node)->right);
	if((*node)->left != nullptr)
		DeleteTree(&(*node)->left);
	delete *node;
	*node = nullptr;
}

// Le o arquivo inteiro e cria o alfabeto e a arvore de Huffman
void Huffman::CreateHuffman(Settings* settings, Node** root, Alphabet** alphabet, bool useAuxiliar)
{
	uint32_t size = 0;
	*alphabet = (Alphabet*)calloc(ALPHABET_SIZE, sizeof(Alphabet));
	std::streampos position, end;

	if (useAuxiliar)
	{
		settings->auxiliar->clear();
		settings->auxiliar->seekg(-1, std::ios_base::end);
		end = settings->auxiliar->tellg();
		settings->auxiliar->seekg(0);
	}
	else
	{
		settings->input->clear();
		settings->input->seekg(-1, std::ios_base::end);
		end = settings->input->tellg();
		settings->input->seekg(0);
	}

	unsigned char byteRead;
	while (true)
	{
		if (useAuxiliar)
		{
			position = settings->auxiliar->tellg();
			settings->auxiliar->read((char*)&byteRead, 1);
			if (settings->auxiliar->eof() || position == end)
				break;
		}
		else
		{
			position = settings->input->tellg();
			settings->input->read((char*)&byteRead, 1);
			if (settings->input->eof())// || position == end)
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

// Codifica
void Huffman::Encode(Settings* settings, bool useAuxiliar)
{
	Node* tree;
	Alphabet* alphabet;
	std::streampos position, end;

	CreateHuffman(settings, &tree, &alphabet, useAuxiliar);

	std::fstream* auxiliar = new std::fstream("auxiliarHuffman.dat", std::ios::in | std::ios::out | std::ios::binary | std::ofstream::trunc);

	auxiliar->write((char*)&settings->offset, 1);

	WriteTree(auxiliar, tree, alphabet);

	// Calcula o offset (bits no caractere final validos)
	for (uint32_t i = 0; i < ALPHABET_SIZE; i++)
	{
		if (alphabet[i].freq > 0)
		{
#ifdef DEBUG
			std::cout << alphabet[i].value << " " << alphabet[i].bits << " " << alphabet[i].freq << std::endl;
#endif // DEBUG
			uint32_t temp = (strlen(alphabet[i].bits) * alphabet[i].freq) % 8;
			char tempChar = temp;
			settings->offset = (settings->offset + tempChar) % 8;
		}
	}

	if (settings->offset == 0)
		settings->offset = 7;

#ifdef DEBUG
	std::cout << (int)settings->offset << std::endl;
	PrintTree(tree);
	std::cout << std::endl;
#endif

	auxiliar->seekp(0);
	auxiliar->write((char*)&settings->offset, 1);
	auxiliar->seekp(0, std::ios_base::end);

	if (useAuxiliar)
	{
		settings->auxiliar->clear();
		settings->auxiliar->seekg(-1, std::ios_base::end);
		end = settings->auxiliar->tellg();
		settings->auxiliar->seekg(0);
	}
	else
	{
		settings->input->clear();
		settings->input->seekg(0, std::ios_base::end);
		end = settings->input->tellg();
		settings->input->seekg(0);
	}

	uint32_t bitWrite = 0;
	unsigned char byteRead;
	unsigned char byteWrite = UCHAR_MAX;
	while (true)
	{
		if (useAuxiliar)
		{
			position = settings->auxiliar->tellg();
			settings->auxiliar->read((char*)&byteRead, 1);
			if (settings->auxiliar->eof() || position == end)
				break;
		}
		else
		{
			position = settings->input->tellg();
			settings->input->read((char*)&byteRead, 1);
			if (settings->input->eof())// || position == end)
				break;
		}
		
		// Bits do byte atual
		char* bits = alphabet[(uint32_t)byteRead].bits;
		uint32_t bitsSize = strlen(bits);

		// Escreve dentro de byteWrite os bits do byte atual
		for (int i = 0; i < bitsSize; i++)
		{
			int index = 7 - bitWrite;
			if(bits[i] == '0')
				byteWrite = byteWrite ^ (1 << index);
			bitWrite++;
			if (bitWrite % 8 == 0)
			{
				auxiliar->put(byteWrite);
#ifdef DEBUG
				std::cout << std::bitset<8>(byteWrite);
#endif // DEBUG
				byteWrite = UCHAR_MAX;
				bitWrite = 0;
			}
		}
	}

	if (settings->offset != 7)
	{
		auxiliar->write((char*)&byteWrite, 1);
#ifdef DEBUG
		std::cout << std::bitset<8>(byteWrite);
#endif // DEBUG
	}
	byteWrite = '\n';
	auxiliar->write((char*)&byteWrite, 1);
#ifdef DEBUG
	std::cout << std::endl;
#endif // DEBUG

	delete alphabet;
	DeleteTree(&tree);

	if (settings->auxiliar != nullptr)
	{
		settings->auxiliar->close();
		delete settings->auxiliar;
	}
	auxiliar->flush();
	settings->auxiliar = auxiliar;
}

// Decodifica
void Huffman::Decode(Settings* settings, bool useAuxiliar)
{
	Node* tree;
	char byteRead;
	std::streampos last, end, position;

	if (useAuxiliar)
	{
		settings->auxiliar->clear();
		settings->auxiliar->seekg(-1, std::ios_base::end);
		end = settings->auxiliar->tellg();
		settings->auxiliar->seekg(-1, std::ios_base::cur);
		last = settings->auxiliar->tellg();
		settings->auxiliar->seekg(0);
		settings->auxiliar->read((char*)&settings->offset, 1);
		ReadTree(settings->auxiliar, &tree);
	}
	else
	{
		settings->input->clear();
		settings->input->seekg(-1, std::ios_base::end);
		end = settings->input->tellg();
		settings->input->seekg(-1, std::ios_base::cur);
		last = settings->input->tellg();
		settings->input->seekg(0);
		settings->input->read((char*)&settings->offset, 1);
		ReadTree(settings->input, &tree);
	}

#ifdef DEBUG
	std::cout << settings->offset << std::endl;
	PrintTree(tree);
	std::cout << std::endl;
#endif

	std::fstream* auxiliar = new std::fstream("auxiliarHuffman.dat", std::ios::in | std::ios::out | std::ios::binary | std::ofstream::trunc);

	Node* currentNode = tree;
	while (true)
	{
		if (useAuxiliar)
		{
			position = settings->auxiliar->tellg();
			settings->auxiliar->read(&byteRead, 1);
			if (settings->auxiliar->eof() || position == end)
				break;
		}
		else
		{
			position = settings->input->tellg();
			settings->input->read(&byteRead, 1);
			if (settings->input->eof() || position == end)
				break;
		}

#ifdef DEBUG
		std::cout << std::bitset<8>(byteRead);
#endif // DEBUG

		// Le o byte, percorrendo a arvore e ao chegar na folha escreve o caractere
		for (int i = 0; i < 8 && (position != last || i < settings->offset + 1); i++)
		{
			int index = 7 - i;
			bool bit = GETBIT(byteRead, index);
			if (bit)
				currentNode = currentNode->right;
			else
				currentNode = currentNode->left;
			if (currentNode->left == nullptr && currentNode->right == nullptr)
			{
				auxiliar->put(currentNode->value);
				currentNode = tree;
			}
		}
	}
	unsigned char byteWrite = '\n';
	auxiliar->put(byteWrite);
#ifdef DEBUG
	std::cout << std::endl;
#endif // DEBUG

	DeleteTree(&tree);

	if (settings->auxiliar != nullptr)
	{
		settings->auxiliar->close();
		delete settings->auxiliar;
	}
	auxiliar->flush();
	settings->auxiliar = auxiliar;
}