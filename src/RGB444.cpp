#include "RGB444.h"
#include <sstream>
#include <cstring>
#include <iostream>
#include <bitset>
#include <iomanip>
#include "FileHandler.h"

uint8_t RGB444::getColorOfPixel(SDL_Color color, int which)
{
	switch (which)
	{
	case 0:
		return color.r;
	case 1:
		return color.g;
	case 2:
		return color.b;
	default:
		return 0;
	}
}

void RGB444::setColorOfPixel(SDL_Color &color, uint8_t value, int which)
{
	switch (which)
	{
	case 0:
		color.r = value;
	case 1:
		color.g = value;
	case 2:
		color.b = value;
	default:
		return;
	}
}

RGB444::RGB444()
    : Image(), algorithm(0) {}
RGB444::RGB444(uint8_t alg) 
    : Image(), algorithm(alg) {}
RGB444::RGB444(const Image &bempe)
	: Image(bempe), algorithm(0)
{
}
RGB444::RGB444(const SDL_Surface *surface)
    : Image(surface), algorithm(0) {}
RGB444::RGB444(const char *file)
    : Image(file), algorithm(0) {}

void RGB444::save(const char *file, uint8_t alg)
{
	this->algorithm = alg;
	Image::save(file);
}

SDL_Surface *RGB444::loadImpl(const char *file)
{
	std::ifstream f;
    unsigned int w, h;
    uint8_t bpp, depth, alg;
    SDL_Surface *surf;

	// Open input file in binary mode
	FileHandler::ifopen(file, f);

    // Read header data
    this->readHeader(f, w, h, depth, bpp, alg);

	// Create new SDL Surface with default masks to save the pixels
	surf = this->makeSurface(w, h, depth);
	
	// Load depending on the alogrithm
	switch (alg) 
	{
		/// Your code goes here..
		/// Your code goes here..
		/// Your code goes here..
		/// Your code goes here..

	case 1:
		this->loadAlg444(surf, f);
		break;

	default:
		std::cerr << "There is no such an algorithm: " << (int)alg << std::endl << "Calling default.." << std::endl;
 	case 0:
		this->loadAlgDefault(surf, f);
		break;
	}

	// Return loaded surface
    return surf;
}

/**
 * @param SDL_Surface* surface private Image::surface
 */
void RGB444::saveImpl(SDL_Surface *surface, const char *file) 
{
	std::ofstream f;

	// Load file to save data in binary mode
	FileHandler::ofopen(file, f);

	// Choose algorithm
	switch (this->algorithm) 
	{
		/// Your code goes here..
		/// Your code goes here..
		/// Your code goes here..
	case 1:
		this->writeHeader(surface->w, surface->h, 12, 2, f);
		this->saveAlg444(surface, f);
		break;

	default:
		std::cerr << "There is no such an algorithm: " << (int)this->algorithm << std::endl << "Calling default.." << std::endl;
	case 0:
		this->writeHeader(surface, f);
		this->saveAlgDefault(surface, f);
		break;
	}

    f.close();
    std::cout << "---- SAVED ----" << std::endl;
    
}

/**
 * [SAVE]
 * Algorithm 0 - Default
 * Saving as it is in surface without compression
 */
void RGB444::saveAlgDefault(SDL_Surface *surface, std::ofstream &f)
{
	uint32_t pixel;
	int x, y;
	size_t bpp;

	bpp = static_cast<size_t>(surface->format->BytesPerPixel);

	// Save pixels one by one
	for (y = 0; y < surface->h; ++y)
	{
		for (x = 0; x < surface->w; ++x)
		{
			// -------------------------> turns on debug
			pixel = this->getPixel(x, y, (x == surface->w / 2));
			f.write(reinterpret_cast<const char *>(&pixel), bpp);
		}
	}
}

void RGB444::loadAlg444(SDL_Surface *surf, std::ifstream &f)
{
	int colorToCode = 0;
	int pixelUse = 0;
	char usedChar = 0;
	int k, x, y;
	SDL_Color pixel;

	x = y = 0;
	while (f.get(usedChar))
	{
		for (k = 0; k < 2; ++k)
		{
			if (k == 0)
				this->setColorOfPixel(pixel, ((usedChar >> 4) << 4), colorToCode);
			else
				this->setColorOfPixel(pixel, (usedChar << 4), colorToCode);

			colorToCode++;
			if (colorToCode == 3)
			{
				if (x == surf->w)
				{
					x = 0;
					y++;
				}

				// ---------------------------------------------------> turns on debug
				this->setPixel(surf, x, y, pixel.r, pixel.g, pixel.b, (y == surf->h/2 ));
				colorToCode = 0;
				x++;
			}
		}
	}

	// Close file
	f.close();
}

