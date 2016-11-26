#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <queue>
#include <map>

#include "Image.h"
#include "ColorCounter.h"
#include "Tree.h"

class Huffman
{
private:
	Image *image;
	std::map<Uint32, std::vector<bool> > *codeMap;

public:
	Huffman(Image *image);
	~Huffman();

	void encode();
	void generateCodes(Node<SingleColorData> *node, std::vector<bool> &code, std::map<Uint32, std::vector<bool> > &map, bool left);
	void printCodes() const;
};


#endif // !HUFFMAN_H

