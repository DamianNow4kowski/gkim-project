#ifndef OWN
#define OWN

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
	SDL_Surface *loadImpl(const char *) {return nullptr;}
public:
	Own();
	Own(SDL_Surface *);
	const char *extension() const {return nullptr;}

	Own &readFile(std::string name);
	Own &saveFile(std::string name);
};
#endif // !OWN
