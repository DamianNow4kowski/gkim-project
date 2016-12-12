#ifndef LZ77_H
#define LZ77_H
#include "Image.h"
#include <map>
#include <fstream>

class LZ77
{
private:
	unsigned int s_buff_size; 			 //size of search buffer
	unsigned int la_buff_size;			 //size of lookahead buffer
	std::map <int, int> s_buff;          //search buffer
	std::map <int, int> la_buff;		 //lookahead buffer
	
public:
	LZ77();
	~LZ77();
	void encode(std::ofstream&, const Image &);
	void decode(std::ifstream&, Image &);
};

#endif // !LZ77_H