void RGB444::saveAlg444(SDL_Surface *surf, std::ofstream &f)
{
	int x, y, k;
	bool isSpace, firstHalf;
	char usedChar;
	SDL_Color color;

	isSpace = true;
	firstHalf = true;
	usedChar = 0;

	for (y = 0; y < surf->h; ++y)
	{
		for (x = 0; x < surf->w; ++x)
		{
			for (k = 0; k < 3; ++k) 
			{
				color = this->getPixelColor(surf, x, y);

				if (firstHalf)
				{
					usedChar = (getColorOfPixel(color, k) >> 4) << 4;
					firstHalf = false;
				}
				else
				{
					usedChar |= getColorOfPixel(color, k) >> 4;
					firstHalf = true;
					isSpace = false;
				}

				if (!isSpace)
				{
					// Binary save
					f.write(&usedChar, sizeof(usedChar));
					isSpace = true;
					usedChar = 0;
				}
			}
		}
	}
}

/**
 * [LOAD]
 * Algorithm 0 - Default
 * Loading just pixel by pixel
 */
void RGB444::loadAlgDefault(SDL_Surface *surf, std::ifstream &f)
{
	int x, y;
	char *data;
	uint8_t *data_ptr, bpp;
	uint32_t pixel;
	size_t size;

	// Get Bytes Per Pixel
	bpp = surf->format->BytesPerPixel;

	// We know exactly how much data we have to load from this: 
	size = static_cast<size_t>(surf->w * surf->h * bpp);

	// Load whole data img data
	data = new char[size];
	f.read(data, size);
	f.close();

	// Sets one byte pointer type to data
	data_ptr = reinterpret_cast<uint8_t*>(data);

	// Get pixels one by one
	for (y = 0; y < surf->h; ++y) 
	{
		for (x = 0; x < surf->w; ++x, data_ptr += bpp)
		{
			// Get pixel data in proper form
			pixel = this->getPixel(data_ptr, bpp);

			// ------------------------------> turns on debug
			this->setPixel(surf, x, y, pixel, (x == surf->w / 2));
		}
	}

	// Clean loaded data
	delete[] data;
}

const char *RGB444::extension() const
{
    return "rgb4";
}

void RGB444::readHeader(std::ifstream &f, unsigned int &w, unsigned int &h, uint8_t &depth, uint8_t &bpp, uint8_t &alg) 
{
    size_t size;
    char *buf;

    // Verify header
    f.read(reinterpret_cast<char *>(&size), sizeof(size));
	if(size > 1000)
		throw RuntimeError("Cannot load header. Either it is not vaild or too long.");
    buf = new char[size + 1];
    buf[size] = '\0'; // finish cstring
    f.read(buf, size);
    // by.. comparing extension
    if(strcmp(buf, this->extension()) != 0)
        throw RuntimeError("Cannot load file due to not vaild header format.");
    delete[] buf;

    // Load dimensions
    f.read(reinterpret_cast<char *>(&w), sizeof(w));
    f.read(reinterpret_cast<char *>(&h), sizeof(h));
    f.read(reinterpret_cast<char *>(&depth), sizeof(depth));
    f.read(reinterpret_cast<char *>(&bpp), sizeof(bpp));
	f.read(reinterpret_cast<char *>(&alg), sizeof(alg));

	// Debug
    std::cout << "Header Details:" << std::endl;
	std::cout << "- Algorithm: " << (int)alg << std::endl;
	std::cout << "- Width: " << w << std::endl;
    std::cout << "- Height: " << h << std::endl;
    std::cout << "- Depth: " << (int)depth << std::endl;
    std::cout << "- BPP: " << (int)bpp << std::endl;
}

void RGB444::writeHeader(SDL_Surface *surf, std::ofstream &f)
{
	this->writeHeader(surf->w, surf->h, surf->format->BitsPerPixel, surf->format->BytesPerPixel, f);
}

void RGB444::writeHeader(int w, int h, uint8_t depth, uint8_t bpp, std::ofstream &f)
{
	size_t ext_size;
	ext_size = strlen(this->extension());

	/**
	* Binary saving..
	* @var size_t the size of extension cstring
	* @var char* extension cstring
	* @var int width
	* @var int height
	* @var uint_8 depth (bits per pixel)
	* @var uint_8 bpp (bytes per pixel)
	* @var uint_8 algorithm
	*/
	f.write(reinterpret_cast<const char *>(&ext_size), sizeof(ext_size));
	f.write(this->extension(), ext_size);
	f.write(reinterpret_cast<const char *>(&w), sizeof(w));
	f.write(reinterpret_cast<const char *>(&h), sizeof(h));
	f.write(reinterpret_cast<const char *>(&depth), sizeof(depth));
	f.write(reinterpret_cast<const char *>(&bpp), sizeof(bpp));
	f.write(reinterpret_cast<const char *>(&this->algorithm), sizeof(this->algorithm));
}