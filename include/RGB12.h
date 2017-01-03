#ifndef RGB12_H
#define RGB12_H

#include "BMP.h"

#include <tuple>
#include <fstream>

class RGB12 : public ImageHandler
{
public:

	enum class Algorithm : uint8_t
	{
		BitDensity,
		Huffman,
		LZ77,
		//GreyScale
	};

	// Indicates which algorithm (defined in Algorithm enum) will be used for future saving process
	// Remarks: stil can read images saved by other compatible algorithms
	Algorithm algorithm;

	virtual std::string extension() const override;
	RGB12(Algorithm = Algorithm::BitDensity);
	RGB12(const BMP &bmp, Algorithm alg = Algorithm::BitDensity);
	RGB12(const RGB12 &);
	RGB12(RGB12 &&);
	RGB12& operator=(const RGB12 &);
	RGB12& operator=(RGB12 &&);


	virtual ~RGB12();

protected:
	virtual void store(const std::string &filename, const Image &image) const override;
	virtual Image recover(const std::string &filename) override;

	std::tuple<int, int, uint8_t, Algorithm> readHeader(std::ifstream &input) const;
	void writeHeader(std::ofstream &output, const Image &img, Algorithm alg) const;

private:

	/// Utility functions

	/**
	 * Creates new Image converted to set DEPTH (RGB12::DEPTH) 
	 * @param Image input
	 * @return Image newly created
	 */
	Image convert(const Image& img) const;

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

	void saveHuffman(std::ofstream &, const Image &) const;
	void loadHuffman(std::ifstream &, Image &);

	void saveLZ77(std::ofstream &, const Image &) const;
	void loadLZ77(std::ifstream &, Image &);

	/// Needed for algorithms
	uint8_t getColorOfPixel(SDL_Color, int) const;
	void setColorOfPixel(SDL_Color &, int, uint8_t) const;

};

#endif // !RGB12_H