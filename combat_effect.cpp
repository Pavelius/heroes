#include "main.h"

static struct effect_t
{
	int			id;
	int			target;
	int			expire;
} objects[LastEffect-FirstEffect+1];

void effect::clear()
{
	memset(objects, 0, sizeof(objects));
}

int effect::get(int rec, int id)
{
	if(rec<(int)FirstEffect || rec>(int)LastEffect)
		return 0;
	effect_t& e = objects[rec-FirstEffect];
	switch(id)
	{
	case Type:
		return e.id;
	case Target:
		return e.target;
	case Expire:
		return e.expire;
	case Last:
		return LastEffect;
	default:
		return 0;
	}
}

void effect::set(int rec, int id, int value)
{
	if(rec<(int)FirstEffect || rec>(int)LastEffect)
		return;
	effect_t& e = objects[rec-FirstEffect];
	switch(id)
	{
	case Type:
		e.id = value;
		break;
	case Target:
		e.target = value;
		break;
	case Expire:
		e.expire = value;
		break;
	case Clear:
		if(value)
			set(rec, Type, 0);
		break;
	}
}