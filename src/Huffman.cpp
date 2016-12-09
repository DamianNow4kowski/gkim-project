#include "Huffman.h"
#include "BitsToFile.h"

#include <iostream>
#include <algorithm>
#include <iterator>
#include <iomanip>

// ??
#include "BMP.h"

Huffman::Huffman(Image *image)
	: image(image), 
	codeVec(std::vector<std::pair<Uint32, std::vector<bool>>>()), 
	colorFreqs(std::vector<std::pair<Uint32, Uint32>>())
{
}

Huffman::~Huffman()
{
}

void Huffman::encode()
{
	std::ofstream ofile("huff", std::ios::binary);
	this->countFreq();
	this->buildTree();
	this->printCodes();
	this->saveHuffHeader(ofile);
	this->saveCodes(ofile);
	ofile.close();

	this->codeVec.clear();
	this->colorFreqs.clear();
}

void Huffman::decode()
{
	std::ifstream ifile("huff", std::ios::binary);
	this->readHuffHeader(ifile);
	this->buildTree();
	this->readCodes(ifile);
	ifile.close();

	this->codeVec.clear();
	this->colorFreqs.clear();
}

void Huffman::countFreq()
{
	Uint32 clr = -1;
	bool found = false;
	for (size_t i = 0; i < this->image->height(); i++)
	{
		for (size_t j = 0; j < this->image->width(); j++)
		{
			clr = this->image->getPixel(this->image->img(), i, j); // get color
			found = false;
			for (auto &v : this->colorFreqs) // check if already appeared
			{
				if (v.first == clr) // increment frequency
				{
					v.second++;
					found = true;
				}
			}

			if (!found) // if not found - Add it
			{
				this->colorFreqs.push_back(std::pair<Uint32, Uint32>(clr, 1));
				found = true;
			}
		}
	}
}

void Huffman::generateCodes(Node *node, std::vector<bool>& code)
{
	if (node == nullptr)
		return;
	if (node->right == nullptr && node->left == nullptr) // is leaf
		this->codeVec.push_back(std::pair<Uint32, std::vector<bool>>(node->colorData.first, code));
	else
	{
		auto leftPref = code;
		leftPref.push_back(false);
		this->generateCodes(node->left, leftPref);

		auto rightPref = code;
		rightPref.push_back(true);
		this->generateCodes(node->right, rightPref);
	}
}

void Huffman::buildTree()
{
	std::priority_queue<Node*, std::vector<Node*>, NodeCmp> trees;
	for (auto &v : this->colorFreqs)
		trees.push(new Node(v));

	// Build Main Tree
	while (trees.size() > 1)
	{
		auto chR = trees.top();
		trees.pop();

		auto chL = trees.top();
		trees.pop();

		auto chP = new Node(chR, chL);
		trees.push(chP);
	}

	auto root = trees.top();

	std::vector<bool> codes; // code for each color

	this->generateCodes(root, codes);

	// sort codes before writing to file
	// speed up decoding - most frequent codes are in the beginning
	std::sort(codeVec.begin(), codeVec.end(),
		[](const std::pair<Uint32, std::vector<bool>> &p1, const std::pair<Uint32, std::vector<bool>> &p2)
	{
		if (p1.second.size() < p2.second.size())
			return true;
		if (p1.second.size() > p2.second.size())
			return false;
		for (size_t i = 0; i < p1.second.size(); i++)
		{
			if (p1.second[i] < p2.second[i])
				return true;
			else if (p1.second[i] > p2.second[i])
				return false;
		}
		return true;
	}
	);
}

void Huffman::printCodes() const
{
	std::cout << "Huffman encoding map:" << std::endl << std::endl;
	for (const auto &v : codeVec)
	{
		std::cout << std::hex << std::setfill('0') << std::setw(6) << v.first << "   ";
		for (const auto &vv : v.second)
			std::cout << vv;
		std::cout << std::endl;
	}
}

void Huffman::saveHuffHeader(std::ofstream &ofile)
{
	// save general image header here
	// then Huffman header

	Uint32 clr;
	unsigned int cntr;
	unsigned int cnt = this->colorFreqs.size();

	ofile.write((char*)(&cnt), sizeof(cnt));

	for (auto &v : this->colorFreqs)
	{
		clr = v.first;
		cntr = v.second;
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

	for (size_t i = 0; i < numOfCol; i++)
	{
		ifile.read((char*)(&clr), sizeof(clr));
		ifile.read((char*)(&cntr), sizeof(cntr));

		this->colorFreqs.push_back(std::pair<Uint32, Uint32>(clr, cntr));
	}
}

void Huffman::saveCodes(std::ofstream &ofile)
{
	Uint32 clr;
	BitsToFile btf(ofile);

	for (size_t j = 0; j < this->image->height(); j++)
	{
		for (size_t i = 0; i < this->image->width(); i++)
		{
			clr = this->image->getPixel(i, j);
			//		btf.to(this->codeMap->find(clr)->second);
			for (auto &v : this->codeVec)
				if (v.first == clr)
				{
					btf.to(v.second);
					break;
				}
		}
	}

	btf.flush();
}

void Huffman::readCodes(std::ifstream &ifile)
{
	int w = 960;
	int h = 960;
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

				for (auto v = this->codeVec.begin(); v != this->codeVec.end(); ++v)
				{
					if (v->second == vec)
					{
						SDL_GetRGB(v->first, this->image->img()->format, &ccc.r, &ccc.g, &ccc.b);
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

