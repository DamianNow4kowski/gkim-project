#include "Huffman.h"

#include "BitsToFile.h"
#include "BMP.h"

#include <iostream>
#include <iomanip>
#include <queue>
#include <iterator>
#include <algorithm>

Huffman::Huffman(Image *image)
	: image(image), 
	codeVec(std::vector<std::pair<uint32_t, std::vector<bool>>>()), 
	colorFreqs(std::vector<std::pair<uint32_t, uint32_t>>())
{
}

Huffman::~Huffman()
{
}

void Huffman::encode()
{
	std::cout << "\n=== HUFFMAN COMPRESSION ===" << std::endl;
	std::ofstream ofile("test/huff", std::ios::binary);
	countFreq();
	buildTree();
	saveHuffHeader(ofile);
	saveCodes(ofile);
	ofile.close();

	codeVec.clear();
	colorFreqs.clear();

	std::cout << "=== HUFFMAN COMPRESSION DONE ===\n" << std::endl;
}

void Huffman::decode()
{
	std::cout << "\n=== HUFFMAN DECOMPRESSION ===" << std::endl;

	std::ifstream ifile("test/huff", std::ios::binary);
	readHuffHeader(ifile);
	buildTree();
	readCodes(ifile);
	ifile.close();

	codeVec.clear();
	colorFreqs.clear();

	std::cout << "=== HUFFMAN COMPRESSION DONE ===\n" << std::endl;
}

void Huffman::countFreq()
{
	std::cout << "Counting colors..." << std::endl;

	uint32_t clr = -1;
	bool found = false; 
	for (unsigned int j = 0; j < image->height(); j++)
	{
		for (unsigned int i = 0; i < image->width(); i++)
		{
			clr = image->getPixel(i, j); // get color
			found = false;
			for (auto &v : colorFreqs) // check if already appeared
			{
				if (v.first == clr) // if found - increment frequency
				{
					v.second++;
					found = true;
				}
			}

			if (!found) // if not found - Add it
			{
				colorFreqs.push_back(std::pair<uint32_t, uint32_t>(clr, 1));
				found = true;
			}
		}
	}

	std::cout << "Number of colors: ." << colorFreqs.size() << std::endl;
}

void Huffman::generateCodes(const Node *node, std::vector<bool>& code)
{
	if (node == nullptr)
		return;
	if (node->right == nullptr && node->left == nullptr) // is leaf - save code of node
		codeVec.push_back(std::pair<uint32_t, std::vector<bool>>(node->colorData.first, code));
	else
	{
		auto leftPref = code;
		leftPref.push_back(false);
		generateCodes(node->left, leftPref); // add 0 and go left

		auto rightPref = code;
		rightPref.push_back(true);
		generateCodes(node->right, rightPref); // add 1 and go right
	}
}

void Huffman::buildTree()
{
	std::cout << "Building tree..." << std::endl;

	std::priority_queue<Node*, std::vector<Node*>, NodeCmp> trees; // Add all colors as single nodes
	for (auto &v : colorFreqs)
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

	std::cout << "Tree build." << std::endl;
	std::cout << "Generating codes & sorting..." << std::endl;

	std::vector<bool> codes; // code for each color
	generateCodes(root, codes);

	delete root; // no longer needed

	// sort codes before writing to file
	// speed up decoding - most frequent codes are in the beginning
	std::sort(codeVec.begin(), codeVec.end(),
		[](const std::pair<uint32_t, std::vector<bool>> &p1, const std::pair<uint32_t, std::vector<bool>> &p2)
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

//	printCodes();
	std::cout << "Codes generated." << std::endl;
}

void Huffman::printCodes() const
{
	std::cout << "Huffman encoding map:" << std::endl << std::endl;
	for (const auto &v : codeVec)
	{
		std::cout << std::hex << std::setfill('0') << std::setw(6) << v.first << "   ";
		for (const auto &vv : v.second)
			std::cout << vv;
		std::cout << std::dec << std::endl;
	}
}

void Huffman::saveHuffHeader(std::ofstream &ofile)
{
	// save general image header here
	// then Huffman header

	std::cout << "Saving header..." << std::endl;

	uint32_t clr;
	unsigned int cntr;
	size_t numberOfColors = colorFreqs.size();

	ofile.write(reinterpret_cast<const char*>(&numberOfColors), sizeof(numberOfColors));

	for (auto &v : colorFreqs)
	{
		clr = v.first;
		cntr = v.second;
		ofile.write(reinterpret_cast<const char*>(&clr), sizeof(clr));
		ofile.write(reinterpret_cast<const char*>(&cntr), sizeof(cntr));
	}

	std::cout << "Header saved." << std::endl;
}

void Huffman::readHuffHeader(std::ifstream &ifile)
{
	// ? read general header

	std::cout << "Reading header..." << std::endl;

	uint32_t clr;
	unsigned int cntr;
	size_t numOfColors;

	ifile.read((char*)(&numOfColors), sizeof(numOfColors));

	for (size_t i = 0; i < numOfColors; i++)
	{
		ifile.read(reinterpret_cast<char *>(&clr), sizeof(clr));
		ifile.read(reinterpret_cast<char *>(&cntr), sizeof(cntr));
		colorFreqs.push_back(std::pair<uint32_t, uint32_t>(clr, cntr));
	}

	std::cout << "Header read." << std::endl;
}

void Huffman::saveCodes(std::ofstream &ofile)
{
	std::cout << "Saving content..." << std::endl;

	uint32_t clr;
	BitsToFile btf(ofile);

	for (unsigned int j = 0; j < image->height(); ++j)
	{
		for (unsigned int i = 0; i < image->width(); ++i)
		{
			clr = image->getPixel(i, j);
			for (auto &v : codeVec)
				if (v.first == clr)
				{
					btf.to(v.second);
					break;
				}
		}
	}

	btf.flush();
	std::cout << "Content saved." << std::endl;
}

void Huffman::readCodes(std::ifstream &ifile)
{
	std::cout << "Reading content..." << std::endl;

	// Reallocate
	image->allocate(image->width(), image->height(), image->depth());

	BitsFromFile bff(ifile);

	//int index = 0;
	std::vector<bool> vec;

	bool found = false;

	for (unsigned int j = 0; j < image->height(); ++j)
	{
		for (unsigned int i = 0; i < image->width(); ++i)
		{
			found = false;

			while (!found)
			{
				vec.push_back(bff.get());
				for (auto v = codeVec.begin(); v != codeVec.end(); ++v)
				{
					if (v->second == vec)
					{
						image->setPixel(i, j, v->first);
						found = true;
						vec.clear();
						break;
					}
				}
			}
			vec.clear();
		}
	}

	std::cout << "Content read." << std::endl;
}

