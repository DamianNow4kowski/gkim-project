#ifndef LZ77_H
#define LZ77_H
#include "Image.h"
#include <map>

class LZ77
{
private:
	Image *image;
	unsigned int s_buff_size; 			 //size of search buffer
	unsigned int la_buff_size;			 //size of lookahead buffer
	std::map <int, int> s_buff;          //search buffer
	std::map <int, int> la_buff;		 //lookahead buffer
	
public:
	LZ77(Image *image);
	~LZ77();
	void encode();
	void decode();
};

#endif // !LZ77_H
