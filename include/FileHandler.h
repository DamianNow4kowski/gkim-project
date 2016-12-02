#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>

namespace FileHandler
{
	bool verifyExtension(const char *, std::size_t, const char *, std::size_t);
	bool verifyExtension(const std::string &, const std::string &);
	bool verifyExtension(const char *, const char *);
	void ifopen(const char *, std::ifstream &);
	void ifopen(const std::string &, std::ifstream &);
	void ofopen(const char *, std::ofstream &);
	void ofopen(const std::string &, std::ofstream &);
}

#endif // !FILE_HANDLER_H