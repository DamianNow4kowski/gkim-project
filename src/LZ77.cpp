#include "LZ77.h"
#include <iterator>

LZ77::LZ77()
{
	//size of search buffer
	s_buff_size = 16;
	
	// size of lookahead buffer
	la_buff_size = 20;
}

LZ77::~LZ77()
{

}



//------------------------------ENCODING------------------------------

/** 
 * @param opened output stream
 * @param vaild Image to save
 */
void LZ77::encode(std::ofstream &ofile, const Image &image)
{
	std::cout<<"\n=== LZ77 COMPRESSION ==="<<std::endl;
	
	//iterator helping loading data to buffers
	unsigned int it = 0;

	//coordinates of pixels
	unsigned int x = 1, y = 0;

	//variables to search the longest sequence
	short max_length=0; 	
	
	//variables to break operations
	bool end_of_picture = 0;  

	//loading first pixel to initialization variables
	SDL_Color clr = image.getPixelColor(0, 0);						

	//colors of the one pixel
	uint8_t color[3];
	color[0] = clr.r >> 4;
	color[1] = clr.g >> 4;
	color[2] = clr.b >> 4;
		
	//variable pointing the subpixel
	short what_color = 1;

	//saving first subpixel
	ofile.write(reinterpret_cast<const char*>(&color[0]), sizeof(color[0]));

	//initialization of search buffer 	
	for (unsigned int i = 0; i < s_buff_size; ++i)
		s_buff.insert(std::pair<int, uint8_t>(i, color[0]));

	first_loading_la_buff(x, y, what_color, color, image, end_of_picture);

	//main part of algorithm- loading data and coding
	while (la_buff.size() > 0)
	{
		insert_elements_to_la_buff(it, max_length, x, y, what_color, color, image, end_of_picture);
		max_length = create_code(ofile);
		insert_elements_to_s_buff(max_length, it);

		popFront(s_buff, max_length);
		popFront(la_buff, max_length);
	}

	la_buff.clear();
	s_buff.clear();
	std::cout<<"\n=== LZ77 COMPRESSION DONE ==="<<std::endl;
}



/** 
 * @param coordinate x
 * @param coordinate y
 * @param variable pointing the subpixel
 * @param loaded colors of the one pixel
 * @param valid Image to save
 * @param variable to break mechanism in the end of picture (here for very small pictures)
 */
void LZ77::first_loading_la_buff(unsigned int &x, unsigned int &y, short &what_color, uint8_t *color, const Image &image, bool &end_of_picture)
{
	unsigned int it=0;
	//the first loading data to la_buff 
	while (la_buff.size() < la_buff_size && end_of_picture == 0)
	{
		if (what_color < 3)
			la_buff.insert(std::pair<int, uint8_t>(it++, color[what_color++]));
		else
		{
			if (x >= image.width())
			{
				x = 0;
				++y;
				if (y >= image.height())
				{
					end_of_picture = 1;
					break;
				}
			}
			SDL_Color clr = image.getPixelColor(x++, y);
			what_color = 1;
			color[0] = clr.r >> 4;
			color[1] = clr.g >> 4;
			color[2] = clr.b >> 4;
			la_buff.insert(std::pair<int, uint8_t>(it++, color[0]));
		}
	}
}


/** 
 * @param iterator helps loading data
 * @param size of the longest sequence
 * @param coordinate x
 * @param coordinate y
 * @param variable pointing the subpixel
 * @param loaded colors of the one pixel
 * @param vaild Image to save
 * @param variable to break mechanism in the ending point of picture
 */
void LZ77::insert_elements_to_la_buff(unsigned int it, unsigned int max_length, unsigned int &x, unsigned int &y, short &what_color, uint8_t *color, const Image &image, bool &end_of_picture)
{ 
	//loading data to la_buff 
	unsigned int it2 = it - max_length;
	while (la_buff.size() < la_buff_size && end_of_picture == 0)
	{
		if (what_color < 3)
			la_buff.insert(std::pair<int, uint8_t>(la_buff_size + 1 + it2++, color[what_color++]));
		else
		{
			if (x >= image.width())
			{
				x = 0;
				++y;
				if (y >= image.height())
				{
					end_of_picture = 1;
					break;
				}
			}
			SDL_Color clr = image.getPixelColor(x++, y);
			what_color = 1;
			color[0] = clr.r >> 4;
			color[1] = clr.g >> 4;
			color[2] = clr.b >> 4;
			la_buff.insert(std::pair<int, uint8_t>(la_buff_size + 1 + it2++, color[0]));
		}
	}	
}


/** 
 * @param opened output stream
 */
short LZ77::create_code(std::ofstream &ofile)
{
	std::map<int, uint8_t>::iterator 
		s_begin = s_buff.begin(),
		s_end = s_buff.end(),
		la_begin = la_buff.begin(),
		count,
		temp;

	short length = 0, max_length = 1, position = 0;
	bool if_max_length_stop = 0;
		
	//searching the longest sequence	
	for(auto k = s_end; k != s_begin; --k)
	{
		count = k;
		temp = la_begin;
		while (true)
		{
			if (count->second == temp->second)
				if (count != s_end)
				{
					if (length >= 9)
					{
						if_max_length_stop = 1;
						break;
					}
					++count;
					++temp;
					++length;
				}
				else
					break;
			else
				break;
		}
		if (max_length < length)
		{
			max_length = length;
			position = k->first;	
		}
		length = 0;
		if (if_max_length_stop == 1)
		{
			if_max_length_stop = 0;
			break;
		}
	}

	//creating code of subpixels
	if (max_length == 1)
		ofile.write(reinterpret_cast<const char*>(&la_begin->second), 1);
	else
	{
		uint8_t code = 128 | (max_length - 2) << 4 | (position - s_begin->first);
		ofile.write(reinterpret_cast<const char*>(&code), sizeof(code));
	}
	return max_length;
}


