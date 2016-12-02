#ifndef IMAGE_H
#define IMAGE_H

#include "SDL_Local.h"
#include "RuntimeError.h"
#include <fstream>

class Image
{
private:
	SDL_Surface *surface;
	unsigned int h, w;
	uint8_t bitspp, bytespp;
	SDL_Texture *texturize(SDL_Renderer *);

protected:
	/**
	 * Custom implamentation of loading image file
	 * @param cstring filename of wanted image
	 * @return SDL_Surface* containing loaded image
	 */
	virtual SDL_Surface *loadImpl(const char *) = 0;
	
	void free();
	bool empty(SDL_Surface* surf);
	virtual void saveImpl(SDL_Surface *, const char *) = 0;
	SDL_Surface* makeSurface(int, int, int);

public:
	// Construct
	Image();
	Image(SDL_Surface *);
	Image(const char *);

	// Virtual desctruct due to abstraction of this class
	virtual ~Image();

	/**
	 * @return accepted file extension
	 */
	virtual const char *extension() const = 0;

	// Functions
	void init(SDL_Surface *);
	void load(const char *, bool = true);
	void save(const char *);
	void preview();
	bool initialized();
	// @unsafe
	SDL_Surface *getSurface(const SDL_PixelFormat* = nullptr);
	// @unsafe
	SDL_Surface *getSurface(uint32_t);
	uint32_t getPixel(uint8_t *, uint8_t) const;
	uint32_t getPixel(const unsigned int &x, const unsigned int &y, bool = false) const;
	SDL_Color getPixelColorRGB(const int &x, const int &y) const;
	void setPixel(SDL_Surface *, const unsigned int &, const unsigned int &, uint32_t, bool = false);
	void setPixel(const unsigned int &x, const unsigned int &y, uint8_t R, uint8_t G, uint8_t B);
	void convertToGreyScale();
	unsigned int width() const;
	unsigned int height() const;
	unsigned int bpp() const;
	unsigned int depth() const;
	size_t size() const;

	/**
	 * @unsafe
	 * Only for tests
	 */
	SDL_Surface* img();
};

#endif // !IMAGE
