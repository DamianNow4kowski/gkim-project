#ifndef BMP_H
#define BMP_H

#include "ImageHandler.h"
class BMP : public ImageHandler
{
protected:
	virtual void store(const std::string &filename, const Image &image) const override;
	virtual Image recover(const std::string &filename) override;
public:
	virtual std::string extension() const override;
	BMP();
	BMP(const BMP &img);
	BMP(BMP &&img);
	BMP& operator=(const BMP &img);
	BMP& operator=(BMP &&img);
};

#endif // !BMP_H