#include "RuntimeError.h"
#include "SDL.h"

// @construct
// BTW Coś mi się wydaje, że za te listy inicjalizacyjne srogo będzie szukał plagiatu w tym
RuntimeError::RuntimeError()
    : exception(), msg(SDL_GetError()) {}
RuntimeError::RuntimeError(const char *m)
    : exception(), msg(m) {}
RuntimeError::RuntimeError(const std::string &m)
    : exception(), msg(m) {}

// @destruct
RuntimeError::~RuntimeError() {}

// TODO: Fix this
// Why init with string and return cstring?
// Either create init with c_string or polymorfize what() to return string
const char *RuntimeError::what() const noexcept
{
    return msg.c_str();
}
