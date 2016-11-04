#ifndef SINGLE_COLOR_DATA_H
#define SINGLE_COLOR_DATA_H 

#include <iostream>
#include <iomanip>
#include <SDL.h>

class SingleColorData
{
public:
	Uint32 color;
	int counter;

	friend bool operator<(const SingleColorData &el1, const SingleColorData &el2)
	{
		return el1.counter < el2.counter;
	}

	friend bool operator>(SingleColorData &el1, SingleColorData &el2)
	{
		return !(el1 < el2);
	}

	friend SingleColorData& operator+(SingleColorData el1, SingleColorData el2)
	{
		SingleColorData *el = new SingleColorData();
		el->counter = el1.counter + el2.counter;
		el->color = el1.color + el2.color;
		return *el;
	}

	friend std::ostream& operator<<(std::ostream& out, SingleColorData &elem)
	{
		out << std::hex << std::setfill('0') << std::setw(6) << elem.color << "   " << std::dec << elem.counter << std::endl;
		return out;
	}
};

#endif // ! SINGLE_COLOR_DATA_H