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
	char				name[14];
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
		return{(short)(map::i2x(index) * 32) - 2 * 32, (short)(map::i2y(index) * 32)};
	}

	void painting(point camera, unsigned paint_flags) const override
	{
		auto pt = getpos() - camera;
		auto tl = map::gettile(index);
		auto town = dwellings[Castle - FirstBuilding] != 2;
		pt.x -= 2 * 32;
		pt.y -= 3 * 32;
		// TODO: Paint Shadow
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

void draw::castle(int x, int y, int tile, int race, bool town)
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

//if(id >= (int)Information)
//{
//	static char temp[256];
//	switch(id - Information + CastleInTown)
//	{
//		return szt("The Tent provides workers to build a castle, provided the materials and the gold are available.", "");
//	case Captain:
//		return szt(, "");
//	case Well2:
//		switch(objects[rec - FirstCastle].type)
//		{
//		case Knight:
//			return szprint(temp, szt("The Farm increases production of Peasants by %1i per week.", ""), CastleIncomeWell2);
//		case Barbarian:
//			return szprint(temp, szt("The Garbage Heap increases production of Goblins by %1i per week.", ""), CastleIncomeWell2);
//		case Sorcerer:
//			return szprint(temp, szt("The Crystal Garden increases production of Sprites by %1i per week.", ""), CastleIncomeWell2);
//		case Warlock:
//			return szprint(temp, szt("The Waterfall increases production of Centaurs by %1i per week.", ""), CastleIncomeWell2);
//		case Wizard:
//			return szprint(temp, szt("The Orchard increases production of Halflings by %1i per week.", ""), CastleIncomeWell2);
//		default:
//			return szprint(temp, szt("The Skull Pile increases production of Skeletons by %1i per week.", ""), CastleIncomeWell2);
//		};
//	case SpecialBuilding:
//		switch(objects[rec - FirstCastle].type)
//		{
//		case Knight:
//		case Barbarian:
//		case Sorcerer:
//		case Warlock:
//			break;
//		case Wizard:
//			return szt("Open one additional spell's scroll for each level of magic guild.", "");
//		default:
//			return szt("Increase Spell power by 2 when hero or captain defend this castle.", "");
//		}
//	case Dwelving1:
//	case Dwelving2:
//	case Dwelving3:
//	case Dwelving4:
//	case Dwelving5:
//	case Dwelving6:
//		return szprint(temp, szt("%1 allow recruit %2.", "%1 позволяет нанимать %2."),
//			bsgets(rec, id - Information + CastleInTown),
//			bsgets(game::getunit(objects[rec - FirstCastle].type, id - Information + CastleInTown), NameMulti));