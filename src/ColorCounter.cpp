#include "ColorCounter.h"

ColorCounter::ColorCounter(Image *img)
{
	this->img = img;
	this->colors = new SingleColorData[img->height() * img->width()];
	this->index = 0;
}

ColorCounter::ColorCounter(const unsigned int &size)
{
	this->img = nullptr;
	this->colors = new SingleColorData[size];
	this->index = size;
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
	for (unsigned int i = 0; i < this->index; i++)
		if (this->colors[i].color == key)
			return i;
	return -1;
}

void ColorCounter::countColors()
{
	Uint32 clr;
	int boolIndex;
	unsigned int i, j;

	for (i = 0; i < this->img->width(); ++i)
		for (j = 0; j < this->img->height(); ++j)
		{
			clr = this->img->getPixel(i, j);
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
}

void ColorCounter::sort()
{
	this->quickSort(0, this->index);
}

void ColorCounter::printColors() const
{
	for (unsigned int i = 0; i < this->index; i++)
		std::cout << std::setw(6) << std::setfill('0') << std::hex << this->colors[i].color << std::dec << "   " << this->colors[i].counter << std::endl;
}

SingleColorData& ColorCounter::getColor(const int &pos) const
{
	return this->colors[pos];
}

void ColorCounter::setColor(const SingleColorData & scd)
{
	this->colors[this->index].color = scd.color;
	this->colors[this->index].counter = scd.counter;
	this->index++;
}

unsigned int ColorCounter::getCountClr() const
{
	return this->index;
}
