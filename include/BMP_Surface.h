#ifndef BMP_SURFACE_H
#define BMP_SURFACE_H

#include <SDL.h>
#include <string>

class BMP_Surface
{
private:
	SDL_Surface *surface;

public:
	BMP_Surface();
	BMP_Surface(const std::string &filename);
	~BMP_Surface();

	// public methods
	Uint32 getPixel(const int &x, const int &y) const;
	int getWidth() const;
	int getHeigth() const;
	bool loadBMP(const std::string &filename);
};

#endif // !BMP_SURFACE_H
