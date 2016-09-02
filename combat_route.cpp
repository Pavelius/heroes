#include "main.h"

unsigned char		path_stack[256];
unsigned char		path_pop;
unsigned char		path_push;

static void snode(unsigned char index, unsigned char cost, bool iswide, tokens widedir)
{
	if(index==0xFF)
		return;
	unsigned char a = combat::movements[index];
	if(a>=BlockSquad)
		return;
	if(iswide)
	{
		int indexw = combat::moveto(index, widedir);
		if(indexw != -1)
		{
			if(combat::movements[indexw] >= BlockSquad)
				return;
		}
	}
	if(a!=0 && cost>=a)
		return;
	path_stack[path_push++] = index;
	combat::movements[index] = cost;
}

tokens combat::backward(tokens direction)
{
	switch(direction)
	{
	case HexLeft:
		return HexRight;
	case HexRight:
		return HexLeft;
	case HexLeftUp:
		return HexRightDown;
	case HexLeftDown:
		return HexRightUp;
	case HexRightUp:
		return HexLeftDown;
	case HexRightDown:
		return HexLeftUp;
	default:
		return direction;
	}
}

static int gnode(int index, int start)
{
	static const tokens directions[] = {HexLeftUp, HexRightDown, HexLeftDown, HexRightUp, HexLeft, HexRight};
	int r1 = -1;
	unsigned char m1 = 255;
	for(auto d : directions)
	{
		int i1 = combat::moveto(index, d);
		if(i1==-1)
			continue;
		if(i1==start)
			return i1;
		if(combat::movements[i1]>=BlockSquad)
			continue;
		if(combat::movements[i1]<m1)
		{
			m1 = combat::movements[i1];
			r1 = i1;
		}
	}
	return r1;
}

tokens combat::direction(int from, int to)
{
	static tokens dir[] = {HexRight, HexLeft, HexLeftUp, HexLeftDown, HexRightUp, HexRightDown};
	for(auto d : dir)
	{
		if(moveto(from, d) == to)
			return d;
	}
	return Empthy;
}

int combat::moveto(int index, int direction)
{
	switch(direction)
	{
	case HexLeft:
		if((index%awd) == 0)
			return -1;
		return index - 1;
	case HexRight:
		if(((index + 1) % awd) == 0)
			return -1;
		return index + 1;
	case HexRightDown:
		if(((index + 1) % awd) == 0 && ((index / awd) % 2) == 0)
			return -1;
		if(index / awd + 1 >= ahd)
			return -1;
		return index + awd + 1 - ((index / awd) % 2);
	case HexRightUp:
		if(((index / awd) % 2) == 0 && ((index + 1) % awd) == 0)
			return -1;
		if(index / awd == 0)
			return -1;
		return index - awd + 1 - ((index / awd) % 2);
	case HexLeftDown:
		if(((index / awd) % 2) != 0 && ((index%awd) == 0))
			return -1;
		if(index / awd + 1 >= ahd)
			return -1;
		return index + awd - ((index / awd) % 2);
	case HexLeftUp:
		if(((index / awd) % 2) != 0 && ((index%awd) == 0))
			return -1;
		if(index / awd == 0)
			return -1;
		return index - awd - ((index / awd) % 2);
	default:
		return -1;
	}
}

void combat::wave(int start, bool wide, bool fly, tokens widedir)
{
	memset(combat::movements, 0, sizeof(combat::movements));
	if(start==-1)
		return;
	for(unsigned i = FirstCombatant; i<=LastCombatant; i++)
	{
		if(!bsget(i, Type))
			continue;
		if(!game::get(i, Count))
			continue;
		int index = bsget(i, Index);
		if(index == start)
			continue;
		movements[index] = BlockSquad;
		if(game::iswide(bsget(i, Type)))
		{
			int indexw = combat::moveto(index, combat::isattacker(i) ? HexRight : HexLeft);
			if(indexw !=-1)
				movements[indexw] = BlockSquad;
		}
	}
	if(fly)
	{
		for(auto& e : movements)
		{
			if(e<BlockSquad)
				e = 1;
		}
	}
	else
	{
		path_push = 0;
		path_pop = 0;
		path_stack[path_push++] = start;
		movements[start] = 1;
		while(path_push != path_pop)
		{
			auto pos = path_stack[path_pop++];
			auto cost = movements[pos];
			if(cost>=BlockSquad)
				break;
			cost++;
			snode(combat::moveto(pos, HexLeft), cost, wide, widedir);
			snode(combat::moveto(pos, HexRight), cost, wide, widedir);
			snode(combat::moveto(pos, HexLeftUp), cost, wide, widedir);
			snode(combat::moveto(pos, HexLeftDown), cost, wide, widedir);
			snode(combat::moveto(pos, HexRightUp), cost, wide, widedir);
			snode(combat::moveto(pos, HexRightDown), cost, wide, widedir);
		}
	}
	movements[start] = BlockSquad;
}

int combat::move(int* result, int start, int target, int speed)
{
	speed = speed - SpeedCrawling + 2;
	int* p = result;
	while(start!=target)
	{
		if(movements[target] && movements[target]<=speed)
			*p++ = target;
		target = gnode(target, start);
		if(target ==-1)
			return 0;
	}
	return p - result;
}