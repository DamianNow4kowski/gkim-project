#ifndef BMP_H
#define BMP_H

#include "Image.h"

class BMP : public Image
{
protected:
  SDL_Surface *loadImpl(const char *);

public:
  BMP();
  BMP(SDL_Surface *);
  BMP(const char *);
  const char *extension() const;
};

#endif // !BMP_H