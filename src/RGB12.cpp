#include "RGB12.h"

#include <iostream>
#include <utility>
#include <algorithm>
#include <sstream>

Image RGB12::convert(const Image& img) const
{
	// More usefull when don't throw RuntimError
	if (img.empty())
	{
		std::cerr << "[RGB12]: RuntimeError(\"Cannot convert not initialized Image.\");" << std::endl;
		return Image();
	}

	// Simply copy surface if it is already in proper format
	if (img.depth() == 12)
		return Image(img);

	// Real conversion method is here
	std::cout << "[RGB12]-> Converting Image to RGB444 format." << std::endl;
	Image converted(img.width(), img.height(), 12);
	SDL_Color color;

	for (unsigned int y = 0; y < converted.height(); ++y)
	{
		for (unsigned int x = 0; x < converted.width(); ++x)
		{
			color = img.getPixelColor(x, y);

			// Convert each color to have only 4 siginificant bits
			color.r = (color.r >> 4) << 4;
			color.g = (color.g >> 4) << 4;
			color.b = (color.b >> 4) << 4;

			converted.setPixel(x, y, color);
		}
	}

	return std::move(converted);
}

void RGB12::load444(std::ifstream &f, Image &img)
{
	std::cout << "[RGB12]-> Run 444 load algorithm." << std::endl;
	int colorToCode = 0;
	int pixelUse = 0;
	char usedChar = 0;
	int x, y;
	SDL_Color pixel;

	x = y = 0;
	while (f.get(usedChar))
	{
		for (int k = 0; k < 2; ++k)
		{
			if (k == 0)
			{
				//setColorOfPixel(pixel, colorToCode, usedChar); // TODO: try and compare investigate performance
				setColorOfPixel(pixel, colorToCode, ((usedChar >> 4) << 4));
			}
			else
				setColorOfPixel(pixel, colorToCode, (usedChar << 4));

			colorToCode++;
			if (colorToCode == 3)
			{
				if (x == img.width())
				{
					x = 0;
					++y;
				}

				// ----------------------> turns on debug on
				img.setPixel(x, y, pixel, (y == img.height() / 2));
				colorToCode = 0;
				++x;
			}
		}
	}
}

