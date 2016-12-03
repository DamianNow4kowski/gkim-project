#include "Huffman.h"
#include "BitsToFile.h"

#include <iostream>
#include <algorithm>
#include <iterator>

// ??
#include "BMP.h"

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
	std::ofstream ofile("huff", std::ios::binary);
	this->countFreq();
	this->buildTree();
	this->saveHuffHeader(ofile);
	this->saveCodes(ofile);
	ofile.close();
}

void Huffman::decode()
{
	std::ifstream ifile("huff", std::ios::binary);
	this->readHuffHeader(ifile);
	this->buildTree();
	this->readCodes(ifile);
	ifile.close();
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
//	this->printCodes();
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

void Huffman::saveHuffHeader(std::ofstream &ofile)
{
	// save general image header here
	// then Huffman header

	Uint32 clr;
	unsigned int cntr;
	unsigned int cnt = this->clrCntr->getCountClr();

	ofile.write((char*)(&cnt), sizeof(this->clrCntr->getCountClr()));

	for (unsigned int i = 0; i < this->clrCntr->getCountClr(); i++)
	{
		clr = this->clrCntr->getColor(i).color;
		cntr = this->clrCntr->getColor(i).counter;
		ofile.write((char*)(&clr), sizeof(clr));
		ofile.write((char*)(&cntr), sizeof(cntr));
	}
}

void Huffman::readHuffHeader(std::ifstream &ifile)
{
	// ? read general header

	Uint32 clr;
	unsigned int cntr;
	unsigned int numOfCol;

	ifile.read((char*)(&numOfCol), sizeof(numOfCol));
	this->clrCntr = new ColorCounter(numOfCol);

	for (unsigned int i = 0; i < numOfCol; i++)
	{
		ifile.read((char*)(&clr), sizeof(clr));
		ifile.read((char*)(&cntr), sizeof(cntr));

		this->clrCntr->colors[i].color = clr;
		this->clrCntr->colors[i].counter = cntr;
	}

}

void Huffman::saveCodes(std::ofstream &ofile)
{
	Uint32 clr;
	BitsToFile btf(ofile);

	for (int j = 0; j < this->image->height(); j++)
	{
		for (int i = 0; i < this->image->width(); i++)
		{
			clr = this->image->getPixel(i, j);
			btf.to(this->codeMap->find(clr)->second);
		}
	}

	btf.flush();
}

void Huffman::readCodes(std::ifstream &ifile)
{
	int w = 500;
	int h = 500;
	BMP *bmp = new BMP();
	SDL_Surface *surf = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
	bmp->init(surf);

	BitsFromFile bff(ifile);

	SDL_Color ccc;
	int index = 0;
	std::vector<bool> vec;

	bool found = false;

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			found = false;

			while (!found)
			{
				vec.push_back(bff.get());

				for (auto v = this->codeMap->begin(); v != this->codeMap->end(); ++v)
				{
					if (v->second == vec)
					{
						SDL_GetRGB(v->first, surf->format, &ccc.r, &ccc.g, &ccc.b);
						//std::cout << ccc.r << " " << ccc.g << " " << ccc.b << std::endl;
						found = true;
						vec.clear();
						break;
					}
				}
			}

			bmp->setPixel(surf, i, j, ccc.r, ccc.g, ccc.b);
			vec.clear();
		}
	}

	bmp->preview();
}

