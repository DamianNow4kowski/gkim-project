#ifndef IMAGE_H
#define IMAGE_H

#include "SDL.h"

class Image
{
private:
  SDL_Surface *image;
  void init(SDL_Surface *);
  void free();

protected:
  virtual SDL_Surface *loadImpl(const char *)
  {
    throw RuntimeError();
  }

public:
  Image();
  Image(const char *);
  ~Image();

  bool load(const char *);
  int width() const;
  int height() const;
};

#endif // !IMAGE