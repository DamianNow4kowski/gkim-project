#ifndef IMAGE_H
#define IMAGE_H

#include "SDL_Local.h"
#include <iostream>
#include <tuple>

class Image
{
private:

	// Deallocates whole previously allocated data
	void free();

	// Swapper for better class construction
	void swap(Image &);

protected:

	/**
	 * Pointer to SDL_Surface structure containing pixel data 
	 * and every needed informations to process an image
	 */
	SDL_Surface *surface;

	/// SDL Utility functions for private purposes 
	/// (they do NOT modify the object directly - so they are const)
	/// !!! Thats what you love the most !!!
	//         V     O    T     E
	//  A) Leave it here
	//  B) Move to "namespace SDL_Utils {}"
	//
	//  Marcin [   ]    Marek  [   ]
	//  Konrad [ A ]    Damian [   ]

	/**
	 * Creates an empty SDL_Surface (RGB) with paramters
	 * @param width of the surface
	 * @param height of the surface
	 * @param color depth of the surface (number of bits each pixel takes in memory)
	 * @return pointer to newly allocated SDL_Surface structure
	 * @throws RuntimError when allocation fails
	 */
	SDL_Surface *create(int, int, int) const;

	/**
	 * Creates a copy of existing SDL_Surface strucutre
	 * @return pointer to newly allocated SDL_Surface structure
	 * @throws RuntimError when allocation fails
	 */
	SDL_Surface *copy(const SDL_Surface *) const;

	/**
	 * Gets (8|16|24|32) bits from pixel data
	 * @param pixel pointer to adress where chosen pixel begins in pixel data
	 * @param bpp size of pixel in number of bytes
	 * @return fully qualifed pixel data (32 bit)
	 */
	uint32_t getPixel(uint8_t*, uint8_t) const;

	/**
	 * Gets pixel data from SDL_Surface
	 * @param pointer to structure with pixel data
	 * @param x x-axis
	 * @param y y-axis cordinate of chosen pixel
	 * @param bool turns on debuging when true
	 * @return full pixel data (32 bit)
	 */
	uint32_t getPixel(const SDL_Surface*, unsigned int, unsigned int, bool = false) const;

	/**
	 * Gets pixel data from SDL_Surface
	 * @param pointer to structure with pixel data
	 * @param x x-axis
	 * @param y y-axis cordinate of chosen pixel
	 * @param bool turns on debuging when true
	 * @return pixel data in SDL_Color structure
	 */
	SDL_Color getPixelColor(const SDL_Surface*, unsigned int, unsigned int, bool = false) const;

	/**
	 * Sets pixel's data to SDL_Surface
	 * @param pointer to structure with pixel data
	 * @param x x-axis
	 * @param y y-axis cordinate of chosen pixel
	 * @param pixel data (32 bits)
	 * @param bool turns on debuging when true
	 */
	void setPixel(SDL_Surface *, unsigned int, unsigned int, uint32_t, bool = false) const;

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
	void setPixel(SDL_Surface *, unsigned int, unsigned int, uint8_t, uint8_t, uint8_t, bool = false) const;


	/**
	 * Converts RGB color to grey scale component
	 * @param SDL_Color structure that stores RGB component colors
	 * @return uint8_t grey scale color made from these RGB components
	 * Standard:
	 * @link https://en.wikipedia.org/wiki/Grayscale#Luma_coding_in_video_systems
	 */
	uint8_t toGreyScale(const SDL_Color &);

public:

	// Default constructor
	Image();

	// Create empty surface constructor
	Image(int, int, int);

	// Attaching surface to this image
	Image(SDL_Surface *);

	// Copying surface
	Image(const SDL_Surface *);

	// Copy constructor
	Image(const Image &);

	// Move constructor
	Image(Image &&);

	// Universal (copy and move assigment)
	Image& operator=(Image);

	// Copy assigment
	//Image& operator=(const Image &);
	// Move assigment
	//Image& operator=(Image &&);

	// Destructor
	~Image();

	// Public interface functions [safetly modify SDL_Surface]
	// @throws RuntimeError
	// TODO: inline some of them
	void allocate(int, int, int);
	void toGreyScale();
	uint32_t getPixel(unsigned int, unsigned int, bool = false) const;
	SDL_Color getPixelColor(unsigned int, unsigned int, bool = false) const;
	void setPixel(unsigned int, unsigned int, uint32_t, bool = false);
	void setPixel(unsigned int, unsigned int, uint8_t, uint8_t, uint8_t, bool = false);
	void setPixel(unsigned int, unsigned int, const SDL_Color &, bool = false);
	void printDetails(std::ostream & = std::cout) const;

	/**
	 * @return pointer to constant SDL_Structure which
	 * allows to access data, but won't let to modify it
	 */
	const SDL_Surface* img() const;

	/**
	 * @param rendering context
	 * @return pointer to newly render texture from this surface
	 * Note: Surface is not freed or modified upon invoking that method
	 * @throws RuntimError on failure
	 */
	SDL_Texture* texture(SDL_Renderer *) const;

	/**
	 * @return number of pixels in horizontal line
	 */
	unsigned int width() const;

	/** 
	 * @return number of pixels in vertical line
	 */
	unsigned int height() const;
	
	/**
	 * @return number of bytes each pixel takes in memory
	 */
	unsigned int bpp() const;

	/**
	 * @return color depth of the image
	 */
	unsigned int depth() const;

	/**
	 * @return number of bytes needed to store pixel data in memory
	 */
	size_t size() const;

	/**
	 * Indicates wheter SDL_Surface structure is initialized or not
	 * @return bool true if so | false otherwise
	 */
	bool empty() const;
};

#endif // !IMAGE_H