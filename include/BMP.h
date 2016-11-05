#ifndef BMP_H
#define BMP_H

#include "Image.h"

class BMP : public Image
{
  protected:
    SDL_Surface *loadImpl(const char *);

  public:
    BMP();
    BMP(const char *);
};

#endif // !BMP_H