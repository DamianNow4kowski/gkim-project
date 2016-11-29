#include <fstream>
#include <bitset>
#include <exception>
#include <vector>
#include <iostream>

using std::ofstream;
using std::ifstream;
using std::move;
using std::exception;
using namespace std;

class BitsToFile
{
private:
	unsigned char c;
	int pos;
	ofstream file;
	BitsToFile &write();

public:
	BitsToFile(ofstream &f);
	BitsToFile &to(bool f);
	BitsToFile &flush();
	void to(std::vector<bool> &vec);
	
};


BitsToFile &BitsToFile::write()
{
	file << c;
	c = 0;
	pos = 0;

	return *this;
}

BitsToFile::BitsToFile(ofstream& f)
{
	this->file = move(f);
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


class BitsFromFile
{
private:
	unsigned char  c;
	int pos;
	ifstream file;

public:
	BitsFromFile(ifstream &f);
	bool get();
};

BitsFromFile::BitsFromFile(ifstream& f)
{
	this->file = move(f);

	if (!this->file.good())
		throw exception("File is not good");
	if (this->file.eof())
		throw exception("File is end");

	this->pos = 8;
	this->c = 0;
}

bool BitsFromFile::get()
{
	if (this->pos == 8)
	{
		if (!this->file.eof())
		{
			this->file >> c;
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