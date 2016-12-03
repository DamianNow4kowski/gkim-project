#ifndef IMAGE_H
#define IMAGE_H

#include "SDL_Local.h"
#include "RuntimeError.h"
#include <fstream>

class Image
{
private:
	SDL_Surface *surface;
	unsigned int w, h;
	uint8_t bitspp, bytespp;
	SDL_Texture *texturize(SDL_Renderer *);

protected:
	/**
	 * Custom implamentation of loading image file
	 * @param char[] filename of wanted image correct with self::extension()
	 * @return SDL_Surface* containing loaded image
	 */
	virtual SDL_Surface *loadImpl(const char *) = 0;
	
	/**
	 * Custom implamentation of saving Image::surface to file
	 * @param SDL_Surface* is always a pointer to Image->surface
	 * @param char[] name of saved file
	 */
	virtual void saveImpl(SDL_Surface *, const char *) = 0;

	void free();
	bool empty(const SDL_Surface* surf);
	SDL_Surface* makeSurface(int, int, int);
	SDL_Surface *copySurface(const SDL_Surface*);

public:
	// Construct
	Image();
	Image(const Image &);
	Image(const SDL_Surface *);
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
	uint32_t getPixel(const SDL_Surface*, unsigned int, unsigned int, bool = false) const;
	uint32_t getPixel(unsigned int, unsigned int, bool = false) const;
	SDL_Color getPixelColor(const SDL_Surface*, unsigned int, unsigned int) const;
	SDL_Color getPixelColor(unsigned int, unsigned int) const;
	void setPixel(SDL_Surface *, unsigned int, unsigned int, uint32_t, bool = false);
	void setPixel(SDL_Surface *, unsigned int, unsigned int, uint8_t, uint8_t, uint8_t, bool = false);
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
