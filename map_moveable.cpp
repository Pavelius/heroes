#include "main.h"

struct map_moveable
{
	short unsigned      type;
	short unsigned      index;
    unsigned char       extension;
    unsigned char       variant;
    short unsigned      count;
};
static map_moveable		objects[LastMoveable-FirstMoveable+1];
static bsmeta::field fields[] = {
	BSREQ(map_moveable, type, Type, Number),
	BSREQ(map_moveable, index, Index, Number),
	BSREQ(map_moveable, variant, Value, Number),
	BSREQ(map_moveable, count, Count, Number),
	{0}
};
BSMETA(map_moveable, "Moveables", "Передвижимое", FirstMoveable);

void map::moveable::load(int* rec, int index, int w, int h)
{
    int x1 = map::i2x(index);
    int y1 = map::i2y(index);
    int x2 = x1 + w;
    int y2 = y1 + h;
    for(auto& e : objects)
    {
        if(!e.type)
            break;
        if(e.index==0xFFFF)
            continue;
        if(e.index<index)
            continue;
        int x = map::i2x(e.index);
        int y = map::i2y(e.index);
        if(x>=x1 && x<=x2 && y>=y1 && y<=y2)
        {
            int j = (y-y1)*w + (x-x1);
            rec[j] = e.type;
        }
    }
}

void map::moveable::block(unsigned* route)
{
    for(auto& e : objects)
    {
        if(!e.type)
            break;
        if(e.index==0xFFFF)
            continue;
        if(e.type>=FirstMonster && e.type<=LastMonster)
        {
            route[e.index] = map::Attack;
            map::route::around(e.index, map::Attack);
        }
        else if(e.type ==TreasureChest
                || (e.type >=FirstResource && e.type <=LastResource))
            route[e.index] = map::Action;
    }
}

int map::moveable::nearest(int index, int id1, int id2, int radius)
{
	int x = map::i2x(index);
	int y = map::i2y(index);
	int x1 = x-radius;
	int x2 = x+radius;
	int y1 = y-radius;
	int y2 = y+radius;
	for(auto& e : objects)
	{
		if(!e.type)
			break;
		if(e.index==0xFFFF)
			continue;
		if(e.type<id1 && e.type>id2)
			continue;
		x = map::i2x(e.index);
		if(x<x1 || x>x2)
			continue;
		y = map::i2y(e.index);
		if(y<y1 || y>y2)
			continue;
		return &e - objects + FirstMoveable;
	}
	return -1;
}

void map::moveable::reaction(int rec, int user)
{
	int id = bsget(rec, Type);
	int ow = bsget(user, Player);
    if(id>=(int)FirstResource && id<=(int)LastResource)
		bsadd(ow, id, bsget(rec, Count));
    else if(id>=(int)FirstArtifact && id<=(int)LastArtifact)
	{

	}
	else if(id==TreasureChest)
	{
	}
    else
		return;
	bsset(rec, Index, -1);
}