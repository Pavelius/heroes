#include "main.h"

static const unsigned char monster_animation_cicle[] =
{
	1, 2, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	3, 4, 5, 4, 3, 0,
	0, 0, 0, 0,
};
static const unsigned char monster_animation_cicl1[] =
{
	1, 2, 1, 0,
	3, 4, 5, 4, 3, 0,
};

struct map_moveable : public drawable
{
	short unsigned      type;
	short unsigned      index;
    unsigned char       extension;
    unsigned char       variant;
    short unsigned      count;

	point getpos() const
	{
		auto x = map::i2x(index) * 32;
		auto y = map::i2y(index) * 32;
		if(type >= FirstMonster && type <= LastMonster)
		{
			x += 16;
			y += 30;
		}
		return{(short)x, (short)y};
	}

	// Fast clipping rect. For hit test use othe method.
	rect getrect() const override
	{
		auto pt = getpos();
		return{pt.x - 32, pt.y-32, pt.x + 32, pt.y + 32};
	}

	point getzpos() const override
	{
		return getpos();
	}

	void painting(point camera, unsigned flags) const override
	{
		auto pt = getpos() - camera;
		if(type >= FirstResource && type <= LastResource)
		{
			draw::image(pt.x - 32, pt.y, res::OBJNRSRC, (type - FirstResource) * 2);
			draw::image(pt.x, pt.y, res::OBJNRSRC, (type - FirstResource) * 2 + 1);
		}
		else if(type >= FirstMonster && type <= LastMonster)
		{
			if(type >= MonsterRnd1)
				draw::image(pt.x, pt.y, res::MONS32, type - FirstMonster);
			else
			{
				draw::image(pt.x, pt.y, res::MINIMON, (type - FirstMonster) * 9);
				switch(bsget(type, AnimationType))
				{
				case 1:
					draw::image(pt.x, pt.y, res::MINIMON, (type - FirstMonster) * 9
						+ 1 + monster_animation_cicl1[(draw::counter + index*index) % (sizeof(monster_animation_cicl1) / sizeof(monster_animation_cicl1[0]))]);
					break;
				default:
					draw::image(pt.x, pt.y, res::MINIMON, (type - FirstMonster) * 9
						+ 1 + monster_animation_cicle[(draw::counter + index*index) % (sizeof(monster_animation_cicle) / sizeof(monster_animation_cicle[0]))]);
					break;
				}
			}
		}
		else if(type >= FirstArtifact  && type <= LastArtifact)
		{
			draw::image(pt.x - 32, pt.y, res::OBJNARTI, (type - FirstArtifact) * 2);
			draw::image(pt.x, pt.y, res::OBJNARTI, (type - FirstArtifact) * 2 + 1);
		}
		else if(type == TreasureChest)
		{
			draw::image(pt.x - 32, pt.y, res::OBJNRSRC, 18);
			draw::image(pt.x, pt.y, res::OBJNRSRC, 19);
		}
	}

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

static struct mapmove_drawable_plugin : public drawable::plugin
{
	void selecting(drawable** result, rect screen, unsigned flags) override
	{
		auto mode = flags&DWMask;
		if(mode!=DWObjects)
			return;
		auto p = result;
		for(auto& e : objects)
		{
			if(!e.type)
				continue;
			if(!e.getrect().intersect(screen))
				continue;
			*p++ = &e;
		}
		*p = 0;
	}
} drawable_plugin;