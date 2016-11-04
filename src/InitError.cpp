#include "InitError.h"
#include "SDL.h"

// @construct 
// BTW Coś mi się wydaje, że za te listy inicjalizacyjne srogo będzie szukał plagiatu w tym
InitError::InitError()
    : exception(), msg(SDL_GetError()) {}
InitError::InitError(const std::string &m)
    : exception(), msg(m) {}

// @destruct
InitError::~InitError() {}

// TODO: Fix this
// Why init with string and return cstring?
// Either create init with c_string or polymorfize what() to return string
const char *InitError::what() const noexcept
{
    return msg.c_str();
}
