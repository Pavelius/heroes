#include "main.h"

draw::screenshoot::screenshoot()
{
	this->bits = new unsigned char[width*height];
	memcpy(this->bits, draw::ptr(0,0), width*height);
}

draw::screenshoot::~screenshoot()
{
	delete this->bits;
	this->bits = 0;
}

void draw::screenshoot::restore()
{
	memcpy(draw::ptr(0,0), this->bits, width*height);
}

void draw::screenshoot::redraw(drawable** objects, unsigned timeout)
{
	restore();
	dworder(objects, zlen(objects));
	dwpaint(objects, {0, 0, draw::width, draw::height}, {0, 0});
	draw::input(false);
	sleep(timeout);
}

void draw::screenshoot::redraw(drawable** objects, unsigned timeout, drawable* e1, int stop)
{
	auto a1 = static_cast<animation*>(e1);
	if(!a1->count)
		return;
	while(true)
	{
		redraw(objects, timeout);
		if(a1->incframe())
			break;
		if(stop && a1->frame >= stop)
			break;
	}
}

void draw::screenshoot::redraw(drawable** objects, unsigned timeout, drawable* e1, drawable* e2)
{
	auto a1 = static_cast<animation*>(e1);
	auto a2 = static_cast<animation*>(e2);
	bool a1_run = true;
	bool a2_run = true;
	while(a1_run || a2_run)
	{
		redraw(objects, timeout);
		if(a1->incframe())
			a1_run = false;
		if(a2->incframe())
			a2_run = false;
	}
}