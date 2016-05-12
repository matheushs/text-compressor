/**********************************
* Gustavo Ceccon          8936822 *
* Matheus Henrique Soares 8066349 *
**********************************/

/*
  Esse programa comprime e descomprime arquivos Run-Length, Huffman e TBW
  Entrada: encode	-i arquivo_original.txt	-o arquivo_binario.bin --bwt=X --txtblck=X --huffman=X –-runl=X
  @param -i arquivo de entrada (TXT).
  @param -o arquivo de saida (BIN).
  @param --bwt Transformada Burrows-Wheeler (BOOL).
  @param --txtblck Tamanho do bloco do BWT (INT).
  @param --huffman Compressão de Huffman (BOOL).
  @param --runl Compressão Run Length (BOOL).
*/

#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <cstring>
#include "settings.h"
#include "runlength.h"
#include "huffman.h"
#include "bwt.h"

#ifndef ENCODE
#define ENCODE 0
#endif // !ENCODE

#ifndef DECODE
#define DECODE 0
#endif // !DECODE

std::string ToUpper(char* text) {
	std::string result;
	char c[2] = "0";
	while (*text)
	{
		c[0] = toupper(*text);
		result.append(c);
		text++;
	}
	return result;
}

#define MIN_ARGS 8
#define INPUT_ARG "-i"
#define OUTPUT_ARG "-o"
#define BWT_ARG "--bwt="
#define TXT_BLOCK_ARG "--txtblck="
#define HUFFMAN_ARG "--huffman="
#define RUN_LENGTH_ARG "--runl="

void ReadArgs(Settings& settings, int argc, char* argv[])
{
	if (argc < MIN_ARGS + 1)
	{
		std::cerr << "Error: Missing arguments" << std::endl;
		exit(EXIT_FAILURE);
	}

	for (int i = 1; i < argc; i++)
	{
		std::string param(argv[i]);
		if (param.find(INPUT_ARG) != std::string::npos)
		{
			i++;
			if (i < argc)
			{
				settings.inputFilename = std::string(argv[i]);
				settings.input.exceptions(std::ifstream::failbit | std::ifstream::badbit);
				try {
					settings.input.open(argv[i]);
				}
				catch (std::ifstream::failure e) {
					std::cerr << "Error: Opening input file" << std::endl;
					std::cerr << e.what() << std::endl;
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				std::cerr << "Error: Missing arguments" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		else if (param.find(OUTPUT_ARG) != std::string::npos)
		{
			i++;
			if (i < argc)
			{
				settings.outputFilename = std::string(argv[i]);
				settings.output.exceptions(std::ifstream::failbit | std::ifstream::badbit);
				try {
					settings.output.open(argv[i], std::ofstream::binary | std::ofstream::trunc);
				}
				catch (std::ofstream::failure e) {
					std::cerr << "Error: Opening output file" << std::endl;
					std::cerr << e.what() << std::endl;
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				std::cerr << "Error: Missing arguments" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			bool paramBool = false;
			size_t pos;
			std::string paramUpper = ToUpper(argv[i]);

			if (paramUpper.find("TRUE") != std::string::npos) {
				paramBool = true;
			}
			else if (paramUpper.find("FALSE") != std::string::npos) {
				paramBool = false;
			}
			else if (param.find(TXT_BLOCK_ARG) == std::string::npos) {
				std::cerr << "Error: Invalid parameter. Expected: True|False" << std::endl;
				exit(EXIT_FAILURE);
			}

			if (param.find(BWT_ARG) != std::string::npos)
				settings.bwt = paramBool;
			else if ((pos = param.find(TXT_BLOCK_ARG)) != std::string::npos)
			{
				const char* txtBlockSize = &param.c_str()[pos + strlen(TXT_BLOCK_ARG)];
				settings.textBlockSize = std::atoi(txtBlockSize);
			}
			else if (param.find(HUFFMAN_ARG) != std::string::npos)
				settings.huffman = paramBool;
			else if (param.find(RUN_LENGTH_ARG) != std::string::npos)
				settings.runLength = paramBool;
			else
			{
				std::cerr << "Error: Invalid parameter." << std::endl;
				exit(EXIT_FAILURE);
			}
		}
	}
}

void WriteFile(Settings& settings, std::string& string)
{
	settings.output.seekp(0);
	settings.output.clear();
	settings.output.seekp(0);
	settings.output.write(string.c_str(), string.size());
}

int main(int argc, char *argv[]) {

	Settings settings;

	ReadArgs(settings, argc, argv);

	std::string bwtString, huffmanString, runlString;

	if (ENCODE)
	{
		if (settings.bwt)
		{
			BWT bwt(settings, true);
			bwt.Encode(bwtString);
		}
		if (settings.huffman)
		{
			if (settings.bwt)
			{
				Huffman huffman(bwtString);
				huffman.Encode(huffmanString);
			}
			else
			{
				Huffman huffman(settings, true);
				huffman.Encode(huffmanString);
			}
		}
		else if (settings.bwt)
			huffmanString = bwtString;
		if (settings.runLength)
		{
			if (!settings.bwt && !settings.huffman)
				RunLength::EncodeFromFile(settings, runlString);
			else
				RunLength::Encode(huffmanString, runlString);
		}
		else if (!settings.bwt && !settings.huffman)
			return EXIT_FAILURE;
		else
			runlString = huffmanString;
		WriteFile(settings, runlString);
	}
	else if(DECODE)
	{
		if (settings.runLength)
		{
			RunLength::DecodeFromFile(settings, runlString);
		}
		if (settings.huffman)
		{
			if (settings.runLength)
			{
				Huffman huffman(runlString);
				huffman.Decode(huffmanString);
			}
			else
			{
				Huffman huffman(settings, false);
				huffman.Decode(huffmanString);
			}
		}
		else if (settings.runLength)
			huffmanString = runlString;
		if (settings.bwt)
		{
			if (!settings.bwt && !settings.huffman)
			{
				BWT bwt(settings, false);
				bwt.Decode(bwtString);
			}
			else
			{
				BWT bwt(huffmanString);
				bwt.Decode(bwtString);
			}
		}
		else if (!settings.runLength && !settings.huffman)
			return EXIT_FAILURE;
		else
			bwtString = huffmanString;
		WriteFile(settings, bwtString);
	}

	settings.input.close();
	settings.output.close();

	return EXIT_SUCCESS;
}
