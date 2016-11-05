#include "BMP.h"

BMP::BMP()
    : Image() {}
BMP::BMP(const char *file)
    : Image(file) {}

SDL_Surface *BMP::loadImpl(const char *file)
{
    return SDL_LoadBMP(file);
}