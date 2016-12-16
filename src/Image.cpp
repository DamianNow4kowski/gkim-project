#include "Image.h"

#include <bitset>	// debug
#include <iomanip>  // debug
#include <sstream>  // thrown errors' messages

void Image::free()
{

#ifdef _DEBUG
	// For debug to see when really memory is deallocated
	if(surface != nullptr)
		std::cout << " -> [Image::free]: Deallocated SDL_Surface." << std::endl;
#endif // _DEBUG
	
	// Note: it is safe to pass NULL to this function
	// @see https://wiki.libsdl.org/SDL_FreeSurface#Remarks
	SDL_FreeSurface(surface);
	surface = nullptr;
}

void Image::swap(Image &img)
{
#ifdef _DEBUG
	std::cout << " -> [Image::swap]" << std::endl;
#endif // _DEBUG
	
	std::swap(surface, img.surface);
}

SDL_Surface * Image::create(int w, int h, int depth) const
{
#ifdef _DEBUG
	std::cout << " -> [Image::create]: Creating new SDL_Surface." << std::endl;
#endif
	SDL_Surface *img = SDL_CreateRGBSurface(0, w, h, depth, 0, 0, 0, 0);
	if (img == nullptr)
		throw RuntimeError();
	return img;
}

SDL_Surface * Image::copy(const SDL_Surface *img) const
{

#ifdef _DEBUG
	std::cout << " -> [Image::copy]: Copying SDL_Surface to new SDL_Surface." << std::endl;
#endif

	if (img == nullptr)
	{
		std::cerr << "!!! [Image::copy]: Copying not existing surface." << std::endl;
		return nullptr;
	}

	SDL_Surface* new_img;
	int w = img->w, h = img->h;

	// Allocate empty surface
	new_img = create(w, h, img->format->BitsPerPixel);

	// Simple copying algorithm
	for (int y = 0; y < h; ++y)
		for (int x = 0; x < w; ++x)
			setPixel(new_img, x, y, getPixel(img, x, y));

	return new_img;
}

uint32_t Image::getPixel(uint8_t *pixel, uint8_t bpp) const
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
	}
}

uint32_t Image::getPixel(const SDL_Surface *img, unsigned int x, unsigned int y) const
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
	

#ifdef _DEBUG
	// Print pixel bits when debugging
	uint32_t ret = getPixel(pixel, bpp);
	std::bitset<32> b(ret);

	if ((img->h / 2) == y)
		std::cout << "Pixel[x=" << std::setw(3) << x << "][y=" << std::setw(3) << y << "] = " << b << std::endl;
	return ret;
#else
	return getPixel(pixel, bpp);
#endif // DEBUG
}

SDL_Color Image::getPixelColor(const SDL_Surface *img, unsigned int x, unsigned int y) const
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

	// Pallette is used in SDL_Surface when depth <= 8-bit
	else if (pixel < static_cast<uint32_t>(img->format->palette->ncolors))
	{
		rgb.r = img->format->palette->colors[pixel].r;
		rgb.g = img->format->palette->colors[pixel].g;
		rgb.b = img->format->palette->colors[pixel].b;
	}
	//SDL_GetRGB(pixel, img->format, &rgb.r, &rgb.g, &rgb.b);

#ifdef _DEBUG
	// Print RGB bits when debugging
	std::bitset<8> r(rgb.r), g(rgb.g), b(rgb.b);

	if ((img->h / 2) == y)
		std::cout << "Pixel[x=" << std::setw(3) << x << "][y=" << std::setw(3) << y << "] = R[" << r << "] G[ " << g << "] B[" << b << "]" << std::endl;
#endif // DEBUG

	return rgb;
}

void Image::setPixel(SDL_Surface *img, unsigned int x, unsigned int y, uint32_t pixel) const
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

#ifdef _DEBUG
	// Show setted pixel bits when debugging
	std::bitset<32> b(pixel);

	if((img->h / 2) == y)
		std::cout << "Pixel[x=" << std::setw(3) << x << "][y=" << std::setw(3) << y << "] = " << b << std::endl;
#endif // DEBUG

	switch (bpp)
	{

	// 8-bit
	case 1:
	{
		*p = pixel;

#ifdef _DEBUG
		std::bitset<8> b0(*p);

		if ((img->h / 2) == y)
			std::cout << std::setw(22) << "Bitmap = " << std::setw(32) << b0 << std::endl;
#endif // DEBUG
	}
		break;

	// 16-bit
	case 2:
	{
		*reinterpret_cast<uint16_t*>(p) = pixel;

#ifdef _DEBUG
		std::bitset<16> b12(*reinterpret_cast<uint16_t*>(p));

		if ((img->h / 2) == y)
			std::cout << std::setw(22) << "Bitmap = " << std::setw(32) << b12 << std::endl;
#endif // DEBUG
	}
		break;

	// 24-bit
	case 3:
	{
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

#ifdef _DEBUG
		std::bitset<8> b1(p[0]), b2(p[1]), b3(p[2]);

		if ((img->h / 2) == y)
			std::cout << std::setw(22) << "Bitmap = " << std::setw(16) << b1 << b2 << b3 << std::endl;
#endif // DEBUG
	}
		break;

	// 32-bit
	case 4:
	{
		*reinterpret_cast<uint32_t*>(p) = pixel;

#ifdef _DEBUG
		std::bitset<32> b4(*reinterpret_cast<uint32_t*>(p));

		if ((img->h / 2) == y)
			std::cout << std::setw(22) << "Bitmap = " << std::setw(32) << b4 << std::endl;
#endif // DEBUG
	}
		break;
	}
}

