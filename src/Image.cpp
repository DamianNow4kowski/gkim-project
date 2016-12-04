#include "Image.h"
#include "FileHandler.h"
#include <iostream>
#include <sstream>
#include <string>
#include <bitset>
#include <fstream>
#include <iomanip>

using namespace std;

/**
 * PRIVATE
 */

/**
 * Deallocate object's dynamic memory
 */
void Image::free()
{
	// Note: it is safe to pass NULL to this function
	// @see https://wiki.libsdl.org/SDL_FreeSurface#Remarks
	SDL_FreeSurface(this->surface);
	this->surface = NULL;
}

bool Image::empty(const SDL_Surface *surf) const
{
	return surf == NULL;
}

/**
 * PUBLIC
 */

 // construct
Image::Image()
	: surface(NULL)
{}

Image::Image(const Image &img)
	: surface(copySurface(img.surface))
{
	std::cout << "[Image]: Public const Image& constructor. (COPIED SURFACE)" << std::endl;
	// copySurface() throws RuntimeError if surf == NULL
}

// Directly attach SDL_Surface (like for planned move constructor or RGB444 convert)
Image::Image(SDL_Surface *surf)
	: surface(surf)
{
	std::cout << "[Image]: Protected [not const] SDL_Surface* constructor. (DIDN'T COPY SURFACE)" << std::endl;
	// it is safe to pass NULL as *surf
}

Image::Image(const SDL_Surface *surf) 
	: surface(copySurface(surf))
{
	std::cout << "[Image]: Public const SDL_Surface* constructor. (COPIED SURFACE)" << std::endl;
	// copySurface() throws RuntimeError if surf == NULL
}

// destruct
Image::~Image()
{
	this->free();
}

/**
 * @important Surround with try/catch block!
 */
void Image::init(SDL_Surface *surface)
{
	if (this->empty(surface))
	{
		std::cerr << "Cannot initialize empty surface." << std::endl;
		// throw RuntimeError("Cannot initialize empty surface.");
		return;
	}

	// Deallocate memory before initializing again
	if (this->initialized())
		this->free();

	this->surface = surface;
}

/**
 * Main function for loading image files
 * @param cstring image filename
 * @param bool if true throws error and terminates program
 *             when extension of image file is wrong
 */
void Image::load(const char *file, bool requireVaildExt)
{
	try
	{
		// Verifying extension
		const char *ext = this->extension();
		if (!FileHandler::verifyExtension(file, ext))
		{
			if (requireVaildExt)
			{
				std::ostringstream error;
				error << "Extension of loaded file, must be *." << ext;
				throw RuntimeError(error.str());
			}
			else
				std::cerr << "Warning: Loaded extension is invaild." << std::endl;
		}

		// Load surface
		this->init(this->loadImpl(file));
	}
	catch (const RuntimeError &err)
	{
		std::cerr << "Error while loading to SDL_Surface*: " << err.what() << std::endl;
	}
}

void Image::save(const char *file) 
{
	if (!this->initialized())
	{
		std::cerr << "Cannot save uninitialized image." << std::endl;
		// throw RuntimeError("Cannot save uninitialized image.");
		return;
	}

	try {
		const char *ext = this->extension();
		if (!FileHandler::verifyExtension(file, ext)) {
			std::ostringstream fn;
			fn << file << '.' << ext;
			this->saveImpl(this->surface, fn.str().c_str());
		}
		else {
			this->saveImpl(this->surface, file);
		}
	}
	catch (const RuntimeError &err)
	{
		std::cerr << "Error saving Image: " << err.what() << std::endl;
	}
}


/**
 * @param pixel pointer to wanted pixel data
 * @param bpp size of pixel in number of bytes
 * @return fully qualifed uint32_t pixel data
 */
uint32_t Image::getPixel(uint8_t *pixel, uint8_t bpp) const {
	switch (bpp)
	{
	case 1:
		return *pixel;

	case 2:
		return *reinterpret_cast<uint16_t *>(pixel);

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return pixel[0] << 16 | pixel[1] << 8 | pixel[2];
		else
			return pixel[0] | pixel[1] << 8 | pixel[2] << 16;

	case 4:
		return *reinterpret_cast<uint32_t *>(pixel);

		// Should not happen, but avoids warnings
	default:
		std::cerr << "Couldn't get pixel BytesPerPixel set to 0.";
		return 0;
	}
}

/**
 * @param x x-axis cordinate of wanted pixel
 * @param y y-axis
 * @return full pixel data in Uint32 format
 */
uint32_t Image::getPixel(const SDL_Surface *surf, unsigned int x, unsigned int y, bool debug) const
{
	uint8_t *pixel, bpp;
	bpp = surf->format->BytesPerPixel;
	pixel = reinterpret_cast<uint8_t *>(surf->pixels) + y * surf->pitch + x * bpp;

	if (!debug)
		return this->getPixel(pixel, bpp);

	// Debugging
	uint32_t ret = this->getPixel(pixel, bpp);
	std::bitset<32> b(ret);
	std::cout << "Pixel[x=" << x << "][y=" << y << "] = " << b << std::endl;
	return ret;
}

