#ifndef RGB444_H
#define RGB444_H

#include <fstream>
#include "Image.h"
#include "BMP.h"

class RGB444 : public Image
{
	uint8_t algorithm;
protected:
	void writeHeader(SDL_Surface *, std::ofstream &);
	void writeHeader(int, int, uint8_t, uint8_t, std::ofstream &);
	void readHeader(std::ifstream &, unsigned int &, unsigned int &, uint8_t &, uint8_t &, uint8_t &);
	SDL_Surface * convertSurface(const SDL_Surface*);

	/**
     * Load/Save methods
	 * Include your algorithms there in switch
	 */
	SDL_Surface *loadImpl(const char *);
	void saveImpl(SDL_Surface *, const char *);

	/**
	 * Algorithms
	 */
	void loadAlgDefault(SDL_Surface*, std::ifstream &);
	void saveAlgDefault(SDL_Surface*, std::ofstream &);
	void loadAlg444(SDL_Surface*, std::ifstream &);
	void saveAlg444(SDL_Surface*, std::ofstream &);
	
	/**
	 * Needed for algorithms
	 */
	uint8_t getColorOfPixel(SDL_Color, int c);
	void setColorOfPixel(SDL_Color &, uint8_t, int);

public:
	RGB444();
	RGB444(const RGB444 &);
	RGB444(const Image &);
	RGB444(const SDL_Surface *);

	void save(const char *, uint8_t = 0);
	const char *extension() const;
};

#endif // !RGB444_H