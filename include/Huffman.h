#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <vector>
#include <fstream>
#include <utility>

#include "Image.h"
#include "Node.h"


class Huffman
{
private:
	Image *image;
	std::vector<std::pair<Uint32, std::vector<bool>>> codeVec;
	std::vector<std::pair<Uint32, Uint32>> colorFreqs;

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

