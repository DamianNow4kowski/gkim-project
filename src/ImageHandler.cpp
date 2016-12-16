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

ImageHandler::ImageHandler(Image &&img)
	: image(std::move(img))
{
	std::cout << "[ImageHandler]: Called protected Image move constructor" << std::endl;
}

ImageHandler::ImageHandler()
{
	std::cout << "[ImageHandler]: Called default constructor." << std::endl;
}

ImageHandler::ImageHandler(const Image &img)
	:image(img)
{
	std::cout << "[ImageHandler]: Called const Image& constructor." << std::endl;
}

ImageHandler::ImageHandler(const ImageHandler &iop)
	:image(Image(iop.image))
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
	std::cout << "[ImageHandler]-> Called copy assigment operator." << std::endl;
	image = iop.image;
	return *this;
}

ImageHandler & ImageHandler::operator=(ImageHandler &&iop)
{
	std::cout << "[ImageHandler]-> Called move assigment operator." << std::endl;
	image = std::move(iop.image); // TODO: investigate beheviour
	return *this;
}

void ImageHandler::preview(bool showDetails)
{
	std::cout << "[ImageHandler]-> Previewing Image." << std::endl;
	if (showDetails)
		image.printDetails(std::cout);

	// Remarks: This function is more usefull when don't throws errors; simply finish execution
	if (image.empty())
	{
		std::cerr << "[ImageHandler]: RuntimeError(\"Cannot preview uninitialized Image.\");" << std::endl;
		return;
	}

	// Calculate drawing area to be center inside window
	SDL_Rect dest = {0, 0, 0, 0};
	dest.w = static_cast<int>(image.width());
	dest.h = static_cast<int>(image.height());

	const int minWidth = 300, minHeight = 300;
	int windowWidth, windowHeight;

	// Calculate width and center x-axis offset
	if (dest.w < minWidth)
	{
		dest.x = (minWidth - dest.w) / 2;
		windowWidth = minWidth;
	}
	else windowWidth = dest.w;

	// Calculate height and center y-axis offset
	if (dest.h < minHeight)
	{
		dest.y = (minHeight - dest.h) / 2;
		windowHeight = minHeight;
	}
	else windowHeight = dest.h;

	// Allocate needed things to create window
	SDL_Window *window = SDL_CreateWindow("Preview Image", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
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
	std::cout << "[ImageHandler]-> Saving Image to file: " << filename << std::endl;
	try 
	{
		// Check wheter it is anything to save
		if (image.empty())
			throw RuntimeError("Cannot save unintialized Image.");

		// Add extension if there is not set (or not proper)
		std::string ext = extension();
		if (!verifyExtension(filename, ext))
			filename.append(ext);

		// Invoke implemented virtual function in derievec class
		// to save the Image to file
		store(filename, image);
	}
	catch (const RuntimeError &error)
	{
		std::cerr << "[ImageHandler]: Error while saving Image: " << error.what() << std::endl;
	}
}

void ImageHandler::save(const char *str) const
{
	std::string filename(str);
	save(filename);
}

void ImageHandler::load(const std::string &filename)
{
	std::cout << "[ImageHandler]-> Loading Image from file: " << filename << std::endl;
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
		// to load the Image from file
		Image recovered = recover(filename);

		// Verify if the recovering process has succeed
		if (recovered.empty())
			throw RuntimeError("Loading Image has failed.");
		
		// Free current object and init recovered one (use move assigment to archieve this)
		else image = std::move(recovered);

	}
	catch (const RuntimeError &error)
	{
		std::cerr << "[ImageHandler]: Error while loading Image: " << error.what() << std::endl;
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
