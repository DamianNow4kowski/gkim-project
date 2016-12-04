#include "BMP.h"

BMP::BMP()
	: Image()
{}

BMP::BMP(const Image &img)
	: Image(img)
{}

BMP::BMP(const SDL_Surface *surface)
	: Image(surface)
{}

SDL_Surface *BMP::loadImpl(const char *file)
{
	SDL_Surface *ret = SDL_LoadBMP(file);
	if (ret == NULL)
		throw RuntimeError(); // SDL_GetError();
	return ret;
}

void BMP::saveImpl(SDL_Surface *surface, const char *file) {
	SDL_SaveBMP(surface, file);
}

const char *BMP::extension() const
{
	return "bmp";
}