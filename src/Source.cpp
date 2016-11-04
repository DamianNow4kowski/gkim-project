#include <iostream>
#include "SDL.h"
#include "BMP_Surface.h"

#include "Huffman.h"

using namespace std;

int main(int argc, char** argv)
{
	try 
	{
		SDL sdl(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	}
	catch (const InitError& err)
	{
		std::cerr << "Error while initializing SDL:  " << err.what() << std::endl;
		return 1;
	}

	BMP_Surface *mSurface = new BMP_Surface();
	if (!mSurface->loadBMP("test.bmp"))
	{
		cout << "BMP coult not be loaded" << endl;
		return 2;
	}

	Huffman *huff = new Huffman(mSurface);
	huff->runHoffman();
	

	delete huff;
	delete mSurface;

	cin.ignore();
	cin.get();
	return 0;
}
