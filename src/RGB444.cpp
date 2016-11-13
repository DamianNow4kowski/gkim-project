#include "RGB444.h"

RGB444::RGB444()
    : Image() {}
RGB444::RGB444(SDL_Surface *surface)
    : Image(surface) {}
RGB444::RGB444(const char *file)
    : Image(file) {}
/**
 * @TODO
RGB444::RGB444(BMP *img) {
   BMPEncoder *encoder = new BMPEncoder(img);
   encoder->run();
   this->image = encoder->img();
   delete encoder;
}*/

SDL_Surface *RGB444::loadImpl(const char *file)
{
    throw RuntimeError("Not implemeted yet");
}

const char *RGB444::extension() const
{
    return "cst";
}