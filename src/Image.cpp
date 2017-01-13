#include "Image.h"
#include "CText.h"

#ifdef _DEBUG
#include <bitset>
#include <iomanip>
#endif

#include <sstream>  // thrown errors' messages

void Image::swap(Image &img)
{
#ifdef _DEBUG
	std::cout << " -> [Image::swap]" << std::endl;
#endif // _DEBUG
	
	std::swap(surface, img.surface);
}

SDL_Surface * Image::create(unsigned int width, unsigned int height, unsigned int depth) const
{
#ifdef _DEBUG
	std::cout << " -> [Image::create]: Creating new SDL_Surface." << std::endl;
#endif
	SDL_Surface *img = SDL_CreateRGBSurface(0, static_cast<int>(width), static_cast<int>(height), static_cast<int>(depth), 0, 0, 0, 0);
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
		std::cerr  << "!!! [Image::copy]: " << CText("Copying not existing surface.") << std::endl;
		return nullptr;
	}

	SDL_Surface* new_img;
	int w = img->w, h = img->h;

	// Allocate empty surface
	new_img = create(w, h, img->format->BitsPerPixel);
	
	// Fast copy raw pixel data
	SDL_memcpy(new_img->pixels, img->pixels, (img->h * img->pitch));

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
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		return pixel[0] << 16 | pixel[1] << 8 | pixel[2];
#else
		return pixel[0] | pixel[1] << 8 | pixel[2] << 16;
#endif

	// 32bit integer
	case 4:
		return *reinterpret_cast<uint32_t *>(pixel);

	// Should not happen, but..
	default:
		std::ostringstream os;
		os << "Not supported BytesPerPixel value: " << bpp;
		std::cerr  << "!!! [Image::getPixel]: " << CText(os.str()) << std::endl;
		return 0;
	}
}

uint32_t Image::getPixel(const SDL_Surface *img, unsigned int x, unsigned int y) const
{
#ifdef _DEBUG
	if (img == nullptr)
	{
		std::cerr  << "!!! [Image::getPixel]: " << CText("Cannot get pixel data of uninitialized surface.")  << std::endl;
		return 0;
	}

	// Verify coordinates
	if (x >= static_cast<unsigned int>(img->w) || y >= static_cast<unsigned int>(img->h))
	{
		std::ostringstream os;
		os << "Cannot get pixel. Out of surface range [w=" << img->w << ", h=" << img->h << "] was x=" << x << " y=" << y << '.';
		std::cerr << "!!! [Image::getPixel]: " << CText(os.str()) << std::endl;
		return 0;
	}
#else 
	if (img == nullptr || x >= static_cast<unsigned int>(img->w) || y >= static_cast<unsigned int>(img->h))
		return 0;
#endif

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
	// like: SDL_GetRGB(pixel, img->format, &rgb.r, &rgb.g, &rgb.b);
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
#ifdef _DEBUG
	else std::cerr  << "!!! [Image::getPixelColor]:" << CText("Not found pixel colors in the Image pallette.")  << std::endl;
#endif

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
#ifdef _DEBUG
	if (img == nullptr)
	{
		std::cerr  << "!!! [Image::setPixel]: " << CText("Cannot set pixel data of uninitialized surface.") << std::endl;
		return;
	}

	// Verify coordinates
	if (x >= static_cast<unsigned int>(img->w) || y >= static_cast<unsigned int>(img->h))
	{
		std::ostringstream os;
		os << "Cannot set pixel. Out of surface range [w=" << img->w << ", h=" << img->h << "] was x=" << x << " y=" << y << '.';
		std::cerr  << "!!! [Image::setPixel]: " << CText(os.str()) << std::endl;
		return;
	}
#else 
	if (img == nullptr || x >= static_cast<unsigned int>(img->w) || y >= static_cast<unsigned int>(img->h))
		return;
#endif

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
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		p[0] = (pixel >> 16) & 0xff;
		p[1] = (pixel >> 8) & 0xff;
		p[2] = pixel & 0xff;
#else
		p[0] = pixel & 0xff;
		p[1] = (pixel >> 8) & 0xff;
		p[2] = (pixel >> 16) & 0xff;
#endif
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

	default:
	{
		std::ostringstream os;
		os << "Not supported BytesPerPixel value: " << bpp;
		std::cerr  << "!!! [Image::setPixel]: " << CText(os.str()) << std::endl;
	}
		break;
	}
}

