#include "Own.h"

Own::Own(SDL_Surface* surface) : Image(surface) {};

Own::Own() : Image() {};

const char *Own::extension() const {
	return "own";
}

SDL_Surface *Own::loadImpl(const char *file)
{
	this->file.open(file, std::ios::binary | std::ios::in);
	this->readHeader();
	int colorToCode = 0;
	int pixelUse = 0;
	char usedChar = 0;
	int i = 0;
	int j = 0;
	SDL_Color pixel;
	while (this->file.get(usedChar))
	{
		for (int k = 0; k < 2; k++)
		{
			if (k == 0)
				this->setColorOfPixel(pixel, colorToCode, (usedChar >> 4) << 4);
			else
				this->setColorOfPixel(pixel, colorToCode, (usedChar << 4));
			colorToCode++;
			if (colorToCode == 3)
			{
				if (i == this->width())
				{
					i = 0;
					j++;
				}
				this->setPixel(i, j, pixel.r, pixel.g, pixel.b);
				colorToCode = 0;
				i++;
			}
		}
	}


	this->file.close();
	return nullptr;
}

void Own::saveImpl(SDL_Surface* surface, const char *file)
{
	this->file.open(file, std::ios::binary| std::ios::out);
	this->writeHeader();
	bool isSpace = true;
	bool firstHalf = true;
	char usedChar = 0;
	for (unsigned int i = 0; i < this->height(); i++)
	{
		for (unsigned int j = 0; j < this->width(); j++)
		{
			for (int k = 0; k < 3; k++)
			{
				if (firstHalf)
				{
					usedChar = (getColorOfPixel(this->getPixelColorRGB(j, i), k) >> 4) << 4;
					firstHalf = false;
				}
				else
				{
					usedChar |= getColorOfPixel(this->getPixelColorRGB(j, i), k) >> 4;
					firstHalf = true;
					isSpace = false;
				}
				if (!isSpace)
				{
					this->file << usedChar;
					isSpace = true;
					usedChar = 0;
				}
			}
		}
	}
	this->file.close();
}

void Own::readHeader()
{
	int w, h, a;
	this->file.read((char *)(&w), sizeof(w));
	this->file.read((char *)(&h), sizeof(h));
	this->file.read((char *)(&a), sizeof(a));
	this->makeSurface(w, h);
}

void Own::writeHeader()
{
	int w = this->width();
	int h = this->height();
	int a = this->algorithm;
	this->file.write((char *)(&w), sizeof(w));
	this->file.write((char *)(&h), sizeof(h));
	this->file.write((char *)(&a), sizeof(a));
}

unsigned char Own::getColorOfPixel(SDL_Color col, int c)
{
	switch (c)
	{
	case 0:
		return col.r;
	case 1:
		return col.g;
	case 2:
		return col.b;
	default:
		return 0;
	}
}

void Own::setColorOfPixel(SDL_Color &pixel, int c, unsigned char value)
{
	switch (c)
	{
	case 0:
		pixel.r = value;
	case 1:
		pixel.g = value;
	case 2:
		pixel.b = value;
	default:
		return;
	}
}