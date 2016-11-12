#include <cstring> // strlen
#include <string>

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
bool verifyExtension(std::string &fileName, std::string &ext)
{
    return verifyExtension(fileName.c_str(), fileName.length(), ext.c_str(), ext.length());
}
bool verifyExtension(const char *fileName, const char *ext)
{
    return verifyExtension(fileName, strlen(fileName), ext, strlen(ext));
}
}