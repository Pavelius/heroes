#include "main.h"

struct mapobject : public drawable
{

	tokens			res;
	short unsigned	frame;
	short unsigned	index;
	short unsigned	count;

	point getpos() const
	{
		auto x = map::i2x(index) * 32;
		auto y = map::i2y(index) * 32;
		return{(short)x, (short)y};
	}

	rect getrect() const override
	{
		auto pt = getpos();
		return{pt.x, pt.y, pt.x + 32, pt.y + 32};
	}

	point getzpos() const override
	{
		return getpos();
	}

	void painting(point position) const override
	{
		auto pt = getpos();
	}

};
static mapobject objects[LastMapObject - FirstMapObject + 1];
static bsmeta::field fields[] = {
	BSREQ(mapobject, res, Type, Number),
	BSREQ(mapobject, frame, Frame, Number),
	BSREQ(mapobject, index, Index, Number),
	BSREQ(mapobject, count, Count, Number),
	{0}
};
BSMETA(mapobject, "Objects", "Îáúåêòû", FirstMapObject);

static struct mapobject_drawable_plugin : public drawable::plugin
{
	void selecting(drawable** result, rect screen, unsigned flags) override
	{
		if((flags & (DFGround|DFHightObjects)) == 0)
			return;
		auto p = result;
		for(auto& e : objects)
		{
			if(!e.res)
				break;
			*p++ = &e;
		}
		*p = 0;
	}
} drawable_plugin;