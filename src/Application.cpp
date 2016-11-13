#include <iostream>
#include "SDL.h"
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
    bmp_surface->load("bin/test.bmp");
    cout << "First image is " << bmp_surface->width() << "x" << bmp_surface->height() << endl;
    bmp_surface->load(test_bmp.c_str(), false);
    cout << "Second image is " << bmp_surface->width() << "x" << bmp_surface->height() << endl;

    bmp_surface->preview();

    Huffman *huff = new Huffman(bmp_surface);
    huff->runHoffman();

    // Clean
    delete huff;
    delete bmp_surface;

    // Return sucess
    return EXIT_SUCCESS;
}
