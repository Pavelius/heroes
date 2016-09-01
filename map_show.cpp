#include "main.h"

const int	    dx = 32;
const int	    dy = 32;
const int       viewx1 = 15;
const int       viewy1 = 14;
int			    hot::index;

static const unsigned char monster_animation_cicle[] =
{
	1, 2, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	3, 4, 5, 4, 3, 0,
	0, 0, 0, 0,
};
static const unsigned char monster_animation_cicl1[] =
{
	1, 2, 1, 0,
	3, 4, 5, 4, 3, 0,
};

//static void offs_hero(int& x, int& y, int direction, int frame)
//{
//	if(!frame)
//		return;
//	switch(direction)
//	{
//	case map::Up:			y += -4 * frame; break;
//	case map::RightUp:		x += 4 * frame; y += -4 * frame; break;
//	case map::LeftUp:		x += -4 * frame; y += -4 * frame; break;
//	case map::RightDown:	x += 4 * frame; y += 4 * frame; break;
//	case map::Down:         y += 4 * frame; break;
//	case map::LeftDown:		x += -4 * frame; y += 4 * frame; break;
//	case map::Right:		x += 4 * frame; break;
//	case map::Left:         x += -4 * frame; break;
//	default: break;
//	}
//}

void animate::hero(int x, int y, int mid, int ticket)
{
	res::tokens icn = res::Empthy;
	int index = 0;
	if(bsget(mid, ShipMaster))
		icn = res::BOAT32;
	else switch(bsget(mid, Type))
	{
	case Knight: icn = res::KNGT32; break;
	case Barbarian: icn = res::BARB32; break;
	case Sorcerer: icn = res::SORC32; break;
	case Warlock: icn = res::WRLK32; break;
	case Wizard: icn = res::WZRD32; break;
	case Necromancer: icn = res::NECR32; break;
	default: return;
	}
	unsigned flags = 0;
	switch(bsget(mid, Direction))
	{
	case map::Up:          index = 0; break;
	case map::RightUp:     index = 9; break;
	case map::Right:       index = 18; break;
	case map::RightDown:   index = 27; break;
	case map::Down:        index = 36; break;
	case map::LeftDown:    index = 27; flags |= AFMirror; break;
	case map::Left:        index = 18; flags |= AFMirror; break;
	case map::LeftUp:      index = 9; flags |= AFMirror; break;
	default: break;
	}
	draw::image(x, y, icn, index, flags);
}

void animate::heroflag(int x, int y, int mid, int index)
{
	res::tokens icn = res::Empthy;
	int index_sprite = 0;
	switch(bsget(mid, Player))
	{
	case PlayerBlue: icn = res::B_FLAG32; break;
	case PlayerGreen: icn = res::G_FLAG32; break;
	case PlayerRed: icn = res::R_FLAG32; break;
	case PlayerYellow: icn = res::Y_FLAG32; break;
	case PlayerOrange: icn = res::O_FLAG32; break;
	case PlayerPurple: icn = res::P_FLAG32; break;
	default: return;
	}
	unsigned flags = 0;
	switch(bsget(mid, Direction))
	{
	case map::Up:        index_sprite = 0; break;
	case map::RightUp:   index_sprite = 9; break;
	case map::Right:     index_sprite = 18; break;
	case map::RightDown: index_sprite = 27; break;
	case map::Down:      index_sprite = 36; break;
	case map::LeftDown:  index_sprite = 27; flags |= AFMirror; x += 8; break;
	case map::Left:      index_sprite = 18; flags |= AFMirror; x += 12; break;
	case map::LeftUp:    index_sprite = 9; flags |= AFMirror; x += 19; break;
	default: break;
	}
	draw::image(x, y, icn, index_sprite + (index % 9), flags);
}

void animate::heroshad(int x, int y, int mid, int index)
{
	res::tokens icn = res::SHADOW32;
	int index_sprite = 0;
	switch(bsget(mid, Direction))
	{
	case map::Up:          index_sprite = 0; break;
	case map::RightUp:     index_sprite = 9; break;
	case map::Right:       index_sprite = 18; break;
	case map::RightDown:   index_sprite = 27; break;
	case map::Down:        index_sprite = 36; break;
	case map::LeftDown:    index_sprite = 45; break;
	case map::Left:        index_sprite = 54; break;
	case map::LeftUp:      index_sprite = 63; break;
	default: break;
	}
	draw::image(x, y, icn, index_sprite + (index % 9));
}

