#include "main.h"

static unsigned char movements[combat::ahd*combat::awd];
static unsigned char path_stack[256];
static unsigned char path_pop;
static unsigned char path_push;

bool combat::ispassable(int index) {
	return movements[index] < BlockSquad;
}

int combat::getindex(int index) {
	if(index < 0 || index >= sizeof(movements) / sizeof(movements[0]))
		return -1;
	switch(movements[index]) {
	case PassableSquadWideRight:
	case BlockSquadWideRight:
		return getindex(moveto(index, HexLeft));
	case PassableSquadWideLeft:
	case BlockSquadWideLeft:
		return getindex(moveto(index, HexRight));
	default:
		return index;
	}
}

int combat::getwideindex(int rec) {
	if(!game::iswide(bsget(rec, Type)))
		return -1;
	auto index = bsget(rec, Index);
	auto wdir = combat::isattacker(rec) ? HexRight : HexLeft;
	return combat::moveto(index, wdir);
}

static void update_wide_index(tokens wdir, int speed) {
	if(wdir == Empthy || !speed)
		return;
	auto wval = (wdir == HexRight) ? PassableSquadWideRight : PassableSquadWideLeft;
	for(int i = 0; i <= sizeof(movements) / sizeof(movements[0]); i++) {
		if(movements[i] >= PassableSquadWideLeft)
			continue;
		if(movements[i] == speed) {
			auto i2 = combat::moveto(i, wdir);
			if(i2 != -1 && movements[i2] < BlockSquad)
				movements[i2] = wval;
		}
	}
}

unsigned char combat::getpassable(int index) {
	index = getindex(index);
	if(index < 0)
		return BlockTerrain;
	return movements[index];
}

static void snode(unsigned char index, unsigned char cost, tokens wdir) {
	if(index == 0xFF)
		return;
	unsigned char a = movements[index];
	if(a >= BlockSquad)
		return;
	if(wdir != Empthy) {
		int indexw = combat::moveto(index, wdir);
		if(indexw != -1) {
			if(movements[indexw] >= BlockSquad)
				return;
		}
	}
	if(a != 0 && cost >= a)
		return;
	path_stack[path_push++] = index;
	movements[index] = cost;
}

tokens combat::backward(tokens direction) {
	switch(direction) {
	case HexLeft: return HexRight;
	case HexRight: return HexLeft;
	case HexLeftUp: return HexRightDown;
	case HexLeftDown: return HexRightUp;
	case HexRightUp: return HexLeftDown;
	case HexRightDown: return HexLeftUp;
	default: return direction;
	}
}

static int gnode(int index, int start) {
	static const tokens directions[] = {HexLeftUp, HexRightDown, HexLeftDown, HexRightUp, HexLeft, HexRight};
	int r1 = -1;
	unsigned char m1 = 255;
	for(auto d : directions) {
		int i1 = combat::moveto(index, d);
		if(i1 == -1)
			continue;
		if(i1 == start)
			return i1;
		if(movements[i1] >= BlockSquad)
			continue;
		if(movements[i1] < m1) {
			m1 = movements[i1];
			r1 = i1;
		}
	}
	return r1;
}

tokens combat::getdirection(int from, int to) {
	int x1 = from % combat::awd;
	int y1 = from / combat::awd;
	int x2 = to % combat::awd;
	int y2 = to / combat::awd;
	if(y2 == y1) {
		if(x2 == x1)
			return HexCenter;
		return (x2 > x1) ? HexRight : HexLeft;
	}
	point p1 = i2h(from);
	point p2 = i2h(to);
	if(y2 < y1)
		return (p2.x < p1.x) ? HexLeftUp : HexRightUp;
	return (p2.x < p1.x) ? HexLeftDown : HexRightDown;
}

int combat::moveto(int index, int direction) {
	switch(direction) {
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

static void block_terrain() {
	for(int i = 0; i <= sizeof(movements) / sizeof(movements[0]); i++) {
		if(movements[i] == 0)
			movements[i] = BlockTerrain;
	}
}

void combat::wave(int start, bool fly, tokens wdir, int speed) {
	memset(movements, 0, sizeof(movements));
	if(start == -1)
		return;
	for(unsigned i = FirstCombatant; i <= LastCombatant; i++) {
		if(!bsget(i, Type))
			continue;
		if(!game::get(i, Count))
			continue;
		int index = bsget(i, Index);
		if(index == start)
			continue;
		movements[index] = BlockSquad;
		int wi = getwideindex(i);
		if(wi != -1)
			movements[wi] = combat::isattacker(i) ? BlockSquadWideRight : BlockSquadWideLeft;
	}
	if(fly) {
		for(auto& e : movements) {
			if(e < BlockSquad)
				e = 1;
		}
	} else {
		path_push = 0;
		path_pop = 0;
		path_stack[path_push++] = start;
		movements[start] = 1;
		while(path_push != path_pop) {
			auto pos = path_stack[path_pop++];
			auto cost = movements[pos];
			if(cost >= BlockSquad)
				break;
			cost++;
			snode(combat::moveto(pos, HexLeft), cost, wdir);
			snode(combat::moveto(pos, HexRight), cost, wdir);
			snode(combat::moveto(pos, HexLeftUp), cost, wdir);
			snode(combat::moveto(pos, HexLeftDown), cost, wdir);
			snode(combat::moveto(pos, HexRightUp), cost, wdir);
			snode(combat::moveto(pos, HexRightDown), cost, wdir);
		}
	}
	movements[start] = BlockSquad;
	block_terrain();
	update_wide_index(wdir, speed);
}

int combat::move(int* result, int start, int target, int speed) {
	int* p = result;
	while(start != target) {
		if(movements[target] <= speed + 2 || movements[target] == PassableSquadWideLeft || movements[target] == PassableSquadWideRight)
			*p++ = target;
		target = gnode(target, start);
		if(target == -1)
			return 0;
	}
	return p - result;
}