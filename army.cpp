#include "main.h"

void army::clear()
{
	memset(&units, 0, sizeof(units));
}

void army::add(int id, int count)
{
	if(!count)
		return;
	for(auto& e : units)
	{
		if(e.isvalid())
			continue;
		e.id = id;
		e.count = count;
		return;
	}
}

void army::input(int id, int base, int& army_index, int parent)
{
	if(id>=base && id<=(int)(base+sizeof(units)/sizeof(units[0])))
	{
		if(army_index==-1)
		{
			if(units[id-base].isvalid())
				army_index = id - base;
		}
		else
		{
			iswap(units[id-base], units[army_index]);
			army_index = -1;
		}
	}
	else if(id==Tooltips)
        show::unitinfo(hot::param, parent);
}