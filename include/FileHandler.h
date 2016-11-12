#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>

namespace FileHandler
{
bool verifyExtension(const char *, std::size_t, const char *, std::size_t);
bool verifyExtension(std::string &, std::string &);
bool verifyExtension(const char *, const char *);
}

#endif // !FILE_HANDLER_H