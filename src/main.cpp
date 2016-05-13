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

#ifdef ENCODE
#define ENCODE 1
#else
#define ENCODE 0
#endif


#ifdef DECODE
#define DECODE 1
#else
#define DECODE 0
#endif

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
				settings.inputFilename = std::string(argv[i]);
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
				settings.outputFilename = std::string(argv[i]);
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

int main(int argc, char *argv[]) {

	Settings settings;
#ifdef DEBUG
	settings.bwt = false;
	settings.huffman = false;
	settings.runLength = true;
	settings.textBlockSize = 64;
	if (ENCODE)
	{
		settings.inputFilename = "input.txt";
		settings.outputFilename = "encoded.dat";
	}
	else if (DECODE)
	{
		settings.inputFilename = "encoded.dat";
		settings.outputFilename = "output.txt";
	}
#else
	ReadArgs(settings, argc, argv);
#endif

	settings.input = new std::ifstream(settings.inputFilename);
	if (settings.input->exceptions())
	{
		std::cerr << "Error: Opening input file" << std::endl;
		exit(EXIT_FAILURE);
	}

	settings.output = new std::ofstream(settings.outputFilename, std::ofstream::binary | std::ofstream::trunc);
	if (settings.output->exceptions())
	{
		std::cerr << "Error: Opening output file" << std::endl;
		exit(EXIT_FAILURE);
	}


	if (!settings.runLength && !settings.huffman && !settings.bwt)
		exit(EXIT_FAILURE);

	if (ENCODE)
	{
		uint64_t size = UINT64_MAX;
		if (settings.bwt)
			BWT::Encode(&settings);
		if (settings.huffman)
			Huffman::Encode(&settings, settings.runLength);
		if (settings.runLength)
			RunLength::Encode(&settings, settings.huffman ? size : UINT64_MAX, settings.bwt || settings.huffman);
	}
	else if (DECODE)
	{
		uint64_t size;
		if (settings.runLength)
			RunLength::Decode(&settings, size);
		if (settings.huffman)
			Huffman::Decode(&settings, settings.runLength);
		if (settings.bwt)
			BWT::Decode(&settings, settings.runLength || settings.huffman);
	}
	else
		std::cerr << "Error: Encode/Decode not defined" << std::endl;

	settings.auxiliar->seekg(0);
	settings.output->seekp(0);

	while (!settings.auxiliar->eof())
	{
		char c = settings.auxiliar->get();
		if (settings.auxiliar->eof())
			break;
		settings.output->put(c);
	}

	settings.auxiliar->close();
	settings.input->close();
	settings.output->close();

	delete settings.auxiliar;
	delete settings.input;
	delete settings.output;

	return EXIT_SUCCESS;
}
