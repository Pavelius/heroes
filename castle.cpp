#include "main.h"

const int FBuildThisTurn = 0x00000001;
const int CastleIncome = 1000;
const int CastleIncomeDungeon = 500;
const int CastleIncomeStatue = 250;
static struct castle : public drawable
{
	tokens				player;
	tokens				type;
	short unsigned		index;
	char				name[16];
	unsigned char		moved;
	short unsigned		creatures[LastCreatureCount - FirstCreatureCount + 1];
	unsigned char		dwellings[LastBuilding - FirstBuilding + 1];
	unsigned char		spells[LastSpell - FirstSpell + 1];
	unsigned short		army[LastTroopsIndex - FirstArtifactIndex + 1];

	point getpos() const
	{
		auto x = map::i2x(index) * 32;
		auto y = map::i2y(index) * 32;
		return{(short)x, (short)y};
	}

	// Fast clipping rect. For hit test use othe method.
	rect getrect() const override
	{
		auto pt = getpos();
		return{pt.x - 2 * 32, pt.y - 3 * 32, pt.x + 3 * 32, pt.y + 3 * 32};
	}

	point getzpos() const override
	{
		return{(short)(map::i2x(index) * 32), (short)(map::i2y(index) * 32) + 8};
	}

	void painting(point camera, unsigned paint_flags) const override
	{
		auto pt = getpos() - camera;
		auto tl = map::gettile(index);
		auto town = dwellings[Castle - FirstBuilding] != 2;
		pt.x -= 2 * 32;
		pt.y -= 3 * 32;
		draw::castle(pt.x, pt.y, map::gettile(index), type, town);
	}

} objects[LastCastle - FirstCastle + 1];
static bsmeta::field fields[] = {
	BSREQ(castle, player, Player, Number),
	BSREQ(castle, type, Type, Number),
	BSREQ(castle, index, Index, Number),
	BSINT(castle, dwellings, FirstBuilding, Number),
	BSINT(castle, army, FirstTroopsIndex, Number),
	BSINT(castle, spells, FirstSpell, Number),
	BSINT(castle, creatures, FirstCreatureCount, Number),
	BSREQ(castle, moved, AlreadyMoved, Number),
	BSREQ(castle, name, Name, Text),
	{0}
};
BSMETA(castle, "Castles", "Замки", FirstCastle);

COMMAND(game_initialize)
{
	for(int i = 0; i < castles.count; i++)
		bsdelete(FirstCastle + i);
	castles.count = 0;
}

void draw::castle(int x, int y, int tile, int race, bool town, bool shadow)
{
	int index;
	switch(tile)
	{
	case Grass:
		index = 0;
		break;
	case Snow:
		index = 10;
		break;
	case Swamp:
		index = 20;
		break;
	case Lava:
		index = 30;
		break;
	case Desert:
		index = 40;
		break;
	case Dirt:
		index = 50;
		break;
	case Wastelands:
		index = 60;
		break;
	case Beach:
		index = 70;
		break;
	default:
		return;
	}
	for(int ii = 0; ii < 5; ++ii)
		image(x + ii * 32, y + 3 * 32, res::OBJNTWBA, index + ii);
	for(int ii = 0; ii < 5; ++ii)
		image(x + ii * 32, y + 4 * 32, res::OBJNTWBA, index + 5 + ii);
	// draw castle
	switch(race)
	{
	case Knight:
		index = 0;
		break;
	case Barbarian:
		index = 32;
		break;
	case Sorcerer:
		index = 64;
		break;
	case Warlock:
		index = 96;
		break;
	case Wizard:
		index = 128;
		break;
	case Necromancer:
		index = 160;
		break;
	default:
		break;
	}
	if(town)
		index += 16;
	if(shadow)
	{
		for(int iy = 0; iy < 4; iy++)
		{
			for(int ix = -2; ix <= 1; ix++)
			{
				int x1 = x + ix * 32;
				int y1 = y + iy * 32;
				if(iy == 3)
					x1 += 32;
				draw::image(x1, y1, res::OBJNTWSH, index + iy * 4 + (ix + 2));
			}
		}
	}
	// Town
	image(x + 2 * 32, y, res::OBJNTOWN, index);
	for(int ii = 0; ii < 5; ++ii)
		image(x + ii * 32, y + 1 * 32, res::OBJNTOWN, index + 1 + ii);
	for(int ii = 0; ii < 5; ++ii)
		image(x + ii * 32, y + 2 * 32, res::OBJNTOWN, index + 6 + ii);
	for(int ii = 0; ii < 5; ++ii)
		image(x + ii * 32, y + 3 * 32, res::OBJNTOWN, index + 11 + ii);
}

static struct castle_drawable_plugin : public drawable::plugin
{
	void selecting(drawable** result, rect screen, unsigned flags) override
	{
		auto mode = flags&DWMask;
		if(mode != DWObjects)
			return;
		auto p = result;
		for(auto& e : objects)
		{
			if(!e.type)
				continue;
			if(!e.getrect().intersect(screen))
				continue;
			*p++ = &e;
		}
		*p = 0;
	}
} drawable_plugin;