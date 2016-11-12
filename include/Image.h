#ifndef IMAGE_H
#define IMAGE_H

#include "SDL.h"

class Image
{
private:
  SDL_Surface *surface;
  unsigned int h, w;
  void init(SDL_Surface *);
  void free();

protected:
  /**
   * Custom implamentation of loading image file
   * @param cstring filename of wanted image
   * @return SDL_Surface* containing loaded image
   */
  virtual SDL_Surface *loadImpl(const char *) = 0;

public:
  Image();
  Image(const char *);
  virtual ~Image();

  void load(const char *);
  Uint32 getPixel(const int &x, const int &y) const;
  unsigned int width() const;
  unsigned int height() const;
  unsigned int size() const;
  bool initialized();
};

#endif // !IMAGE