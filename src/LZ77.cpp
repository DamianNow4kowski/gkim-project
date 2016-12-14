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
int licz=0;
	unsigned int i, it = 0, it2 = 0;			//iterators
	unsigned int x = 1, y = 0; 					//coordinates of pixels

	int length = 0, max, cpmax = 0, position = 0; //variables to search the longest sequence

	bool end_of_picture = 0, if_max_length_stop = 0;

	SDL_Color clr = image.getPixelColor(0, 0);

	short what_color = 1;						//which subpixel

	Uint8 color[3], code;

	color[0] = clr.r >> 4;
	code = color[0];
	ofile.write(reinterpret_cast<const char*>(&code), sizeof(code));							//the first color goes to file	
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

		// better approach for reverse iterating is:
		//for (auto k = s_buff.rbegin(); k != s_buff.rend(); ++k)
		for(auto k = s_buff.end(); k != s_buff.begin(); --k)
		{
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
			//code=code<<3;
			ofile.write(reinterpret_cast<const char*>(&code), sizeof(code));
			cout << (int)code << " ";
			//licz=licz+1;
		}
		else
		{
			code = 128 | (max - 2) << 4 | (position - s_begin->first);
			ofile.write(reinterpret_cast<const char*>(&code), sizeof(code));
			cout << (int)code << " ";
			//licz=licz+max;
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
			//if (petla<2)
				//cout<<s_begin->first<<" ";
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
	cout<<licz;
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
	unsigned int x=1, y=0; 				//coordinates of pixels
	Uint8 color[3], code, first_bit;
	short what_color=1;					//which subpixel
	ifile.read(reinterpret_cast<char*>(&code), sizeof(code));
	//cout << "Code = " << code << endl;
	color[0] = code << 4;
	//cout<<(int)color[0];
	map<int, int>::iterator la_begin;
	map<int, int>::iterator s_begin;
	int petla=0;

	//initialization of search buffer
	for (i = 0; i < s_buff_size; ++i)
	{
		s_buff.insert(pair<int, int>(i, (int)code));
		//cout<<s_buff[i]<<" ";
	}

	while (!ifile.eof())
	{
		petla++;
		ifile.read(reinterpret_cast<char*>(&code), sizeof(code));
		//cout<<(int)code<<" ";

		length = 0;
		la_begin = la_buff.begin();
		s_begin = s_buff.begin();
		first_bit = code >> 7;
		//if (petla<20)
		//cout<<"b:"<<(int)first_bit<<":b ";
		if (first_bit)
		{
			//cout<<(int)code<<" ";
			length = ((int)code >> 4) - 8;
			position = (int)code - 128 - (length << 4);
			length += 2;
			it = 0;
		
		while (it < length){
				la_buff.insert(pair<int, int>(la_begin->first, s_buff[s_begin->first + position + it]));
//if (petla<5)
//cout<<s_begin->first+position+it<<" ";
		++it;
		//if (petla<3)
			//cout<<"S:"<<la_begin->first<<" ";
}
			//cout<<s_buff[s_begin->first+position+it]<<" ";	
			//--it;
		}
		else
		{
			//cout<<(int)code<<" ";
			code = code << 4;
			la_buff.insert(pair<int, int>(la_begin->first, (int)code));
			length = 1;
		}

		//inserting elements to search buffer
		int cplength=length;
		s_begin=s_buff.begin();
		la_begin=la_buff.begin();
		it=0;
		while (cplength-- > 0)
		{
			s_buff.insert(pair<int, int>(s_begin->first+s_buff_size+it, la_begin->second));
			//cout<<la_begin->second<<" ";
//if (petla<1500)
	//		cout<<s_buff[s_begin->first+s_buff_size+it]<<" ";
			++la_begin;
			++it;
			
		}
		
		//erase elements in search buffer
		cplength = length;
		while (cplength-- > 0)
		{
			s_buff.erase(s_begin->first);
			++s_begin; // [ERROR]
			//if (petla<1533)
				//cout<<s_begin->second<<" ";
		}
		
	

	la_begin=la_buff.begin();
	map<int, int>::iterator la_end=la_buff.end();
//if(petla<3)
	//cout<<endl;
//if(petla<3)
	//for(auto k=la_begin; k!=la_end; k++)
//	cout<<"L:"<<k->first<<":L ";

		la_begin=la_buff.begin();
		while (length-- > 0)
		{
	iter++;
	//if (petla<20) 
		//cout<<"F:"<<la_begin->first<<":F ";

			//---here saving colors to image----			
			if(x>=image.width())
			{
				x=0;
				y++;
			}
			if (what_color>=3)
			{
				if(y<image.height())
				image.setPixel(x, y, color[0], color[1], color[2], 0);
				
				x++;
				what_color=0;
			}
			cout<<la_begin->second<<" ";
			what_color++;
			color[what_color]=la_begin->second;

			la_buff.erase(la_begin);
			la_begin++;

		}
	}

cout<<"iteracji: "<<iter;

	s_buff.clear();
}
