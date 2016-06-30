#include "bwt.h"

#include <bitset>

// Codifica
void BWT::Encode(Settings* settings, bool useAuxiliar)
{
	char byteRead;
	uint32_t count = 0;
	bool quit = false;
	std::vector<std::string> text;
	std::string result;
	std::string input;
	uint32_t line = 0;
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


	std::fstream* auxiliar = new std::fstream("BWT Encode.dat", std::ios::in | std::ios::out | std::ios::binary | std::ofstream::trunc);

	while (!quit)
	{

		if (useAuxiliar)
		{
			settings->auxiliar->read(&byteRead, 1);
			position = settings->input->tellg();
			if (settings->auxiliar->eof() || position == end)
				quit = true;
			else
			{
				input.push_back(byteRead);
				count++;
			}
		}
		else
		{
			settings->input->read(&byteRead, 1);
			//position = settings->input->tellg();
			if (settings->input->eof())// || position == end)
				quit = true;
			else
			{
				input.push_back(byteRead);
				count++;
			}
		}
		
		if (count == settings->textBlockSize || (quit && count != 0))
		{
			count = 0;

			// Rotaciona a string original
			BWT::RotateWord(text, input);

			// Ordena
			std::sort(text.begin(), text.end());

			// Codifica e guarda indice para reconstrucao
			for (std::vector<std::string>::iterator it = text.begin(); it != text.end(); it++) {
				if ((*it).compare(input) == 0) {
					line = std::distance(text.begin(), it);
				}
				result.push_back((*it)[(*it).size() - 1]);
			}
			
			// Escreve string ordenada 
			auxiliar->write(result.c_str(), result.size());
			auxiliar->write((char*)&line, sizeof(line));
#ifdef DEBUG
			std::cout << result.c_str() << std::bitset<sizeof(line) * 8>(line);
#endif // DEBUG

			text.clear();
			input.clear();
			result.clear();
		}
	}
	unsigned char byteWrite = '\n';
	auxiliar->put(byteWrite);
#ifdef DEBUG
	std::cout << std::endl;
#endif // DEBUG

	if (settings->auxiliar != nullptr)
	{
		settings->auxiliar->close();
		delete settings->auxiliar;
	}
	auxiliar->flush();
	settings->auxiliar = auxiliar;
}

// Decodifica
void BWT::Decode(Settings* settings, bool useAuxiliar)
{
	char byteRead;
	std::streampos position, end;
	uint32_t count = 0;
	bool quit = false;
	std::vector<uint32_t> index;
	std::string result;
	std::string input;
	uint32_t line = 0;

	bool* zeros = (bool*)calloc(settings->textBlockSize, sizeof(bool));
	bool* control = (bool*)calloc(settings->textBlockSize, sizeof(bool));

	if (useAuxiliar)
	{
		settings->auxiliar->clear();
		settings->auxiliar->seekg(-static_cast<int32_t>(sizeof(line)) - 1, std::ios_base::end);
		end = settings->auxiliar->tellg();
		settings->auxiliar->seekg(0);
	}
	else
	{
		settings->input->clear();
		settings->input->seekg(-static_cast<int32_t>(sizeof(line)) - 1, std::ios_base::end);
		end = settings->input->tellg();
		settings->input->seekg(0);
	}


	std::fstream* auxiliar = new std::fstream("BWT Decode.dat", std::ios::in | std::ios::out | std::ios::binary | std::ofstream::trunc);

	while (!quit)
	{

		if (useAuxiliar)
		{
			position = settings->auxiliar->tellg();
			if (position == end)
				quit = true;
			else
			{
				settings->auxiliar->read(&byteRead, 1);
				input.push_back(byteRead);
				count++;
			}
		}
		else
		{
			position = settings->input->tellg();
			if (position == end)
				quit = true;
			else
			{
				settings->input->read(&byteRead, 1);
				input.push_back(byteRead);
				count++;
			}
		}

		// Ao terminar de ler o bloco ou chegar no final
		if (count == settings->textBlockSize || (quit && count != 0))
		{
			count = 0;
			// Le linha da rotacao
			if(useAuxiliar)
				settings->auxiliar->read((char*)&line, sizeof(line));
			else
				settings->input->read((char*)&line, sizeof(line));

			// Indices para reconstruir
			BWT::GetIndex(index, control, input);

			// Reconstroi a string original
			for (int i = 0; i < input.length(); i++) {
				char aux = input[line];
				result.insert(result.begin(), aux);
				line = index[line];
			}

			// Escreve no arquivo
			auxiliar->write(result.c_str(), result.size());
#ifdef DEBUG
			std::cout << result.c_str() << std::bitset<sizeof(line)*8>(line);
#endif // DEBUG

			index.clear();
			input.clear();
			result.clear();
			std::memcpy(control, zeros, settings->textBlockSize * sizeof(bool));
		}
	}
	unsigned char byteWrite = '\n';
	auxiliar->put(byteWrite);
#ifdef DEBUG
	std::cout << std::endl;
#endif // DEBUG

	if (settings->auxiliar != nullptr)
	{
		settings->auxiliar->close();
		delete settings->auxiliar;
	}
	auxiliar->flush();
	settings->auxiliar = auxiliar;
}

// Rotaciona a palavra
void BWT::RotateWord(std::vector<std::string> &rotate, std::string& word)
{
	for (int i = 0; i < word.size(); i++) {
		std::string cat;

		cat.append(word, i, std::string::npos);
		cat.append(word.begin(), word.begin() + i);
		rotate.push_back(cat);
	}

#ifdef DEBUG
	for (std::vector<std::string>::iterator it = rotate.begin(); it != rotate.end(); it++) {
		std::cout << *it << std::endl;
	}
#endif
}

// Retorna vetor de indices para reconstruir a string original
void BWT::GetIndex(std::vector<std::uint32_t>& index, bool* control, std::string& input)
{
	std::string sort(input);

	std::sort(sort.begin(), sort.end());

	for (int i = 0; i < input.length(); i++) {
		for (int j = 0; j < sort.length(); j++) {
			if (sort[j] == input[i] && control[j] == false) {
				control[j] = true;
				index.push_back(j);
				break;
			}
		}
	}
}