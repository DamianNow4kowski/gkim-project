#include "ImageHandler.h"

#include <iostream>  //debug
#include <utility>
#include <algorithm>
#include <sstream>  // error handling

bool ImageHandler::verifyExtension(const std::string &filename, const std::string &extension) const
{
	return filename.size() >= extension.size()
		&& std::equal(filename.end() - extension.size(), filename.end(), extension.begin());
}

void ImageHandler::openStream(const std::string &filename, std::ifstream &input) const
{
	input.open(filename, std::ios::in | std::ios::binary);

	// @remarks: http://stackoverflow.com/questions/24097580/ifstreamis-open-vs-ifstreamfail
	if (!input) {
		std::ostringstream os;
		os << "Cannot open file in load mode [filename = '" << filename << "']";
		throw RuntimeError(os.str());
	}
}

void ImageHandler::openStream(const std::string &filename, std::ofstream &output) const
{
	output.open(filename, std::ios::out | std::ios::binary);
	if (!output) {
		std::ostringstream os;
		os << "Cannot open file in save mode [filename = '" << filename << "']";
		throw RuntimeError(os.str());
	}
}

ImageHandler::ImageHandler(SDL_Surface *surface)
	: image(ImageSurface(surface))
{
	std::cout << "[ImageHandler]: Called protected SDL_Surface* constructor." << std::endl;
}

ImageHandler::ImageHandler(const SDL_Surface *surface)
	: image(ImageSurface(surface))
{
	std::cout << "[ImageHandler]: Called public const SDL_Surface* constructor." << std::endl;
}

ImageHandler::ImageHandler()
{
	std::cout << "[ImageHandler]: Called default constructor." << std::endl;
}

ImageHandler::ImageHandler(const ImageSurface &img)
	:image(img)
{
	std::cout << "[ImageHandler]: Called const ImageSurface& constructor." << std::endl;
}

ImageHandler::ImageHandler(const ImageHandler &iop)
	:image(ImageSurface(iop.image))
{
	std::cout << "[ImageHandler]: Called copy constructor." << std::endl;
}

ImageHandler::ImageHandler(ImageHandler &&iop)
	:image(std::move(iop.image)) // TODO: investigate behaviour
{
	std::cout << "[ImageHandler]: Called move constructor." << std::endl;
}

ImageHandler & ImageHandler::operator=(const ImageHandler &iop)
{
	std::cout << "[ImageHandler]-> Called copy assigment." << std::endl;
	image = iop.image;
	return *this;
}

ImageHandler & ImageHandler::operator=(ImageHandler &&iop)
{
	std::cout << "[ImageHandler]-> Called move assigment." << std::endl;
	image = std::move(iop.image); // TODO: investigate beheviour
	return *this;
}

void ImageHandler::preview(bool showDetails)
{
	std::cout << "[ImageHandler]-> Previewing ImageSurface." << std::endl;
	if (showDetails)
		image.printDetails();

	if (image.empty())
		throw RuntimeError("Cannot preview uninitialized ImageSurface.");

	// Calculate drawing area to be center inside window
	SDL_Rect dest = {0, 0, 0, 0};

	const int minWidth = 300, 
		minHeight = 300;

	int width = image.img()->w, 
		height = image.img()->h;

	dest.w = width;
	dest.h = height;
	
	if (width < minWidth)
	{
		dest.x = (minWidth - width) / 2;
		width = minWidth;
	}
	if (height < minHeight)
	{
		dest.y = (minHeight - height) / 2;
		height = minHeight;
	}

	// Allocate needed things to create window
	SDL_Window *window = SDL_CreateWindow("Preview Image", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Texture* texture = image.texture(renderer);
	SDL_Event e;
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, &dest);
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

	// Free memory
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void ImageHandler::save(std::string &filename) const
{
	std::cout << "[ImageHandler]-> Saving ImageSurface to file: " << filename << std::endl;
	try 
	{
		// Check wheter it is anything to save
		if (image.empty())
			throw RuntimeError("Cannot save unintialized ImageSurface.");

		// Add extension if there is not set (or not proper)
		std::string ext = extension();
		if (!verifyExtension(filename, ext))
			filename.append(ext);

		// Invoke implemented virtual function in derievec class
		// to save the ImageSurface to file
		store(filename, image);
	}
	catch (const RuntimeError &error)
	{
		std::cerr << "[ImageHandler]: Error while saving ImageSurface: " << error.what() << std::endl;
	}
}

void ImageHandler::save(const char *str) const
{
	save(std::string(str));
}

void ImageHandler::load(const std::string &filename)
{
	std::cout << "[ImageHandler]-> Loading ImageSurface from file: " << filename << std::endl;
	try 
	{
		// Validate extension of filename
		if (!verifyExtension(filename, extension()))
		{
			std::ostringstream os;
			os << "Cannot load file '" << filename 
				<< "' due to unproper extension. [required ext = " << extension() << ']';
			throw RuntimeError(os.str());
		}

		// Invoke implemented virtual function in derieved class 
		// to load the ImageSurface from file
		ImageSurface recovered = recover(filename);

		// Verify if the recovering process has succeed
		if (recovered.empty())
			throw RuntimeError("Loading ImageSurface has failed.");
		
		// Free current object and init recovered one (use move assigment to archieve this)
		else image = std::move(recovered);

	}
	catch (const RuntimeError &error)
	{
		std::cerr << "[ImageHandler]: Error while saving ImageSurface: " << error.what() << std::endl;
	}
}

void ImageHandler::load(const char *str)
{
	load(std::string(str));
}


ImageHandler::~ImageHandler()
{
	std::cout << "[ImageHandler]: Called virtual destructor." << std::endl;
}
