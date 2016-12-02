#include "RGB444.h"
#include <sstream>
#include <cstring>
#include <iostream>
#include <bitset>
#include <iomanip>
#include "FileHandler.h"

RGB444::RGB444()
    : Image(), algorithm(0) {}
RGB444::RGB444(uint8_t alg) 
    : Image(), algorithm(alg) {}
RGB444::RGB444(SDL_Surface *surface)
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
    char *data;
    size_t size;
    uint8_t bpp, depth, alg;
    SDL_Surface *surf;

	// Open input file in binary mode
	FileHandler::ifopen(file, f);

    // Read header data
    this->readHeader(f, w, h, depth, bpp, alg);
    
    // We know exactly how much data we have to load from this: 
    size = static_cast<size_t>(w*h*bpp);

    // Load whole data img data
    data = new char[size];
    f.read(data, size);
    f.close();

	// Create new SDL Surface with default masks to save the pixels
	surf = this->makeSurface(w, h, depth);
	
	// Load depending on the alogrithm
	switch (alg) 
	{
		/// Your code goes here..
		/// Your code goes here..
		/// Your code goes here..
		/// Your code goes here..

	default:
		std::cerr << "There is no such an algorithm: " << (int)alg << std::endl << "Calling default.." << std::endl;
 	case 0:
		this->loadAlgDefault(surf, data);
		break;
	}

	// Clean an return loaded SDL_Surface
    delete data;
    return surf;
}

void RGB444::saveImpl(SDL_Surface *surface, const char *file) 
{
	std::ofstream f;

	// Load file to save data in binary mode
	FileHandler::ofopen(file, f);

    // SDL_Surface *surface = private Image::surface
    this->writeHeader(surface, f);

	// Choose algorithm
	switch (this->algorithm) 
	{
		/// Your code goes here..
		/// Your code goes here..
		/// Your code goes here..

	default:
		std::cerr << "There is no such an algorithm: " << (int)this->algorithm << std::endl << "Calling default.." << std::endl;
	case 0:
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

/**
 * [LOAD]
 * Algorithm 0 - Default
 * Loading just pixel by pixel
 */
void RGB444::loadAlgDefault(SDL_Surface *surf, char *data)
{
	int x, y;
	uint8_t *data_ptr, bpp;
	uint32_t pixel;

	// Get Bytes Per Pixel
	bpp = surf->format->BytesPerPixel;

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
	if(size <= 0 || size > 1000)
		throw RuntimeError("Cannot load header. Either it is not vaild or too long.");
    buf = new char[size + 1];
    buf[size] = '\0'; // finish cstring
    f.read(buf, size);
    // by.. comparing extension
    if(strcmp(buf, this->extension()) != 0)
        throw RuntimeError("Cannot load file due to not vaild header format.");
    delete buf;

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
    size_t ext_size;
    ext_size = strlen(this->extension());

    /**
     * Saving..
     * @var size_t the size of extension cstring
     * @var char* extension cstring
     * @var int width
     * @var  int height
	 * @var uint_8 depth (bits per pixel)
     * @var uint_8 bpp (bytes per pixel)
	 * @var uint_8 algorithm
     */
    f.write(reinterpret_cast<const char *>(&ext_size), sizeof(ext_size));
    f.write(this->extension(), ext_size);
    f.write(reinterpret_cast<const char *>(&surf->w), sizeof(surf->w));
    f.write(reinterpret_cast<const char *>(&surf->h), sizeof(surf->h));
    f.write(reinterpret_cast<const char *>(&surf->format->BitsPerPixel), sizeof(surf->format->BitsPerPixel));
	f.write(reinterpret_cast<const char *>(&surf->format->BytesPerPixel), sizeof(surf->format->BytesPerPixel));
	f.write(reinterpret_cast<const char *>(&this->algorithm), sizeof(this->algorithm));
}