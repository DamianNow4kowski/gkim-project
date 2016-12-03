#ifndef BMP_H
#define BMP_H

#include "Image.h"

class BMP : public Image
{
protected:
  SDL_Surface *loadImpl(const char *);
  void saveImpl(SDL_Surface*, const char *);

public:
  BMP();
  BMP(const Image &);
  BMP(const SDL_Surface *);
  BMP(const char *);
  const char *extension() const;
};

#endif // !BMP_H