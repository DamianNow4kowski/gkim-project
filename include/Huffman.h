#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "Image.h"

class Huffman
{
private:
	Image *image;

public:
	Huffman(Image *image);
	~Huffman();

	void runHoffman();
};


#endif // !HUFFMAN_H

