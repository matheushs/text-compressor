#pragma once

#include <iostream>
#include <fstream>
#include <cstdlib>

#define GETBIT(data, index) ((bool)((data & (1 << index)) >> index))

/*
Configuracao de execucao
*/
struct Settings
{
	// Controles
	bool bwt = false;
	bool huffman = false;
	bool runLength = false;

	// Tamanho de bloco
	uint32_t textBlockSize = 0;
	//Offset binario
	unsigned char offset = 7;

	// Nomes dos arquivos de input e output
	std::string inputFilename;
	std::string outputFilename;

	// Ponteiros para os arquivos
	std::ifstream* input;
	std::ofstream* output;
	std::fstream* auxiliar = nullptr;
};