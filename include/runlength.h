#ifndef _RUN_LENGTH_H_
#define _RUN_LENGTH_H_

#include <iostream>
#include <stdlib.h>
#include <string>

//TODO: Testar e debugar o Run-Length

class RunLength {
public:
  std::string Encode(std::string text){
    int count = 0;
    char last = text[0];
    std::string encoded;

    //Loop que realiza a codificação
    for(std::string::iterator it=text.begin()+1; it!=text.end(); it++){ //Começa a partir do segundo caracter,
      if(*it == last){                                                  //contando até que encontra um diferente,
        count++;                                                        //então será adicionado na string de retorno
      } else{
        encoded += (std::to_string(count) + last);  //NOTE: MinGW não suporta 'to_string', deve rodar no Linux
        count = 0;
      }
    }

    return encoded;
  }

  std::string Decode(std::string text){
    std::string decoded;
    int loop;
    char letter;

    //Loop que realiza a decodificação
    for(std::string::iterator it = text.begin(); it != text.end(); it+2){ //A string segue o exemplo abaixo:
      letter = *it;                                                       //[0,5,1,2,0,13]
      loop = *it+1;                                                       //Para decodificar, os caracteres ficam nas
        for(int i = 0; i < loop; i++){                                    //posições pares, e a quantidade nas impares:
          decoded += letter;                                              //{0,5} -> 00000
        }                                                                 //00000 é concatenado na string de retorno
    }

    return decoded;
  }

private:
};

#endif
