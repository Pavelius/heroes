#include "main.h"

bool cost::operator==(const cost& e) const
{
	return gold==e.gold
		&& ore==e.ore
		&& wood==e.wood
		&& crystal==e.crystal
		&& gems==e.gems
		&& mercury==e.mercury
		&& sulfur==e.sulfur;
}

bool cost::operator<=(const cost& e) const
{
	return gold<=e.gold
		&& ore<=e.ore
		&& wood<=e.wood
		&& crystal<=e.crystal
		&& gems<=e.gems
		&& mercury<=e.mercury
		&& sulfur<=e.sulfur;
}

void cost::operator-=(const cost& e)
{
	gold -= e.gold;
	ore -= e.ore;
	wood -= e.wood;
	crystal -= e.crystal;
	gems -= e.gems;
	mercury -= e.mercury;
	sulfur -= e.sulfur;
	correct();
}

void cost::clear()
{
	memset(this, 0, sizeof(cost));
}

bool cost::set(int id, int value)
{
    switch(id)
    {
    case Gold: gold = value; break;
	case Sulfur: sulfur = value; break;
	case Mercury: mercury = value; break;
	case Gems: gems = value; break;
	case Wood: wood = value; break;
	case Ore: ore = value; break;
	case Crystal: crystal = value; break;
	case Artifact: artifact = value; break;
	default: return false;
    }
	correct();
	return true;
}

int cost::get(int id) const
{
	switch(id)
	{
	case Gold: return gold;
	case Gems: return gems;
	case Wood: return wood;
	case Ore: return ore;
	case Mercury: return mercury;
	case Sulfur: return sulfur;
	case Crystal: return crystal;
	default: return 0;
	}
}

int cost::mindiv(const cost& e) const
{
	int m = 0;
	for(int i = (int)Wood; i<=(int)Gold; i++)
	{
		int j1 = get(i);
		int j2 = e.get(i);
		if(!j2)
			continue;
		int r = j1/j2;
		if(r<m)
			m = r;
	}
	return m;
}

void cost::correct()
{
	if(gold<0)
		gold = 0;
	if(sulfur<0)
		sulfur = 0;
	if(crystal<0)
		crystal = 0;
	if(mercury<0)
		mercury = 0;
	if(gems<0)
		gems = 0;
	if(wood<0)
		wood = 0;
	if(ore<0)
		ore = 0;
}

char* cost::tostring(char* r) const
{
    for(int i=(int)FirstResource; i<=(int)LastResource; i++)
    {
        int n = get(i);
        if(!n)
            continue;
        szprint(r, "%%r%1i(%2i)", i-FirstResource, n);
        r = zend(r);
    }
    return r;
}