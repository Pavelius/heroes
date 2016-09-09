#include "main.h"

struct mapobject : public drawable
{

	res::tokens		icn;
	short unsigned	frame;
	short unsigned	index;
	short unsigned	count;

	int getid() const override;

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

	void painting(point camera, unsigned flags) const override
	{
		auto pt = getpos() - camera;
		auto nframe = indexes::animate(icn, frame, draw::counter + index*index, false);
		draw::image(pt.x, pt.y, icn, frame);
		if(nframe)
			draw::image(pt.x, pt.y, icn, nframe);
	}

	int priority() const override
	{
		return getid();
	}

};
static mapobject objects[LastMapObject - FirstMapObject + 1];
static bsmeta::field fields[] = {
	BSREQ(mapobject, icn, Type, Number),
	BSREQ(mapobject, frame, Frame, Number),
	BSREQ(mapobject, index, Index, Number),
	BSREQ(mapobject, count, Count, Number),
	{0}
};
BSMETA(mapobject, "Objects", "Îáúåêòû", FirstMapObject);

int mapobject::getid() const
{
	return FirstMapObject + (this - objects);
}

static struct mapobject_drawable_plugin : public drawable::plugin
{
	void selecting(drawable** result, rect screen, unsigned flags) override
	{
		auto mode = flags&DWMask;
		if(mode!=DWHightObjects && mode!=DWObjects)
			return;
		auto p = result;
		for(auto& e : objects)
		{
			if(!e.icn)
				break;
			if(mode==DWHightObjects)
			{
				if(!res::ishight(e.icn, e.frame))
					continue;
			}
			else
			{
				if(res::ishight(e.icn, e.frame))
					continue;
			}
			if(!e.getrect().intersect(screen))
				continue;
			*p++ = &e;
		}
		*p = 0;
	}
} drawable_plugin;