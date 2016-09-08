#include "main.h"

static unsigned short	path_stack[256*256];
static unsigned short	path_push;
static unsigned short	path_pop;
static unsigned short	path_goal;
static unsigned short	path_start;

static void snode(unsigned short index, unsigned cost)
{
	if(index==-1)
		return;
	auto a = map::show::route[index];
	if(a>map::Passable)
		return;
	if(a!=0 && cost>=a)
		return;
	path_stack[path_push++] = index;
	map::show::route[index] = cost;
}

static void gnext(int index, unsigned& level, int& pos)
{
	if(index==-1)
		return;
	auto nlevel = map::show::route[index];
	if(!nlevel)
		return;
	if(nlevel <= level)
	{
		level = nlevel;
		pos = index;
	}
}

bool map::route::pathable(int index)
{
	static const map::directions direction[] = {
		map::Left, map::Right, map::Up, map::Down,
		map::LeftUp, map::LeftDown, map::RightUp, map::RightDown
	};
	for(auto a : direction)
	{
		int i = map::moveto(index, a);
		if(i!=-1 && map::show::route[i] && map::show::route[i]<=map::Passable)
			return true;
	}
	return false;
}

void map::route::around(int index, unsigned m)
{
	static const map::directions direction[] = {
		map::Left, map::Right, map::Up, map::Down,
		map::LeftUp, map::LeftDown, map::RightUp, map::RightDown
	};
	for(auto a : direction)
	{
		int i = map::moveto(index, a);
		if(i!=-1 && map::show::route[i]<m)
			map::show::route[i] = m;
	}
}

unsigned map::movecost(int index)
{
	if(index==-1)
		return 0xFFFFFFFF;
	return map::show::route[index];
}

unsigned map::movecost(int index, unsigned char direct, unsigned pathfinding)
{
	//         none   basc   advd   expr
	// Desert  2.00   1.75   1.50   1.00
	// Snow    1.75   1.50   1.25   1.00
	// Swamp   1.75   1.50   1.25   1.00
	// Cracked 1.25   1.00   1.00   1.00
	// Beach   1.25   1.00   1.00   1.00
	// Lava    1.00   1.00   1.00   1.00
	// Dirt    1.00   1.00   1.00   1.00
	// Grass   1.00   1.00   1.00   1.00
	// Water   1.00   1.00   1.00   1.00
	// Road    0.75   0.75   0.75   0.75
	if(map::is(index, isroad, direct))
		return 75;
	unsigned result = 100;
	switch(gettile(index))
	{
	case Desert:
		switch(pathfinding)
		{
		case 3:	break;
		case 2:	result += 50; break;
		case 1:	result += 75; break;
		default: result += 100; break;
		}
		break;
	case Snow:
	case Swamp:
		switch(pathfinding)
		{
		case 3:	break;
		case 2:	result += 25; break;
		case 1:	result += 50; break;
		default: result += 75; break;
		}
		break;
	case Wastelands:
	case Beach:
		result += (pathfinding==0) ? 25 : 0;
		break;
	default: break;
	}
	switch(direct)
	{
	case map::RightDown:
	case map::RightUp:
	case map::LeftDown:
	case map::LeftUp:
		result += result * 55 / 100;
	}
	return result;
}

static unsigned movecost2(int from, map::directions direct, unsigned pathfinding)
{
	const auto cost1 = map::movecost(from, direct, pathfinding); // penalty: for [cur] out
	const auto cost2 = map::movecost(map::moveto(from, direct), map::revers(direct), pathfinding); // penalty: for [tmp] in
	return (cost1 + cost2) >> 1;
}

int map::revers(int direction)
{
	switch(direction)
	{
	case map::Left:
		return map::Right;
	case map::Right:
		return map::Left;
	case map::Down:
		return map::Up;
	case map::Up:
		return map::Down;
	case map::RightDown:
		return map::LeftUp;
	case map::LeftUp:
		return map::RightDown;
	case map::RightUp:
		return map::LeftDown;
	case map::LeftDown:
		return map::RightUp;
	default:
		return -1;
	}
}

