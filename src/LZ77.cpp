#include "LZ77.h"
#include "RuntimeError.h"
#include <iostream>
using namespace std;

LZ77::LZ77()
{
	//(changing require to change code //size of search buffer
	s_buff_size = 16;
	
	// size of lookahead buffer
	la_buff_size = 20;
	
	if (la_buff_size < 10)
		throw RuntimeError("Too small size of lookahead buffer.");
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
	//pozniej sie podzieli na funkcje, teraz latwiej mi pisac ciagiem
	//pozniej posprzatam, zakomentowane rzeczy odkomentowuje do testow

	unsigned int i, it = 0, it2 = 0;			//iterators
	unsigned int x = 1, y = 0; 					//coordinates of pixels

	int length = 0, max, cpmax = 0, position = 0; //variables to search the longest sequence

	bool end_of_picture = 0, if_max_length_stop = 0;

	SDL_Color clr = image.getPixelColor(0, 0);

	short what_color = 1;						//which subpixel

	Uint8 color[3], code;

	color[0] = clr.r >> 4;
	code = color[0];
	ofile << code;							//the first color goes to file	
	color[1] = clr.g >> 4;
	color[2] = clr.b >> 4;

	map<int, int>::iterator la_begin;
	map<int, int>::iterator s_begin;
	map<int, int>::iterator count;
	map<int, int>::iterator temp;
	int petla = -1;//do testow, pozniej usune

				   //initialization of search buffer 	
	for (i = 0; i < s_buff_size; ++i)
		s_buff.insert(pair<int, int>(i, color[0]));

	map<int, int>::iterator s_end = s_buff.end();

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

	while (la_buff.size() > 0 && end_of_picture == 0)
	{
		petla++;

		//loading data to la_buff 
		it2 = it - cpmax;
		//if (petla<327)
		while (la_buff.size() < la_buff_size && end_of_picture == 0)
		{
			//cout<<"x: "<<x<<endl;
			//cout<<"y: "<<y<<endl;
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

		//preview of la_buff
		/*//if(petla<327)
		{
		cout<<endl<<"Rozmiar: "<<la_buff.size()<<" liczby: ";
		for (map<int, int>::iterator k=la_buff.begin(); k!=la_buff.end(); ++k)
		cout<<k->second<<" ";
		cout<<endl;
		}*/

		la_begin = la_buff.begin();
		s_begin = s_buff.begin();

		//searching the longest sequence
		max = 1;
		for (map<int, int>::iterator k = s_end; k != s_begin;)
		{
			--k;
			count = k;
			temp = la_begin;
			while (1)
			{
				if (count != s_end)
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

		//if (petla<327)
		//{
		//	cout<<endl<<"code: ";		
		//creating code of subpixels
		if (max == 1)
		{
			code = la_begin->second;
			ofile << code;
			cout << (int)code << " ";
		}
		else
		{
			code = 128 | (max - 2) << 4 | (position - s_begin->first);
			ofile << code;
			cout << (int)code << " ";
		}

		//	if (petla<327)
		//	cout<<"max dlugosc: "<<max<<endl;
		cpmax = max;

		//inserting elements into search buffer
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
			++s_begin; // [ERROR]
			max--;
		}
		max = cpmax;

		//cout<<"petla: "<<petla;	

		//preview of s_buff
		/*//if (petla<327)
		{
		cout<<endl;
		for (map<int, int>::iterator k=s_begin; k!=s_end; ++k)
		cout<<k->second<<" ";
		}*/

		//erase elements in la_buffer
		la_begin = la_buff.begin();
		while (max-- > 0)
		{
			la_buff.erase(la_begin->first);
			++la_begin;
		}

	}
	s_buff.clear();
}


/**
 * @param opened input stream
 * @param allocated empty Image with proper width/etc
 */
void LZ77::decode(std::ifstream &ifile, Image &image)
{
	unsigned int i, iter = 0;
	short length, position, it;
	//unsigned int x=1, y=0; 				//coordinates of pixels
	Uint8 color[3], code, first_bit;
	//short what_color=1;					//which subpixel
	ifile >> code;
	std::cout << "Code = " << code << std::endl;
	color[0] = code << 4;
	//cout<<(int)color[0];
	map<int, int>::iterator la_begin;
	map<int, int>::iterator s_begin;


	//initialization of search buffer
	for (i = 0; i < s_buff_size; ++i)
	{
		s_buff.insert(pair<int, int>(i, (int)code));
		//cout<<s_buff[i]<<" ";
	}

	while (!ifile.eof())
	{
		ifile >> code;
		//cout<<(int)code<<" ";

		length = 0;
		la_begin = la_buff.begin();
		s_begin = s_buff.begin();
		first_bit = code >> 7;
		//cout<<(int)first_bit<<" ";
		if (first_bit)
		{
			length = ((int)code >> 4) - 8;
			position = (int)code - 128 - (length << 4);
			length += 2;
			it = 0;
			//cout<<s_buff[s_begin->first+position+it]<<" ";
			while (it++ < length)
				la_buff.insert(pair<int, int>(la_begin->first + it, s_buff[s_begin->first + position + it]));
			//cout<<s_buff[s_begin->first+position+it]<<" ";	
			--length;
		}
		else
		{
			code = code << 4;
			la_buff.insert(pair<int, int>(la_begin->first, (int)code));
			length = 1;
		}


		while (length-- > 0)
		{
			//cout<<"usuwanie ";
			//cout<<iter<<" ";
			la_buff.erase(iter++);
			la_begin++;
		}

	}
	//cout<<la_buff.size();
	s_buff.clear();
}
