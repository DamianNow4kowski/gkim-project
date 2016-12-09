#pragma once
#include <string>
#include "SDL_Local.h"

/**
 * Utility functions for project and SDL library
 */
namespace SDL_Utils 
{
	/// Overall utility functions

	/** 
	 * Verifies extension of file
	 * @param cstring filename (with extension)
	 * @param size_t number of characters in filename
	 * @param cstring extension name (without dot)
	 * @param cstring number of extension characters
	 * @return true if extension matches | false otherwise
	 */
	bool verifyExtension(const char *, std::size_t, const char *, std::size_t);
	bool verifyExtension(const std::string &, const std::string &);
	bool verifyExtension(const char *, const char *);

	/**
	 * Opens input file stream
	 * @throws RuntimeError
	 */
	void ifopen(const char *, std::ifstream &);
	void ifopen(const std::string &, std::ifstream &);

	/**
	 * Opens output file stream
	 * @throws RuntimeError
	 */
	void ofopen(const char *, std::ofstream &);
	void ofopen(const std::string &, std::ofstream &);

	/// SDL Utility functions

	/**
	 * Gets (8|16|24|32) bits from pixel data
	 * @param pixel pointer to adress where chosen pixel begins in pixel data
	 * @param bpp size of pixel in number of bytes
	 * @return fully qualifed pixel data (32 bit)
	 */
	uint32_t getPixel(uint8_t*, uint8_t);


	/**
	 * Gets pixel data from SDL_Surface
	 * @param pointer to structure with pixel data
	 * @param x x-axis
	 * @param y y-axis cordinate of chosen pixel
	 * @param bool turns on debuging when true
	 * @return full pixel data (32 bit)
	 */
	uint32_t getPixel(const SDL_Surface*, unsigned int, unsigned int, bool = false);

	/**
	 * Gets pixel data from SDL_Surface
	 * @param pointer to structure with pixel data
	 * @param x x-axis
	 * @param y y-axis cordinate of chosen pixel
	 * @param bool turns on debuging when true
	 * @return pixel data in SDL_Color structure
	 */
	SDL_Color getPixelColor(const SDL_Surface*, unsigned int, unsigned int, bool = false);


	/**
	 * Sets pixel's data to SDL_Surface
	 * @param pointer to structure with pixel data
	 * @param x x-axis
	 * @param y y-axis cordinate of chosen pixel
	 * @param pixel data (32 bits)
	 * @param bool turns on debuging when true
	 */
	void setPixel(SDL_Surface *, unsigned int, unsigned int, uint32_t, bool = false);

	/**
	 * Sets pixel's color's data to SDL_Surface
	 * @param pointer to structure with pixel data
	 * @param x x-axis
	 * @param y y-axis cordinate of chosen pixel
	 * @param red component color value (8 bit)
	 * @param green component color value (8 bit) 
	 * @param blue component color value (8 bit)
	 * @param bool turns on debuging when true
	 */
	void setPixel(SDL_Surface *, unsigned int, unsigned int, uint8_t, uint8_t, uint8_t, bool = false);


	/**
	 * Sets pixel's color's data to SDL_Surface
	 * @param pointer to structure with pixel data
	 * @param x x-axis
	 * @param y y-axis cordinate of chosen pixel
	 * @param SDL_Color structure
	 * @param bool turns on debuging when true
	 */
	void setPixel(SDL_Surface *, unsigned int, unsigned int, const SDL_Color &, bool = false);


	/**
	 * Creates new SDL_Surface structure with default masks
	 * @param width
	 * @param height
	 * @param depth (bits per pixel)
	 * @return pointer to new SDL_Surface (remember to use SDL_FreeSurface())
	 */
	SDL_Surface *makeSurface(int, int, int);

}