#include "main.h"

const int FOneTime = 0x00000080;
const int FDisabled = 0x00000100;
const int FComputer = 0x00000200;

static struct event_t
{
	int				index;
	unsigned		flags;
	const char*		text;
	struct cost		award;
} objects[LastEvent-FirstEvent+1];

static int object_get(int rec, int id)
{
	switch(id)
	{
	case OneTime:
		return (objects[rec].flags&FOneTime) ? 1 : 0;
	case PlayerBlue:
	case PlayerGreen:
	case PlayerRed:
	case PlayerOrange:
	case PlayerPurple:
	case PlayerYellow:
		return (objects[rec].flags&(1<<(id-FirstPlayer))) ? 1 : 0;
	default:
		return objects[rec].award.get(id);
	}
}

static void object_set(int rec, int id, int value)
{
	switch(id)
	{
	case OneTime:
		if(value==1)
			objects[rec].flags |= FOneTime;
		else
			objects[rec].flags &= ~FOneTime;
		break;
	case Computer:
		if(value==1)
			objects[rec].flags |= FComputer;
		else
			objects[rec].flags &= ~FComputer;
		break;
	default:
		if(id>=(int)FirstPlayer && id<=(int)LastPlayer)
		{
			if(value==1)
				objects[rec].flags |= (1<<(id-FirstPlayer));
			else
				objects[rec].flags &= ~(1<<(id-FirstPlayer));
		}
		else
			objects[rec].award.set(id, value);
		break;
	}
}
static void object_sets(int rec, int id, const char* value)
{
	switch(id)
	{
	case Name:
		objects[rec].text = szdup(value);
		break;
	}
}