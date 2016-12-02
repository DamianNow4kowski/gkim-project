#ifndef RGB444_H
#define RGB444_H

#include <fstream>
#include "Image.h"
//#include "BMP.h"

class RGB444 : public Image
{
	uint8_t algorithm;
protected:
	void writeHeader(SDL_Surface *, std::ofstream &);
	void readHeader(std::ifstream &, unsigned int &, unsigned int &, uint8_t &, uint8_t &, uint8_t &);

	/**
     * Load/Save methods 
	 * !!! Include algorithms here (in switch) !!!
	 */
	SDL_Surface *loadImpl(const char *);
	void saveImpl(SDL_Surface *, const char *);

	/**
	 * Algorithms
	 */
	void loadAlgDefault(SDL_Surface*, char *);
	void saveAlgDefault(SDL_Surface*, std::ofstream &);

public:
	RGB444();
	RGB444(uint8_t);
	RGB444(SDL_Surface *);
	RGB444(const char *);
	void save(const char *, uint8_t = 0);
	//RGB444(BMP *);
	const char *extension() const;
};

#endif // !RGB444_H