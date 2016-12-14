#include "BitsToFile.h"

BitsToFile &BitsToFile::write()
{
	file.write(&c, sizeof(c));
	c = 0;
	pos = 0;

	return *this;
}

BitsToFile::BitsToFile(std::ofstream& f)
	: file(f)
{
	c = 0;
	pos = 0;
}

BitsToFile& BitsToFile::flush()
{
	if (pos)
	{
		write();
	}

	return *this;
}

void BitsToFile::to(std::vector<bool>& vec)
{
	for (auto v : vec)
		to(v);
}

BitsToFile& BitsToFile::to(bool f)
{
	c <<= 1;
	if (f)
		c |= 1;
	else
		c |= 0;
	pos++;
	if (pos == 8)
		write();

	return *this;
}


BitsFromFile::BitsFromFile(std::ifstream& f)
	: file(f)
{
	if (!file.good())
		throw RuntimeError("File is not good");
	if (file.eof())
		throw RuntimeError("File is end");

	pos = 8;
	c = 0;
}

bool BitsFromFile::get()
{
	if (pos == 8)
	{
		if (!file.eof())
		{
			file.read(&c, sizeof(c));
			pos = 0;
		}
		else std::cout << "EOF" << std::endl;
	}
	char help = c & 128;
	help >>= 7;
	c <<= 1;
	pos++;
	
	// Fixed performance warning
	if (help)
		return true;
	else
		return false;
}