#ifndef IMAGE_H
#define IMAGE_H

#include "SDL_Local.h"
#include "RuntimeError.h"
#include <fstream>

class Image
{
private:
	// Default container for pixels
	SDL_Surface *surface;

	/**
	 * Needed for Image::preview()
	 */
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
	bool empty(const SDL_Surface* surf) const;
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
	bool initialized() const;

	/**
	 * Gets the full pixel data
	 */
	uint32_t getPixel(uint8_t *, uint8_t) const;
	uint32_t getPixel(const SDL_Surface*, unsigned int, unsigned int, bool = false) const;
	uint32_t getPixel(unsigned int, unsigned int, bool = false) const;

	/**
	 * Gets SDL_Color of pixel
	 */
	SDL_Color getPixelColor(const SDL_Surface*, unsigned int, unsigned int) const;
	SDL_Color getPixelColor(unsigned int, unsigned int) const;

	/**
	 * Sets pixel
	 */
	void setPixel(SDL_Surface *, unsigned int, unsigned int, uint32_t, bool = false);
	void setPixel(SDL_Surface *, unsigned int, unsigned int, uint8_t, uint8_t, uint8_t, bool = false);
	void convertToGreyScale();

	/**
	 * Gets information of the surface
	 */
	unsigned int width() const;
	unsigned int height() const;
	unsigned int bpp() const;
	unsigned int depth() const;
	size_t size() const;

	/**
	 * !!! ATTENTION !!!
	 * Marked for delete
	 * !!! ATTENTION !!!
	 */
	SDL_Surface* img();
	SDL_Surface *getSurface(const SDL_PixelFormat* = nullptr);
	SDL_Surface *getSurface(uint32_t);
};

#endif // !IMAGE_H
