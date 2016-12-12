#include "SDL_Local.h"
#include "Huffman.h"
#include "RGB12.h"

#include <iostream>
#include <array>

int main(int argc, char *argv[])
{
	std::string test_bmp;

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

	// Get BMP File
	if (argc == 3 && strcmp(argv[1], "-f") == 0)
		test_bmp = argv[2];
	else 
	{
		std::cout << "Podaj plik *.bmp do kompresji: "; 
		std::cin >> test_bmp;
	}

	// Load image
	BMP bmp;
	bmp.load(test_bmp);
	RGB12 rgb(std::move(bmp));
	rgb.preview();

	// Attach to huffman
	Image &img = rgb.image;
	Huffman huffman(&img);
	huffman.encode();
	huffman.decode();

	// Preview decoded from huffman
	rgb.preview();

	#ifndef  __linux
		system("PAUSE");
	#endif // ! __linux

	// Return sucess
	return EXIT_SUCCESS;
}
