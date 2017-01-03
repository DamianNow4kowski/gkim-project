#ifndef BITS_TO_FILE_H
#define BITS_TO_FILE_H

#include "RuntimeError.h"

#include <fstream>
#include <vector>
#include <iostream>

class BitsToFile
{
private:
	char c;
	int pos;
	std::ofstream &file;
	BitsToFile &write();

public:
	BitsToFile(std::ofstream &f);
	BitsToFile &to(bool f);
	BitsToFile &flush();
	void to(std::vector<bool> &vec);
};

class BitsFromFile
{
private:
	std::vector<char> buffer;
	std::vector<char>::iterator c;
	short pos;
	std::vector<char> read(std::ifstream &f);

public:
	BitsFromFile(std::ifstream &f);
	bool get();
};

#endif