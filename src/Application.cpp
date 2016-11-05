#include <iostream>
#include "SDL.h"
#include "BMP_Surface.h"
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
    if(argc == 3 && strcmp(argv[1], "-f") == 0) {
      test_bmp = argv[2];
    } else {
      cout << "Podaj plik *.bmp do kompresji: "; cin >> test_bmp;
    }

    // Load BMP
    BMP *bmp_surface = new BMP();
    bmp_surface->load(test_bmp.c_str());
    cout << "First image is " << bmp_surface->width() << "x" << bmp_surface->height() << endl;
    bmp_surface->load("bin/test2.bmp");
    cout << "Second image is " << bmp_surface->width() << "x" << bmp_surface->height() << endl;

    BMP_Surface *mSurface = new BMP_Surface();
    if (!mSurface->loadBMP(test_bmp))
    {
      // Delete last 3 characters which stands for appname;
      argv[0][strlen(argv[0])-3]= '\0';
      cout << "BMP file: '" << test_bmp << "' could not be loaded! [Debug] called from: " << argv[0] << endl;
      return 2;
    }

    Huffman *huff = new Huffman(mSurface);
    huff->runHoffman();

    // Clean
    delete huff;
    delete mSurface;

    // Return sucess
    return EXIT_SUCCESS;
}
