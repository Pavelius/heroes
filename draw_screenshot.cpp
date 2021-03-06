#include "main.h"

extern unsigned char pallette[256][4];

draw::screenshoot::screenshoot()
{
	this->bits = new unsigned char[width*height];
	memcpy(this->bits, draw::ptr(0, 0), width*height);
}

draw::screenshoot::~screenshoot()
{
	memcpy(draw::ptr(0, 0), this->bits, width*height);
	delete this->bits;
	this->bits = 0;
}

void draw::screenshoot::restore()
{
	memcpy(draw::ptr(0, 0), this->bits, width*height);
}

void draw::screenshoot::blend(screenshoot& source)
{
	auto bits = ptr(0, 0);
	auto rgb_lookup = (unsigned char*)res::get(res::BinRGBLOOKP);
	auto rgb_pal = (unsigned char*)res::get(res::PalKB);
	if(!rgb_lookup || !bits)
		return;
	for(int j = 0; j < 16; j++)
	{
		unsigned char a = j * 4;
		for(int i = 0; i < 640 * 480; i++)
		{
			// This will better visual effect and speed
			if(source.bits[i] == this->bits[i])
				continue;
			unsigned char res[4];
			unsigned char* c1 = rgb_pal + source.bits[i]*3;
			unsigned char* c2 = rgb_pal + this->bits[i]*3;
			res[0] = (((int)c1[0] * a + c2[0] * (64 - a))) >> (6 + 1);
			res[1] = (((int)c1[1] * a + c2[1] * (64 - a))) >> (6 + 1);
			res[2] = (((int)c1[2] * a + c2[2] * (64 - a))) >> (6 + 1);
			bits[i] = rgb_lookup[(res[0] << 10) + (res[1] << 5) + res[2]];
		}
		draw::input(false);
		sleep(50);
	}
}