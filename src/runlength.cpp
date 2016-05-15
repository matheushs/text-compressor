#include "runlength.h"

#include <vector>

void RunLength::Encode(Settings* settings, bool useAuxiliar)
{
	uint16_t count = 0;
	bool current = false;
	char byteRead;
	std::streampos inputSize, position;

	std::fstream* auxiliar = new std::fstream("auxiliarRunLength.dat", std::ios::in | std::ios::out | std::ios::binary | std::ofstream::trunc);

	if (useAuxiliar)
	{
		settings->auxiliar->clear();
		settings->auxiliar->seekg(0, std::ios_base::end);
		inputSize = settings->auxiliar->tellg();
		settings->auxiliar->seekg(0);
	}
	else
	{
		settings->input->clear();
		settings->input->seekg(0, std::ios_base::end);
		inputSize = settings->input->tellg();
		settings->input->seekg(0);
	}

	auxiliar->write(&settings->offset, 1);

	while (true)
	{
		if (useAuxiliar)
		{
			settings->auxiliar->read(&byteRead, 1);
			if (settings->auxiliar->eof())
				break;
			position = settings->auxiliar->tellg();
		}
		else
		{
			settings->input->read(&byteRead, 1);
			if (settings->input->eof())
				break;
			position = settings->input->tellg();
		}

		for (int i = 7; i >= 0 && (position != inputSize || i > 7 - settings->offset); i--)
		{
			if (current != GETBIT(byteRead, i))
			{
#ifdef DEBUG
				std::cout << count << std::endl;
#endif // DEBUG
				auxiliar->write((char*)&count, sizeof(count));
				current = !current;
				count = 1;
			}
			else
				count++;
		}
	}
	auxiliar->write((char*)&count, sizeof(count));

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
	std::streampos inputSize, position;
	char bitWrite = 0;

	std::fstream* auxiliar = new std::fstream("auxiliarRunLength.dat", std::ios::in | std::ios::out | std::ios::binary | std::ofstream::trunc);
	if (useAuxiliar)
	{
		settings->auxiliar->clear();
		settings->auxiliar->seekg(0, std::ios_base::end);
		inputSize = settings->auxiliar->tellg();
		settings->auxiliar->seekg(0);
		settings->auxiliar->read(&settings->offset, 1);
	}
	else
	{
		settings->input->clear();
		settings->input->seekg(0, std::ios_base::end);
		inputSize = settings->input->tellg();
		settings->input->seekg(0);
		settings->input->read(&settings->offset, 1);
	}
	//auxiliar->write((char*)&settings->offset, sizeof(settings->offset));

	unsigned char byteWrite = 0;
	while (true)
	{
		if (useAuxiliar)
		{
			settings->auxiliar->read((char*)&count, sizeof(count));
			if (settings->auxiliar->eof())
				break;
			position = settings->auxiliar->tellg();
		}
		else
		{
			settings->input->read((char*)&count, sizeof(count));
			if (settings->input->eof())
				break;
			position = settings->input->tellg();
		}
#ifdef DEBUG
		std::cout << count << std::endl;
#endif // DEBUG
		int shift = current == true ? 1 : 0;
		for (int i = 0; i < count && (position != inputSize || i < settings->offset); i++)
		{
			int index = 7 - bitWrite;
			byteWrite += shift << index;
			bitWrite++;
			if (bitWrite % 8 == 0)
			{
#ifdef DEBUG
				std::cout << byteWrite << std::endl;
#endif // DEBUG
				auxiliar->put(byteWrite);
				byteWrite = 0;
				bitWrite = 0;
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