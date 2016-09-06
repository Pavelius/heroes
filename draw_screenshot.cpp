#include "main.h"

draw::screenshoot::screenshoot()
{
	this->bits = new unsigned char[width*height];
	memcpy(this->bits, draw::ptr(0,0), width*height);
}

draw::screenshoot::~screenshoot()
{
	memcpy(draw::ptr(0, 0), this->bits, width*height);
	delete this->bits;
	this->bits = 0;
}

void draw::screenshoot::restore()
{
	memcpy(draw::ptr(0,0), this->bits, width*height);
}