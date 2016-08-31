#include "main.h"

struct buildings_stats
{
	int			id;
	int			race;
	struct cost	cost;
};

static buildings_stats data[] =
{
	{Empthy, 0, {0, 0, 0, 0, 0, 0, 0}},
	// gold wood mercury ore sulfur crystal gems
	{ThievesGuild, 0, {750, 5, 0, 0, 0, 0, 0}},
	{Tavern, 0, {500, 5, 0, 0, 0, 0, 0}},
	{Shipyard, 0, {2000, 20, 0, 0, 0, 0, 0}},
	{Well, 0, {500, 0, 0, 0, 0, 0, 0}},
	{Statue, 0, {1250, 0, 0, 5, 0, 0, 0}},
	{LeftTurret, 0, {1500, 0, 0, 5, 0, 0, 0}},
	{RightTurret, 0, {1500, 0, 0, 5, 0, 0, 0}},
	{MarketPlace, 0, {500, 5, 0, 0, 0, 0, 0}},
	{Moat, 0, {750, 0, 0, 0, 0, 0, 0}},
	{CastleInTown, 0, {5000, 20, 0, 20, 0, 0, 0}},
	{Captain, 0, {500, 0, 0, 0, 0, 0, 0}},
	{MageGuild, 0, {2000, 5, 0, 5, 0, 0, 0}},
	//
	{Well2, Knight, {1000, 0, 0, 0, 0, 0, 0}},
	{Well2, Barbarian, {1000, 0, 0, 0, 0, 0, 0}},
	{Well2, Sorcerer, {1000, 0, 0, 0, 0, 0, 0}},
	{Well2, Warlock, {1000, 0, 0, 0, 0, 0, 0}},
	{Well2, Wizard, {1000, 0, 0, 0, 0, 0, 0}},
	{Well2, Necromancer, {1000, 0, 0, 0, 0, 0, 0}},
	//
	{SpecialBuilding, Knight, {1500, 5, 0, 15, 0, 0, 0}},
	{SpecialBuilding, Barbarian, {2000, 10, 0, 10, 0, 0, 0}},
	{SpecialBuilding, Sorcerer, {1500, 0, 0, 0, 0, 10, 0}},
	{SpecialBuilding, Warlock, {3000, 5, 0, 10, 0, 0, 0}},
	{SpecialBuilding, Wizard, {1500, 5, 5, 5, 5, 5, 5}},
	{SpecialBuilding, Necromancer, {1000, 0, 10, 0, 10, 0, 0}},
	//{BUILD_SHRINE, Necromancer, {4000, 10, 0, 0, 0, 10, 0}},
	//
	{Dwelving1, Knight, {200, 0, 0, 0, 0, 0, 0}},
	{Dwelving2, Knight, {1000, 0, 0, 0, 0, 0, 0}},
	{Upgrade2, Knight, {1500, 5, 0, 0, 0, 0, 0}},
	{Dwelving3, Knight, {1000, 0, 0, 5, 0, 0, 0}},
	{Upgrade3, Knight, {1500, 0, 0, 5, 0, 0, 0}},
	{Dwelving4, Knight, {2000, 10, 0, 10, 0, 0, 0}},
	{Upgrade4, Knight, {2000, 5, 0, 5, 0, 0, 0}},
	{Dwelving5, Knight, {3000, 20, 0, 0, 0, 0, 0}},
	{Upgrade5, Knight, {3000, 10, 0, 0, 0, 0, 0}},
	{Dwelving6, Knight, {5000, 20, 0, 0, 0, 20, 0}},
	{Upgrade6, Knight, {5000, 10, 0, 0, 0, 10, 0}},
	//
	{Dwelving1, Barbarian, {300, 0, 0, 0, 0, 0, 0}},
	{Dwelving2, Barbarian, {800, 5, 0, 0, 0, 0, 0}},
	{Upgrade2, Barbarian, {1200, 5, 0, 0, 0, 0, 0}},
	{Dwelving3, Barbarian, {1000, 0, 0, 0, 0, 0, 0}},
	{Dwelving4, Barbarian, {2000, 10, 0, 10, 0, 0, 0}},
	{Upgrade4, Barbarian, {3000, 5, 0, 5, 0, 0, 0}},
	{Dwelving5, Barbarian, {4000, 0, 0, 20, 0, 0, 0}},
	{Upgrade5, Barbarian, {2000, 0, 0, 10, 0, 0, 0}},
	{Dwelving6, Barbarian, {6000, 0, 0, 20, 0, 20, 0}},
	//
	{Dwelving1, Sorcerer, {500, 5, 0, 0, 0, 0, 0}},
	{Dwelving2, Sorcerer, {1000, 5, 0, 0, 0, 0, 0}},
	{Upgrade2, Sorcerer, {1500, 5, 0, 0, 0, 0, 0}},
	{Dwelving3, Sorcerer, {1500, 0, 0, 0, 0, 0, 0}},
	{Upgrade3, Sorcerer, {1500, 5, 0, 0, 0, 0, 0}},
	{Dwelving4, Sorcerer, {1500, 0, 0, 10, 0, 0, 0}},
	{Upgrade4, Sorcerer, {1500, 0, 5, 0, 0, 0, 0}},
	{Dwelving5, Sorcerer, {3000, 10, 0, 0, 0, 0, 10}},
	{Dwelving6, Sorcerer, {10000, 0, 20, 30, 0, 0, 0}},
	//
	{Dwelving1, Warlock, {500, 0, 0, 0, 0, 0, 0}},
	{Dwelving2, Warlock, {1000, 0, 0, 10, 0, 0, 0}},
	{Dwelving3, Warlock, {2000, 0, 0, 0, 0, 0, 0}},
	{Dwelving4, Warlock, {3000, 0, 0, 0, 0, 0, 10}},
	{Upgrade4, Warlock, {2000, 0, 0, 0, 0, 0, 5}},
	{Dwelving5, Warlock, {4000, 0, 0, 0, 10, 0, 0}},
	{Dwelving6, Warlock, {15000, 0, 0, 30, 20, 0, 0}},
	{Upgrade6, Warlock, {5000, 0, 0, 5, 10, 0, 0}},
	{Upgrade62, Warlock, {5000, 0, 0, 5, 10, 0, 0}},
	//
	{Dwelving1, Wizard, {400, 0, 0, 0, 0, 0, 0}},
	{Dwelving2, Wizard, {800, 0, 0, 0, 0, 0, 0}},
	{Dwelving3, Wizard, {1500, 5, 0, 5, 0, 0, 0}},
	{Upgrade3, Wizard, {1500, 0, 5, 0, 0, 0, 0}},
	{Dwelving4, Wizard, {3000, 5, 0, 0, 0, 0, 0}},
	{Dwelving5, Wizard, {3500, 5, 5, 5, 5, 5, 5}},
	{Upgrade5, Wizard, {4000, 5, 0, 5, 0, 0, 0}},
	{Dwelving6, Wizard, {12500, 5, 0, 5, 0, 0, 20}},
	{Upgrade6, Wizard, {12500, 5, 0, 5, 0, 0, 20}},
	//
	{Dwelving1, Necromancer, {400, 0, 0, 0, 0, 0, 0}},
	{Dwelving2, Necromancer, {1000, 0, 0, 0, 0, 0, 0}},
	{Upgrade2, Necromancer, {1000, 0, 0, 0, 0, 0, 0}},
	{Dwelving3, Necromancer, {1500, 0, 0, 10, 0, 0, 0}},
	{Upgrade3, Necromancer, {1500, 0, 0, 5, 0, 0, 0}},
	{Dwelving4, Necromancer, {3000, 10, 0, 0, 0, 0, 0}},
	{Upgrade4, Necromancer, {4000, 5, 0, 0, 0, 10, 10}},
	{Dwelving5, Necromancer, {4000, 10, 0, 0, 10, 0, 0}},
	{Upgrade5, Necromancer, {3000, 0, 0, 5, 0, 5, 0}},
	{Dwelving6, Necromancer, {10000, 10, 5, 10, 5, 5, 5}},
};

