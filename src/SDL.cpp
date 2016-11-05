#include "SDL.h"

SDL::SDL(Uint32 flags)
{
    if (SDL_Init(flags) != 0)
	throw RuntimeError();
}

SDL::~SDL()
{
    SDL_Quit();
}
