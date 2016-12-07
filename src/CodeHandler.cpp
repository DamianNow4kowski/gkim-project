#include "CodeHandler.h"

CodeHandler::CodeHandler(const size_t & size)
	: size(size), index(0), codes(new std::pair<Uint32, std::vector<bool>>[size])
{
}

CodeHandler::~CodeHandler()
{
}

void CodeHandler::add(const Uint32 & clr, const std::vector<bool>& vec)
{
	this->codes[this->index++] = std::pair<Uint32, std::vector<bool>>(clr, vec);
}

void CodeHandler::print()
{
	for (int i = 0; i < this->size; i++)
	{
		std::cout << this->codes[i].first << "   ";
		for (auto &v : this->codes[i].second)
			std::cout << v;
		std::cout << std::endl;
	}
}
