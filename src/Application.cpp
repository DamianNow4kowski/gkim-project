#include <iostream>
#include "SDL_Local.h"
#include "BMP.h"
#include "Huffman.h"

using namespace std;

int main(int argc, char *argv[])
{
	string test_bmp;

	// Initialize SDL
	try
	{
		SDL sdl(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	}
	catch (const RuntimeError &err)
	{
		cerr << "Error while initializing SDL:  " << err.what() << endl;
		return 1;
	}

	// Get BMP File
	if (argc == 3 && strcmp(argv[1], "-f") == 0)
		test_bmp = argv[2];
	else 
	{
		cout << "Podaj plik *.bmp do kompresji: "; 
		cin >> test_bmp;
	}

	// Load BMP
	BMP *bmp_surface = new BMP();
	//bmp_surface->load("bird.bmp");
	//cout << "Hardcoded image dimensions: " << bmp_surface->width() << "x" << bmp_surface->height() << endl;
	bmp_surface->load(test_bmp.c_str(), false);
	cout << "Loaded image dimensions: " << bmp_surface->width() << "x" << bmp_surface->height() << endl;

	//bmp_surface->preview();

	Huffman *huff = new Huffman(bmp_surface);
	huff->decode();


	cout << "Finished" << endl;

	// Clean
	delete huff;
	delete bmp_surface;

	// I don't want it on linux, sorry
	// We don't use linux, sorry
	#ifndef __linux
		cin.ignore();
		cin.get();
	#endif
	
	// Return sucess
	return EXIT_SUCCESS;
}
