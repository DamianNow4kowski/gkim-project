#include "Huffman.h"

#include <iostream>
#include <algorithm>
#include <iterator>

#include <fstream>

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
	this->countFreq();
	this->buildTree();
	this->saveHuffHeader();
}

void Huffman::decode()
{
	this->readHuffHeader();
	this->buildTree();
}

void Huffman::buildTree()
{
	std::cout << "Building Huffman tree..." << std::endl;
	std::priority_queue<Tree<SingleColorData>*, std::vector<Tree<SingleColorData>*>, TreesCmp> trees;

	for (unsigned int i = 0; i < this->clrCntr->getCountClr(); i++)
		trees.push(new Tree<SingleColorData>(*(new Node<SingleColorData>(clrCntr->getColor(i)))));

	while (trees.size() > 1)
	{
		auto t1 = trees.top();
		trees.pop();

		auto t2 = trees.top();
		trees.pop();

		trees.push(&(*t1 + *t2));
	}

	std::cout << "Huffman tree has been built." << std::endl;

	std::vector<bool> codes;
	this->generateCodes(trees.top()->getRoot(), codes, *this->codeMap);
	this->printCodes();
}

void Huffman::generateCodes(Node<SingleColorData>* node, std::vector<bool>& code, 
	std::map<Uint32, std::vector<bool>>& map)
{
	if (node == nullptr)
		return;
	if (node->next == nullptr && node->prev == nullptr) // is leaf
		map[node->getVar().color] = code;
	else
	{
		auto leftPref = code;
		leftPref.push_back(false);
		this->generateCodes(node->prev, leftPref, map);

		auto rightPref = code;
		rightPref.push_back(true);
		this->generateCodes(node->next, rightPref, map);
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

	//for(unsigned int i = 0; i < this->clrCntr->getCountClr(); i++)
	//	std::cout << std::hex << std::setfill('0') << std::setw(6) << this->clrCntr->getColor(i).color
	//	<< "   " << std::dec << this->clrCntr->getColor(i).counter << std::endl;
}

void Huffman::countFreq()
{
	std::cout << "Counting colors..." << std::endl;

	this->clrCntr = new ColorCounter(this->image);
	this->clrCntr->countColors();
	this->clrCntr->sort();

	std::cout << "Finished counting." << std::endl;
	std::cout << "Number of colors in image: " << this->clrCntr->getCountClr() << std::endl;
}

void Huffman::saveHuffHeader()
{
	// save general image header here
	// then Huffman header
	std::fstream file;
	file.open("huff", std::ios::binary | std::ios::out | std::fstream::app);

	Uint32 clr;
	unsigned int cntr;
	unsigned int cnt = this->clrCntr->getCountClr();

	file.write((char*)(&cnt), sizeof(this->clrCntr->getCountClr()));

	for (unsigned int i = 0; i < this->clrCntr->getCountClr(); i++)
	{
		clr = this->clrCntr->getColor(i).color;
		cntr = this->clrCntr->getColor(i).counter;
		file.write((char*)(&clr), sizeof(clr));
		file.write((char*)(&cntr), sizeof(cntr));
	}

	file.close();
}

void Huffman::readHuffHeader()
{
	// ? read general header
	std::fstream file;
	file.open("huff", std::ios::binary | std::ios::in);

	Uint32 clr;
	unsigned int cntr;
	unsigned int numOfCol;

	file.read((char*)(&numOfCol), sizeof(numOfCol));
	this->clrCntr = new ColorCounter(numOfCol);

	for (unsigned int i = 0; i < numOfCol; i++)
	{
		file.read((char*)(&clr), sizeof(clr));
		file.read((char*)(&cntr), sizeof(cntr));

		this->clrCntr->colors[i].color = clr;
		this->clrCntr->colors[i].counter = cntr;
	}

	file.close();
}

