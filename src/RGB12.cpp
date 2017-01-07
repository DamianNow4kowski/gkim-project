#include "RGB12.h"
#include "LZ77.h"
#include "Huffman.h"
#include "CText.h"

#include <iostream>
#include <utility>
#include <algorithm>
#include <sstream>

Image RGB12::convert(const Image& img) const
{
	// More usefull when don't throw RuntimError
	if (img.empty())
	{
		std::cerr << "!!! [RGB12::convert]: " << CText("Cannot convert not initialized Image.") << std::endl;
		return Image();
	}

	// Simply copy surface if it is already in proper format
	if (img.depth() == 12u)
		return Image(img);

#ifdef _DEBUG
	std::cout << "-> [RGB12::convert]: Converting Image to RGB444 format." << std::endl;
#endif

	// Start conversion
	unsigned int w = img.width(),
		h = img.height();
	Image converted(w, h, 12u);
	SDL_Color color;

	for (unsigned int y = 0; y < h; ++y)
	{
		for (unsigned int x = 0; x < w; ++x)
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

RGB12 & RGB12::toGrayScale()
{
#ifdef _DEBUG
	std::cout << " -> [RGB12::toGrayScale]: Converting Image to grey scale." << std::endl;
#endif // _DEBUG

	unsigned int h = image.height(),
		w = image.width();

	for (unsigned int y = 0; y < h; ++y)
	{
		for (unsigned int x = 0; x < w; ++x)
		{
			image.setPixel(x, y, static_cast<uint8_t>((image.getGrayColor(x, y) >> 4) << 4));
		}
	}

	if (algorithm == Algorithm::BitDensity)
		algorithm = Algorithm::GreyScale;

	return *this;
}

void RGB12::load444(std::ifstream &f, Image &img)
{
#ifdef _DEBUG
	std::cout << "-> [RGB12::load444]: Run BitDensity load algorithm." << std::endl;
#endif

	int colorToCode = 0;
	char usedChar = 0;
	unsigned int x, y, width = img.width();
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
				if (x == width)
				{
					x = 0;
					++y;
				}

				img.setPixel(x, y, pixel);
				colorToCode = 0;
				++x;
			}
		}
	}
}

void RGB12::saveGray(std::ofstream & output, const Image & img) const
{

	unsigned int
		width = img.width(),
		height = img.height();

	uint8_t block;

	bool save = false;
	for (unsigned int y = 0; y < height; ++y)
	{
		for (unsigned int x = 0; x < width; ++x)
		{
			if (save)
			{
				block |= (img.getGrayColor(x, y) >> 4);
				output.write(reinterpret_cast<char*>(&block), sizeof(block));
				save = false;
			}
			else
			{
				block = (img.getGrayColor(x, y) >> 4) << 4;
				save = true;
			}
		}
	}

	// Save remaining one pixel
	if(!save)
		output.write(reinterpret_cast<char*>(&block), sizeof(block));
}

