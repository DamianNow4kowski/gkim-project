#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "Image.h"
#include "Node.h"

#include <vector>
#include <fstream>
#include <iostream>

class Huffman
{
private:
	Image *image;
	std::vector<std::pair<uint32_t, std::vector<bool>>> codeVec;
	std::vector<std::pair<uint32_t, uint32_t>> colorFreqs;

	void countFreq();
	void generateCodes(const Node *node, std::vector<bool> &code);
	void buildTree();

	void printCodes() const;

	void saveHuffHeader(std::ofstream &ofile);
	void readHuffHeader(std::ifstream &ifile);
	void saveCodes(std::ofstream &ofile);
	void readCodes(std::ifstream &ifile);

public:
	Huffman(Image *image);
	~Huffman();

	void encode();
	void decode();
};


#endif // !HUFFMAN_H

