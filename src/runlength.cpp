#pragma once

#include "runlength.h"

std::string RunLength::Encode(std::string text) {
	unsigned char count = 0;
	char last = text[0];
	std::string encoded;

	//Loop que realiza a codifica��o

	// Come�a a partir do segundo caracter,
	// Contando at� que encontra um diferente,
	// Ent�o ser� adicionado na string de retorno
	for (std::string::iterator it = text.begin() + 1; it != text.end(); it++) {
		if (*it == last && count < UCHAR_MAX) {
			count++;
		}
		else {
			//std::stringbuffer buffer;
			//buffer << count;
			//encoded += buffer.str() + last;
			encoded += count;
			encoded += last;
			count = 0;
			last = *it;
		}
	}

	return encoded;
}

std::string RunLength::Decode(std::string text) {
	std::string decoded;
	unsigned char loop;
	char letter;

	// Loop que realiza a decodifica��o

	// A string segue o exemplo abaixo:
	// [0,5,1,2,0,13]
	// Para decodificar, os caracteres ficam nas
	// Posi��es pares, e a quantidade nas impares:
	// {0,5} -> 00000
	// 00000 � concatenado na string de retorno

	for (std::string::iterator it = text.begin(); it != text.end(); it + 2) {
		letter = *it + 1;
		loop = *it;
		for (unsigned char i = 0; i < loop; i++) {
			decoded += letter;
		}
	}

	return decoded;
}

void RunLength::EncodeFromFile(Settings& settings, std::string& out, bool writeFile)
{

}

void RunLength::DecodeFromFile(Settings& settings, std::string& out, bool writeFile)
{

}