void animate::monster(int x, int y, int mid, int ticket)
{
	if(mid >= (int)MonsterRnd1)
		draw::image(x, y, res::MONS32, mid - FirstMonster);
	else
	{
		draw::image(x, y, res::MINIMON, (mid - FirstMonster) * 9);
		switch(bsget(mid, AnimationType))
		{
		case 1:
			draw::image(x, y, res::MINIMON, (mid - FirstMonster) * 9
				+ 1 + monster_animation_cicl1[((draw::timestamp / 100) + ticket*ticket) % (sizeof(monster_animation_cicl1) / sizeof(monster_animation_cicl1[0]))]);
			break;
		default:
			draw::image(x, y, res::MINIMON, (mid - FirstMonster) * 9
				+ 1 + monster_animation_cicle[((draw::timestamp / 100) + ticket*ticket) % (sizeof(monster_animation_cicle) / sizeof(monster_animation_cicle[0]))]);
			break;
		}
	}
}

void animate::artifact(int x, int y, int mid, int ticket)
{
	draw::image(x - 32, y, res::OBJNARTI, (mid - FirstArtifact) * 2);
	draw::image(x, y, res::OBJNARTI, (mid - FirstArtifact) * 2 + 1);
}

void draw::map(int x, int y, int* objects, int* route)
{
	int di = map::width - viewx1;
	int x2 = x + viewx1*dx;
	int y2 = y + viewy1*dy;
	hot::index = -1;
	// В пределах одной клетки спрайта делаем такой вывод
	// 1) Земля и наземные объекты
	// 1) Герои, города, ресурсы
	// 2) Высокие объекты, которые закрывают другие собою
	int i = map::view();
	int ix1 = map::i2x(i);
	int iy1 = map::i2y(i);
	// Landscape and overlays
	for(int y1 = y; y1 < y2; y1 += dy)
	{
		for(int x1 = x; x1 < x2; x1 += dx)
		{
			if(hot::mouse.x >= x1 && hot::mouse.y >= y1 && hot::mouse.x <= x1 + 31 && hot::mouse.y <= y1 + 31)
				hot::index = i;
			imager(x1, y1, res::TisGROUND32, map::show::tiles[i], map::show::flags[i] % 4);
			for(auto e : map::show::objects[i])
			{
				if(!e[0])
					break;
				int index = e[1];
				res::tokens res = res::map(e[0]);
				if(res::ishight(res, index))
					continue;
				image(x1, y1, res, index);
				index = indexes::animate(res, index, (draw::timestamp / 100) + index*index, false);
				if(index)
					image(x1, y1, res, index);
			}
			i++;
		}
		i += di;
	}
	// Object is last layer in render
	i = map::view();
	for(int y1 = y; y1 < y2; y1 += dy)
	{
		for(int x1 = x; x1 < x2; x1 += dx)
		{
			int ticket = i*i;
			int ix = map::i2x(i) - ix1;
			int iy = map::i2y(i) - iy1;
			int j = iy*viewx1 + ix;
			int id = objects[j];
			if(id)
			{
				//draw::rectb(x1,y1,x1+32,y1+32, 0xC4);
				if(id >= (int)FirstResource && id <= (int)LastResource)
				{
					draw::image(x1 - 32, y1, res::OBJNRSRC, (id - FirstResource) * 2);
					draw::image(x1, y1, res::OBJNRSRC, (id - FirstResource) * 2 + 1);
				}
				else if(id >= (int)FirstMonster && id <= (int)LastMonster)
					animate::monster(x1 + 16, y1 + 30, id, ticket);
				else if(id >= (int)FirstArtifact && id <= (int)LastArtifact)
					animate::artifact(x1, y1, id, ticket);
				else if(id == TreasureChest)
				{
					draw::image(x1 - 32, y1, res::OBJNRSRC, 18);
					draw::image(x1, y1, res::OBJNRSRC, 19);
				}
				else if(id >= (int)FirstHero && id <= (int)LastHero)
				{
					animate::heroshad(x1, y1 + 26, id, 0);
					animate::hero(x1, y1 + 26, id, 0);
					animate::heroflag(x1, y1 + 26, id, 0);
				}
			}
			for(auto e : map::show::objects[i])
			{
				if(!e[0])
					break;
				int index = e[1];
				res::tokens res = res::map(e[0]);
				if(!res::ishight(res, index))
					continue;
				image(x1, y1, res, index);
				index = indexes::animate(res, index, (draw::timestamp / 100) + ticket, false);
				if(index)
					image(x1, y1, res, index);
			}
			i++;
			id++;
		}
		i += di;
	}
	draw::route(x, y, route, viewx1, viewy1, 500);
}