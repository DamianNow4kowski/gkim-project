#include <iostream>
#include <string>
#include <chrono>
#include <utility>
#include "SDL_Local.h"
#include "Huffman.h"
#include "BMP.h"
#include "RGB12.h"
#include "LZ77.h"

using namespace std;

///----UTILITIES FOR TESTS------
void showDuration(std::chrono::steady_clock::time_point begin, std::chrono::steady_clock::time_point end, const char *cstring = "Measured", std::ostream& o = std::cout)
{
	// Default (10^-3)
	char magnitude_prefix = 'm';
	long long duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
	
	// Change to microseconds (10^-6)
	if (duration <= 1)
	{
		magnitude_prefix = 'u';
		duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	}

	// Change to nanoseconds (10^-9)
	if(duration <= 1)
	{
		magnitude_prefix = 'n';
		duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
	}

	o << cstring << " duration time: " << duration << ' ' << magnitude_prefix << 's' << std::endl;
}
/**
 * Usage:
 * auto begin = std::chrono::steady_clock::now();
 * auto end = std::chrono::steady_clock::now();
 * showDuration(begin, end, "Test");
 *
 */
///-----------------------------

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
	bmp.load("test/smalltest_8bit.bmp");
	bmp.preview();

	// Convert construct
	RGB12 rgb(bmp);
	rgb.preview();
	bmp.preview();

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
	rgb2.preview(true); //should fail

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
	rgb3.preview();

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
	rgb4.preview(true); // should fail
}

void test_BitDensity()
{
	BMP bmp;
	bmp.load("test/rgbcube.bmp");
	//bmp.load("test/smalltest_24bit.bmp");
	bmp.preview();

	// Save
	RGB12 rgb(bmp, RGB12::Algorithm::BitDensity);
	rgb.save("test/image");

	// Load
	RGB12 rgb2(RGB12::Algorithm::Huffman);
	rgb2.load("test/image.rgb12");
	rgb2.preview(true);

}

void test_Huffman()
{
	BMP bmp;
	bmp.load("test/wide.bmp");
	//bmp.load("test/1x1.bmp");
	//bmp.load("test/rgbcube.bmp"); // [Compression ratio = 2.861]
	//bmp.load("test/test.bmp"); // [Compression ratio = 4.455]
	//bmp.load("test/smalltest_24bit.bmp");
	//bmp.load("test/smalltest_8bit.bmp");
	bmp.preview();

	/// ENCODING
	RGB12 rgb(bmp, RGB12::Algorithm::Huffman);
	rgb.preview();
	
	auto begin = std::chrono::steady_clock::now();
	rgb.save("test/huffman");
	auto end = std::chrono::steady_clock::now();
	showDuration(begin, end, "Huffman fully encode");
	/**
	 * Records:
	 * test/rgbcube.bmp - 43ms (g++ -03/Linux/x64/notebook 2-core i7) 
	 * test/rgbcube.bmp - 123ms (VS Release/x64/notebook 2-core i7) 
	 * test/test.bmp - 8ms (g++ -O3/Linux/x64/notebook 2-core i7) 
	 * test/test.bmp - 30ms (VS Release/x64/notebook 2-core i7)
	 * test/smalltest_8bit.bmp - 248us (g++ -03/Linux/x64/notebook 2-core i7)
     * test/smalltest_8bit.bmp - 30ms (VS Release/x64/notebook 2-core i7)
	 * test/smalltest_24bit.bmp - 28ms (VS Release/x64/notebook 2-core i7)
 	 */

	/// DECODING
	RGB12 rgb2;

	begin = std::chrono::steady_clock::now();
	rgb2.load("test/huffman.rgb12");
	end = std::chrono::steady_clock::now();
	showDuration(begin, end, "Huffman fully decoded");
	/**
	 * Records:
	 * test/rgbcube.bmp - 982ms (VS Release/x64/notebook 2-core i7)
	 * test/rgbcube.bmp - 1482ms (g++ -03/Linux/x64/notebook 2-core i7) (!????)
	 * test/test.bmp - 10ms (g++ -O3/Linux/x64/notebook 2-core i7)
     * test/test.bmp - 65ms (VS Release/x64/notebook 2-core i7)
     * test/smalltest_8bit.bmp - 250us (g++ -03/Linux/x64/notebook 2-core i7)
	 * test/smalltest_8bit.bmp - 30ms (VS Release/x64/notebook 2-core i7)
	 * test/smalltest_24bit.bmp - 36ms (VS Release/x64/notebook 2-core i7)
 	 */

	rgb2.preview(true);
}

void test_LZ77()
{
	BMP bmp;
	bmp.load("test/rgbcube.bmp");
	//bmp.load("test/test.bmp");
	//bmp.load("test/smalltest_24bit.bmp");
	//bmp.load("test/smalltest_8bit.bmp");
	bmp.preview();

	RGB12 rgb(bmp, RGB12::Algorithm::LZ77);
	rgb.preview();
	rgb.save("test/lz77");
	rgb.load("test/lz77.rgb12");
	rgb.preview();

}

void test_Image()
{
	BMP bmp, test;
	bmp.load("test/smalltest_24bit.bmp");

	test.image = bmp.image; // pixels
	bmp.preview();
	test.preview();

	bmp.image.toGreyScale(); // pixels
	test.image = std::move(bmp.image);
	bmp.preview(); // should fail
	test.preview();

	Image copied(test.image); // pixels
	test.image = copied; // pixels
	test.preview();

	Image moved(std::move(copied));
	test.image = std::move(copied);
	test.preview(); // should fail
	test.image = std::move(moved);
	test.preview(); // should ok
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
	//test_Image();
	//test_BMPHandler();
	//test_RGB12Handler();
	//test_BitDensity();
	test_Huffman();
	//test_LZ77();

	#ifndef __linux
		system("PAUSE");
	#endif
	return 0;
}