/** 
 * @param size of the longest sequence
 * @param iterator pointing the beginning of search buffer
 */
void LZ77::insert_elements_to_s_buff(short max_length, unsigned int &it)
{
	//inserting elements into search buffer
	std::map<int, uint8_t>::iterator la_begin=la_buff.begin();
	while (max_length-- > 0)
	{
		s_buff.insert(std::pair<int, uint8_t>(s_buff_size + it, la_begin->second));
		++la_begin;
		++it;
	}
}

void LZ77::popFront(std::map<int, uint8_t> & buffer, size_t n)
{
	auto end = buffer.begin();
	n = (n < buffer.size()) ? n : buffer.size();		// security condition for debugging
	std::advance(end, n);
	buffer.erase(buffer.begin(), end);
}



//------------------------------DECODING------------------------------

/**
 * @param opened input stream
 * @param allocated empty Image with proper width/etc
 */
void LZ77::decode(std::ifstream &ifile, Image &image)
{
	std::cout<<"\n=== LZ77 DECOMPRESSION ==="<<std::endl;

	//variables to decoding
	short length=0;
	
	//coordinates of pixels
	unsigned int x=0, y=0; 	
		
	//colors of the one pixel
	uint8_t color[3];

	//variable to keep code readed from binary file
	uint8_t code;
	
	//variable pointing subpixel
	short what_color=1;

	//loading first byte				
	ifile.read(reinterpret_cast<char*>(&code), sizeof(code));
	color[0] = code << 4;

	//initialization of search buffer
	for (unsigned int i = 0; i < s_buff_size; ++i)
		s_buff.insert(std::pair<int, uint8_t>(i, color[0]));

	while (!ifile.eof())
	{
		ifile.read(reinterpret_cast<char*>(&code), sizeof(code));
		length=read_code(code);
		put_elements_into_s_buff(length);

		popFront(s_buff, length);

		build_image(x, y, length, what_color, color, image);
	}
	la_buff.clear();
	s_buff.clear();
	std::cout<<"\n=== LZ77 DECOMPRESSION DONE ==="<<std::endl;
}


/** 
 * @param readed code
 */
short LZ77::read_code(uint8_t code)
{	
	std::map<int, uint8_t>::iterator la_begin = la_buff.begin();
	std::map<int, uint8_t>::iterator s_begin = s_buff.begin();
	uint8_t first_bit = code >> 7;
	short length;
	//checking what we have- one subpixel or sequence
	if (first_bit)
	{
		//decoding sequence of subpixels in one byte
		length = ((int)code >> 4) - 8;
		short position = (int)code - 128 - (length << 4);
		length += 2;
		short it = 0;
		while (it < length)
		{
			la_buff.insert(std::pair<int, uint8_t>(la_begin->first, s_buff[s_begin->first + position + it]));
			++it;
		}
	}
	else
	{
		//decoding one subpixel
		code = code << 4;
		la_buff.insert(std::pair<int, uint8_t>(la_begin->first, (int)code));
		length = 1;
	}
	return length;
}


/** 
 * @param length of sequence of subpixels or one subpixel (=1)
 */
void LZ77::put_elements_into_s_buff(short length)
{
	//inserting elements to search buffer
	std::map<int, uint8_t>::iterator s_begin=s_buff.begin();
	std::map<int, uint8_t>::iterator la_begin=la_buff.begin();
	short it=0;
	while (length-- > 0)
	{
		s_buff.insert(std::pair<int, uint8_t>(s_begin->first+s_buff_size+it, la_begin->second));
		++la_begin;
		++it;
	}
}


/** 
 * @param coordinate x
 * @param coordinate y
 * @param length of sequence of subpixels or one subpixel (=1)
 * @param variable pointing the subpixel
 * @param loading colors of the one pixel
 * @param vaild Image to save
 */
void LZ77::build_image(unsigned int &x, unsigned int &y, short &length, short &what_color, uint8_t *color, Image &image)
{
	//erasing elements from la_buff and saving pixels to image
	/*std::map<int, uint8_t>::iterator la_begin=la_buff.begin();
	while (length-- > 0)
	{
		if(x>=image.width())
		{
			x=0;
			++y;
		}
		if (what_color>=3)
		{
			image.setPixel(x, y, color[0], color[1], color[2]);
			++x;
			what_color=0;
		}
		
		color[what_color]=la_begin->second;
		++what_color;
		if(la_begin != la_buff.end())
			la_buff.erase(la_begin);
		++la_begin;
	}*/

	for (auto & la_begin = la_buff.begin(); length > 0; ++la_begin, --length)
	{
		if (x >= image.width())
		{
			x = 0;
			++y;
		}
		if (what_color >= 3)
		{
			image.setPixel(x, y, color[0], color[1], color[2]);
			++x;
			what_color = 0;
		}

		color[what_color] = la_begin->second;
		++what_color;
		
		popFront(la_buff, 1);
		//la_buff.erase(la_begin); // @error tu wywala blad na viusalu
	}
}
