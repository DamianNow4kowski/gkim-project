#include <iostream>
#include <string>
#include <cstring>
#include <chrono>
#include <thread>
#include "FileHandler.h"
#include "SDL.h"
#include "Huffman.h"
#include "BMP.h"

using namespace std;

const string bmp_test("test/test.bmp");

void testFileHandler()
{
    bool e;
    string filename("file.txt"), extension("txt"), wrong("bmp");
    e = FileHandler::verifyExtension(filename, extension);
    cout << "Test 1 of verifyExtension(): " << ((e) ? "SUCCEED" : "FAILED") << endl;
    e = FileHandler::verifyExtension(filename.c_str(), extension.c_str());
    cout << "Test 2 of verifyExtension(): " << ((e) ? "SUCCEED" : "FAILED") << endl;
    e = !(FileHandler::verifyExtension(filename.c_str(), strlen(filename.c_str()), wrong.c_str(), strlen(wrong.c_str())));
    cout << "Test 3 of verifyExtension(): " << ((e) ? "SUCCEED" : "FAILED") << endl;
    e = FileHandler::verifyExtension(".txt", "txt");
    cout << "Test 4 of verifyExtension(): " << ((e) ? "SUCCEED" : "FAILED") << endl;
    e = !(FileHandler::verifyExtension("txt", "txt"));
    cout << "Test 5 of verifyExtension(): " << ((e) ? "SUCCEED" : "FAILED") << endl;
    e = !(FileHandler::verifyExtension(".dtxt", "txt"));
    cout << "Test 6 of verifyExtension(): " << ((e) ? "SUCCEED" : "FAILED") << endl;
}

void testSDL_RGB444()
{
    cout << "Testing original SDL_Convert from BMP to RGB444" << endl;
    BMP *bmp_surface = new BMP();
    bmp_surface->load(bmp_test.c_str());
    
    cout << "-- Not converted Image --" << endl;
    SDL_PixelFormat* pxformat = bmp_surface->img()->format;
    const char* pxformatName = SDL_GetPixelFormatName(pxformat->format);
    cout << "Pixel format name: " << pxformatName << endl;
    Huffman *alg = new Huffman(bmp_surface);
    alg->runHoffman();
    delete alg;
    //bmp_surface->preview();

    this_thread::sleep_for(chrono::milliseconds(500)); // try to no exit next window

    cout << "Converting.." << endl;
    SDL_Surface *surface = SDL_ConvertSurfaceFormat(bmp_surface->img(), SDL_PIXELFORMAT_RGB444, 0);
    bmp_surface->init(surface);

    cout << "-- Converted Image --" << endl;
    pxformat = surface->format;
    pxformatName = SDL_GetPixelFormatName(pxformat->format);
    cout << "Pixel format name: " << pxformatName << endl;
    //bmp_surface->preview();
    alg = new Huffman(bmp_surface);
    alg->runHoffman();
    delete alg;
    delete bmp_surface;
}

int main()
{
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
    cout << "Testing.." << endl;
    testFileHandler();
    testSDL_RGB444();
}