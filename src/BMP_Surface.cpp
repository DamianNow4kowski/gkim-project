#include "BMP_Surface.h"

BMP_Surface::BMP_Surface()
{
	this->surface = nullptr;
}

BMP_Surface::BMP_Surface(const std::string &filename)
{
	this->surface = SDL_LoadBMP(filename.c_str());
}

BMP_Surface::~BMP_Surface()
{
	SDL_FreeSurface(this->surface);
}

Uint32 BMP_Surface::getPixel(const int & x, const int & y) const
{
	int bpp = this->surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)this->surface->pixels + y * this->surface->pitch + x * bpp;

	switch (bpp) 
	{
	case 1:
		return *p;

	case 2:
		return *(Uint16 *)p;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;

	case 4:
		return *(Uint32 *)p;

	default:
		return 0;       /* shouldn't happen, but avoids warnings */
	}
}

int BMP_Surface::getWidth() const
{
	if (this->surface == nullptr)
		return 0;
	return this->surface->w;
}

int BMP_Surface::getHeigth() const
{
	if (this->surface == nullptr)
		return 0;
	return this->surface->h;
}

bool BMP_Surface::loadBMP(const std::string & filename)
{
	this->surface = SDL_LoadBMP(filename.c_str());
	return this->surface != NULL ? true : false;
}
