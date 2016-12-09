#include "SDL_Local.h"
#include "Huffman.h"
#include "BMP.h"

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
		std::getline(std::cin, test_bmp);
	}

	// Load BMP
	BMP *bmp_surface = new BMP();
	//bmp_surface->load("bird.bmp");
	//cout << "Hardcoded image dimensions: " << bmp_surface->width() << "x" << bmp_surface->height() << endl;
	bmp_surface->load(test_bmp.c_str(), false);
	std::cout << "Loaded image dimensions: " << bmp_surface->width() << "x" << bmp_surface->height() << std::endl;

	//bmp_surface->preview();

	Huffman *huff = new Huffman(bmp_surface);
	huff->encode();
	huff->decode();
	std::cout << "Finished" << std::endl;

	// Clean
	delete huff;
	delete bmp_surface;

	// I don't want it on linux, sorry
	// We don't use linux, sorry
	#ifndef __linux
		std::cin.ignore();
		std::cin.get();
	#endif
	
	// Return sucess
	return EXIT_SUCCESS;
}
