#include <cstring>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <bitset>

#include "RuntimeError.h"
#include "SDL_Local.h"
#include "..\include\SDL_Utils.h"

namespace SDL_Utils
{
	bool verifyExtension(const char *fileName, std::size_t nameSize, const char *ext, std::size_t extSize)
	{
		if (nameSize <= extSize)
			return false;
		const char *ptr = fileName;
		ptr += nameSize - extSize - 1;
		if (*ptr == '.')
		{
			return strcmp(++ptr, ext) == 0;
		}
		return false;
	}
	bool verifyExtension(const std::string &fileName, const std::string &ext)
	{
		return verifyExtension(fileName.c_str(), fileName.length(), ext.c_str(), ext.length());
	}
	bool verifyExtension(const char *fileName, const char *ext)
	{
		return verifyExtension(fileName, strlen(fileName), ext, strlen(ext));
	}

	
	void ifopen(const char *file, std::ifstream &input) 
	{
		input.open(file, std::ios::in | std::ios::binary);
		if (!input.is_open()) {
			std::ostringstream os;
			os << "Cannot open file in load mode: " << file;
			throw RuntimeError(os.str());
		}
	}

	void ifopen(const std::string &s, std::ifstream &input) {
		ifopen(s.c_str(), input);
	}

	void ofopen(const char *file, std::ofstream &output)
	{
		output.open(file, std::ios::out | std::ios::binary);
		if (!output.is_open()) {
			std::ostringstream os;
			os << "Cannot open file in write mode: " << file;
			throw RuntimeError(os.str());
		}
	}

	void ofopen(const std::string &s, std::ofstream &output) {
		ofopen(s.c_str(), output);
	}

	
	uint32_t getPixel(uint8_t *pixel, uint8_t bpp) 
	{
		switch (bpp)
		{
		// 8bit integer
		case 1:
			return *pixel;

		// 16bit integer
		case 2:
			return *reinterpret_cast<uint16_t *>(pixel);

		// 24bit integer
		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return pixel[0] << 16 | pixel[1] << 8 | pixel[2];
			else
				return pixel[0] | pixel[1] << 8 | pixel[2] << 16;

		// 32bit integer
		case 4:
			return *reinterpret_cast<uint32_t *>(pixel);

		// Should not happen, but avoids warnings
		default:
			throw RuntimeError("Couldn't get pixel. BytesPerPixel set to 0.");
			return 0;
		}
	}

	uint32_t getPixel(const SDL_Surface *surface, unsigned int x, unsigned int y, bool debug)
	{
		if (surface == NULL)
			throw RuntimeError("Cannot get pixel data of uninitialized surface.");

		// Verify coordinates
		if (x >= static_cast<unsigned int>(surface->w) || y >= static_cast<unsigned int>(surface->h))
		{
			std::ostringstream os;
			os << "Cannot get pixel. Out of surface range [w=" << surface->w << ", h=" << surface->h << "] was x=" << x << " y=" << y << '.';
			throw RuntimeError(os.str());
		}

		uint8_t *pixel, bpp;
		bpp = surface->format->BytesPerPixel;
		pixel = reinterpret_cast<uint8_t *>(surface->pixels) + y * surface->pitch + x * bpp;
		uint32_t ret = getPixel(pixel, bpp);

		// Print pixel bits when debugging
		if (debug) {
			std::bitset<32> b(ret);
			std::cout << "Pixel[x=" << std::setw(3) << x << "][y=" << std::setw(3) << y << "] = " << b << std::endl;
		}
		return ret;
	}

	// TODO: test it
	SDL_Color getPixelColor(const SDL_Surface *surface, unsigned int x, unsigned int y, bool debug)
	{
		// Get pixel data
		uint32_t pixel = getPixel(surface, x, y);
		
		// Get rgb color components
		SDL_Color rgb = {0, 0, 0, 0};
		if (surface->format->palette == NULL)
		{
			rgb.r = ((pixel & surface->format->Rmask) >> surface->format->Rshift) << surface->format->Rloss;
			rgb.g = ((pixel & surface->format->Gmask) >> surface->format->Gshift) << surface->format->Gloss;
			rgb.b = ((pixel & surface->format->Bmask) >> surface->format->Bshift) << surface->format->Bloss;
		}
		// Pallette is used in SDL when depth <= 8
		else if (pixel < static_cast<uint32_t>(surface->format->palette->ncolors))
		{
			rgb.r = surface->format->palette->colors[pixel].r;
			rgb.g = surface->format->palette->colors[pixel].g;
			rgb.b = surface->format->palette->colors[pixel].b;
		}
		
		// Print RGB bits when debugging
		if (debug) {
			std::bitset<8> r(rgb.r), g(rgb.g), b(rgb.b);
			std::cout << "Pixel[x=" << std::setw(3) << x << "][y=" << std::setw(3) << y << "] = R[" << r << "] G[ " << g << "] B[" << b << "]" << std::endl;
		}
		return rgb;
	}