map::directions map::orient(int from, int to)
{
	static const directions orientations[25] =
	{
		map::LeftUp, map::LeftUp, map::Up, map::RightUp, map::RightUp,
		map::LeftUp, map::LeftUp, map::Up, map::RightUp, map::RightUp,
		map::Left, map::Left, map::Center, map::Right, map::Right,
		map::LeftDown, map::LeftDown, map::Down, map::RightDown, map::RightDown,
		map::LeftDown, map::LeftDown, map::Down, map::RightDown, map::RightDown
	};
	int dx = i2x(to) - i2x(from);
	int dy = i2y(to) - i2y(from);
	int div = imax(iabs(dx), iabs(dy));
	if(!div)
		return map::Center; //default
	if(div>3)
		div /= 2;
	int ax = dx/div;
	int ay = dy/div;
	return orientations[(ay+2)*5+ax+2];
}

bool map::isroad(unsigned char object, unsigned char index, unsigned char direct)
{
	switch(res::map(object))
	{
		// from sprite road
	case res::ROAD:
		if(0 == index || 4 == index || 5 == index || 13 == index || 26 == index)
			return (direct&(Up|Down))!=0;
		else if(2 == index || 21 == index || 28 == index)
			return (direct&(Left|Right))!=0;
		else if(17 == index || 29 == index)
			return (direct & (LeftUp|RightDown))!=0;
		else if(18 == index || 30 == index)
			return (direct & (RightUp|LeftDown))!=0;
		else if(3 == index)
			return (direct & (Up | Down | Left | Right))!=0;
		else if(6 == index)
			return (direct & (Up | Down | Right))!=0;
		else if(7 == index)
			return (direct & (Up | Right))!=0;
		else if(9 == index)
			return (direct & (Down | Right))!=0;
		else if(12 == index)
			return (direct & (Down | Left))!=0;
		else if(14 == index)
			return (direct & (Up | Down | Left))!=0;
		else if(16 == index)
			return (direct & (Up | Left))!=0;
		else if(19 == index)
			return (direct & (LeftUp | RightDown))!=0;
		else if(20 == index)
			return (direct & (RightUp | LeftDown))!=0;
		// castle and tower (gate)
	case res::OBJNTOWN:
		if(13 == index ||
			29 == index ||
			45 == index ||
			61 == index ||
			77 == index ||
			93 == index ||
			109 == index ||
			125 == index ||
			141 == index ||
			157 == index ||
			173 == index ||
			189 == index)
			return (direct & (Up | Down))!=0;
		// castle lands (gate)
	case res::OBJNTWBA:
		if(7 == index ||
			17 == index ||
			27 == index ||
			37 == index ||
			47 == index ||
			57 == index ||
			67 == index ||
			77 == index)
			return (direct & (Up | Down))!=0;
	default:
		return false;
	}
}

