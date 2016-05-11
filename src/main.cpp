/**********************************
* Gustavo Ceccon                  *
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
#include "runlength.h"

std::string ToUpper(std::string text){
  std::string result;
  std::transform(text.begin(), text.end(), result, toupper);
  return result;
}

int main(int argc, char *argv[]){
  if(argc < 9) {
    std::cout << "Error: Missing Arguments" << std::endl;
    return EXIT_FAILURE;
  }

  //TODO: Pegar texto dos arquivos (fstream e stringstream)
  std::string inputFile = argv[2];
  std::string outputFile = argv[4];
  std::string bwtParam = ToUpper(argv[5]);
  std::string huffmanParam = ToUpper(argv[7]);
  std::string runlParam = ToUpper(argv[8]);

  std::ifstream input;
  std::ofstream output;

  input.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try{
    input.open(argv[2]);
  }
  catch(std::ifstream::failure e){
    std::cout << "Erro: Arquivo de entrada inválido." << std::endl;
    std::cout << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  output.exceptions(std::ofstream::failbit | std::ofstream::badbit);
  try{
    output.open(argv[4], std::ofstream::binary | std::ofstream::trunc);
  }
  catch(std::ofstream::failure e){
    std::cout << "Erro: Arquivo de saída inválido." << std::endl;
    std::cout << e.what() << std::endl;
    return EXIT_FAILURE;
  }


  bool bwt;
  bool huffman;
  bool runl;

  std::stringstream buffer;
  buffer << input.rdbuf();
  std::string result(buffer.str());

  if(bwtParam.find("TRUE") != std::string::npos){
    bwt = true;
  }else if (bwtParam.find("FALSE") != std::string::npos){
    bwt = false;
  }else {
    std::cout << "Error: Invalid Parameter for BWT. Expected: True|False" << std::endl;
    return EXIT_FAILURE;
  }

  //TODO: Checar por erros no textBlock
  std::string::size_type sz;
  int txtBlck = std::stoi(argv[6], &sz);

  if(huffmanParam.find("TRUE") != std::string::npos){
    huffman = true;
  }else if (huffmanParam.find("FALSE") != std::string::npos){
    huffman = false;
  }else {
    std::cout << "Error: Invalid Parameter for Huffman. Expected: True|False" << std::endl;
    return EXIT_FAILURE;
  }

  if(runlParam.find("TRUE") != std::string::npos){
    runl = true;
  }else if (runlParam.find("FALSE") != std::string::npos){
    runl = false;
  }else {
    std::cout << "Error: Invalid Parameter for Run-Length. Expected: True|False" << std::endl;
    return EXIT_FAILURE;
  }

  if(bwt){
    //result = btw(result);
  }
  if(huffman){
    //result = huffman(result);
  }
  if(runl){
    //result = runl(result);
  }

  input.close();
  output.close();

  return EXIT_SUCCESS;
}
