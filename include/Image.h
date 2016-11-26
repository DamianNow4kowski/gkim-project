#ifndef IMAGE_H
#define IMAGE_H

#include "SDL.h"
#include "RuntimeError.h"
#include <fstream>

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
  virtual void saveImpl(SDL_Surface *, const char *) = 0;
  void makeSurface(int w, int h);
  void writeHeader(std::ofstream &);
  void readHeader(std::ifstream &, unsigned int &, unsigned int &, unsigned int &);

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
  void save(const char *);
  void preview();
  bool initialized();
  SDL_Surface *getSurface(const SDL_PixelFormat* = nullptr);
  SDL_Surface *getSurface(Uint32);  
  Uint32 getPixel(const unsigned int &x, const unsigned int &y) const;
  SDL_Color getPixelColorRGB(const int &x, const int &y) const;
  void setPixel(const unsigned int &x, const unsigned int &y, Uint8 R, Uint8 G, Uint8 B);
  void convertToGreyScale();
  unsigned int width() const;
  unsigned int height() const;
  unsigned int size() const;
  
  /**
   * @unsafe
   * Only for tests
   */
  SDL_Surface* img();
};

#endif // !IMAGE
