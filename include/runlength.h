#ifndef _RUN_LENGTH_H_
#define _RUN_LENGTH_H_

#include <iostream>
#include <stdlib.h>
#include <string>

//TODO: Testar e debugar o Run-Length

class RunLength {
public:
	std::string Encode(std::string text) {
		unsigned char count = 0;
		char last = text[0];
		std::string encoded;

		//Loop que realiza a codificação

		// Começa a partir do segundo caracter,
		// Contando até que encontra um diferente,
		// Então será adicionado na string de retorno
		for (std::string::iterator it = text.begin() + 1; it != text.end(); it++) {
			if (*it == last) {
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

	std::string Decode(std::string text) {
		std::string decoded;
		unsigned char loop;
		char letter;

		// Loop que realiza a decodificação

		// A string segue o exemplo abaixo:
		// [0,5,1,2,0,13]
		// Para decodificar, os caracteres ficam nas
		// Posições pares, e a quantidade nas impares:
		// {0,5} -> 00000
		// 00000 é concatenado na string de retorno

		for (std::string::iterator it = text.begin(); it != text.end(); it + 2) {
			letter = *it + 1;
			loop = *it;
			for (unsigned char i = 0; i < loop; i++) {
				decoded += letter;
			}
		}

		return decoded;
	}

private:
};

#endif
