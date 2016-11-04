#ifndef COLOR_COUNTER_H
#define COLOR_COUNTER_H

#include "BMP_Surface.h"
#include "SingleColorData.h"

class ColorCounter
{
private:
	BMP_Surface *bmp;
	int index;

	Uint32 partition(Uint32 p, Uint32 q);
	void quickSort(Uint32 p, Uint32 q);
	int ifExists(const Uint32 &key);

public:
	SingleColorData *colors;
	ColorCounter(BMP_Surface *bmp);
	~ColorCounter();

	Uint32 countColors();
	void sort();
	void printColors() const;
	SingleColorData& getColor(const int &pos) const;
};

#endif // !COLOR_COUNTER_H