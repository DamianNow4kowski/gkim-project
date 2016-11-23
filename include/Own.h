#ifndef OWN_H
#define OWN_H

#include "Image.h"
#include <string>
#include "SDL.h"
#include <fstream>
#include <iostream>

class Own : public Image
{
private:
	int algorithm;
	std::fstream file;
	void writeHeader();
	void readHeader();
	unsigned char getColorOfPixel(SDL_Color col, int c);
	void setColorOfPixel(SDL_Color &pixel, int c, unsigned char value);

protected:
	SDL_Surface *loadImpl(const char *);
	void saveImpl(SDL_Surface *, const char *);
public:
	Own();
	Own(SDL_Surface *);
	const char *extension() const;
};
#endif // !OWN_H
