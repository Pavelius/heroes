#include "main.h"

static struct player_t
{
	int		race;
	int		type;
	cost	resources;
} objects[LastPlayer-FirstPlayer+1];

void player::clear()
{
	memset(objects, 0, sizeof(objects));
}

int player::get(int rec, int id)
{
    switch(id)
    {
    case Gold:
		return objects[rec-FirstPlayer].resources.gold;
	case Ore:
		return objects[rec-FirstPlayer].resources.ore;
	case Wood:
		return objects[rec-FirstPlayer].resources.wood;
	case Mercury:
		return objects[rec-FirstPlayer].resources.mercury;
	case Gems:
		return objects[rec-FirstPlayer].resources.gems;
	case Crystal:
		return objects[rec-FirstPlayer].resources.crystal;
	case Sulfur:
		return objects[rec-FirstPlayer].resources.sulfur;
	case Type:
		return objects[rec-FirstPlayer].race;
	case PlayerType:
		return objects[rec-FirstPlayer].type;
	case PlayerNeutral:
		return rec==PlayerNeutral;
	case Valid:
		return objects[rec-FirstPlayer].race!=0 ? 1 : 0;
    default:
        return 0;
    }
}

static int any_free_hero()
{
	int temp[LastHero-FirstHero+2];
	bsselect(temp, FirstHero, LastHero);
	zshuffle(temp, zlen(temp));
	for(int* p = temp; *p; p++)
	{
		if(bsget(*p, Recruit))
			continue;
		if(bsget(*p, Player)!=PlayerNeutral)
			continue;
		return *p;
	}
	return 0;
}

void player::set(int rec, int id, int value)
{
    switch(id)
    {
    case Recruit:
		if(value==-1)
		{
			int i1 = any_free_hero();
			bsset(i1, Recruit, rec);
			int i2 = any_free_hero();
			bsset(i2, Recruit, rec);
		}
        break;
	case Type:
		objects[rec-FirstPlayer].race = value;
		break;
	case PlayerType:
		objects[rec-FirstPlayer].type = value;
		break;
	case Gold:
		objects[rec-FirstPlayer].resources.gold = value;
		break;
	case Ore:
		objects[rec-FirstPlayer].resources.ore = value;
		break;
	case Wood:
		objects[rec-FirstPlayer].resources.wood = value;
		break;
	case Mercury:
		objects[rec-FirstPlayer].resources.mercury = value;
		break;
	case Gems:
		objects[rec-FirstPlayer].resources.gems = value;
		break;
	case Crystal:
		objects[rec-FirstPlayer].resources.crystal = value;
		break;
	case Sulfur:
		objects[rec-FirstPlayer].resources.sulfur = value;
		break;
    }
}

void player::add(int rec, int id, int value)
{
	set(rec, id, get(rec, id) + value);
}

cost& player::gcost(int rec)
{
	return objects[rec-FirstPlayer].resources;
}