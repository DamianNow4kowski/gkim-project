#ifndef CODE_HANDLER_H
#define CODE_HANDLER_H

#include <utility>
#include <vector>
#include <iostream>
#include "SDL_Local.h"

class CodeHandler
{
private:
	std::pair<Uint32, std::vector<bool>> *codes;
	size_t size;
	size_t index;

public:
	CodeHandler(const size_t &size);
	~CodeHandler();

	void add(const Uint32 &clr, const std::vector<bool> &vec);
	void print();
};


#endif // !CODE_HANDLER_H

