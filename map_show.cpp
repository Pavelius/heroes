#include "main.h"

const int	    dx = 32;
const int	    dy = 32;
const int       viewx1 = 15;
const int       viewy1 = 14;
int			    hot::index;

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