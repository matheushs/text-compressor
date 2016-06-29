#include "runlength.h"

#include <vector>
#include <bitset>


// Codifica usando run-length byte a byte
void RunLength::EncodeByte(Settings* settings, bool useAuxiliar)
{
	// Estado binario, nao e necessario escrever o estado, comeca contagens de bits 0 e troca o current
	uint16_t count = 0;
	char current;
	char byteRead;
	std::streampos end, position;

	std::fstream* auxiliar = new std::fstream("auxiliarRunLength.dat", std::ios::in | std::ios::out | std::ios::binary | std::ofstream::trunc);

	if (useAuxiliar)
	{
		settings->auxiliar->clear();
		settings->auxiliar->seekg(0, std::ios_base::end);
		end = settings->auxiliar->tellg();
		settings->auxiliar->seekg(0);
	}
	else
	{
		settings->input->clear();
		settings->input->seekg(0, std::ios_base::end);
		end = settings->input->tellg();
		settings->input->seekg(0);
	}

	auxiliar->write((char*)&settings->offset, 1);

	while (true)
	{
		if (useAuxiliar)
		{
			position = settings->auxiliar->tellg();
			settings->auxiliar->read((char*)&byteRead, 1);
			if (settings->auxiliar->eof() || position == end)
				break;
		}
		else
		{
			position = settings->input->tellg();
			settings->input->read((char*)&byteRead, 1);
			if (settings->input->eof())// || position == end)
				break;
		}

		if ((int)position == 0)
			current = byteRead;
		if (byteRead == current)
		{
			count++;
			continue;
		}

		auxiliar->write((char*)&count, sizeof(count));
		auxiliar->write(&current, 1);
		count = 1;
		current = byteRead;
#ifdef DEBUG
			std::cout << count << "," << current << ";";
#endif // DEBUG
				
	}

	auxiliar->write((char*)&count, sizeof(count));
	auxiliar->write(&current, 1);
#ifdef DEBUG
	std::cout << count << "," << current << std::endl;
#endif // DEBUG

	if (settings->auxiliar != nullptr)
	{
		settings->auxiliar->close();
		delete settings->auxiliar;
	}
	auxiliar->flush();
	settings->auxiliar = auxiliar;
}

// Decodifica run-length byte a byte
void RunLength::DecodeByte(Settings* settings, bool useAuxiliar)
{
	std::streampos end, position;

	std::fstream* auxiliar = new std::fstream("auxiliarRunLength.dat", std::ios::in | std::ios::out | std::ios::binary | std::ofstream::trunc);
	if (useAuxiliar)
	{
		settings->auxiliar->clear();
		settings->auxiliar->seekg(0, std::ios_base::end);
		end = settings->auxiliar->tellg();
		settings->auxiliar->seekg(0);
		settings->auxiliar->read((char*)&settings->offset, 1);
	}
	else
	{
		settings->input->clear();
		settings->input->seekg(0, std::ios_base::end);
		end = settings->input->tellg();
		settings->input->seekg(0);
		settings->input->read((char*)&settings->offset, 1);
	}

	uint16_t count;
	char current;
	while (true)
	{
		if (useAuxiliar)
		{
			position = settings->auxiliar->tellg();
			settings->auxiliar->read((char*)&count, sizeof(count));
			settings->auxiliar->read(&current, 1);
			if (settings->auxiliar->eof() || position == end)
				break;
		}
		else
		{
			position = settings->input->tellg();
			settings->input->read((char*)&count, sizeof(count));
			settings->input->read(&current, 1);
			if (settings->input->eof() || position == end)
				break;
		}

#ifdef DEBUG
		std::cout << count << "," << current << ";";
#endif // DEBUG

		while (count--)
			auxiliar->write(&current, 1);

	}
		
	}

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

