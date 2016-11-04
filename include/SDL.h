#ifndef SDL_H
#define SDL_H

#include <SDL.h>
#include "InitError.h"

class SDL 
{
public:
	SDL(Uint32 flags = 0);
	virtual ~SDL();
};

#endif //!SDL_H