cost& buildings::gcost(int race, int building, int level)
{
	for(auto& e : data)
	{
		if((e.race && e.race!=race) || e.id!=building)
			continue;
		return e.cost;
	}
	return data[0].cost;
}

bool buildings::requipment(int race, int building, int req, int level)
{
	switch(race)
	{
	case Knight:
		switch(building)
		{
		case Dwelving2:
			return req==Dwelving1;
		case Dwelving3:
			return req==Dwelving1
				|| req==Well;
		case Dwelving4:
			return req==Dwelving1
				|| req==Dwelving2
				|| req==Tavern;
		case Dwelving5:
			return req==Dwelving3
				|| req==Dwelving4;
		case Dwelving6:
			return req==Dwelving3
				|| req==Dwelving2;
		}
	case Barbarian:
		switch(building)
		{
		case Dwelving2:
		case Dwelving3:
		case Dwelving4:
			return req==Dwelving1;
		case Dwelving5:
			return req==Dwelving4;
		case Dwelving6:
			return req==Dwelving5;
		}
		break;
	case Sorcerer:
		switch(building)
		{
		case Dwelving2:
			return req==Dwelving1
				|| req==Tavern;
		case Upgrade2:
			return req==Well;
		case Dwelving3:
			return req==Dwelving1;
		case Upgrade3:
			return req==Dwelving4;
		case Dwelving4:
			return req==Dwelving3
				|| req==MageGuild;
		case Dwelving5:
			return req==Dwelving4;
		case Dwelving6:
			return req==Dwelving5;
		}
	case Necromancer:
		switch(building)
		{
		case Dwelving2:
			return req==Dwelving1;
		case Dwelving3:
			return req==Dwelving1;
		case Dwelving4:
			return req==Dwelving3
				|| req==ThievesGuild;
		case Dwelving5:
			return req==Dwelving2
				|| req==MageGuild;
		case Upgrade5:
			return req==MageGuild && level==2;
		case Dwelving6:
			return req==Dwelving5;
		}
		break;
	case Warlock:
		switch(building)
		{
		case Dwelving2:
			return req==Dwelving1;
		case Dwelving3:
			return req==Dwelving1;
		case Dwelving4:
			return req==Dwelving2;
		case Dwelving5:
			return req==Dwelving3;
		case Dwelving6:
			return req==Dwelving4
				|| req==Dwelving5;
		}
	case Wizard:
		switch(building)
		{
		case Dwelving2:
			return req==Dwelving1;
		case Upgrade3:
			return req==Well;
		case Dwelving4:
			return req==Dwelving2;
		case Dwelving5:
			return req==Dwelving3
				|| req==MageGuild;
		case Upgrade5:
			return req==Well2;
		case Dwelving6:
			return req==Dwelving4
				|| req==Dwelving5;
		}
		break;
	}
	return false;
}

