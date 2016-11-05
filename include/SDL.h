#ifndef SDL_H
#define SDL_H

#ifdef __linux
	#include <SDL2/SDL.h>
#else
	#include <SDL.h>
#endif

#include "RuntimeError.h"

class SDL 
{
public:
	SDL(Uint32 flags = 0);
	virtual ~SDL();
};

#endif //!SDL_H