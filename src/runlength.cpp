#include "runlength.h"

#include <vector>

void RunLength::Encode(Settings* settings, bool useAuxiliar)
{
	uint16_t count = 0;
	bool current = false;
	char readByte[1];
	uint32_t sizeRead = 0;

	std::fstream* auxiliar = new std::fstream("auxiliarRunLegth.dat", std::ios::in | std::ios::out | std::ios::binary | std::ofstream::trunc);

	if (useAuxiliar)
		settings->auxiliar->seekg(0);
	else
		settings->input->seekg(0);

	auxiliar->write((char*)&settings->size, sizeof(settings->size));

	while (true)
	{
		if (useAuxiliar)
		{
			settings->auxiliar->read(readByte, 1);
			if (settings->auxiliar->eof())
				break;
		}
		else
		{
			settings->input->read(readByte, 1);
			if (settings->input->eof())
				break;
		}

		for (int i = 7; i >= 0 && sizeRead + 8 - i < settings->size; i--)
		{
			if (current != GETBIT(readByte[0], i))
			{
#ifdef DEBUG
				std::cout << count << std::endl;
#endif // DEBUG
				auxiliar->write((char*)&count, 2);
				current = !current;
				count = 1;
			}
			else
				count++;
		}
		sizeRead += 8;
	}
	auxiliar->write((char*)&count, 2);

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
	uint16_t count;
	bool current = false;
	char readBytes[2];
	uint32_t sizeRead = 0;

	std::fstream* auxiliar = new std::fstream("auxiliarRunLegth.dat", std::ios::in | std::ios::out | std::ios::binary | std::ofstream::trunc);
	if (useAuxiliar)
	{
		settings->auxiliar->seekg(0);
		settings->auxiliar->read((char*)&settings->size, sizeof(settings->size));
	}
	else
	{
		settings->input->seekg(0);
		settings->input->read((char*)&settings->size, sizeof(settings->size));
	}

	char writeChar = 0;
	while (true)
	{
		if (useAuxiliar)
		{
			settings->auxiliar->read(readBytes, 2);
			if (settings->auxiliar->eof())
				break;
		}
		else
		{
			settings->input->read(readBytes, 2);
			if (settings->input->eof())
				break;
		}

		count = *((uint16_t*)&readBytes);
#ifdef DEBUG
		std::cout << count << std::endl;
#endif // DEBUG
		int shift = current == true ? 1 : 0;
		for (int i = 0; i < count; i++)
		{
			int index = 7 - (sizeRead) % 8;
			writeChar += shift << index;
			sizeRead++;
			if (sizeRead % 8 == 0)
			{
#ifdef DEBUG
				std::cout << writeChar << std::endl;
#endif // DEBUG
				auxiliar->put(writeChar);
				writeChar = 0;
			}
		}
		current = !current;
	}

	if (settings->auxiliar != nullptr)
	{
		settings->auxiliar->close();
		delete settings->auxiliar;
	}
	auxiliar->flush();
	settings->auxiliar = auxiliar;
}