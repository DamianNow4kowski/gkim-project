#ifndef BITS_TO_FILE_H
#define BITS_TO_FILE_H

#include <fstream>
#include <bitset>
#include <exception>
#include <vector>
#include <iostream>
#include <utility>
#include "RuntimeError.h"

using std::ifstream;
using std::ofstream;
using std::endl;
using std::cout;
using std::move;

class BitsToFile
{
private:
	unsigned char c;
	int pos;
	ofstream &file;
	BitsToFile &write();

public:
	BitsToFile(ofstream &f);
	BitsToFile &to(bool f);
	BitsToFile &flush();
	void to(std::vector<bool> &vec);
};

class BitsFromFile
{
private:
	unsigned char  c;
	int pos;
	ifstream &file;

public:
	BitsFromFile(ifstream &f);
	bool get();
};

#endif