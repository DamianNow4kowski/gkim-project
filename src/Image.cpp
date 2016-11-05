#include "Image.h"
#include <iostream>

/**
 * PRIVATE
 */
void Image::free()
{
    SDL_FreeSurface(this->image);
}

void Image::init(SDL_Surface *image)
{
    if (this->image != nullptr)
        this->free();

    this->image = image;
}

/**
 * PUBLIC
 */

// construct
Image::Image()
{
    this->image = nullptr;
}

Image::Image(const char *file)
{
    this->init(this->loadImpl(file));
}

// destruct
Image::~Image()
{
    this->free();
}

// functions
bool Image::load(const char *file)
{
    SDL_Surface *temp;
    try
    {
        temp = this->loadImpl(file);
        if (temp == nullptr)
            throw RuntimeError();
    }
    catch (const RuntimeError &err)
    {
        std::cerr << "Error while loading SDL Surface:  " << err.what() << std::endl;
        return false;
    }
    this->init(temp);
    return true;
}

int Image::height() const {
    return (this->image == nullptr) ? 0 : this->image->h;
}

int Image::width() const {
    return (this->image == nullptr) ? 0 : this->image->w;
}