static int routeindex(int from, int throught, int to, int mod)
{
	// ICN::ROUTE
	// start index 1, 25, 49, 73, 97, 121 (size arrow path)
	// 1 - from left to up+
	// 9 - from down to up+
	int index = 1;
	if(mod>=200)
		index = 121;
	else if(mod>=175)
		index = 97;
	else if(mod>=150)
		index = 73;
	else if(mod>=125)
		index = 49;
	else if(mod>=100)
		index = 25;
	map::directions d = map::orient(from, throught);
	map::directions d1 = map::orient(throught, to);
	if(from==throught)
	{
		switch(d)
		{
		case map::Up:		index += 8; break;
		case map::RightUp:	index += 17; break;
		case map::Right:	index += 18; break;
		case map::Left:		index += 6; break;
		case map::LeftUp:	index += 7; break;
		case map::LeftDown:	index += 5; break;
		case map::RightDown:index += 19; break;
		default: 			index = 0; break;
		}
	}
	else switch(d)
	{
	case map::Up:
		switch(d1)
		{
		case map::Up:		index += 8; break;
		case map::RightUp:	index += 17; break;
		case map::Right:	index += 18; break;
		case map::Left:		index += 6; break;
		case map::LeftUp:	index += 7; break;
		case map::LeftDown:	index += 5; break;
		case map::RightDown:index += 19; break;
		default: 			index = 0; break;
		}
		break;
    case map::RightUp:
		switch(d1)
		{
		case map::Up:		index += 0; break;
		case map::RightUp:	index += 9; break;
		case map::Right:	index += 18; break;
		case map::RightDown:index += 19; break;
		case map::LeftUp:	index += 7; break;
		case map::Down:		index += 20; break;
		case map::Left:		index += 6; break;
		default: 			index = 0; break;
		}
		break;
	case map::Right:
		switch(d1)
		{
		case map::Up:		index += 0; break;
		case map::Down:		index += 20; break;
		case map::RightDown:index += 19; break;
		case map::Right:	index += 10; break;
		case map::RightUp:	index += 1; break;
		case map::LeftUp:	index += 7; break;
		case map::LeftDown:	index += 21; break;
		default: 			index = 0; break;
		}
		break;
	case map::RightDown:
		switch(d1)
		{
		case map::RightUp:	index += 1; break;
		case map::Right:	index += 2; break;
		case map::RightDown:index += 11; break;
		case map::Down:		index += 20; break;
		case map::LeftDown:	index += 21; break;
		case map::Up:		index += 0; break;
		case map::Left:		index += 22; break;
		default: 			index = 0; break;
		}
		break;
	case map::Down:
		switch(d1)
		{
		case map::Right:	index += 2; break;
		case map::RightDown:index += 3; break;
		case map::Down:		index += 12; break;
		case map::LeftDown:	index += 21; break;
		case map::Left:		index += 22; break;
		case map::LeftUp:	index += 16; break;
		case map::RightUp:	index += 1; break;
		default: 			index = 0; break;
		}
		break;
	case map::LeftDown:
		switch(d1)
		{
		case map::RightDown:index += 3; break;
		case map::Down:		index += 4; break;
		case map::LeftDown:	index += 13; break;
		case map::Left:		index += 22; break;
		case map::LeftUp:	index += 23; break;
		case map::Up:		index += 16; break;
		case map::Right:	index += 2; break;
		default: 			index = 0; break;
		}
		break;
	case map::Left:
		switch(d1)
		{
		case map::Up:		index += 16; break;
		case map::Down:		index += 4; break;
		case map::LeftDown:	index += 5; break;
		case map::Left:		index += 14; break;
		case map::LeftUp:	index += 23; break;
		case map::RightUp:	index += 17; break;
		case map::RightDown:index += 3; break;
		default: 			index = 0; break;
		}
		break;
	case map::LeftUp:
		switch(d1)
		{
		case map::Up:		index += 16; break;
		case map::RightUp:	index += 17; break;
		case map::LeftDown:	index += 5; break;
		case map::Left:		index += 6; break;
		case map::LeftUp:	index += 15; break;
		case map::Down:		index += 4; break;
		case map::Right:	index += 18; break;
		default: 			index = 0; break;
		}
		break;
	default: index = 0; break;
	}
	return index;
}

// First, make wave and see what cell on map is passable
void map::route::wave(int start, int skill, int ship_master)
{
	memset(map::show::route, 0, sizeof(map::show::route));
	if(start==-1)
		return;
	int count = map::width*map::height;
	if(ship_master)
	{
		for(int i = 0; i<count; i++)
			map::show::route[i] = map::ispassable(i) ? 0 : 0xFFFFFFFF;
	}
	else
	{
		map::moveable::block(map::show::route);
		for(int i = 0; i<count; i++)
		{
			if(!map::ispassable(i) || map::gettile(i)==Water)
				map::show::route[i] = map::Blocked;
		}
	}
	path_push = 0;
	path_pop = 0;
	path_stack[path_push++] = start;
	map::show::route[start] = 1;
	while(path_push != path_pop)
	{
		auto pos = path_stack[path_pop++];
		auto cost = map::show::route[pos];
		if(cost>=Passable-200)
			break;
		snode(moveto(pos, map::Left), cost+movecost2(pos, map::Left, skill));
		snode(moveto(pos, map::Right), cost+movecost2(pos, map::Right, skill));
		snode(moveto(pos, map::Up), cost+movecost2(pos, map::Up, skill));
		snode(moveto(pos, map::Down), cost+movecost2(pos, map::Down, skill));
		snode(moveto(pos, map::LeftUp), cost+movecost2(pos, map::LeftUp, skill));
		snode(moveto(pos, map::LeftDown), cost+movecost2(pos, map::LeftDown, skill));
		snode(moveto(pos, map::RightUp), cost+movecost2(pos, map::RightUp, skill));
		snode(moveto(pos, map::RightDown), cost+movecost2(pos, map::RightDown, skill));
	}
	path_pop = 0;
	path_push = 0;
	path_goal = -1;
	path_start = start;
}

