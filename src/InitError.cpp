#include "InitError.h"

InitError::InitError() : exception(), msg(SDL_GetError()) {}

InitError::InitError(const std::string& m) : exception(), msg(m) {}

InitError::~InitError() {}

const char* InitError::what() const
{
	return msg.c_str();
}
