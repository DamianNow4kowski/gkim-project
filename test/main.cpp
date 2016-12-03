#include <iostream>
#include <string>
#include <cstring>
#include <chrono>
#include <thread>
#include "FileHandler.h"
#include "SDL_Local.h"
#include "Huffman.h"
#include "BMP.h"
#include "RGB444.h"

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
    alg->printCodes();
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
    alg->printCodes();
    delete alg;
    delete bmp_surface;
}

void test_GreyScale() {
    BMP *bmp_surface = new BMP();
    bmp_surface->load("test/togrey.bmp");
    bmp_surface->preview();
    bmp_surface->convertToGreyScale();
    bmp_surface->preview();
    delete bmp_surface;
}

void test_openSaveOpenBMP() {
    BMP *bmp_surface = new BMP();
    bmp_surface->load("test/togrey.bmp");
    bmp_surface->preview();
    bmp_surface->convertToGreyScale();
    bmp_surface->save("test/saved");
    bmp_surface->load("test/saved.bmp");
    bmp_surface->preview();
    delete bmp_surface;
}

void test_RGB444Conversion() {
    BMP *bmp = new BMP();
    bmp->load("test/rgbcube.bmp");
    bmp->preview();

    // @unsafe here
    SDL_Surface *surface = bmp->getSurface(SDL_PIXELFORMAT_RGB444);
    RGB444 *test = new RGB444(surface);
    delete bmp;

    /**
     * - Too much previews() causes segmentation fault ???
     */

    test->convertToGreyScale();
    test->preview();

    delete test;
}

void test_openBMPGreysaveLoad() 
{
    //RGB444 rgb2("test/file.rgb4"); // doesnt work yet
    BMP bmp;
    bmp.load("test/togrey.bmp");
    bmp.convertToGreyScale();
    RGB444 rgb2;

	try {
		rgb2.init(bmp.getSurface());
	}
	catch (const RuntimeError &e) {
		cout << "Error: " << e.what() << endl;
	}

    rgb2.save("test/file", 100);
    rgb2.load("test/file.rgb4");
    rgb2.preview();
}

void test_saveOpen444()
{
	BMP bmp;
	bmp.load("test/togrey.bmp");
	bmp.convertToGreyScale();

	RGB444 rgb2;
	try {
		rgb2.init(bmp.getSurface());
	}
	catch (const RuntimeError &e) {
		cout << "Error: " << e.what() << endl;
	}

	rgb2.save("test/file", 1);
	rgb2.load("test/file.rgb4");
	rgb2.preview();
}

void test_copyConstructors()
{
	BMP bmp, *bmpDyn;
	bmp.load("test/togrey.bmp");

	RGB444 rgb(bmp);
	rgb.convertToGreyScale();
	//rgb.preview();

	bmp.preview();
	bmpDyn = new BMP(rgb);
	bmpDyn->preview();
	delete bmpDyn;
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
    //testFileHandler();
    //testSDL_RGB444();
    //test_GreyScale();
    //test_openSaveOpenBMP();
    //test_RGB444Conversion();
    //test_openBMPGreysaveLoad();
	//test_saveOpen444();
	test_copyConstructors();

	system("PAUSE");
	return 0;
}