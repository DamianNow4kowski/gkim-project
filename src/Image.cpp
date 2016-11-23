#include "Image.h"
#include "FileHandler.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

/**
 * PRIVATE
 */
void Image::free()
{
	SDL_FreeSurface(this->surface);
}

/**
 * PUBLIC
 */

 // construct
Image::Image()
{
	this->surface = nullptr;
	this->w = 0;
	this->h = 0;
}

Image::Image(SDL_Surface *surface) {
	try {
		this->init(surface);
	}
	catch (const RuntimeError &err) {
		std::cerr << "Error while loading to SDL_Surface*: " << err.what() << std::endl;
	}
}

Image::Image(const char *file)
{
	this->load(file);
}

// destruct
Image::~Image()
{
	this->free();
}

void Image::init(SDL_Surface *surface)
{
	if (surface == nullptr)
		throw RuntimeError();

	if (this->initialized())
		this->free();

	this->surface = surface;
	this->w = static_cast<unsigned int>(surface->w);
	this->h = static_cast<unsigned int>(surface->h);
}

/**
 * Main function for loading image files
 * @param cstring image filename
 * @param bool if true throws error and terminates program
 *             when extension of image file is wrong
 */
void Image::load(const char *file, bool requireVaildExt)
{
	SDL_Surface *temp = nullptr;
	try
	{
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
		temp = this->loadImpl(file);
		this->init(temp);
	}
	catch (const RuntimeError &err)
	{
		std::cerr << "Error while loading to SDL_Surface*: " << err.what() << std::endl;
	}
}

void Image::save(const char *file) {
	try {
		const char *ext = this->extension();
		if (!FileHandler::verifyExtension(file, ext)) {
			std::ostringstream fn;
			fn << file << '.' << ext;
			this->saveImpl(this->surface, fn.str().c_str());
		} else {
			this->saveImpl(this->surface, file);
		}
	} 
	catch (const RuntimeError &err)
	{
		std::cerr << "Error saving Image: " << err.what() << std::endl;
	}
}

/**
 * @param x x-axis cordinate of wanted pixel
 * @param y y-axis
 * @return full pixel data in Uint32 format
 */
Uint32 Image::getPixel(const int &x, const int &y) const
{
	if(x < 0 || x >= this->w || y < 0 || y >= this->h)
		throw RuntimeError("Error during getting pixel.");

	Uint8 bpp, *pixel;
	bpp = this->surface->format->BytesPerPixel;
	pixel = (Uint8 *)this->surface->pixels + y * this->surface->pitch + x * bpp;

	switch (bpp)
	{
	case 1:
		return *pixel;

	case 2:
		return *(Uint16 *)pixel;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return pixel[0] << 16 | pixel[1] << 8 | pixel[2];
		else
			return pixel[0] | pixel[1] << 8 | pixel[2] << 16;

	case 4:
		return *(Uint32 *)pixel;

	// Should not happen, but avoids warnings
	default:
		return 0;
	}
}

SDL_Color Image::getPixelColorRGB(const int &x, const int &y) const
{
	Uint32 color = this->getPixel(x,y);
	SDL_Color rgb;
	SDL_GetRGB(color, this->surface->format, &rgb.r, &rgb.g, &rgb.b);
	return rgb;
}

unsigned int Image::height() const
{
	return this->h;
}

unsigned int Image::width() const
{
	return this->w;
}

unsigned int Image::size() const
{
	return (this->w * this->h);
}

bool Image::initialized()
{
	return this->surface != nullptr;
}

SDL_Texture *Image::texturize(SDL_Renderer* renderer) {
	return SDL_CreateTextureFromSurface(renderer, this->surface);
}

void Image::preview()
{
	if (!this->initialized())
		throw RuntimeError("Cannot preview not initilized Image");

	SDL_Window *window = SDL_CreateWindow("Preview Image", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->w, this->h, SDL_WINDOW_SHOWN);
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

SDL_Surface *Image::img() {
	if (!this->initialized())
		throw RuntimeError("Trying to access uninitialized surface.");

	return this->surface;
}


void Image::setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
	if(x < 0 || x >= this->w || y < 0 || y >= this->h)
		throw RuntimeError("Error during getting pixel.");

	Uint32 pixel;
	Uint8 bpp, *p;

	// Zamieniamy poszczególne skladowe koloru na format koloru pixela
	pixel = SDL_MapRGB(this->surface->format, R, G, B);

	// Pobieramy informacji ile bajtów zajmuje jeden pixel
	bpp = this->surface->format->BytesPerPixel;

	/// Obliczamy adres pixela
	p = (Uint8 *)this->surface->pixels + y * this->surface->pitch + x * bpp;

	// Ustawiamy wartosc pixela, w zale¿znosci od formatu powierzchni
	switch (bpp)
	{
	// 8-bit
	case 1:
		*p = pixel;
		break;
	// 16-bit
	case 2:
		*(Uint16 *)p = pixel;
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
		break;
	// 32-bit
	case 4:
		*(Uint32 *)p = pixel;
		break;

	}
}

void Image::makeSurface(int w, int h)
{
	this->surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
	this->w = w;
	this->h = h;
}

void Image::convertToGreyScale()
{
	unsigned int x, y;
	SDL_Color color;
	Uint8 bw;
	for (x = 0; x < this->w; ++x)
	{
		for (y = 0; y < this->h; ++y)
		{
			color = this->getPixelColorRGB(x, y);
			
			/**
			 * Greyscale standard?
			 * @link https://en.wikipedia.org/wiki/Grayscale#Luma_coding_in_video_systems
			 */
			// bw = color.r*0.299 + color.g*0.587 + color.b*0.114;
			bw = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
			this->setPixel(x, y, bw, bw, bw);
		}
	}
}