#include "BitsToFile.h"

BitsToFile &BitsToFile::write()
{
	//file << c;
	file.write(reinterpret_cast<char *>(&c), sizeof(c));
	c = 0;
	pos = 0;

	return *this;
}

BitsToFile::BitsToFile(ofstream& f)
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


BitsFromFile::BitsFromFile(ifstream& f)
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
			//file >> c;
			file.read(reinterpret_cast<char*>(&c), sizeof(c)); // why not change c type from unsigned char to char? (no need casting)
			pos = 0;
		}
		else
			cout << "EOF" << endl;
	}
	unsigned char help = c & 128;
	help >>= 7;
	c <<= 1;
	pos++;
	
	// Fixed performance warning
	if (help)
		return true;
	else
		return false;
}