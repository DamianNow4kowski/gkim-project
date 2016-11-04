#include "SDL.h"

SDL::SDL(Uint32 flags)
{
    if (SDL_Init(flags) != 0)
	throw InitError();
}

SDL::~SDL()
{
    SDL_Quit();
}
