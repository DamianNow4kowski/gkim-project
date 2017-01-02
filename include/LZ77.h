#ifndef LZ77_H
#define LZ77_H
#include "Image.h"
#include "RuntimeError.h"
#include <map>
#include <fstream>
#include <iostream>

class LZ77
{
private:
	// size of search buffer
	unsigned int s_buff_size;

	// size of lookahead buffer
	unsigned int la_buff_size;

	// search buffer
	std::map <int, uint8_t> s_buff;

	// lookahead buffer
	std::map <int, uint8_t> la_buff;

	//encoding functions
	void first_loading_la_buff(unsigned int &x, unsigned int &y, short &what_color, uint8_t *color, const Image &image, bool &end_of_picture);
	void insert_elements_to_la_buff(unsigned int it, unsigned int max_length, unsigned int &x, unsigned int &y, short &what_color, uint8_t *color, const Image &image, bool &end_of_picture);
	short create_code(std::ofstream &ofile);
	void insert_elements_to_s_buff(short max_length, unsigned int &it);

	/**
	 * Deletes n first elements from the map
	 * @param map<int, uint8_t> buffer 
	 * @param size_t n
	 */
	void popFront(std::map<int, uint8_t> &buffer, size_t n);

	//decoding functions
	short read_code(uint8_t code);
	void put_elements_into_s_buff(short length);
	void build_image(unsigned int &x, unsigned int &y, short &length, short &what_color, uint8_t *color, Image &image);

public:
	LZ77();
	~LZ77();
	void encode(std::ofstream&, const Image &);
	void decode(std::ifstream&, Image &);
};

#endif // !LZ77_H
