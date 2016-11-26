#include "Huffman.h"

#include <iostream>
#include <algorithm>
#include <iterator>

Huffman::Huffman(Image *image)
{
	this->image = image;
	this->codeMap = new std::map<Uint32, std::vector<bool>>();
}

Huffman::~Huffman()
{
}

void Huffman::encode()
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

	std::vector<bool> code;
	this->generateCodes(trees.top()->getRoot(), code, *this->codeMap, true);

	this->printCodes();
}

void Huffman::generateCodes(Node<SingleColorData>* node, std::vector<bool>& code, 
	std::map<Uint32, std::vector<bool>>& map, bool left)
{
	if (node == nullptr)
		return;
	if (node->next == nullptr && node->prev == nullptr) // is leaf
		map[node->getVar().color] = code;
	else
	{
		auto leftPref = code;
		leftPref.push_back(false);
		this->generateCodes(node->prev, leftPref, map, true);

		auto rightPref = code;
		rightPref.push_back(true);
		this->generateCodes(node->next, rightPref, map, false);
	}
}

void Huffman::printCodes() const
{
	// auto = std::map<Uint32, std::vector<bool>>::const_iterator
	std::cout << "Huffman encoding map:" << std::endl << std::endl;
	for (auto it = codeMap->begin(); it != codeMap->end(); ++it)
	{
		std::cout << std::hex << std::setfill('0') << std::setw(6) << it->first << " ";
		std::copy(it->second.begin(), it->second.end(),
			std::ostream_iterator<bool>(std::cout));
		std::cout << std::endl;
	}
}
