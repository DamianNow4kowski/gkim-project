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

/*void testSDL_RGB444()
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
}*/

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

void test_RGB444Conversion() 
{
    BMP *bmp = new BMP();
    bmp->load("test/rgbcube.bmp");
    bmp->preview();

    RGB444 *test = new RGB444(bmp->img());
    delete bmp;

	test->preview();

    delete test;
}

void test_openBMPtoGreySaveRGB44Load() 
{
    BMP bmp;
    bmp.load("test/togrey.bmp");
	bmp.preview();

    RGB444 rgb2(bmp);
	rgb2.convertToGreyScale();
    rgb2.save("test/file", 100);

	RGB444 rgb3;
    rgb3.load("test/file.rgb4");
    rgb3.preview();
}

void test_saveOpen444()
{
	BMP bmp;
	bmp.load("test/togrey.bmp");

	RGB444 rgb2(bmp);
	//rgb2.convertToGreyScale();
	rgb2.save("test/file", 1);
	rgb2.load("test/file.rgb4");
	rgb2.preview();
	bmp.preview();
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

void test_convertSurface() 
{
	BMP bmp;
	bmp.load("test/togrey.bmp");
	bmp.preview();

	// Convert then not copy
	RGB444 rgb(bmp.img());
	rgb.preview();

	// Convert then not copy
	RGB444 rgb2(bmp);
	rgb2.preview();

	// Copy then not copy
	SDL_Surface* copy = bmp.getSurface(SDL_PIXELFORMAT_RGB444);
	RGB444 rgb3(copy);
	rgb3.preview();

	// Copy
	BMP *dynbmp = new BMP(bmp);
	//BMP bmp2(std::move(*dynbmp)); todo
	BMP bmp2(*dynbmp);
	delete dynbmp;
	bmp2.preview();
	
	// Copy
	RGB444 rgb4(rgb3);
	rgb4.convertToGreyScale();
	rgb4.preview();
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
	//test_openBMPtoGreySaveRGB44Load();
	//test_saveOpen444();
	//test_copyConstructors();
	test_convertSurface();

	system("PAUSE");
	return 0;
}