uint32_t Image::getPixel(unsigned int x, unsigned int y, bool debug) const
{
	return this->getPixel(this->surface, x, y, debug);
}

SDL_Color Image::getPixelColor(const SDL_Surface *surf, unsigned int x, unsigned int y) const
{
	uint32_t color = this->getPixel(surf, x, y);
	SDL_Color rgb;
	SDL_GetRGB(color, surf->format, &rgb.r, &rgb.g, &rgb.b);
	return rgb;
}

SDL_Color Image::getPixelColor(unsigned int x, unsigned int y) const
{
	return this->getPixelColor(this->surface, x, y);
}

unsigned int Image::height() const
{
	if (!this->initialized())
		return 0;
	return static_cast<unsigned int>(this->surface->h);
}

unsigned int Image::width() const
{
	if(!this->initialized())
		return 0;
	return static_cast<unsigned int>(this->surface->w);
}

unsigned int Image::bpp() const 
{
	if (!this->initialized())
		return 0;
	return static_cast<unsigned int>(this->surface->format->BytesPerPixel);
}

unsigned int Image::depth() const 
{
	if (!this->initialized())
		return 0;
	return static_cast<unsigned int>(this->surface->format->BitsPerPixel);
}

size_t Image::size() const
{
	return this->width() * this->height() * this->bpp();
}

bool Image::initialized() const 
{
	return !this->empty(this->surface);
}

SDL_Texture *Image::texturize(SDL_Renderer* renderer) {
	return SDL_CreateTextureFromSurface(renderer, this->surface);
}

