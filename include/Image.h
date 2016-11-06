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
    // When this virtual methods is not implemented exception is thrown
    throw RuntimeError("Virtual method loadImpl() is not implemented");
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
