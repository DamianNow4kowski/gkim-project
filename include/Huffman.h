#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>

#include "BMP_Surface.h"
#include "ColorCounter.h"
#include "List.h"

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

