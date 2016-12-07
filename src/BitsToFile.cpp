#include "BitsToFile.h"

BitsToFile &BitsToFile::write()
{
	//file << c;
	file.write((char*)&c, sizeof(char));
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
	if (this->pos)
	{
		this->write();
	}

	return *this;
}

void BitsToFile::to(std::vector<bool>& vec)
{
	for (auto v : vec)
		this->to(v);
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
		this->write();

	return *this;
}


BitsFromFile::BitsFromFile(ifstream& f)
	: file(f)
{
	if (!this->file.good())
		throw RuntimeError("File is not good");
	if (this->file.eof())
		throw RuntimeError("File is end");

	this->pos = 8;
	this->c = 0;
}

bool BitsFromFile::get()
{
	if (this->pos == 8)
	{
		if (!this->file.eof())
		{
			//this->file >> c;
			file.read((char*)&c, sizeof(char));
			this->pos = 0;
		}
		else
			cout << "EOF" << endl;
	}
	unsigned char help = this->c & 128;
	help >>= 7;
	this->c <<= 1;
	this->pos++;
	return static_cast<bool>(help);
}