	void setPixel(SDL_Surface *surface, unsigned int x, unsigned int y, uint32_t pixel, bool debug)
	{
		if (surface == NULL)
			throw RuntimeError("Cannot set pixel data of uninitialized surface.");

		// Verify coordinates
		if (x >= static_cast<unsigned int>(surface->w) || y >= static_cast<unsigned int>(surface->h))
		{
			std::ostringstream os;
			os << "Cannot set pixel. Out of surface range [w=" << surface->w << ", h=" << surface->h << "] was x=" << x << " y=" << y << '.';
			throw RuntimeError(os.str());
		}

		uint8_t *p, bpp;
		bpp = surface->format->BytesPerPixel;
		p = reinterpret_cast<uint8_t*>(surface->pixels) + y * surface->pitch + x * bpp;

		// Show setted pixel bits when debugging
		if (debug) {
			std::bitset<32> b(pixel);
			std::cout << "Pixel[x=" << std::setw(3) << x << "][y=" << std::setw(3) << y << "] = " << b << std::endl;
		}

		switch (bpp)
		{
		// 8-bit
		case 1:
			*p = pixel;

			// Debug
			if (debug) 
			{
				std::bitset<8> b1(*p);
				std::cout << std::setw(22) << "Bitmap = " << std::setw(32) << b1 << std::endl;
			}
			break;
		// 16-bit
		case 2:
			*reinterpret_cast<uint16_t*>(p) = pixel;

			// Debug
			if (debug) 
			{
				std::bitset<16> b1(*reinterpret_cast<uint16_t*>(p));
				std::cout << std::setw(22) << "Bitmap = " << std::setw(32) << b1 << std::endl;
			}
			break;
		// 24-bit
		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = pixel & 0xff;
			}
			else {
				p[0] = pixel & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}

			// Debug
			if (debug) 
			{
				std::bitset<8> b1(p[0]);
				std::bitset<8> b2(p[1]);
				std::bitset<8> b3(p[2]);
				std::cout << std::setw(22) << "Bitmap = " << std::setw(16) << b1 << b2 << b3 << std::endl;
			}
			break;
		// 32-bit
		case 4:
			*reinterpret_cast<uint32_t*>(p) = pixel;

			// Debug
			if (debug) 
			{
				std::bitset<32> b1(*reinterpret_cast<uint32_t*>(p));
				std::cout << std::setw(22) << "Bitmap = " << std::setw(32) << b1 << std::endl;
			}
			break;
		}
	}

	// TODO: Test it
	void setPixel(SDL_Surface *surface, unsigned int x, unsigned int y, uint8_t R, uint8_t G, uint8_t B, bool debug)
	{
		if (surface == NULL)
			throw RuntimeError("Cannot set pixel data of uninitialized surface.");

		uint32_t rgb;
		if (surface->format->palette == NULL)
		{
			rgb = (R >> surface->format->Rloss) << surface->format->Rshift
				| (G >> surface->format->Gloss) << surface->format->Gshift
				| (B >> surface->format->Bloss) << surface->format->Bshift;
				//| surface->format->Amask; // I think it is unneseccary
		}
		else throw RuntimeError("Cannot set RGB components values of palettized surface.");
		// if it has pallete have to use default SDL function (tried implement it but failed)
		//rgb = SDL_MapRGB(surface->format, r, g, b);

		setPixel(surface, x, y, rgb, debug);
	}

	void setPixel(SDL_Surface *surface, unsigned int x, unsigned int y, const SDL_Color &component, bool debug)
	{
		setPixel(surface, x, y, component.r, component.g, component.b, debug);
	}

	SDL_Surface* makeSurface(int w, int h, int depth)
	{
		SDL_Surface* surface;
		// RuntimeError handles SDL_Error() messages;
		if ((surface = SDL_CreateRGBSurface(0, w, h, depth, 0, 0, 0, 0)) == NULL)
			throw RuntimeError();
		return surface;
	}

}