void RGB12::save444(std::ofstream &f, const Image &img) const
{
	std::cout << "[RGB12]-> Run 444 save algorithm." << std::endl;
	bool isSpace, firstHalf;
	char usedChar;
	SDL_Color color;

	isSpace = true;
	firstHalf = true;
	usedChar = 0;

	for (unsigned int y = 0; y < img.height(); ++y)
	{
		for (unsigned int x = 0; x < img.width(); ++x)
		{
			for (int k = 0; k < 3; ++k)
			{
				// ----------------------------> turns on debug on
				color = img.getPixelColor(x, y, (y == img.height() / 2));

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

uint8_t RGB12::getColorOfPixel(SDL_Color color, int which) const
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

void RGB12::setColorOfPixel(SDL_Color &color, int which, uint8_t value) const
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

void RGB12::store(const std::string & filename, const Image & img) const
{
	std::cout << "[RGB12]-> STARTED: Storing image process." << std::endl;
	std::ofstream f;

	// Load file to save data in binary mode
	openStream(filename, f);

	// Choose algorithm
	uint8_t alg = algorithm;
	switch (algorithm)
	{
	default:
		std::cerr << "[RGB12]: RuntimeError(\"There is no such an algorithm: " << static_cast<unsigned int>(algorithm) << ". Calling default..\");" << std::endl;
		alg = 0;
	case 0:
		writeHeader(f, img, alg, true);
		save444(f, img);
		break;
	}
	f.close();
	std::cout << "[RGB12]<- COMPLETED: Storing image process." << std::endl;
}

Image RGB12::recover(const std::string & filename)
{
	std::cout << "[RGB12]-> STARTED: Recovering image process." << std::endl;
	std::ifstream f;

	// Open input file in binary mode
	openStream(filename, f);

	// Read header data
	unsigned int width, height;
	uint8_t depth, bpp, alg;
	std::tie(width, height, depth, bpp, alg) = readHeader(f, true);

	// Create new Image
	Image recovered(width, height, depth);

	// Load depending on the alogrithm
	switch (alg)
	{
	case 0:
		load444(f, recovered);
		break;
	default:
		std::ostringstream os;
		os << "Saved with uknown algorithm: " << static_cast<unsigned int>(alg);
		throw RuntimeError(os.str());
	}
	f.close();

	// Return recovered Image
	std::cout << "[RGB12]<- COMPLETED: Recovering image process." << std::endl;
	return std::move(recovered);
}

/**
 * @return {
 *		unsigned int width,
 *		unsigned int height,
 *		uint8_t depth (bits per pixel),
 *		uint8_t bpp (bytes per pixel),
 *		uint8_t algorithm }
*/
std::tuple<int, int, uint8_t, uint8_t, uint8_t> RGB12::readHeader(std::ifstream &f, bool debug) const
{
	std::cout << "[RGB12]-> Reading header." << std::endl;

	// Read header verifier
	size_t str_size;
	char *cstr;
	f.read(reinterpret_cast<char *>(&str_size), sizeof(str_size));
	if (str_size >= 1000)
		throw RuntimeError("Header is possibly invaild.");

	cstr = new char[str_size + 1];
	f.read(cstr, str_size);
	cstr[str_size] = '\0';
	std::string ext(cstr);
	delete[] cstr;

	// Verify header
	if (debug)
	{
		std::cout << "[RGB12]-> Header Details:" << std::endl;
		std::cout << "- string(" << str_size << "): " << ext << std::endl;
	}

	if(!std::equal(ext.begin(), ext.end(), extension().begin()))
		throw RuntimeError("Header of processed file is not vaild.");

	// Load actually required things
	unsigned int width, height;
	uint8_t depth, bpp, alg;
	f.read(reinterpret_cast<char *>(&width), sizeof(width));
	f.read(reinterpret_cast<char *>(&height), sizeof(height));
	f.read(reinterpret_cast<char *>(&depth), sizeof(depth));
	f.read(reinterpret_cast<char *>(&bpp), sizeof(bpp));
	f.read(reinterpret_cast<char *>(&alg), sizeof(alg));

	// Debug
	if (debug)
	{
		std::cout << "- Algorithm: " << static_cast<unsigned int>(alg) << std::endl;
		std::cout << "- Width: " << width << std::endl;
		std::cout << "- Height: " << height << std::endl;
		std::cout << "- Depth: " << static_cast<unsigned int>(depth) << std::endl;
		std::cout << "- BPP: " << static_cast<unsigned int>(bpp) << std::endl;
	}

	return std::make_tuple(width, height, depth, bpp, alg);
}

void RGB12::writeHeader(std::ofstream &f, const Image &img, uint8_t alg, bool debug) const
{
	std::cout << "[RGB12]-> Saving header." << std::endl;
	if (debug)
	{
		img.printDetails(std::cout);
		std::cout << " - Algorithm: " << static_cast<unsigned int>(algorithm) << std::endl;
	}

	// Saving Image informations
	std::string ext = extension();
	size_t ext_size = ext.size();
	const unsigned int 
		width = img.width(), 
		height = img.height();
	const uint8_t
		depth = static_cast<uint8_t>(img.depth()),
		bpp = static_cast<uint8_t>(img.bpp());

	f.write(reinterpret_cast<const char *>(&ext_size), sizeof(ext_size));
	f.write(ext.c_str(), ext_size);
	f.write(reinterpret_cast<const char *>(&width), sizeof(width));
	f.write(reinterpret_cast<const char *>(&height), sizeof(height));
	f.write(reinterpret_cast<const char *>(&depth), sizeof(depth));
	f.write(reinterpret_cast<const char *>(&bpp), sizeof(bpp));
	f.write(reinterpret_cast<const char *>(&alg), sizeof(alg));
}

std::string RGB12::extension() const
{
	return std::string(".rgb12");
}

RGB12::RGB12(uint8_t alg)
	: algorithm(alg)
{
	std::cout << "[RGB12]: Called default constructor." << std::endl;
}

RGB12::RGB12(const BMP &bmp, uint8_t alg)
	: ImageHandler(std::move(convert(bmp.image))), algorithm(alg)
{
	std::cout << "[RGB12]: Called copy BMP constructor." << std::endl;
}

RGB12::RGB12(const RGB12 &rgb)
	: ImageHandler(rgb), algorithm(rgb.algorithm)
{
	std::cout << "[RGB12]: Called copy constructor." << std::endl;
}

RGB12::RGB12(RGB12 &&rgb)
	:ImageHandler(std::move(rgb)), algorithm(rgb.algorithm)
{
	std::cout << "[RGB12]: Called move constructor." << std::endl;
}

RGB12 & RGB12::operator=(const RGB12 &rgb)
{
	std::cout << "[RGB12]-> Called copy assigment operator." << std::endl;
	ImageHandler::operator=(rgb);
	algorithm = rgb.algorithm;
	return *this;
}

RGB12 & RGB12::operator=(RGB12 &&rgb)
{
	std::cout << "[RGB12]-> Called move assigment operator." << std::endl;
	ImageHandler::operator=(std::move(rgb));
	algorithm = rgb.algorithm;
	return *this;
}


RGB12::~RGB12()
{
	std::cout << "[RGB12]: Called virtual destructor." << std::endl;
}
