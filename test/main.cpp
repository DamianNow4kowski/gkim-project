#include <iostream>
#include <string>
#include <cstring>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <utility>
#include "SDL_Local.h"
#include "Huffman.h"
#include "BMP.h"
#include "RGB12.h"
#include "LZ77.h"

using namespace std;

void test_BMPHandler()
{
	BMP bmp;
	bmp.load("test/smalltest_24bit.bmp");
	bmp.preview(true);
	try {
		bmp.image.toGreyScale();
	}
	catch (const RuntimeError &e)
	{
		cerr << "Error: " << e.what() << endl;
	}
	bmp.save("lol");
	bmp.load("lol.bmp");
	bmp.preview(true);
	
	// copy constructor;
	BMP bmp2(bmp);
	bmp2.preview();

	// move constructor
	BMP bmp3(std::move(bmp2));
	bmp3.preview();
	bmp2.preview(); //should fail

	// Copy assigment
	ImageHandler *bmp4 = new BMP();
	bmp4->load("test/smalltest_24bit.bmp");
	*bmp4 = bmp3;
	bmp4->preview();

	// Move assigment
	BMP bmp5;
	bmp5.load("test/smalltest_24bit.bmp");
	bmp5 = std::move(*reinterpret_cast<BMP *>(bmp4));
	bmp5.preview();
	bmp4->preview(); //should fail
	delete bmp4;

}

void test_RGB12Handler()
{
	BMP bmp;
	bmp.load("test/rgbcube.bmp");
	bmp.preview();

	// Convert construct
	RGB12 rgb(bmp);
	rgb.preview();

	// Copy construct
	RGB12 rgb2(rgb);
	try {
		rgb2.image.toGreyScale();
	}
	catch (const RuntimeError &e)
	{
		cerr << "Error: " << e.what() << endl;
	}
	rgb2.preview();

	// Move construct
	RGB12 rgb3(std::move(rgb2));
	try {
		rgb3.image.toGreyScale();
	}
	catch (const RuntimeError &e)
	{
		cerr << "Error: " << e.what() << endl;
	}
	rgb3.preview();
	rgb2.preview(); //should fail

	// Initize with convert construct to force object to be constructed then
	// Copy assigment
	RGB12 rgb4(bmp);
	rgb4 = rgb3;
	try {
		rgb4.image.toGreyScale();
	}
	catch (const RuntimeError &e)
	{
		cerr << "Error: " << e.what() << endl;
	}
	rgb4.preview();

	// Initize with convert construct to force object to be constructed then
	// Move assigment
	RGB12 rgb5(bmp);
	rgb5 = std::move(rgb4);
	try {
		rgb5.image.toGreyScale();
	}
	catch (const RuntimeError &e)
	{
		cerr << "Error: " << e.what() << endl;
	}
	rgb5.preview();
	rgb4.preview(); // should fail
}

void test_saveLoadRGB12()
{
	BMP bmp;
	bmp.load("test/rgbcube.bmp");
	//bmp.load("test/smalltest_24bit.bmp");
	bmp.preview();

	// Save
	RGB12 rgb(bmp, 0);
	rgb.save("test/image");

	// Load
	RGB12 rgb2(1);
	rgb2.load("test/image.rgb12");
	rgb2.preview();
	rgb2.image.toGreyScale();
	rgb2.save("test/image2");


	RGB12 rgb3(2);
	rgb3.load("test/image2.rgb12");
	rgb3.preview();

}

void testHuffman()
{
	BMP bmp;
	bmp.load("test/rgbcube.bmp");
	//bmp.load("test/test.bmp");
	//bmp.load("test/smalltest_24bit.bmp");
	//bmp.load("test/smalltest_8bit.bmp");
	bmp.preview();

	RGB12 rgb(bmp);
	rgb.preview();

	Image &img = rgb.image;
	
	Huffman huffman(&img);
	huffman.encode();
	huffman.decode();

	rgb.preview();
}

void test_LZ77()
{
	BMP bmp;
	//bmp.load("test/rgbcube.bmp");
	//bmp.load("test/test.bmp");
	bmp.load("test/smalltest_24bit.bmp");
	//bmp.load("test/smalltest_8bit.bmp");
	bmp.preview();

	// '2' indicates saving using LZ77
	RGB12 rgb(bmp, 2);
	rgb.preview();
	rgb.save("test/lz77");
	rgb.load("test/lz77.rgb12");
	rgb.preview();

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
	srand(static_cast<unsigned int>(time(NULL)));

    cout << "Testing.." << endl;
	//test_BMPHandler();
	//test_RGB12Handler();
	//test_saveLoadRGB12();
	//testHuffman();
	test_LZ77();

	#ifndef __linux
		system("PAUSE");
	#endif
	return 0;
}