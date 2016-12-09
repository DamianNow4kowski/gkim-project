#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>

namespace FileHandler
{
	/// Overall utility functions

	/**
	* Verifies extension of file
	* @param cstring filename (with extension)
	* @param size_t number of characters in filename
	* @param cstring extension name (without dot)
	* @param cstring number of extension characters
	* @return true if extension matches | false otherwise
	*/
	bool verifyExtension(const char *, std::size_t, const char *, std::size_t);
	bool verifyExtension(const std::string &, const std::string &);
	bool verifyExtension(const char *, const char *);

	/**
	* Opens input file stream
	* @throws RuntimeError
	*/
	void ifopen(const char *, std::ifstream &);
	void ifopen(const std::string &, std::ifstream &);

	/**
	* Opens output file stream
	* @throws RuntimeError
	*/
	void ofopen(const char *, std::ofstream &);
	void ofopen(const std::string &, std::ofstream &);
}

#endif // !FILE_HANDLER_H