#ifndef RGB444_H
#define RGB444_H

#include <fstream>
#include "Image.h"
//#include "BMP.h"

class RGB444 : public Image
{
  protected:
    SDL_Surface *loadImpl(const char*);
    void saveImpl(SDL_Surface *, const char*);

  public:
    RGB444();
    RGB444(SDL_Surface*);
    RGB444(const char *);
    //RGB44(BMP *);
    const char *extension() const;
};

#endif // !RGB444_H