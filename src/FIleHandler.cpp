#include <cstring>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <bitset>

#include "RuntimeError.h"
#include "SDL_Local.h"

namespace FileHandler
{
	bool verifyExtension(const char *fileName, std::size_t nameSize, const char *ext, std::size_t extSize)
	{
		if (nameSize <= extSize)
			return false;
		const char *ptr = fileName;
		ptr += nameSize - extSize - 1;
		if (*ptr == '.')
		{
			return strcmp(++ptr, ext) == 0;
		}
		return false;
	}
	bool verifyExtension(const std::string &fileName, const std::string &ext)
	{
		return verifyExtension(fileName.c_str(), fileName.length(), ext.c_str(), ext.length());
	}
	bool verifyExtension(const char *fileName, const char *ext)
	{
		return verifyExtension(fileName, strlen(fileName), ext, strlen(ext));
	}

	
	void ifopen(const char *file, std::ifstream &input) 
	{
		input.open(file, std::ios::in | std::ios::binary);
		if (!input.is_open()) {
			std::ostringstream os;
			os << "Cannot open file in load mode: " << file;
			throw RuntimeError(os.str());
		}
	}

	void ifopen(const std::string &s, std::ifstream &input) {
		ifopen(s.c_str(), input);
	}

	void ofopen(const char *file, std::ofstream &output)
	{
		output.open(file, std::ios::out | std::ios::binary);
		if (!output.is_open()) {
			std::ostringstream os;
			os << "Cannot open file in write mode: " << file;
			throw RuntimeError(os.str());
		}
	}

	void ofopen(const std::string &s, std::ofstream &output) {
		ofopen(s.c_str(), output);
	}

}