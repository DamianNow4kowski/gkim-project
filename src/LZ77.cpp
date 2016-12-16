#include "LZ77.h"
#include "RuntimeError.h"
#include <iostream>
using namespace std;

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

/** 
 * @param opened output stream
 * @param vaild Image to save
 */
void LZ77::encode(std::ofstream &ofile, const Image &image)
{
	//iterators
	unsigned int i, it = 0, it2 = 0;			

	//coordinates of pixels
	unsigned int x = 1, y = 0; 					

	//variables to search the longest sequence
	int length = 0, max, cpmax = 0, position = 0; 	

	//variables to break operations
	bool end_of_picture = 0, if_max_length_stop = 0;  
	
	//loading first pixel to initialization variables
	SDL_Color clr = image.getPixelColor(0, 0);
	
	//variable pointing subpixel
	short what_color = 1;						

	Uint8 color[3], code;   
                  
	color[0] = clr.r >> 4;
	code = color[0];
	color[1] = clr.g >> 4;
	color[2] = clr.b >> 4;

	//saving first subpixel
	ofile.write(reinterpret_cast<const char*>(&code), sizeof(code));
	
	//iterators to work with maps
	map<int, int>::iterator la_begin;
	map<int, int>::iterator s_begin;
	map<int, int>::iterator count;
	map<int, int>::iterator temp;

	//initialization of search buffer 	
	for (i = 0; i < s_buff_size; ++i)
		s_buff.insert(pair<int, int>(i, color[0]));

	//the first loading data to la_buff 
	while (la_buff.size() < la_buff_size && end_of_picture == 0)
	{
		if (what_color < 3)
			la_buff.insert(pair<int, int>(it2++, color[what_color++]));
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
			clr = image.getPixelColor(x++, y);
			what_color = 1;
			color[0] = clr.r >> 4;
			color[1] = clr.g >> 4;
			color[2] = clr.b >> 4;
			la_buff.insert(pair<int, int>(it2++, color[0]));
		}
	}


	//main part of algorithm- loading data and coding
	while (la_buff.size() > 0 && end_of_picture == 0)
	{
		//loading data to la_buff 
		it2 = it - cpmax;
		while (la_buff.size() < la_buff_size && end_of_picture == 0)
		{
			if (what_color < 3)
				la_buff.insert(pair<int, int>(la_buff_size + 1 + it2++, color[what_color++]));
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
				clr = image.getPixelColor(x++, y);
				what_color = 1;
				color[0] = clr.r >> 4;
				color[1] = clr.g >> 4;
				color[2] = clr.b >> 4;
				la_buff.insert(pair<int, int>(la_buff_size + 1 + it2++, color[0]));
			}
		}	

		//searching the longest sequence
		max = 1;
		for(auto k = s_buff.end(); k != s_buff.begin(); --k)
		{
			count = k;
			temp = la_buff.begin();
			while (1)
			{
				if (count != s_buff.end())
					if (count->second == temp->second)
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
			if (max < length)
			{
				max = length;
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
		s_begin = s_buff.begin();
		la_begin = la_buff.begin();
		if (max == 1)
		{
			code = la_begin->second;
			ofile.write(reinterpret_cast<const char*>(&code), sizeof(code));
			cout << (int)code << " ";
		}
		else
		{
			code = 128 | (max - 2) << 4 | (position - s_begin->first);
			ofile.write(reinterpret_cast<const char*>(&code), sizeof(code));
			cout << (int)code << " ";
		}

		//inserting elements into search buffer
		cpmax = max;
		while (max-- > 0)
		{
			s_buff.insert(pair<int, int>(s_buff_size + it, la_begin->second));
			++la_begin;
			++it;
		}

		//erase elements in search buffer
		max = cpmax;
		while (max > 0)
		{
			s_buff.erase(s_begin->first);
			++s_begin;
			max--;
		}

		//erase elements in la_buffer
		max = cpmax;
		la_begin = la_buff.begin();
		while (max-- > 0)
		{
			la_buff.erase(la_begin->first);
			++la_begin;
		}

	}

	la_buff.clear();
	s_buff.clear();
}


/**
 * @param opened input stream
 * @param allocated empty Image with proper width/etc
 */
void LZ77::decode(std::ifstream &ifile, Image &image)
{
	//iterators
	unsigned int i, iter = 0;

	//variables to decoding
	short length, cplength, position, it;
	
	//coordinates of pixels
	unsigned int x=1, y=0; 	
			
	Uint8 color[3], code, first_bit;
	
	//variable pointing subpixel
	short what_color=1;	

	//loading first byte				
	ifile.read(reinterpret_cast<char*>(&code), sizeof(code));
	color[0] = code << 4;

	//iterators to work with maps
	map<int, int>::iterator la_begin;
	map<int, int>::iterator la_end;
	map<int, int>::iterator s_begin;

	//initialization of search buffer
	for (i = 0; i < s_buff_size; ++i)
		s_buff.insert(pair<int, int>(i, (int)code));

	while (!ifile.eof())
	{
		ifile.read(reinterpret_cast<char*>(&code), sizeof(code));
		length = 0;
		la_begin = la_buff.begin();
		s_begin = s_buff.begin();
		first_bit = code >> 7;

		//checking what we have- one subpixel or sequence
		if (first_bit)
		{
			//decoding sequence of subpixels in one byte
			length = ((int)code >> 4) - 8;
			position = (int)code - 128 - (length << 4);
			length += 2;
			it = 0;
		
			while (it < length)
			{
				la_buff.insert(pair<int, int>(la_begin->first, s_buff[s_begin->first + position + it]));
				++it;
			}
		}
		else
		{
			//decoding one subpixel
			code = code << 4;
			la_buff.insert(pair<int, int>(la_begin->first, (int)code));
			length = 1;
		}

		//inserting elements to search buffer
		cplength=length;
		s_begin=s_buff.begin();
		la_begin=la_buff.begin();
		it=0;
		while (cplength-- > 0)
		{
			s_buff.insert(pair<int, int>(s_begin->first+s_buff_size+it, la_begin->second));
			++la_begin;
			++it;
		}
		
		//erasing elements in search buffer
		cplength = length;
		while (cplength-- > 0)
		{
			s_buff.erase(s_begin->first);
			++s_begin; // [ERROR]
				//cout<<s_begin->second<<" ";
		}
		
		//erasing elements from la_buff and saving pixels to image
		la_end=la_buff.end();
		la_begin=la_buff.begin();
		while (length > 0)
		{
			iter++;
		
			if(x>=image.width())
			{
				x=0;
				y++;
			}
			if (what_color>=3)
			{
				if(y<image.height())
					image.setPixel(x, y, color[0], color[1], color[2]);
				x++;
				what_color=0;
			}
			
			color[what_color]=la_begin->second;
			what_color++;
			if(la_begin != la_buff.end())
				la_buff.erase(la_begin); // ERROR VS15
			la_begin++;
			length--;
		}
	}

	la_buff.clear();
	s_buff.clear();
}
