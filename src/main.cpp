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
#include <cstdlib>
#include <cstring>
#include <ctime>
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


/*
Le os argumentos e cria configuracao
*/
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
	settings.bwt = true;
	settings.huffman = true;
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
	
	// Abre arquivo para input
	settings.input = new std::ifstream(settings.inputFilename, std::ofstream::binary);
	if (settings.input->exceptions())
	{
		std::cerr << "Error: Opening input file" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Abre arquivo para output
	settings.output = new std::ofstream(settings.outputFilename, std::ofstream::binary | std::ofstream::trunc);
	if (settings.output->exceptions())
	{
		std::cerr << "Error: Opening output file" << std::endl;
		exit(EXIT_FAILURE);
	}


	if (!settings.runLength && !settings.huffman && !settings.bwt)
		exit(EXIT_FAILURE);

	time_t after, before;

	// Le do arquivo de input pela primeira vez, codifica e guarda no arquivo auxiliar
	// A proxima codificacao le o arquivo auxiliar, codifica e guarda em outro arquivo auxiliar
	// Finalmente le o ultimo arquivo auxiliar e escreve no output

	// Codifica
	if (ENCODE)
	{
		std::cout << "Begining encoding: " << std::endl;
		if (settings.bwt)
		{
			std::cout << "Begining BWT " << settings.textBlockSize << std::endl;
			before = time(NULL);
			BWT::Encode(&settings, false);
			after = time(NULL);
			std::cout << difftime(after, before) << std::endl;

			if (settings.runLength)
			{
				std::cout << "Begining Run Length Byte" << std::endl;
				before = time(NULL);
				RunLength::EncodeByte(&settings, true);
				after = time(NULL);
				std::cout << difftime(after, before) << std::endl;
			}
		}
		if (settings.huffman)
		{
			if (settings.runLength && !settings.bwt)
			{
				std::cout << "Begining Run Length Bit" << std::endl;
				before = time(NULL);
				RunLength::EncodeByte(&settings, false);
				after = time(NULL);
				std::cout << difftime(after, before) << std::endl;
			}

			std::cout << "Begining Huffman" << std::endl;
			before = time(NULL);
			Huffman::Encode(&settings, settings.bwt | settings.runLength);
			after = time(NULL);
			std::cout << difftime(after, before) << std::endl;
		}
		if (settings.runLength && !settings.bwt && !settings.huffman)
		{
			std::cout << "Begining Run Length Byte" << std::endl;
			before = time(NULL);
			RunLength::EncodeByte(&settings, false);
			after = time(NULL);
			std::cout << difftime(after, before) << std::endl;
		}
	}
	// Decodifica
	else if (DECODE)
	{
		std::cout << "Begining decoding: " << std::endl;

		if (settings.runLength && !settings.bwt && !settings.huffman)
		{
			std::cout << "Begining Run Length Byte" << std::endl;
			before = time(NULL);
			RunLength::DecodeByte(&settings, false);
			after = time(NULL);
			std::cout << difftime(after, before) << std::endl;
		}
		if (settings.huffman)
		{
			std::cout << "Begining Huffman" << std::endl;
			before = time(NULL);
			Huffman::Decode(&settings, false);
			after = time(NULL);
			std::cout << difftime(after, before) << std::endl;

			if (settings.runLength && !settings.bwt)
			{
				std::cout << "Begining Run Length Bit" << std::endl;
				before = time(NULL);
				RunLength::EncodeByte(&settings, true);
				after = time(NULL);
				std::cout << difftime(after, before) << std::endl;
			}
		}
		if (settings.bwt)
		{
			if (settings.runLength)
			{
				std::cout << "Begining Run Length Byte" << std::endl;
				before = time(NULL);
				RunLength::DecodeByte(&settings, settings.huffman);
				after = time(NULL);
				std::cout << difftime(after, before) << std::endl;
			}

			std::cout << "Begining BWT " << settings.textBlockSize << std::endl;
			before = time(NULL);
			BWT::Decode(&settings, settings.runLength | settings.huffman);
			after = time(NULL);
			std::cout << difftime(after, before) << std::endl;
		}
	}
	else
		std::cerr << "Error: Encode/Decode not defined" << std::endl;

	// Le arquivo auxiliar e copia

	settings.auxiliar->clear();
	if(ENCODE)
		settings.auxiliar->seekg(0, std::ios_base::end);
	else
		settings.auxiliar->seekg(-1, std::ios_base::end);
	std::streampos end;
	end = settings.auxiliar->tellg();
	settings.auxiliar->seekg(0);
	settings.output->seekp(0);
	// Copia auxiliar para output
	while (settings.auxiliar->tellg() != end)
	{
		char c = settings.auxiliar->get();
		if (settings.auxiliar->eof())
			break;
		settings.output->put(c);
	}

	// Clean up
	settings.auxiliar->close();
	settings.input->close();
	settings.output->close();

	delete settings.auxiliar;
	delete settings.input;
	delete settings.output;

	return EXIT_SUCCESS;
}