void Image::setPixel(SDL_Surface *img, unsigned int x, unsigned int y, uint8_t R, uint8_t G, uint8_t B) const
{
	if (img == nullptr)
		throw RuntimeError("Cannot set pixel data of uninitialized surface.");

	uint32_t rgb = (R >> img->format->Rloss) << img->format->Rshift
			| (G >> img->format->Gloss) << img->format->Gshift
			| (B >> img->format->Bloss) << img->format->Bshift
			| img->format->Amask;

	setPixel(img, x, y, rgb);
}

uint8_t Image::toGreyScale(const SDL_Color &color)
{
	//return static_cast<uint8_t>(color.r * 0.299 + color.g * 0.587 + color.b * 0.114);
	return static_cast<uint8_t>(0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b);
}

Image::Image()
	: surface(nullptr)
{
#ifdef _DEBUG
	std::cout << "[Image]: Called default constructor." << std::endl;
#endif // DEBUG
}

Image::Image(int width, int height, int depth)
	:surface(create(width, height, depth))
{
#ifdef _DEBUG
	std::cout << "[Image]: Called create empty surface constructor." << std::endl;
#endif // DEBUG
}

Image::Image(SDL_Surface *moved_surface)
	: Image()
{
#ifdef _DEBUG
	std::cout << "[Image]: Called SDL_Surface* move constructor." << std::endl
		<< " -> [Image]: Hardcoded SDL_Surface* swapping." << std::endl;
#endif // DEBUG

	// Zero-out given pointer, and attach surface
	std::swap(surface, moved_surface);
}

Image::Image(const SDL_Surface *img)
	: surface(copy(img))
{
#ifdef _DEBUG
	std::cout << "[Image]: Called SDL_Surface* copy constructor." << std::endl;
#endif // DEBUG
}

Image::Image(const Image &img)
	: surface(copy(img.surface))
{
#ifdef _DEBUG
	std::cout << "[Image]: Called copy constructor." << std::endl;
#endif // _DEBUG
}

Image::Image(Image &&img)
	: Image()
{
#ifdef _DEBUG
	std::cout << "[Image]: Called better move constructor." << std::endl;
#endif // _DEBUG

	// Zero-out moved image, and attach its data
	swap(img);
}

Image & Image::operator=(Image img)
{
#ifdef _DEBUG
	std::cout << " -> [Image::operator=]: Called universal assigment operator." << std::endl;
#endif // _DEBUG
	
	swap(img);
	return *this;
}

Image::~Image()
{
#ifdef _DEBUG
	std::cout << "[Image]: Called destructor." << std::endl;
#endif // _DEBUG
	
	free();
}

void Image::allocate(int x, int y, int depth)
{
#ifdef _DEBUG
	std::cout << " -> [Image::allocate]: Allocating new SDL_Surface in Image." << std::endl;
#endif // _DEBUG
	
	free();
	surface = create(x, y, depth);
}

void Image::toGreyScale()
{
#ifdef _DEBUG
	std::cout << " -> [Image::toGreyScale]: Converting Image to grey scale." << std::endl;
#endif // _DEBUG
	
	uint8_t grey;
	for (unsigned int y = 0; y < height(); ++y)
	{
		for (unsigned int x = 0; x < width(); ++x)
		{
			grey = toGreyScale(getPixelColor(x, y));
			setPixel(x, y, grey, grey, grey);
		}
	}
}

uint32_t Image::getPixel(unsigned int x, unsigned int y) const
{
	return getPixel(surface, x, y);
}

SDL_Color Image::getPixelColor(unsigned int x, unsigned int y) const
{
	return getPixelColor(surface, x, y);
}

void Image::setPixel(unsigned int x, unsigned int y, uint32_t pixel)
{
	setPixel(surface, x, y, pixel);
}

void Image::setPixel(unsigned int x, unsigned int y, uint8_t r, uint8_t g, uint8_t b)
{
	setPixel(surface, x, y, r, g, b);
}

void Image::setPixel(unsigned int x, unsigned int y, const SDL_Color &color)
{
	setPixel(surface, x, y, color.r, color.g, color.b);
}

void Image::printDetails(std::ostream &out) const
{
#ifdef _DEBUG
	out << " -> [Image::printDetails]" << std::endl;
#endif
	out << " - Empty: " << (empty() ? "true" : "false") << std::endl
		<< " - Width: " << width() << std::endl
		<< " - Height: " << height() << std::endl
		<< " - Depth: " << depth() << std::endl
		<< " - BPP: " << bpp() << std::endl
		<< " - Size: " << size() << std::endl;
}

const SDL_Surface * Image::img() const
{
	return surface;
}

SDL_Texture * Image::texture(SDL_Renderer *renderer) const
{
#ifdef _DEBUG
	std::cout << " -> [Image::texture]: Creating texture." << std::endl;
#endif // _DEBUG
	
	SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, surface);
	if (text == nullptr)
		throw RuntimeError();
	return text;
}

unsigned int Image::width() const
{
	return empty() ? 0 : static_cast<unsigned int>(surface->w);
}

unsigned int Image::height() const
{
	return empty() ? 0 : static_cast<unsigned int>(surface->h);
}

unsigned int Image::bpp() const
{
	return empty() ? 0 : static_cast<unsigned int>(surface->format->BytesPerPixel);
}

unsigned int Image::depth() const
{
	return empty() ? 0 : static_cast<unsigned int>(surface->format->BitsPerPixel);
}

size_t Image::size() const
{
	return empty() ? 0 : width() * height() * bpp();
}

bool Image::empty() const
{
	return surface == nullptr;
}