// Second calculate path to any cell on map use wave result
void map::route::path(int start, int goal)
{
	path_push = 0;
	path_goal = -1;
	int pos = goal;
	unsigned level = Passable;
	while(pos != start)
	{
		auto n = pos;
		gnext(map::moveto(pos, Left), level, n);
		gnext(map::moveto(pos, Right), level, n);
		gnext(map::moveto(pos, Up), level, n);
		gnext(map::moveto(pos, Down), level, n);
		gnext(map::moveto(pos, LeftDown), level, n);
		gnext(map::moveto(pos, LeftUp), level, n);
		gnext(map::moveto(pos, RightDown), level, n);
		gnext(map::moveto(pos, RightUp), level, n);
		if(pos==n)
			return;
		pos = n;
		path_stack[path_push++] = n;
		level = map::show::route[pos];
	}
	path_goal = goal;
}

void map::route::load(int* rec, int index, int w, int h)
{
	if(!path_push || path_goal==-1)
		return;
	int x1 = map::i2x(index);
	int y1 = map::i2y(index);
	int p0 = path_goal;
	int x = map::i2x(p0);
	int y = map::i2y(p0);
	if(x>=x1 && x<x1+w && y>=y1 && y<y1+h)
		rec[(y-y1)*w + (x-x1)] = 0;
	for(int i = 0; i<path_push; i++)
	{
		int p1 = path_stack[i];
		if(p1==path_start)
			break;
		int p2 = path_stack[i+1];
		int x = map::i2x(p1);
		int y = map::i2y(p1);
		if(x<x1 || x>=x1+w)
			continue;
		if(y<y1 || y>=y1+h)
			continue;
		int m = (y-y1)*w + (x-x1);
		rec[m] = routeindex(p2, p1, p0, map::show::route[p2]-map::show::route[p1]);
		p0 = p1;
		p1 = p2;
	}
}

int map::route::get(int index)
{
	return map::show::route[index];
}

static bool makestep(int rec, int pos2)
{
	int mp = bsget(rec, MovePoints);
    int pos1 = bsget(rec, Index);
	map::directions d = map::orient(pos1, pos2);
	bsset(rec, Direction, d);
	int mc = movecost2(pos1, d, bsget(rec, SkillPathfinding));
	if(mp<mc)
        return false;
    bsset(rec, MovePoints, mp-mc);
    return true;
}

static bool movingto(int rec, int pos2, void(*callback)())
{
	if(!makestep(rec, pos2))
		return false;
	if(callback)
		callback();
	bsset(rec, Index, pos2);
	return true;
}

static bool reactionto(int rec, int pos2, void(*callback)())
{
	if(!makestep(rec, pos2))
		return false;
	int m = bsfind(FirstMoveable, Index, pos2);
    if(!m)
        return false;
	map::moveable::reaction(m, rec);
	bsset(rec, MoveTo, bsget(rec, Index));
	if(callback)
		callback();
	return true;
}

static void battle(int rec, int pos2, void(*callback)())
{
	if(!makestep(rec, pos2))
		return;
	bsset(rec, Index, pos2);
	int enemy = map::moveable::nearest(pos2, FirstMonster, LastMonster, 1);
	if(enemy==-1)
		return;
	combat::start(rec, enemy);
}

void map::route::move(int rec, void(*callback)())
{
	if(!path_push)
		return;
    // Walk by calculated path
	path_push--;
	while(path_push)
	{
		int pos2 = path_stack[--path_push];
		if(!movingto(rec, pos2, callback))
            break;
	}
	// Last step to goal can be done
	// only if there is no any obstacle
	auto pv = map::show::route[path_goal];
	if(pv<=Passable)
		movingto(rec, path_goal, callback);
    else if(pv==Action)
        reactionto(rec, path_goal, callback);
	else if(pv==Attack)
		battle(rec, path_goal, callback);
	wave(bsget(rec, Index),
      bsget(rec, SkillPathfinding),
      bsget(rec, ShipMaster));
}

void draw::route(int x, int y, int* rec, int w, int h, int distance)
{
	// route
	const int dy = 32;
	const int dx = 32;
	int x2 = x + w*dx;
	int y2 = y + h*dy;
	int ri = 0;
	for(int y1 = y; y1<y2; y1 += dy)
	{
		for(int x1 = x; x1<x2; x1 += dx)
		{
			auto id = rec[ri];
			if(id!=-1)
				draw::image(x1-12, y1, res::ROUTE, id);
			ri++;
		}
	}
}