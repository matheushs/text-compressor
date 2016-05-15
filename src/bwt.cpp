#include "bwt.h"
/*
	@var input String a ser transformada
	@var line Linha da string original na matriz (escrever no arquivo)
	@var result Resultado da BWT
*/
void BWT::Encode(Settings* settings, bool useAuxiliar)
{
	std::vector<std::string> text;
	std::string result;
	std::string input;
	int line = 0;

	text = BWT::RotateWord(input);
	
	std::sort(text.begin(), text.end());

	for (std::vector<std::string>::iterator it = text.begin(); it != text.end(); it++) {
		if ((*it).compare(input) == 0) {
			line = std::distance(text.begin(), it);
		}
		result.push_back((*it)[(*it).size() - 1]);
	}
}

/*
	@var input String transformada a ser aplicada a transformada reversa
	@var line Linha onde se encontra a string original (vem do arquivo)
	@var word Saida da transformarda reversa
*/
void BWT::Decode(Settings* settings, bool useAuxiliar)
{
	std::string input;
	std::vector<int> index;
	std::string word;
	int line;

	index = BWT::GetIndex(input);

	for (int i = 0; i < input.length(); i++) {
		char aux = input[line];
		word.insert(word.begin(), aux);
		line = index[line];
	}
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

std::vector<int> BWT::GetIndex(std::string ori) {
	std::string sort(ori);
	std::vector<int> index;
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