#include "bwt.h"

#include <bitset>

/*
	@var input String a ser transformada
	@var line Linha da string original na matriz (escrever no arquivo)
	@var result Resultado da BWT
*/
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


	std::fstream* auxiliar = new std::fstream("auxiliarBWT.dat", std::ios::in | std::ios::out | std::ios::binary | std::ofstream::trunc);

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
		if (count == settings->textBlockSize - 1 || (quit && count != 0))
		{
			count = 0;
			text = BWT::RotateWord(input);

			std::sort(text.begin(), text.end());

			for (std::vector<std::string>::iterator it = text.begin(); it != text.end(); it++) {
				if ((*it).compare(input) == 0) {
					line = std::distance(text.begin(), it);
				}
				result.push_back((*it)[(*it).size() - 1]);
			}

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

/*
	@var input String transformada a ser aplicada a transformada reversa
	@var line Linha onde se encontra a string original (vem do arquivo)
	@var word Saida da transformarda reversa
*/
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


	std::fstream* auxiliar = new std::fstream("auxiliarBWT.dat", std::ios::in | std::ios::out | std::ios::binary | std::ofstream::trunc);

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

		if (count == settings->textBlockSize - 1 || (quit && count != 0))
		{
			count = 0;
			if(useAuxiliar)
				settings->auxiliar->read((char*)&line, sizeof(line));
			else
				settings->input->read((char*)&line, sizeof(line));

			index = BWT::GetIndex(input);

			for (int i = 0; i < input.length(); i++) {
				char aux = input[line];
				result.insert(result.begin(), aux);
				line = index[line];
			}

			auxiliar->write(result.c_str(), result.size());
#ifdef DEBUG
			std::cout << result.c_str() << std::bitset<sizeof(line)*8>(line);
#endif // DEBUG

			index.clear();
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

std::vector<std::string> BWT::RotateWord(std::string word) {
	std::vector<std::string> ret;

	for (int i = 0; i < word.size(); i++) {
		std::string cat;

		cat.append(word, i, std::string::npos);
		cat.append(word.begin(), word.begin() + i);
		//cat.replace(0, std::string::npos, cat, i, std::string::npos);

		ret.push_back(cat);
	}

	for (std::vector<std::string>::iterator it = ret.begin(); it != ret.end(); it++) {
		std::cout << *it << std::endl;
	}

	return ret;
}

std::vector<uint32_t> BWT::GetIndex(std::string ori) {
	std::string sort(ori);
	std::vector<uint32_t> index;
	//std::map<int, bool> control();
	std::vector<bool> control(ori.length(), false);

	std::sort(sort.begin(), sort.end());

	for (int i = 0; i < ori.length(); i++) {
		for (int j = 0; j < sort.length(); j++) {
			if (sort[j] == ori[i] && control[j] == false) {
				control[j] = true;
				index.push_back(j);
				break;
			}
		}
	}

	return index;
}