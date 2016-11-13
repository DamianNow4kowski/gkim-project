#include "BMP.h"

BMP::BMP()
    : Image() {}
BMP::BMP(SDL_Surface *surface)
    : Image(surface) {}
BMP::BMP(const char *file)
    : Image(file) {}

SDL_Surface *BMP::loadImpl(const char *file)
{
    return SDL_LoadBMP(file);
}

const char *BMP::extension() const
{
    return "bmp";
}