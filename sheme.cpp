#include "main.h"

unsigned char	sheme::red_path[256];
unsigned char	sheme::gray_font[256];

static void init(unsigned char data[256])
{
	for(int i=0;i<256;i++)
		data[i] = i;
}

void sheme::initialize()
{
	// red path
	init(red_path);
	red_path[0x55] = 0xB0;
	red_path[0x5C] = 0xB7;
	red_path[0x60] = 0xBB;
	// gray font
	init(gray_font);
	int j = 0x0A;
	for(int i=j; i<j+11; i++)
		gray_font[i] = i+5;
}