void Image::setPixel(SDL_Surface *img, unsigned int x, unsigned int y, uint8_t R, uint8_t G, uint8_t B) const
{
	if (img == nullptr)
	{
#ifdef _DEBUG
		std::cerr << "!!! [Image::setPixel]: " << CText("Cannot set pixel data of uninitialized surface.") << std::endl;
#endif
		return;
	}

	uint32_t rgb = (R >> img->format->Rloss) << img->format->Rshift
			| (G >> img->format->Gloss) << img->format->Gshift
			| (B >> img->format->Bloss) << img->format->Bshift
			| img->format->Amask;

	setPixel(img, x, y, rgb);
}

uint8_t Image::toGrayScale(const SDL_Color &color) const
{
	//return static_cast<uint8_t>(color.r * 0.299 + color.g * 0.587 + color.b * 0.114); // different grayscale alg.
	return static_cast<uint8_t>(0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b);
}

Image::Image()
	: surface(nullptr)
{
#ifdef _DEBUG
	std::cout << "[Image]: Called default constructor." << std::endl;
#endif // DEBUG
}

Image::Image(unsigned int width, unsigned int height, unsigned int depth)
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
	std::cout << "[Image]: Called SDL_Surface* move [" << CText("hardcoded swap", CText::Color::YELLOW) << "] constructor." << std::endl;
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
	std::cout << " -> [" << CText("~Image", CText::Color::MAGENTA) << "]" << ((surface != nullptr) ? ": Deallocated SDL_Surface." : "")  << std::endl;
#endif // _DEBUG

	// Remarks: it is safe to pass NULL to SDL_FreeSurface function
	// @see https://wiki.libsdl.org/SDL_FreeSurface#Remarks
	SDL_FreeSurface(surface);
	surface = nullptr;
}

/// Get/set pixel functions which probably going to be inlined

uint32_t Image::getPixel(unsigned int x, unsigned int y) const
{
	return getPixel(surface, x, y);
}

SDL_Color Image::getPixelColor(unsigned int x, unsigned int y) const
{
	return getPixelColor(surface, x, y);
}

uint8_t Image::getGrayColor(unsigned int x, unsigned int y) const
{
	return toGrayScale(getPixelColor(surface, x, y));
}

void Image::setPixel(unsigned int x, unsigned int y, uint8_t gray)
{
	setPixel(surface, x, y, gray, gray, gray);
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
#ifndef _DEBUG
	return SDL_CreateTextureFromSurface(renderer, surface);
#else
	std::cout << " -> [Image::texture]: Creating texture." << std::endl;
	SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, surface);
	if (text == nullptr)
		std::cerr << "!!! [Image::texture]: " << CText("Failed to create texture.") << std::endl;
	return text;
#endif
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
	return empty() ? 0 : (width() * height() * bpp());
}

bool Image::empty() const
{
	return surface == nullptr;
}

Image::pixel_iterator::pixel_iterator(SDL_Surface * surface)
	: s(surface), x(0), y(0),
	current(reinterpret_cast<uint8_t *>(s->pixels))
{}

Image::pixel_iterator::pixel_iterator(SDL_Surface * surface, size_t x, size_t y)
	: s(surface), x(x), y(y),
	current(reinterpret_cast<uint8_t *>(s->pixels) + y * s->pitch + x * s->format->BytesPerPixel)
{}

Image::pixel_iterator & Image::pixel_iterator::operator++()
{
	++x;
	if (x != static_cast<size_t>(s->w))
	{
		current += s->format->BytesPerPixel;
	}
	else
	{
		x = 0;
		++y;
		current = reinterpret_cast<uint8_t *>(s->pixels) + y * s->pitch;
	}

	return *this;
}

Image::pixel_iterator Image::pixel_iterator::operator++(int)
{
	pixel_iterator ret = *this;

	++x;
	if (x != static_cast<size_t>(s->w))
	{
		current += s->format->BytesPerPixel;
	}
	else
	{
		x = 0;
		++y;
		current = reinterpret_cast<uint8_t *>(s->pixels) + y * s->pitch;
	}

	return std::move(ret);
}

inline std::pair<size_t, size_t> Image::pixel_iterator::xy() const
{
	return std::make_pair(x, y);
}

