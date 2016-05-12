#pragma once

#include "runlength.h"

void RunLength::Encode(std::string text, std::string& out)
{
	unsigned char count = 0;
	char last = text[0];

	//Loop que realiza a codificação

	// Começa a partir do segundo caracter,
	// Contando até que encontra um diferente,
	// Então será adicionado na string de retorno
	for (std::string::iterator it = text.begin() + 1; it != text.end(); it++) {
		if (*it == last && count < UCHAR_MAX) {
			count++;
		}
		else {
			out += last;
			out += count;
			count = 0;
			last = *it;
		}
	}
}

void RunLength::Decode(std::string text, std::string& out)
{
	unsigned char loop;
	char letter;

	// Loop que realiza a decodificação

	// A string segue o exemplo abaixo:
	// [0,5,1,2,0,13]
	// Para decodificar, os caracteres ficam nas
	// Posições pares, e a quantidade nas impares:
	// {0,5} -> 00000
	// 00000 é concatenado na string de retorno

	for (std::string::iterator it = text.begin(); it != text.end(); it++) {
		letter = *it;
		loop = *(++it);
		for (unsigned char i = 0; i < loop; i++) {
			out += letter;
		}
	}
}

void RunLength::EncodeFromFile(Settings& settings, std::string& out)
{
	settings.input.seekg(0);

	unsigned char count = 0;
	char last = settings.input.get();
	
	while (settings.input.eof()) {
		char c = settings.input.get();
		if (c == last && count < UCHAR_MAX) {
			count++;
		}
		else {
			out += count;
			out += last;
			count = 0;
			last = c;
		}
	}
}

void RunLength::DecodeFromFile(Settings& settings, std::string& out)
{
	settings.input.seekg(0);

	unsigned char loop;
	char letter;

	while (!settings.input.eof()) {
		letter = settings.input.get();
		loop = settings.input.get();
		for (unsigned char i = 0; i < loop; i++) {
			out += letter;
		}
	}
}