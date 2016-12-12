#ifndef RGB12_H
#define RGB12_H

#include "BMP.h"

#include <tuple>
#include <fstream>

class RGB12 : public ImageHandler
{
private:

	/// Utility functions

	Image convert(const Image&) const;

	/// Algorithms:

	/**
	 * Interfaces:
	 *
	 * - Load algorithm interafce:
	 *
	 *    @param std::ifstream& opened input file stream in binary mode
	 *    @param Image& properly intialized Image (width, height, bpp etc.)
	 * 
	 * - Save algorithm interface:
	 *
	 *    @param std::ofstream& opened output file stream in binary mode
	 *    @param const Image& vaild Image
	 */

	// Saves binary every pixel as 12 bit RGB (without spaces)
	void save444(std::ofstream &, const Image &) const;

	// Loads pixel data from every pixel saved in RGB444 format (without spaces)
	void load444(std::ifstream &, Image &);

	// TODO: Implement Huffman
	void saveHuffman(std::ofstream &, const Image &) const;
	void loadHuffman(std::ifstream &, Image &);

	// TODO: Implement LZ77
	void saveLZ77(std::ofstream &, const Image &) const;
	void loadLZ77(std::ifstream &, Image &);

	/// Needed for algorithms
	uint8_t getColorOfPixel(SDL_Color, int) const;
	void setColorOfPixel(SDL_Color &, int, uint8_t) const;

protected:
	virtual void store(const std::string &filename, const Image &image) const override;
	virtual Image recover(const std::string &filename) override;

	std::tuple<int, int, uint8_t, uint8_t, uint8_t> readHeader(std::ifstream &, bool = false) const;
	void writeHeader(std::ofstream &, const Image &, uint8_t, bool = false) const;

public:
	uint8_t algorithm;
	virtual std::string extension() const override;

	/**
	 * 0 - Algorithm 'Bit density RGB 444'
	 * 1 - Huffman
	 * 2 - LZ77
	 */
	RGB12(uint8_t = 0);
	RGB12(const BMP &, uint8_t = 0);
	RGB12(const RGB12 &);
	RGB12(RGB12 &&);
	RGB12& operator=(const RGB12 &);
	RGB12& operator=(RGB12 &&);


	virtual ~RGB12();
};

#endif // !RGB12_H