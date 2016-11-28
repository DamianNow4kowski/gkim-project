#include "RGB444.h"
#include <sstream>
#include <cstring>
#include <iostream>
#include <bitset>
#include <iomanip>

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

/**
 * Uint8 = char
 */


SDL_Surface *RGB444::loadImpl(const char *file)
{
    unsigned int x, y, w, h;
    Uint32 pixel;
    long pos, size;
    char *data;
    uint8_t *data_ptr, bpp, depth;
    SDL_Surface *surf;

    // TODO: make FileHandler::function for this
    std::ifstream f;
    f.open(file, std::ios::out | std::ios::binary);
    if(!f.is_open()) {
        std::ostringstream os;
        os << "Cannot open file: " << file;
        throw RuntimeError(os.str());
    }

    // Read header data
    this->readHeader(f, w, h, depth, bpp);
    
    // Get current read byte position
    pos = f.tellg();
    
    // Get number of img bytes
    f.seekg(0, f.end);
    size = f.tellg() - pos;
    
    // Go back to img bytes start
    f.seekg(pos);

    // Load whole data img data
    data = new char[size];
    f.read(data, size);
    f.close();
    data_ptr = reinterpret_cast<uint8_t*>(data);

    // Create new SDL Surface
    // TODO: function
    surf = this->makeSurface(w, h, depth);
    /*else {
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            std::cout << "Big endian" << std::endl;
        else
            std::cout << "Little endian" << std::endl;
        std::bitset<32> r(surface->format->Rmask);
        std::bitset<32> g(surface->format->Gmask);
        std::bitset<32> b3(surface->format->Bmask);
        std::bitset<32> a(surface->format->Amask);
        std::cout << std::setw(22) << "Rmask = " << r << std::endl;
        std::cout << std::setw(22) << "Gmask = " << g << std::endl;
        std::cout << std::setw(22) << "Bmask = " << b3 << std::endl;
        std::cout << std::setw(22) << "Amask = " << a << std::endl;
    }*/
    
    // Get pixels one by one
    for(x = 0; x < w; ++x) {
        for(y = 0; y < h; ++y) 
        {
            // Get pixel data in proper form
            pixel = this->getPixel(data_ptr, bpp);

            if(x == w/2 && y < h) {
                this->setPixel(surf, x, y, pixel, true);
            } else {
                this->setPixel(surf, x, y, pixel);
            }

            data_ptr += bpp;
        }
    }

    // Remeber to not acess data_ptr after,
    // becouse it is 4 bytes out of range in memory
    delete data;
    return surf;
}

void RGB444::saveImpl(SDL_Surface *surface, const char *file) 
{
    unsigned int x, y, w, h;
    uint8_t bpp, depth;
    uint32_t pixel;
    std::ofstream f;

    f.open(file, std::ios::out | std::ios::binary);
    if(!f.is_open()) {
        std::ostringstream os;
        os << "Cannot open file: " << file;
        throw RuntimeError(os.str());
    }

    w = static_cast<unsigned int>(surface->w);
    h = static_cast<unsigned int>(surface->h);
    bpp = surface->format->BytesPerPixel;
    depth = surface->format->BitsPerPixel;
    this->writeHeader(f, w, h, depth, bpp);
    

    for(x = 0; x < w; ++x) {
        for(y = 0; y < h; ++y) {
            if(x == w/2 && y < h) {
                std::bitset<32> b(this->getPixel(x,y));
                std::cout << "Pixel[x="  << x << "][y=" << y << "] = " << b << std::endl;
            }
            pixel = this->getPixel(x,y);
            f.write(reinterpret_cast<const char *>(&pixel), bpp);
        }
    }

    f.close();
    std::cout << "---- SAVED ----" << std::endl;
    
}

const char *RGB444::extension() const
{
    return "rgb4";
}

void RGB444::readHeader(std::ifstream &f, unsigned int &w, unsigned int &h, uint8_t &depth, uint8_t &bpp) 
{
    size_t size;
    char *buf;

    // Verify header
    f.read(reinterpret_cast<char *>(&size), sizeof(size));
	if(size <= 0 || size > 1000)
		throw RuntimeError("Cannot load header. Either it is not vaild or too long.");
    buf = new char[size + 1];
    buf[size] = '\0'; // finish cstring
    f.read(buf, size);
    // by.. comparing extension
    if(strcmp(buf, this->extension()) != 0)
        throw RuntimeError("Cannot load file due to not vaild header format.");
    delete buf;

    // Load dimensions
    f.read(reinterpret_cast<char *>(&w), sizeof(w));
    f.read(reinterpret_cast<char *>(&h), sizeof(h));
    f.read(reinterpret_cast<char *>(&depth), sizeof(depth));
    f.read(reinterpret_cast<char *>(&bpp), sizeof(bpp));

    std::cout << "Header Details:" << std::endl;
	std::cout << "- Width: " << w << std::endl;
    std::cout << "- Height: " << h << std::endl;
    std::cout << "- Depth: " << (int)depth << std::endl;
    std::cout << "- BPP: " << (int)bpp << std::endl;
}

void RGB444::writeHeader(std::ofstream &f, unsigned int &w, unsigned int &h, uint8_t &depth, uint8_t &bpp) 
{
    size_t ext_size;
    ext_size = strlen(this->extension());

    /**
     * Saving..
     * @var size_t the size of extension cstring
     * @var char* extension cstring
     * @var unsigned int width
     * @var unsigned int height
     * @var unsigned int bytes per pixel
     */
    f.write(reinterpret_cast<const char *>(&ext_size), sizeof(ext_size));
    f.write(this->extension(), ext_size);
    f.write(reinterpret_cast<const char *>(&w), sizeof(w));
    f.write(reinterpret_cast<const char *>(&h), sizeof(h));
    f.write(reinterpret_cast<const char *>(&depth), sizeof(depth));
	f.write(reinterpret_cast<const char *>(&bpp), sizeof(bpp));
}