std::array<uint8_t, 3> Image::pixel_iterator::rgb() const
{
	std::array<uint8_t, 3> rgb = {0, 0, 0};
	uint32_t val = value();

	if (s->format->palette == nullptr)
	{
		rgb[0] = ((val & s->format->Rmask) >> s->format->Rshift) << s->format->Rloss;
		rgb[1] = ((val & s->format->Gmask) >> s->format->Gshift) << s->format->Gloss;
		rgb[2] = ((val & s->format->Bmask) >> s->format->Bshift) << s->format->Bloss;
	}
	else if (val < static_cast<uint32_t>(s->format->palette->ncolors))
	{
		rgb[0] = s->format->palette->colors[val].r;
		rgb[1] = s->format->palette->colors[val].g;
		rgb[2] = s->format->palette->colors[val].b;
	}

	return rgb;
}

uint32_t Image::pixel_iterator::value() const
{

#ifdef _DEBUG
	if (x >= static_cast<size_t>(s->w) || y >= static_cast<size_t>(s->h))
	{
		std::cerr << "!!! [Image::pixel_iterator::value]: " << CText("Wanted pixel out of Image range.") << std::endl;
		return 0;
	}
#endif

	switch (s->format->BytesPerPixel)
	{
	case 1:
		return *current;
	case 2:
		return *reinterpret_cast<uint16_t *>(current);
	case 3:
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		return current[0] << 16 | current[1] << 8 | current[2];
#else
		return current[0] | current[1] << 8 | current[2] << 16;
#endif
	case 4:
		return *reinterpret_cast<uint32_t *>(current);
	default:
		return 0;
	}
}

uint8_t Image::pixel_iterator::gray() const
{
	SDL_Color col = color();
	return static_cast<uint8_t>(0.2126 * col.r + 0.7152 * col.g + 0.0722 * col.b);
}

SDL_Color Image::pixel_iterator::color() const
{
	uint32_t RGB = value();

	SDL_Color COLOR = { 0, 0, 0, 0 };
	if (s->format->palette == nullptr)
	{
		COLOR.r = ((RGB & s->format->Rmask) >> s->format->Rshift) << s->format->Rloss;
		COLOR.g = ((RGB & s->format->Gmask) >> s->format->Gshift) << s->format->Gloss;
		COLOR.b = ((RGB & s->format->Bmask) >> s->format->Bshift) << s->format->Bloss;
	}
	else if (RGB < static_cast<uint32_t>(s->format->palette->ncolors))
		return s->format->palette->colors[RGB];

	return COLOR;
}

void Image::pixel_iterator::value(uint32_t RGB)
{

#ifdef _DEBUG
	if (x >= static_cast<size_t>(s->w) || y >= static_cast<size_t>(s->h))
	{
		std::cerr << "!!! [Image::pixel_iterator::value]: " << CText("Setting pixel out of Image range.") << std::endl;
		return;
	}
#endif

	switch (s->format->BytesPerPixel)
	{
	case 1:
		*current = RGB;
		return;

	case 2:
		*reinterpret_cast<uint16_t*>(current) = RGB;
		return;

	case 3:
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		current[0] = (RGB >> 16) & 0xff;
		current[1] = (RGB >> 8) & 0xff;
		current[2] = RGB & 0xff;
#else
		current[0] = RGB & 0xff;
		current[1] = (RGB >> 8) & 0xff;
		current[2] = (RGB >> 16) & 0xff;
#endif
		return;

	case 4:
		*reinterpret_cast<uint32_t*>(current) = RGB;
		return;
	}
}

void Image::pixel_iterator::value(uint8_t R, uint8_t G, uint8_t B)
{
	// Convert to full width uint32_t pixel
	uint32_t RGB = (R >> s->format->Rloss) << s->format->Rshift
		| (G >> s->format->Gloss) << s->format->Gshift
		| (B >> s->format->Bloss) << s->format->Bshift
		| s->format->Amask;
	
	value(RGB);
}

bool Image::pixel_iterator::operator==(const pixel_iterator & it) const
{
	return s == it.s && x == it.x && y == it.y;
}

bool Image::pixel_iterator::operator!=(const pixel_iterator & it) const
{
	return x != it.x || y != it.y || s != it.s;
}

bool Image::pixel_iterator::operator<(const pixel_iterator & it) const
{
	return s == it.s && ((y < it.y) || (y == it.y && x < it.x));
}

Image::pixel_iterator Image::begin() const
{
	return pixel_iterator(surface);
}

Image::pixel_iterator Image::end() const
{
	return pixel_iterator(surface, 0, surface->h);
}
