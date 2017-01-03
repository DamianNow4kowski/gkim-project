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
		c <<= 8 - pos;
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

std::vector<char> BitsFromFile::read(std::ifstream &f)
{
	return std::vector<char>(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
}

BitsFromFile::BitsFromFile(std::ifstream& f)
	: buffer(read(f)), c(buffer.begin()), pos(0)
{}

bool BitsFromFile::get()
{
	if (pos == 8)
	{
		pos = 0;
		++c;
	}

	char help = *c & 128;
	help >>= 7;
	*c <<= 1;
	++pos;
	
	return (help) ? true : false;
}