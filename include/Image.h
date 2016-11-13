#ifndef IMAGE_H
#define IMAGE_H

#include "SDL.h"
#include "RuntimeError.h"

class Image
{
private:
  SDL_Surface *surface;
  unsigned int h, w;
  void free();
  SDL_Texture *texturize(SDL_Renderer *);

protected:
  /**
   * Custom implamentation of loading image file
   * @param cstring filename of wanted image
   * @return SDL_Surface* containing loaded image
   */
  virtual SDL_Surface *loadImpl(const char *) = 0;

public:
  // Construct
  Image();
  Image(SDL_Surface *);
  Image(const char *);

  // Virtual desctruct due to abstraction of this class
  virtual ~Image();

  /**
   * @return accepted file extension
   */
  virtual const char *extension() const = 0;

  // Functions
  void init(SDL_Surface *);
  void load(const char *, bool = true);
  Uint32 getPixel(const int &x, const int &y) const;
  unsigned int width() const;
  unsigned int height() const;
  unsigned int size() const;
  bool initialized();
  void preview();
};

#endif // !IMAGE
