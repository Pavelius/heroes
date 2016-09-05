#include "main.h"

const int FBuildThisTurn = 0x00000001;
const int CastleIncome = 1000;
const int CastleIncomeDungeon = 500;
const int CastleIncomeStatue = 250;
static struct castle
{
	tokens				player;
	tokens				type;
	short unsigned		index;
	char				name[14];
	unsigned char		moved;
	short unsigned		creatures[LastCreatureCount - FirstCreatureCount +1];
	unsigned char		dwellings[LastBuilding - FirstBuilding + 1];
	unsigned char		spells[LastSpell - FirstSpell + 1];
	unsigned short		army[LastTroopsIndex - FirstArtifactIndex + 1];
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