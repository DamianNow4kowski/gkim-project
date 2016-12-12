#include "BMP.h"

#include <utility>

void BMP::store(const std::string & filename, const Image & image) const
{
	std::cout << "[BMP]-> Storing Image." << std::endl;
	
	// Remarks: I know it is undefined behaviour but SDL_SaveBMP() requires [not const] SDL_Surface*
	// even though it only access it in read mode (const-incorrect API)
	SDL_SaveBMP(const_cast<SDL_Surface*>(image.img()), filename.c_str());
}

Image BMP::recover(const std::string & filename)
{
	std::cout << "[BMP]-> Recovering Image." << std::endl;
	return Image(SDL_LoadBMP(filename.c_str()));
}

std::string BMP::extension() const
{
	return std::string(".bmp");
}

BMP::BMP()
{
	std::cout << "[BMP]: Called default constructor." << std::endl;
}

BMP::BMP(const BMP &img)
	: ImageHandler(img)
{
	std::cout << "[BMP]: Called copy constructor." << std::endl;
}

BMP::BMP(BMP &&img)
	: ImageHandler(std::move(img))
{
	std::cout << "[BMP]: Called move constructor." << std::endl;
}

BMP & BMP::operator=(const BMP &img)
{
	std::cout << "[BMP]: Called copy assigment operator." << std::endl;
	ImageHandler::operator=(img);
	return *this;
}

BMP & BMP::operator=(BMP &&img)
{
	std::cout << "[BMP]: Called move assigment operator." << std::endl;
	ImageHandler::operator=(std::move(img));
	return *this;
}


BMP::~BMP()
{
	std::cout << "[BMP]: Called virtual destructor." << std::endl;
}
