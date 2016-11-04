#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "BMP_Surface.h"

class Huffman
{
private:
	BMP_Surface *surface;

public:
	Huffman(BMP_Surface *surface);
	~Huffman();

	void runHoffman();
};


#endif // !HUFFMAN_H

