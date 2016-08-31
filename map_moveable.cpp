#include "main.h"

struct moveable_t
{
    int			        id;
    int			        index;
    unsigned char       extension;
    unsigned char       variant;
    short unsigned      count;
};
static moveable_t		objects[LastMoveable-FirstMoveable+1];

void map::moveable::load(int* rec, int index, int w, int h)
{
    int x1 = map::i2x(index);
    int y1 = map::i2y(index);
    int x2 = x1 + w;
    int y2 = y1 + h;
    for(auto& e : objects)
    {
        if(!e.id)
            break;
        if(e.index==-1)
            continue;
        if(e.index<index)
            continue;
        int x = map::i2x(e.index);
        int y = map::i2y(e.index);
        if(x>=x1 && x<=x2 && y>=y1 && y<=y2)
        {
            int j = (y-y1)*w + (x-x1);
            rec[j] = e.id;
        }
    }
}

void map::moveable::block(unsigned* route)
{
    for(auto& e : objects)
    {
        if(!e.id)
            break;
        if(e.index==-1)
            continue;
        if(e.id>=(int)FirstMonster && e.id<=(int)LastMonster)
        {
            route[e.index] = map::Attack;
            map::route::around(e.index, map::Attack);
        }
        else if(e.id==TreasureChest
                || (e.id>=(int)FirstResource && e.id<=(int)LastResource))
            route[e.index] = map::Action;
    }
}

static int object_get(int rec, int id)
{
    auto& e = objects[rec-FirstMoveable];
    switch(id)
    {
    case Position:
        return e.index;
    case Type:
        return e.id;
    case Valid:
        return e.id ? 1 : 0;
	case Count:
		return e.count;
    default:
        return 0;
    }
}

static void object_set(int rec, int id, int value)
{
    auto& e = objects[rec-FirstMoveable];
    switch(id)
    {
    case Position:
        e.index = value;
        break;
    case Type:
        e.id = value;
        break;
    case Count:
        e.count = value;
        break;
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
		if(!e.id)
			break;
		if(e.index==-1)
			continue;
		if(e.id<id1 && e.id>id2)
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
	bsset(rec, Position, -1);
}