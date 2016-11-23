#include "Huffman.h"
#include "ColorCounter.h"
#include "Tree.h"
#include <iostream>
#include <queue>


Huffman::Huffman(Image *image)
{
	this->image = image;
}

Huffman::~Huffman()
{
}

void Huffman::runHoffman()
{
	ColorCounter *colorCntr = new ColorCounter(this->image);
	int numColors = colorCntr->countColors();
	colorCntr->sort();
	std::cout << "Number of colors in image: " << numColors << std::endl;

	std::priority_queue<Tree<SingleColorData>*, std::vector<Tree<SingleColorData>*>, TreesCmp> trees;

	for (int i = 0; i < numColors; i++)
		trees.push(new Tree<SingleColorData>(*(new Node<SingleColorData>(colorCntr->getColor(i)))));

	while (trees.size() > 1)
	{
		auto t1 = trees.top();
		trees.pop();

		auto t2 = trees.top();
		trees.pop();

		trees.push(&(*t1 + *t2));
	}

	std::cout << "\n\n-------------------------------\n";
	std::cout << "---------- HUFFMAN CODED ----------\n\n";
	std::cout << *trees.top();
}
