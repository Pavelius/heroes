#include "main.h"

const unsigned Blocked = 0xFFFFFFFA;
const unsigned BlockedUpDown = 0xFFFFFFF9;
static unsigned			path[144 * 144];
static unsigned short	path_stack[256 * 256];
static unsigned short	path_push;
static unsigned short	path_pop;
static unsigned short	path_goal;
static unsigned short	path_start;

static void snode(unsigned short index, unsigned cost)
{
	if(index == -1)
		return;
	auto a = path[index];
	if(a >= Blocked)
		return;
	if(a != 0 && cost >= a)
		return;
	path_stack[path_push++] = index;
	path[index] = cost;
}

static void gnext(int index, unsigned& level, int& pos)
{
	if(index == -1)
		return;
	auto nlevel = path[index];
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
	return path[index] < Blocked;
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
		if(i != -1 && path[i] < m)
			path[i] = m;
	}
}

unsigned map::movecost(int index)
{
	if(index == -1)
		return 0xFFFFFFFF;
	return path[index];
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
	if(map::isroad(index, direct))
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
		result += (pathfinding == 0) ? 25 : 0;
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

map::directions map::revers(map::directions direction)
{
	switch(direction)
	{
	case map::Left: return map::Right;
	case map::Right: return map::Left;
	case map::Down: return map::Up;
	case map::Up: return map::Down;
	case map::RightDown: return map::LeftUp;
	case map::LeftUp: return map::RightDown;
	case map::RightUp: return map::LeftDown;
	case map::LeftDown: return map::RightUp;
	default: return map::Center;
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
	if(div > 3)
		div /= 2;
	int ax = dx / div;
	int ay = dy / div;
	return orientations[(ay + 2) * 5 + ax + 2];
}

static void apply_map_type()
{
	for(int y = 0; y < map::height; y++)
	{
		int i1 = map::m2i(0, y);
		int i2 = i1 + map::width;
		for(; i1 < i2; i1++)
		{
			switch(map::show::type[i1])
			{
			case TypeBlock:
			case TypeAction:
				path[i1] = Blocked;
				break;
			case TypeAttack:
				path[i1] = Blocked;
				break;
			}
		}
	}
}

// First, make wave and see what cell on map is passable
void map::route::wave(int start, int skill, int ship_master)
{
	static point block_castle[] = {{-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {1, 0}, {2, 0}};
	path_push = 0;
	path_pop = 0;
	memset(path, 0, sizeof(path));
	memset(map::show::type, 0, sizeof(map::show::type));
	if(start == -1)
		return;
	if(ship_master)
	{
	}
	else
	{
		// Block all water part
		for(int y = 0; y < map::height; y++)
		{
			for(int x = 0; x < map::width; x++)
			{
				auto i = map::m2i(x, y);
				if(map::gettile(i) == Water)
					show::type[i] = TypeBlock;
			}
		}
		// Block all known castle parts
		int rec2 = bsget(FirstCastle, Last);
		for(int rec = FirstCastle; rec < rec2; rec++)
		{
			int index = bsget(rec, Index);
			int x = map::i2x(index);
			int y = map::i2y(index);
			for(auto pt : block_castle)
			{
				int x1 = x + pt.x;
				int y1 = y + pt.y;
				if(y1 < 0 || y1 >= map::height)
					continue;
				if(x1 < 0 || x1 >= map::width)
					continue;
				int i = map::m2i(x1, y1);
				show::type[i] = TypeBlock;
			}
			show::type[index] = TypeAction;
		}
		// Block overland part
		command::execute("map_block");
	}
	apply_map_type();
	path_stack[path_push++] = start;
	path[start] = 1;
	while(path_push != path_pop)
	{
		auto pos = path_stack[path_pop++];
		auto cost = path[pos];
		if(cost >= Blocked - 200)
			break;
		snode(moveto(pos, map::Left), cost + movecost2(pos, map::Left, skill));
		snode(moveto(pos, map::Right), cost + movecost2(pos, map::Right, skill));
		snode(moveto(pos, map::Up), cost + movecost2(pos, map::Up, skill));
		snode(moveto(pos, map::Down), cost + movecost2(pos, map::Down, skill));
		snode(moveto(pos, map::LeftUp), cost + movecost2(pos, map::LeftUp, skill));
		snode(moveto(pos, map::LeftDown), cost + movecost2(pos, map::LeftDown, skill));
		snode(moveto(pos, map::RightUp), cost + movecost2(pos, map::RightUp, skill));
		snode(moveto(pos, map::RightDown), cost + movecost2(pos, map::RightDown, skill));
	}
	path_pop = 0;
	path_push = 0;
	path_goal = -1;
	path_start = start;
}

// Second calculate path to any cell on map use wave result
void map::route::walk(int start, int goal)
{
	path_push = 0;
	path_goal = -1;
	int pos = goal;
	unsigned level = Blocked;
	path_stack[path_push++] = goal;
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
		if(pos == n)
			return;
		pos = n;
		path_stack[path_push++] = n;
		level = path[pos];
	}
	path_goal = goal;
}

short unsigned* map::route::getpath()
{
	return path_stack;
}

int	map::route::getpathcount()
{
	return path_push;
}

void game::moveto(int hero, int player)
{
	if(path_push < 2)
		return;
	while(path_push - 1)
	{
		auto from = bsget(hero, Index);
		auto to = path_stack[path_push - 2];
		auto d = map::orient(from, to);
		unsigned mp = bsget(hero, MovePoints);
		auto mc = movecost2(from, d, bsget(hero, SkillPathfinding));
		if(mp < mc)
			return;
		if(d != map::Center)
			bsset(hero, Direction, d);
		bsset(hero, MovePoints, mp - mc);
		//
		if(map::show::type[to] == TypeAction)
		{
			bsset(hero, MoveTo, -1);
			path_push = 0;
			auto object = bsfind(FirstCastle, Index, to);
			if(!object)
				object = bsfind(FirstMapObject, Index, to);
			if(object)
				game::interact(to, object, hero, player);
			return;
		}
		else
		{
			bsset(hero, Index, to);
			path_push--;
		}
		// Interactive show movement
		show::adventure::move(from, to, hero, player);
	}
}