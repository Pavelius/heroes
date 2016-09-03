#include "main.h"

const int FBuildThisTurn = 0x00000001;
const int CastleIncome = 1000;
const int CastleIncomeDungeon = 500;
const int CastleIncomeStatue = 250;
static struct castle
{
	tokens			player;
	tokens			type;
	int				index;
	char			name[14];
	unsigned		flags;
	unsigned char	dwellings[LastBuilding - FirstBuilding + 1];
	short unsigned	recruiters[RecruitLast - Recruit + 1];
	short unsigned	army[LastTroopsIndex - FirstTroopsIndex + 1];
} objects[LastCastle - FirstCastle + 1];
static bsmeta::field fields[] = {
	BSREQ(castle, player, Player, Number),
	BSREQ(castle, type, Type, Number),
	BSREQ(castle, index, Index, Number),
	BSINT(castle, dwellings, FirstBuilding, Number),
	BSINT(castle, army, FirstTroopsIndex, Number),
	BSREQ(castle, name, Name, Text),
	{0}
};
BSMETA(castle, "Castles", "Замки", FirstCastle);

//if(id >= (int)Information)
//{
//	static char temp[256];
//	switch(id - Information + CastleInTown)
//	{
//	case CastleInTown:
//		return szprint(temp, szt("The Castle improves town defense and increases income to %1i gold per day.", ""), 1000);
//	case Statue:
//		return szprint(temp, szt("The Statue increases your town's income by %1i per day.", ""), CastleIncomeStatue);
//	case MageGuild:
//		return szt("The Mage Guild allows heroes to learn spells and replenish their spell points.", "");
//	case ThievesGuild:
//		return szt("The Thieves' Guild provides information on enemy players. Thieves' Guilds can also provide scouting information on enemy towns.", "");
//	case Tavern:
//		return szt("The Tavern increases morale for troops defending the castle.", "");
//	case Shipyard:
//		return szt("The Shipyard allows ships to be built.", "");
//	case Well:
//		return szprint(temp, szt("The Well increases the growth rate of all dwellings by %1i creatures per week.", ""), CastleIncomeWell);
//	case LeftTurret:
//		return szt("The Left Turret provides extra firepower during castle combat.", "");
//	case RightTurret:
//		return szt("The Right Turret provides extra firepower during castle combat.", "");
//	case MarketPlace:
//		return szt("The Marketplace can be used to convert one type of resource into another. The more marketplaces you control, the better the exchange rate.", "");
//	case Moat:
//		return szt("The Moat slows attacking units. Any unit entering the moat must end its turn there and becomes more vulnerable to attack.", "");
//	case Tent:
//		return szt("The Tent provides workers to build a castle, provided the materials and the gold are available.", "");
//	case Captain:
//		return szt("The Captain's Quarters provides a captain to assist in the castle's defense when no hero is present.", "");
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