#include "SDL_Local.h"
#include "Huffman.h"
#include "RGB12.h"
#include "InputHandler.h"

#include <iostream>
#include <array>

int main(int argc, char *argv[])
{
	InputHandler cli(argc, argv, true);

	// Initialize SDL
	try
	{
		SDL sdl(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	}
	catch (const RuntimeError &err)
	{
		std::cerr << "Error while initializing SDL:  " << err.what() << std::endl;
		return 1;
	}

	if (cli.option("open"))
	{
		std::string filename;
		while (cli.get(filename))
		{
			// VERY SIMPLE OPENING
			BMP bmp;
			bmp.load(filename);
			if (bmp.image.empty())
			{
				RGB12 rgb;
				rgb.load(filename);
				rgb.preview();
			}
			else bmp.preview();
		}
	}	

	#ifndef  __linux
		system("PAUSE");
	#endif // ! __linux

	// Return sucess
	return EXIT_SUCCESS;
}
