#include "runlength.h"

#include <vector>
#include <bitset>

void RunLength::Encode(Settings* settings, bool useAuxiliar)
{
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

void RunLength::Decode(Settings* settings, bool useAuxiliar)
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