#include "main.h"

static unsigned char decode_ru[256] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//1
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,//2
	16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,//3
	32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,//4
	48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,//5
	64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,//6
	80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,//7
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//8
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//9
	0, 0, 0, 0, 0, 0, 0, 0, 128, 0, 0, 0, 0, 0, 0, 0,//A
	0, 0, 0, 0, 0, 0, 0, 0, 161, 0, 0, 0, 0, 0, 0, 0,//B
	96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,//C
	112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
	129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144,
	145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160,
};
extern unsigned char        pallette_yellow[256];
static unsigned char*		text_color;

static int spacewidth(res::tokens icn)
{
	return res::width(icn, 'i' - 0x20);
}

res::tokens draw::font;

int draw::texth()
{
	return res::height(font, 'I' - 0x20) + 2;
}

int draw::textbc(const char* string, int size, int width)
{
	int i = 0;
	int i1 = 0;
	res::tokens icn = font;
	while(string[i])
	{
		unsigned char ch = (unsigned char)string[i];
		if(ch<=0x20)
		{
			width -= spacewidth(icn);
			if(ch==0x20)
			{
				if(!i || string[i-1]!=0x20)
					i1 = i;
			}
		}
		else
		{
			ch = decode_ru[ch];
			width -= res::width(icn, ch);
		}
		if(width<=0)
		{
			if(i1)
				return i1;
			break;
		}
		i++;
	}
	return i;
}

int draw::textw(const char* string, int count)
{
	int result = 0;
	if(count == -1)
		count = zlen(string);
	res::tokens icn = font;
	while(count)
	{
		unsigned char ch = *string++;
		if(ch<=0x20)
			result += spacewidth(icn);
		else
		{
			ch = decode_ru[ch];
			result += res::width(icn, ch);
		}
		count--;
	}
	return result;
}

void draw::text(int x, int y, const char* string, int count)
{
	if(count == -1)
		count = zlen(string);
	res::tokens icn = font;
	const char* pe = string + count;
	while(string<pe)
	{
		unsigned char ch = (unsigned char)*string++;
		if(ch<=0x20)
			x += spacewidth(icn);
		else
		{
			ch = decode_ru[ch];
			image(x, y, icn, ch, 0, text_color);
			x += res::width(icn, ch);
		}
	}
}

int draw::textm(int x, int y, int width, justify jf, const char* string)
{
	int y1 = y;
	while(string[0])
	{
		int c = textbc(string, -1, width);
		if(!c)
			break;
		text(x, y1, width, jf, string, c);
		y1 += texth();
		string = zskipspcr(string+c);
	}
	return y1 - y;
}

int draw::texth(const char* string, int width)
{
	int result = 0;
	while(string[0])
	{
		int c = textbc(string, -1, width);
		if(!c)
			break;
		result += texth();
		string = zskipspcr(string+c);
	}
	return result;
}

void draw::textm(int x, int y, int width, int height, justify jf, const char* string)
{
	y -= (height - texth(string, width))/2;
	textm(x, y, width, jf, string);
}

void draw::text(int x, int y, int width, draw::justify jf, const char* string, int count)
{
    switch(jf)
    {
    case Left:
        return text(x, y, string, count);
    case Right:
        return text(x+width-textw(string,count), y, string, count);
    case Center:
        return text(x+(width-textw(string,count))/2, y, string, count);
    }
}

void draw::edit(int x, int y, char* value, int maximum)
{
    if(hot::key==KeyBackspace)
    {
        char* p = zend(value);
        if(value!=p)
            p[-1] = 0;
    }
    else if(hot::symbol>=0x20)
    {
        char* p = zend(value);
        p[0] = szupper(hot::symbol);
        p[1] = 0;
        hot::symbol = 0;
    }
    text(x-draw::width/2, y, draw::width, Center, value);
}

void draw::edit(int x, int y, int& value, int maximum, int minimum)
{
    if(hot::key==KeyBackspace)
        value = value/10;
    else if(hot::symbol>='0' && hot::symbol<='9')
        value = value*10 + hot::symbol - '0';
    if(value<minimum)
        value = minimum;
    if(value>maximum)
        value = maximum;
    char temp[32];
    sznum(temp, value);
    text(x-draw::width/2, y, draw::width, Center, temp);
}

draw::fontgr::fontgr(unsigned char* data)
{
	push = text_color;
	text_color = data;
}

draw::fontgr::~fontgr()
{
	text_color = push;
}