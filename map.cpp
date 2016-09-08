#include "main.h"

const int		viewsize = 14;
unsigned char	map::width;
unsigned char	map::height;
unsigned short	map::show::tiles[144*144];
unsigned char	map::show::flags[144*144];
unsigned		map::show::route[144*144];
point			map::camera;

int	map::moveto(int index, map::directions direction)
{
	if(index==-1)
		return -1;
	int x = i2x(index);
	int y = i2y(index);
	switch(direction)
	{
	case map::LeftUp:
		y--;
	case map::Left:
		x--;
		break;
	case map::RightDown:
		y++;
	case map::Right:
		x++;
		break;
	case map::RightUp:
		x++;
	case map::Up:
		y--;
		break;
	case map::LeftDown:
		x--;
	case map::Down:
		y++;
		break;
	default:
		return -1;
	}
	if(x<0 || x>=width || y<0 || y>=height)
		return -1;
	return m2i(x,y);
}

static void setcamera(int x, int y)
{
	if(x<0)
		x = 0;
	if(y<0)
		y = 0;
	if(x>map::width-viewsize)
		x = map::width - viewsize;
	if(y>map::height-viewsize)
		y = map::height-viewsize;
	map::camera.x = x * 32;
	map::camera.y = y * 32;
}

void map::jumpto(int index)
{
	setcamera(i2x(index) - viewsize /2, i2y(index) - viewsize /2);
}

void map::slide(map::directions type)
{
	int i = moveto(m2i(camera.x/32, camera.y/32), type);
	if(i==-1)
		return;
	setcamera(i2x(i), i2y(i));
}

tokens map::gettile(int index)
{
	index = map::show::tiles[index];
	if(30 > index) // 30 indexes
		return Water;
	else if(92 > index) // 62 indexes
		return Grass;
	else if(146 > index) // 54
		return Snow;
	else if(208 > index) // 62
		return Swamp;
	else if(262 > index) // 44
		return Lava;
	else if(321 > index)
		return Desert;
	else if(361 > index)
		return Dirt;
	else if(415 > index)
		return Wastelands;
	return Beach;
}

bool map::is(int index, bool(*callback)(unsigned char object, unsigned char index, unsigned char param), unsigned char param)
{
	//for(auto e : map::show::objects[index])
	//{
	//	if(callback(e[0], e[1], param))
	//		return true;
	//}
	return false;
}