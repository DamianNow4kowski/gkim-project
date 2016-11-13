#ifndef ENCODER_H
#define ENCODER_H

#include "Image.h"

// Class converting from BMP/RGB444 to Huffman encoded and LZ77 encoded
class Encoder
{
protected:
  Image *image;
  virtual void free();

public:
  Encoder();
  Encoder(Image *);
  Encoder(Image &);
  virtual ~Encoder();

  // Functions
  virtual void run() = 0;
  void init(Image *);
  void init(Image &);
  const Image *img();
};

#endif // !ENCODER_H