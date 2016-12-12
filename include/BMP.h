#pragma once
#include "ImageHandler.h"
class BMP : public ImageHandler
{
protected:
	virtual void store(const std::string &, const Image &) const override;
	virtual Image recover(const std::string &) override;
public:
	virtual std::string extension() const override;
	BMP();
	BMP(const BMP &);
	BMP(BMP &&);
	BMP& operator=(const BMP &);
	BMP& operator=(BMP &&);
	virtual ~BMP();
};

