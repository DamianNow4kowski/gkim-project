#include "RGB444.h"
#include <sstream>
#include <cstring>
#include <iostream>
#include <bitset>

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
    //Uint32 rmask,gmask,bmask,amask;
    unsigned int w, h, bpp;

    std::ifstream f;
    f.open(file, std::ios::out | std::ios::binary);
    if(!f.is_open()) {
        std::ostringstream os;
        os << "Cannot open file: " << file;
        throw RuntimeError(os.str());
    }


    this->readHeader(f, w, h, bpp);
    std::cout << "Loaded data:\n";
    std::cout << "Width: " << w << std::endl;
    std::cout << "Height: " << h << std::endl;
    std::cout << "BPP: " << bpp << std::endl;
    
    long pos = f.tellg();
    f.seekg(0, f.end);
    long size = f.tellg() - pos;
    f.seekg(pos);
    
    char *data = new char[size];
    f.read(data, size);
    f.close();
    
    Uint32 *pixel = reinterpret_cast<Uint32 *>(data);


    std::cout << "OK after rendering\n";

    // testing pixels
    for(int i = 0; i < 100; ++i) {
        std::bitset<32> b(pixel[i]); 
        std::cout << "Pixel nr " << i << " - " << b << std::endl;
    }

    return nullptr;

    // Test czy tyle samo
    //pos = f.tellg();
    //f.seekg(0, f.end);
    //size = f.tellg() - pos;
    /*

    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
    #else // little endian, like x86
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
    #endif
    depth = 32;
    pitch = 4*w;
    SDL_Surface *surf;
    surf = SDL_CreateRGBSurfaceFrom((void*)data, w, h, depth, pitch, rmask, gmask, bmask, amask);
    return surf;*/

}

void RGB444::saveImpl(SDL_Surface *surface, const char *file) 
{
    unsigned int x, y, bpp;
    Uint32 pixel;
    std::ofstream f;

    f.open(file, std::ios::out | std::ios::binary);
    if(!f.is_open()) {
        std::ostringstream os;
        os << "Cannot open file: " << file;
        throw RuntimeError(os.str());
    }

    this->writeHeader(f);
    bpp = surface->format->BytesPerPixel;
    for(x = 0; x < this->width(); ++x) {
        for(y = 0; y < this->height(); ++y) {
            if(x == 0 && y < 100) {
                std::bitset<32> b(this->getPixel(x,y));
                std::cout << "Pixel nr "  << y << " - " << b << std::endl;
            }
            pixel = this->getPixel(x,y);
            f.write(reinterpret_cast<const char *>(&pixel), sizeof(pixel));
        }
    }

    f.close();
    
}

const char *RGB444::extension() const
{
    return "rgb4";
}