# Text-compressor

Compressor de arquivo usando codificacao BWT, Huffman ou Run-Length

# Linux

Para compilar o codificador, use:

```make encode```

Para compilar o decodificador, use:

```make decode```

# Windows

Use Visual Studio 2015 Community, abra o projeto, compile usando as configurações Encode ou Decode

# Argumentos

Use os argumentos para rodar tanto o codificador quanto o decodificador:

```encode -i arquivo_original.txt -o arquivo_binario.bin --bwt=X --txtblck=X --huffman=X –-runl=X```