int buildings::unit(int race, int building)
{
	switch(race)
	{
	case Knight:
		switch(building)
		{
		case Dwelving1: return Peasant;
		case Dwelving2: return Archer; case Upgrade2: return Ranger;
		case Dwelving3: return Pikeman; case Upgrade3: return VeteranPikeman;
		case Dwelving4: return Swordsman; case Upgrade4: return MasterSwordsman;
		case Dwelving5: return Cavalry;
		case Dwelving6: return Paladin; case Upgrade6: return Crusader;
		}
		break;
	case Barbarian:
		switch(building)
		{
		case Dwelving1: return Goblin;
		case Dwelving2: return Orc; case Upgrade2: return OrcChief;
		case Dwelving3: return Wolf;
		case Dwelving4: return Ogre; case Upgrade4: return OgreLord;
		case Dwelving5: return Troll; case Upgrade5: return WarTroll;
		case Dwelving6: return Cyclop;
		}
		break;
	case Sorcerer:
		switch(building)
		{
		case Dwelving1: return Sprite;
		case Dwelving2: return Dwarf; case Upgrade2: return BattleDwarf;
		case Dwelving3: return Elf; case Upgrade3: return GrandElf;
		case Dwelving4: return Druid; case Upgrade4: return GreaterDruid;
		case Dwelving5: return Unicorn;
		case Dwelving6: return Phoenix;
		}
		break;
	case Necromancer:
		switch(building)
		{
		case Dwelving1: return Skeleton;
		case Dwelving2: return Zombie; case Upgrade2: return MutantZombie;
		case Dwelving3: return Mummy; case Upgrade3: return RoyalMummy;
		case Dwelving4: return Vampire; case Upgrade4: return VampireLord;
		case Dwelving5: return Lich; case Upgrade5: return PowerLich;
		case Dwelving6: return BoneDragon;
		}
		break;
	case Wizard:
		switch(building)
		{
		case Dwelving1: return Halfling;
		case Dwelving2: return Boar;
		case Dwelving3: return IronGolem; case Upgrade3: return SteelGolem;
		case Dwelving4: return Roc;
		case Dwelving5: return Mage; case Upgrade5: return ArchMage;
		case Dwelving6: return Giant; case Upgrade6: return Titan;
		}
		break;
	case Warlock:
		switch(building)
		{
		case Dwelving1: return Centaur;
		case Dwelving2: return Gargoyle;
		case Dwelving3: return Griffin;
		case Dwelving4: return Minotaur; case Upgrade4: return MinotaurKing;
		case Dwelving5: return Hydra;
		case Dwelving6: return GreenDragon; case Upgrade6: return RedDragon; case Upgrade62: return BlackDragon;
		}
		break;
	}
	return 0;
}