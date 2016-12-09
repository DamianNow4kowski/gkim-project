#include "ImageSurface.h"

#include <iostream> // debug
#include <bitset>	// debug
#include <iomanip>  // debug
#include <sstream>  // thrown errors' messages

void ImageSurface::free()
{
	if (!empty())
		SDL_FreeSurface(surface);
}

SDL_Surface * ImageSurface::create(int w, int h, int depth) const
{
	SDL_Surface *img = SDL_CreateRGBSurface(0, w, h, depth, 0, 0, 0, 0);
	if (img == nullptr)
		throw RuntimeError();
	return img;
}

SDL_Surface * ImageSurface::copy(const SDL_Surface *img) const
{
	std::cout << "[Debug]: *** Copying surface." << std::endl;
	if (img == nullptr)
		throw RuntimeError("Cannot copy not existing surface.");

	int x, y, w, h;
	SDL_Surface* new_img;
	w = img->w;
	h = img->h;

	// Allocate empty surface
	new_img = create(w, h, img->format->BitsPerPixel);

	// Simple copying algorithm
	for (y = 0; y < h; ++y)
		for (x = 0; x < w; ++x)
			setPixel(new_img, x, y, getPixel(img, x, y));

	return new_img;
}

uint32_t ImageSurface::getPixel(uint8_t *pixel, uint8_t bpp) const
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

uint32_t ImageSurface::getPixel(const SDL_Surface *img, unsigned int x, unsigned int y, bool debug) const
{
	if (img == nullptr)
		throw RuntimeError("Cannot get pixel data of uninitialized surface.");

	// Verify coordinates
	if (x >= static_cast<unsigned int>(img->w) || y >= static_cast<unsigned int>(img->h))
	{
		std::ostringstream os;
		os << "Cannot get pixel. Out of surface range [w=" << img->w << ", h=" << img->h << "] was x=" << x << " y=" << y << '.';
		throw RuntimeError(os.str());
	}

	uint8_t *pixel, bpp;
	bpp = img->format->BytesPerPixel;
	pixel = reinterpret_cast<uint8_t *>(img->pixels) + y * img->pitch + x * bpp;
	uint32_t ret = getPixel(pixel, bpp);

	// Print pixel bits when debugging
	if (debug) {
		std::bitset<32> b(ret);
		std::cout << "Pixel[x=" << std::setw(3) << x << "][y=" << std::setw(3) << y << "] = " << b << std::endl;
	}
	return ret;
}

SDL_Color ImageSurface::getPixelColor(const SDL_Surface *img, unsigned int x, unsigned int y, bool debug) const
{
	// Get pixel data
	uint32_t pixel = getPixel(img, x, y);

	// Get rgb color components
	SDL_Color rgb = { 0, 0, 0, 0 };
	if (img->format->palette == nullptr)
	{
		/** 
		 * At first get interesting color component bits using mask 
		 * from 32-bit integer containing wanted pixel data
		 * then shift it down to 8-bit
		 * at the end expand to full 8-bit number
		 */
		rgb.r = ((pixel & img->format->Rmask) >> img->format->Rshift) << img->format->Rloss;
		rgb.g = ((pixel & img->format->Gmask) >> img->format->Gshift) << img->format->Gloss;
		rgb.b = ((pixel & img->format->Bmask) >> img->format->Bshift) << img->format->Bloss;
	}
	// Pallette is used in SDL when depth <= 8-bit
	else if (pixel < static_cast<uint32_t>(img->format->palette->ncolors))
	{
		rgb.r = img->format->palette->colors[pixel].r;
		rgb.g = img->format->palette->colors[pixel].g;
		rgb.b = img->format->palette->colors[pixel].b;
	}
	//SDL_GetRGB(pixel, img->format, &rgb.r, &rgb.g, &rgb.b);

	// Print RGB bits when debugging
	if (debug) {
		std::bitset<8> r(rgb.r), g(rgb.g), b(rgb.b);
		std::cout << "Pixel[x=" << std::setw(3) << x << "][y=" << std::setw(3) << y << "] = R[" << r << "] G[ " << g << "] B[" << b << "]" << std::endl;
	}
	return rgb;
}

void ImageSurface::setPixel(SDL_Surface *img, unsigned int x, unsigned int y, uint32_t pixel, bool debug) const
{
	if (img == nullptr)
		throw RuntimeError("Cannot set pixel data of uninitialized surface.");

	// Verify coordinates
	if (x >= static_cast<unsigned int>(img->w) || y >= static_cast<unsigned int>(img->h))
	{
		std::ostringstream os;
		os << "Cannot set pixel. Out of surface range [w=" << img->w << ", h=" << img->h << "] was x=" << x << " y=" << y << '.';
		throw RuntimeError(os.str());
	}

	uint8_t *p, bpp;
	bpp = img->format->BytesPerPixel;
	p = reinterpret_cast<uint8_t*>(img->pixels) + y * img->pitch + x * bpp;

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

void ImageSurface::setPixel(SDL_Surface *img, unsigned int x, unsigned int y, uint8_t R, uint8_t G, uint8_t B, bool debug) const
{
	if (img == nullptr)
		throw RuntimeError("Cannot set pixel data of uninitialized surface.");

	uint32_t rgb;
	if (img->format->palette == nullptr)
	{
		rgb = (R >> img->format->Rloss) << img->format->Rshift
			| (G >> img->format->Gloss) << img->format->Gshift
			| (B >> img->format->Bloss) << img->format->Bshift;
		///| img->format->Amask; // I think it is unneseccary
	}
	else throw RuntimeError("Cannot set RGB components values of palettized surface.");
	// if it has pallete have to use default SDL function (tried implement it but failed)
	//rgb = SDL_MapRGB(img->format, r, g, b);

	setPixel(img, x, y, rgb, debug);
}

void ImageSurface::setPixel(SDL_Surface *img, unsigned int x, unsigned int y, const SDL_Color &component, bool debug) const
{
	setPixel(img, x, y, component.r, component.g, component.b, debug);
}

ImageSurface::ImageSurface()
	: surface(nullptr)
{}

ImageSurface::ImageSurface(SDL_Surface *img)
	:surface(img)
{}

ImageSurface::ImageSurface(const SDL_Surface *img)
	: surface(copy(img))
{}

ImageSurface::ImageSurface(const ImageSurface &img)
	: surface(copy(img.surface))
{}

ImageSurface::ImageSurface(ImageSurface &&img)
	: surface(img.surface)
{
	img.surface = nullptr;
}

ImageSurface & ImageSurface::operator=(const ImageSurface &img)
{
	free();
	surface = copy(img.surface);
	return *this;
}

ImageSurface & ImageSurface::operator=(ImageSurface &&img)
{
	free();
	surface = img.surface;
	img.surface = nullptr;
	return *this;
}

ImageSurface::~ImageSurface()
{
	free();
}

const SDL_Surface * ImageSurface::img() const
{
	return surface;
}

unsigned int ImageSurface::width() const
{
	return static_cast<unsigned int>(surface->w);
}

unsigned int ImageSurface::height() const
{
	return static_cast<unsigned int>(surface->h);
}

unsigned int ImageSurface::bpp() const
{
	return static_cast<unsigned int>(surface->format->BytesPerPixel);
}

unsigned int ImageSurface::depth() const
{
	return static_cast<unsigned int>(surface->format->BitsPerPixel);
}

size_t ImageSurface::size() const
{
	return width() * height() * bpp();
}

bool ImageSurface::empty() const
{
	return surface == nullptr;
}