void Image::preview()
{ 

	std::cout << "Preview Image's Details:" << std::endl;
	std::cout << "- Width: " << this->width() << std::endl;
	std::cout << "- Height: " << this->height() << std::endl;
	std::cout << "- Depth: " << this->depth() << std::endl;
	std::cout << "- Size (bytes): " << this->size() << std::endl;

	if (!this->initialized())
	{
		std::cerr << "Cannot preview uninitialized image." << std::endl;
		// throw RuntimeError("Cannot preview uninitialized image.");
		return;
	}

	SDL_Window *window = SDL_CreateWindow("Preview Image", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->surface->w, this->surface->h, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Texture* texture = this->texturize(renderer);
	SDL_Event e;
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);

	// Simple loop
	std::cout << "Press key 'Q' or 'ESC' to exit view." << std::endl;
	while (SDL_WaitEvent(&e))
	{
		if (e.type == SDL_KEYDOWN) {
			std::cout << "Pressed key '" << SDL_GetKeyName(e.key.keysym.sym) << "'" << std::endl;
			if (e.key.keysym.sym == SDLK_ESCAPE || e.key.keysym.sym == SDLK_q) {
				std::cout << "Exiting view.." << std::endl;
				break;
			}
		}
		else if (e.type == SDL_QUIT) {
			std::cout << "User requested to close window." << std::endl;
			break;
		}
	}

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

const SDL_Surface *Image::img() const {
	if (!this->initialized())
		throw RuntimeError("Trying to access uninitialized surface.");

	return this->surface;
}


void Image::setPixel(SDL_Surface *surface, unsigned int x, unsigned int y, uint32_t pixel, bool debug)
{
	if (x >= static_cast<unsigned int>(surface->w) || y >= static_cast<unsigned int>(surface->h))
		throw RuntimeError("Specified coordinates are above the surface range.");

	uint8_t *p, bpp;
	bpp = surface->format->BytesPerPixel;
	p = reinterpret_cast<uint8_t*>(surface->pixels) + y * surface->pitch + x * bpp;

	switch (bpp)
	{
		// 8-bit
	case 1:
		*p = pixel;
		if (debug) {
			std::bitset<32> b(pixel);
			std::bitset<8> b1(*p);
			std::cout << "Pixel[x=" << std::setw(3) << x << "][y=" << std::setw(3) << y << "] = " << b << std::endl;
			std::cout << setw(22) << "Bitmap = " << std::setw(32) << b1 << std::endl;
		}
		break;
		// 16-bit
	case 2:
		*reinterpret_cast<uint16_t*>(p) = pixel;
		if (debug) {
			std::bitset<32> b(pixel);
			std::bitset<16> b1(*reinterpret_cast<uint16_t*>(p));
			std::cout << "Pixel[x=" << std::setw(3) << x << "][y=" << std::setw(3) << y << "] = " << b << std::endl;
			std::cout << setw(22) << "Bitmap = " << std::setw(32) << b1 << std::endl;
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
		if (debug) {
			std::bitset<32> b(pixel);
			std::bitset<8> b1(p[0]);
			std::bitset<8> b2(p[1]);
			std::bitset<8> b3(p[2]);
			std::cout << "Pixel[x=" << std::setw(3) << x << "][y=" << std::setw(3) << y << "] = " << b << std::endl;
			std::cout << setw(22) << "Bitmap = " << std::setw(16) << b1 << b2 << b3 << std::endl;
		}
		break;
		// 32-bit
	case 4:
		*reinterpret_cast<uint32_t*>(p) = pixel;
		if (debug) {
			std::bitset<32> b(pixel);
			std::bitset<32> b1(*reinterpret_cast<uint32_t*>(p));
			std::cout << "Pixel[x=" << std::setw(3) << x << "][y=" << std::setw(3) << y << "] = " << b << std::endl;
			std::cout << setw(22) << "Bitmap = " << std::setw(32) << b1 << std::endl;
		}
		break;
	}
}

void Image::setPixel(SDL_Surface *surf, unsigned int x, unsigned int y, uint8_t R, uint8_t G, uint8_t B, bool debug)
{
	uint32_t pixel;
	pixel = SDL_MapRGB(surf->format, R, G, B);

	// Invoke main function
	this->setPixel(surf, x, y, pixel, debug);
}

void Image::setPixel(unsigned int x, unsigned int y, uint8_t r, uint8_t g, uint8_t b, bool debug)
{
	this->setPixel(this->surface, x, y, r, g, b, debug);
}

SDL_Surface* Image::makeSurface(int w, int h, int depth)
{
	SDL_Surface *surf = SDL_CreateRGBSurface(0, w, h, depth, 0, 0, 0, 0);
	if(empty(surf))
		throw RuntimeError();
	return surf;
}

/**
 * Creates copy of SDL_Surface
 * @param SDL_Surface* pointer to surface that is being copied
 * @return SDL_Surface* pointer to newly created surface
 */
SDL_Surface *Image::copySurface(const SDL_Surface *surf)
{
	std::cout << "[Debug]: *** Copying surface." << std::endl;
	if (this->empty(surf))
		throw RuntimeError("Cannot copy not existing surface.");

	uint32_t pix;
	int x, y, w, h;
	SDL_Surface* new_surface;
	w = surf->w;
	h = surf->h;

	new_surface = this->makeSurface(w, h, surf->format->BitsPerPixel);

	// Simple copying algorithm
	for (y = 0; y < h; ++y) {
		for (x = 0; x < w; ++x) {
			pix = this->getPixel(surf, x, y);
			this->setPixel(new_surface, x, y, pix);
		}
	}

	return new_surface;
}

void Image::convertToGreyScale()
{
	unsigned int x, y, w, h;
	SDL_Color color;
	uint8_t bw;
	w = this->width();
	h = this->height();
	for (x = 0; x < w; ++x)
	{
		for (y = 0; y < h; ++y)
		{
			color = this->getPixelColor(x, y);

			/**
			 * Greyscale standard?
			 * @link https://en.wikipedia.org/wiki/Grayscale#Luma_coding_in_video_systems
			 */
			 // bw = color.r*0.299 + color.g*0.587 + color.b*0.114;
			bw = static_cast<uint8_t>(0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b);
			this->setPixel(this->surface, x, y, bw, bw, bw);
		}
	}
}

/**
 * Copies this->surface in particular format
 * @param SDL_PixelFormat* format [=nullptr_t]
 *   @see https://wiki.libsdl.org/SDL_PixelFormat
 * @return SDL_Surface* pointer to newly created SDL_Surface()
 */
SDL_Surface *Image::getSurface(const SDL_PixelFormat* format)
{
	SDL_Surface *surf = nullptr;
	try {
		if (!this->initialized())
			throw RuntimeError("Cannot get uninitialized surface.");

		if (format == nullptr)
			format = this->surface->format;

		if ((surf = SDL_ConvertSurface(this->surface, format, 0)) == NULL)
			throw RuntimeError();

	}
	catch (const RuntimeError &e) {
		std::cerr << "Error while copying surface: " << e.what() << std::endl;
	}
	return surf;
}

/**
 * Copies this->surface in particular format
 * @param Uint32 sdl pixel format id
 * 	@see https://wiki.libsdl.org/SDL_ConvertSurfaceFormat#format
 * @return SDL_Surface* pointer to newly created SDL_Surface()
 */
SDL_Surface *Image::getSurface(uint32_t format_id)
{
	SDL_Surface *surf = nullptr;
	try {
		if (!this->initialized())
			throw RuntimeError("Cannot get uninitialized surface.");

		if ((surf = SDL_ConvertSurfaceFormat(this->surface, format_id, 0)) == NULL)
			throw RuntimeError();

	}
	catch (const RuntimeError &e) {
		std::cerr << "Error while copying surface: " << e.what() << std::endl;
	}
	return surf;
}