// Codifica usando run-length bit a bit
void RunLength::EncodeBit(Settings* settings, bool useAuxiliar)
{
	// Estado binario, nao e necessario escrever o estado, comeca contagens de bits 0 e troca o current
	uint16_t count = 0;
	bool current = false;
	unsigned char byteRead;
	std::streampos end, position;

	std::fstream* auxiliar = new std::fstream("auxiliarRunLength.dat", std::ios::in | std::ios::out | std::ios::binary | std::ofstream::trunc);

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
		settings->input->seekg(0, std::ios_base::end);
		end = settings->input->tellg();
		settings->input->seekg(0);
	}

	auxiliar->write((char*)&settings->offset, 1);

	while (true)
	{
		if (useAuxiliar)
		{
			position = settings->auxiliar->tellg();
			settings->auxiliar->read((char*)&byteRead, 1);
			if (settings->auxiliar->eof() || position == end)
				break;
		}
		else
		{
			position = settings->input->tellg();
			settings->input->read((char*)&byteRead, 1);
			if (settings->input->eof())// || position == end)
				break;
		}

		// Conta bits dentro do byteRead e conta bits iguais
		for (int i = 0; i < 8 && (position != end || i < settings->offset); i++)
		{
			int index = 7 - i;
			if (current != GETBIT(byteRead, index))
			{
#ifdef DEBUG
				std::cout << count << ",";
#endif // DEBUG
				auxiliar->write((char*)&count, sizeof(count));
				current = !current;
				count = 1;
			}
			else
				count++;
		}
	}
	unsigned char byteWrite = '\n';
	auxiliar->write((char*)&count, sizeof(count));
	auxiliar->write((char*)&byteWrite, 1);
#ifdef DEBUG
	std::cout << count << std::endl;
#endif // DEBUG

	if (settings->auxiliar != nullptr)
	{
		settings->auxiliar->close();
		delete settings->auxiliar;
	}
	auxiliar->flush();
	settings->auxiliar = auxiliar;
}

// Decodifica run-length bit a bit
void RunLength::DecodeBit(Settings* settings, bool useAuxiliar)
{
	std::streampos end, position;

	std::fstream* auxiliar = new std::fstream("auxiliarRunLength.dat", std::ios::in | std::ios::out | std::ios::binary | std::ofstream::trunc);
	if (useAuxiliar)
	{
		settings->auxiliar->clear();
		settings->auxiliar->seekg(-1, std::ios_base::end);
		end = settings->auxiliar->tellg();
		settings->auxiliar->seekg(0);
		settings->auxiliar->read((char*)&settings->offset, 1);
	}
	else
	{
		settings->input->clear();
		settings->input->seekg(-1, std::ios_base::end);
		end = settings->input->tellg();
		settings->input->seekg(0);
		settings->input->read((char*)&settings->offset, 1);
	}

	// Estado binario, nao e necessario escrever o estado, comeca contagens de bits 0 e troca o current
	uint16_t count;
	bool current = false;
	unsigned char bitWrite = 0;
	unsigned char byteWrite = UCHAR_MAX;
	while (true)
	{
		if (useAuxiliar)
		{
			position = settings->auxiliar->tellg();
			settings->auxiliar->read((char*)&count, sizeof(count));
			if (settings->auxiliar->eof() || position == end)
				break;
		}
		else
		{
			position = settings->input->tellg();
			settings->input->read((char*)&count, sizeof(count));
			if (settings->input->eof() || position == end)
				break;
		}

#ifdef DEBUG
		std::cout << count << ",";
#endif // DEBUG

		// Cria bits dentro do byteWrite dependendo do tamanho
		int shift = current == true ? 0 : 1;
		for (int i = 0; i < count; i++)
		{
			int index = 7 - bitWrite;
			byteWrite = byteWrite ^ (shift << index);
			bitWrite++;
			if (bitWrite % 8 == 0)
			{
				auxiliar->put(byteWrite);
#ifdef DEBUG
				std::cout << std::bitset<8>(byteWrite);
#endif // DEBUG
				byteWrite = UCHAR_MAX;
				bitWrite = 0;
			}
		}
		current = !current;
	}
	byteWrite = '\n';
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
