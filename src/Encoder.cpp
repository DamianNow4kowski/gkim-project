#include "Encoder.h"

/**
 * CONSTRUCT
 */
Encoder::Encoder()
    : image(nullptr) {}

Encoder::Encoder(Image *img)
{
    this->init(img);
}

Encoder::Encoder(Image &img)
{
    this->init(img);
}

/**
 * DESCTRUCT
 */

Encoder::~Encoder()
{
    this->free();
}

/**
 * PROTECTED
 */
void Encoder::free() {}

/**
 * PUBLIC
 */
void Encoder::init(Image *img)
{
    this->image = img;
}

void Encoder::init(Image &img)
{
    this->image = &img;
}

const Image *Encoder::img()
{
    return this->image;
}