void RGB12::loadGray(std::ifstream & input, Image & img)
{
	std::vector<char> buffer = std::vector<char>(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
	unsigned int
		width = img.width(),
		height = img.height();

	uint8_t gray;
	bool iterate = false;
	auto it = buffer.begin();

	for (unsigned int y = 0; y < height; ++y)
	{
		for (unsigned int x = 0; x < width; ++x)
		{
			if (iterate)
			{
				gray = *it << 4;
				++it;
				iterate = false;
			}
			else
			{
				gray = (*it >> 4) << 4;
				iterate = true;
			}
			img.setPixel(x, y, gray);
		}
	}
		
}

void RGB12::saveHuffman(std::ofstream &os, const Image &img) const
{
	Huffman huffman; // TODO: maybe create constructor like this: Huffman(std::ofstream &os, const Image &img); to encode? and same for decode()
	huffman.encode(os, img);
}

void RGB12::loadHuffman(std::ifstream &is, Image &img)
{
	Huffman huffman;
	huffman.decode(is, img);
}

void RGB12::save444(std::ofstream &f, const Image &img) const
{
#ifdef _DEBUG
	std::cout << "-> [RGB12::save444]: Run BitDensity save algorithm." << std::endl;
#endif

	bool isSpace = true, 
		firstHalf = true;
	char usedChar = 0;
	SDL_Color color;
	unsigned int width = img.width(),
		height = img.height();

	for (unsigned int y = 0; y < height; ++y)
	{
		for (unsigned int x = 0; x < width; ++x)
		{
			for (int k = 0; k < 3; ++k)
			{
				color = img.getPixelColor(x, y);

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

void RGB12::saveLZ77(std::ofstream &ofs, const Image &img) const
{
	LZ77 lz;
	lz.encode(ofs, img);
}

void RGB12::loadLZ77(std::ifstream &ifs, Image &img)
{
	LZ77 lz;
	lz.decode(ifs, img);
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
#ifdef _DEBUG
	std::cout << "\n -> [RG12::store]: Storing Image to file process has just begun." << std::endl;
#endif
	std::ofstream f;

	// Load file to save data in binary mode
	openStream(filename, f);

	// Save global header needed to recover Image
	writeHeader(f, img, algorithm);

	// Save by chosen (or default) algorithm
	switch (algorithm)
	{
	case Algorithm::BitDensity:
		save444(f, img);
		break;
	case Algorithm::Huffman:
		saveHuffman(f, img);
		break;
	case Algorithm::LZ77:
		saveLZ77(f, img);
		break;
	case Algorithm::GreyScale:
		saveGray(f, img);
		break;
	}

	// Close file
	f.close();
#ifdef _DEBUG
	std::cout << " <- [RGB12::store]: Finished.\n" << std::endl;
#endif
}

Image RGB12::recover(const std::string & filename)
{
#ifdef _DEBUG
	std::cout << "\n -> [RG12::recover]: Recovering Image from file process has just begun." << std::endl;
#endif
	std::ifstream f;

	// Open input file in binary mode
	openStream(filename, f);

	// Read global header data
	unsigned int width, height;
	uint8_t depth;
	Algorithm alg;
	std::tie(width, height, depth, alg) = readHeader(f);

	// Create new empty Image
	Image recovered(width, height, depth);

	// Load depending on the alogrithm
	switch (alg)
	{
	case Algorithm::BitDensity:
		load444(f, recovered);
		break;
	case Algorithm::Huffman:
		loadHuffman(f, recovered);
		break;
	case Algorithm::LZ77:
		loadLZ77(f, recovered);
		break;
	case Algorithm::GreyScale:
		loadGray(f, recovered);
		break;
	default:
		std::ostringstream os;
		os << "Saved with uknown algorithm: [unsigned int] " << static_cast<unsigned int>(alg);
		throw RuntimeError(os.str());
	}

	// Close file
	f.close();

#ifdef _DEBUG
	std::cout << " <- [RG12::recover]: Finished.\n" << std::endl;
#endif

	// Return recovered Image
	return std::move(recovered);
}

/**
 * @return {
 *		unsigned int width,
 *		unsigned int height,
 *		uint8_t depth (bits per pixel),
 *		Algorithm chosen compression algorithm }
*/
std::tuple<int, int, uint8_t, RGB12::Algorithm> RGB12::readHeader(std::ifstream &input) const
{
#ifdef _DEBUG
	std::cout << " -> [RGB12::readHeader]: Getting stored informations about this file." << std::endl;
#endif

	// Read size of "verifier" string
	size_t str_size;
	input.read(reinterpret_cast<char *>(&str_size), sizeof(str_size));
	if (str_size >= 1000)
		throw RuntimeError("Header of processed file is possibly invaild.");

	// Read cstring "verifier" and convert it to "string"
	char *cstr = new char[str_size + 1];
	input.read(cstr, str_size);
	cstr[str_size] = '\0';
	std::string ext(cstr);
	delete[] cstr;

#ifdef _DEBUG
	std::cout << "- string(" << str_size << "): " << ext << std::endl;
#endif

	// Verify header
	if(!std::equal(ext.begin(), ext.end(), extension().begin()))
		throw RuntimeError("Header of processed file is not vaild.");

	// Load actually required things
	unsigned int width, height;
	uint8_t depth;
	Algorithm alg;

	input.read(reinterpret_cast<char *>(&width), sizeof(width));
	input.read(reinterpret_cast<char *>(&height), sizeof(height));
	input.read(reinterpret_cast<char *>(&depth), sizeof(depth));
	input.read(reinterpret_cast<char *>(&alg), sizeof(alg));

#ifdef _DEBUG
	std::cout << "- Algorithm: " << static_cast<unsigned int>(alg) << std::endl;
	std::cout << "- Width: " << width << std::endl;
	std::cout << "- Height: " << height << std::endl;
	std::cout << "- Depth: " << static_cast<unsigned int>(depth) << std::endl;
#endif

	return std::make_tuple(width, height, depth, alg);
}

void RGB12::writeHeader(std::ofstream &output, const Image &img, Algorithm alg) const
{

#ifdef _DEBUG
	std::cout << " -> [RGB12::writeHeader]: Save informations about Image to file." << std::endl;
	img.printDetails(std::cout);
	std::cout << " - Algorithm: " << static_cast<unsigned int>(alg) << std::endl;
#endif

	// Saving Image informations
	std::string ext = extension();
	size_t ext_size = ext.size();
	const unsigned int 
		width = img.width(), 
		height = img.height();
	const uint8_t
		depth = static_cast<uint8_t>(img.depth());

	output.write(reinterpret_cast<const char *>(&ext_size), sizeof(ext_size));
	output.write(ext.c_str(), ext_size);
	output.write(reinterpret_cast<const char *>(&width), sizeof(width));
	output.write(reinterpret_cast<const char *>(&height), sizeof(height));
	output.write(reinterpret_cast<const char *>(&depth), sizeof(depth));
	output.write(reinterpret_cast<const char *>(&alg), sizeof(alg));
}

std::string RGB12::extension() const
{
	return std::string(".rgb12");
}

RGB12::RGB12(Algorithm alg)
	: algorithm(alg)
{
#ifdef _DEBUG
	std::cout << "[RGB12]: Called default constructor." << std::endl;
#endif
}

RGB12::RGB12(const ImageHandler &img, Algorithm alg)
	: ImageHandler(convert(img.image)), algorithm(alg)
{
#ifdef _DEBUG
	std::cout << "[RGB12]: Called convert ImageHandler constructor." << std::endl;
#endif
}

RGB12::RGB12(const RGB12 &rgb)
	: ImageHandler(rgb), algorithm(rgb.algorithm)
{
#ifdef _DEBUG
	std::cout << "[RGB12]: Called copy constructor." << std::endl;
#endif
}

RGB12::RGB12(RGB12 &&rgb)
	:ImageHandler(std::move(rgb)), algorithm(rgb.algorithm)
{
#ifdef _DEBUG
	std::cout << "[RGB12]: Called move constructor." << std::endl;
#endif
}

RGB12 & RGB12::operator=(const RGB12 &rgb)
{
#ifdef _DEBUG
	std::cout << "-> [RGB12::operator=]: Called copy assigment operator." << std::endl;
#endif
	
	ImageHandler::operator=(rgb);
	algorithm = rgb.algorithm;
	return *this;
}

RGB12 & RGB12::operator=(RGB12 &&rgb)
{
#ifdef _DEBUG
	std::cout << "-> [RGB12::operator=]: Called move assigment operator." << std::endl;
#endif
	ImageHandler::operator=(std::move(rgb));
	algorithm = rgb.algorithm;
	return *this;
}