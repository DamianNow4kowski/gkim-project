#ifndef COLOR_COUNTER_H
#define COLOR_COUNTER_H

#include "Image.h"
#include "SingleColorData.h"

class ColorCounter
{
private:
	Image *img;
	unsigned int index;

	Uint32 partition(Uint32 p, Uint32 q);
	void quickSort(Uint32 p, Uint32 q);
	int ifExists(const Uint32 &key);

public:
	SingleColorData *colors;
	ColorCounter(const unsigned int &size);
	ColorCounter(Image *img);
	~ColorCounter();

	void countColors();
	void sort();
	void printColors() const;
	SingleColorData& getColor(const int &pos) const;
	void setColor(const SingleColorData &scd);
	unsigned int getCountClr() const;
};

#endif // !COLOR_COUNTER_H