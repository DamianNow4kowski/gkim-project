#include "ColorCounter.h"

ColorCounter::ColorCounter(BMP_Surface * bmp)
{
	this->bmp = bmp;
	this->colors = new SingleColorData[bmp->getWidth() * bmp->getHeigth()];
	this->index = 0;
}

ColorCounter::~ColorCounter()
{
	delete this->colors;
}

Uint32 ColorCounter::partition(Uint32 p, Uint32 q)
{
	Uint32 x = this->colors[(p + q) / 2].counter;
	Uint32 i = p, j = q;
	Uint32 buf;
	while (true)
	{
		while (this->colors[j].counter > x)
			j--;
		while (this->colors[i].counter < x)
			i++;
		if (i < j)
		{
			// swap
			buf = this->colors[i].counter;
			this->colors[i].counter = this->colors[j].counter;
			this->colors[j].counter = buf;

			buf = this->colors[i].color;
			this->colors[i].color = this->colors[j].color;
			this->colors[j].color = buf;

			i++;
			j--;
		}
		else
			return j;
	}
}

void ColorCounter::quickSort(Uint32 p, Uint32 q)
{
	if (p < q)
	{
		Uint32 m = partition(p, q);
		quickSort(p, m);
		quickSort(m + 1, q);
	}
}

int ColorCounter::ifExists(const Uint32 & key)
{
	for (int i = 0; i < this->index; i++)
		if (this->colors[i].color == key)
			return i;
	return -1;
}

Uint32 ColorCounter::countColors()
{
	Uint32 clr;
	int boolIndex;

	for (int i = 0; i < this->bmp->getWidth(); i++)
		for (int j = 0; j < this->bmp->getHeigth(); j++)
		{
			clr = this->bmp->getPixel(i, j);
			// check if already added
			boolIndex = ifExists(clr);
			if (boolIndex == -1)
			{
				this->colors[this->index].color = clr;
				this->colors[this->index].counter = 1;
				this->index++;
			}
			else
				this->colors[boolIndex].counter++;
		}
	return this->index;
}

void ColorCounter::sort()
{
	this->quickSort(0, this->index);
}

void ColorCounter::printColors() const
{
	for (int i = 0; i < this->index; i++)
		std::cout << std::setw(6) << std::setfill('0') << std::hex << this->colors[i].color << std::dec << "   " << this->colors[i].counter << std::endl;
}

SingleColorData& ColorCounter::getColor(const int &pos) const
{
	return this->colors[pos];
}
