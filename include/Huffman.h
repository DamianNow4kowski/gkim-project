#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <queue>
#include <map>
#include <fstream>

#include "Image.h"
#include "ColorCounter.h"
#include "Tree.h"

class Huffman
{
private:
	Image *image;
	std::map<Uint32, std::vector<bool>> *codeMap;
	ColorCounter *clrCntr;

public:
	Huffman(Image *image);
	~Huffman();

	void encode();
	void decode();
	void buildTree();
	void generateCodes(Node<SingleColorData> *node, std::vector<bool> &code, std::map<Uint32, std::vector<bool>> &map);
	void printCodes() const;
	void countFreq();

	void saveHuffHeader(std::ofstream &ofile);
	void readHuffHeader(std::ifstream &ifile);
	void saveCodes(std::ofstream &ofile);
	void readCodes(std::ifstream &ifile);
};


#endif // !HUFFMAN_H

