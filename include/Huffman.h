#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <queue>
#include <fstream>
#include <array>
#include <utility>

#include "Image.h"
#include "ColorCounter.h"
#include "Tree.h"

class Huffman
{
private:
	Image *image;
	ColorCounter *clrCntr;

	std::vector <std::pair<Uint32, std::vector<bool>>> codeVec;

public:
	Huffman(Image *image);
	~Huffman();

	void encode();
	void decode();
	void buildTree();
	void generateCodes(Node<SingleColorData> *node, std::vector<bool> &code);
	void printCodes() const;
	void countFreq();

	void saveHuffHeader(std::ofstream &ofile);
	void readHuffHeader(std::ifstream &ifile);
	void saveCodes(std::ofstream &ofile);
	void readCodes(std::ifstream &ifile);
};


#endif // !HUFFMAN_H

