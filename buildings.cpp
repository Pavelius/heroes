#include "main.h"

struct buildings_stats
{
	int			id;
	int			race;
	int			level;
	int			cost[LastResource - FirstResource + 1];
};
static buildings_stats data[] =
{
	{Empthy, 0, 0, {0, 0, 0, 0, 0, 0, 0}},
	// gold wood mercury ore sulfur crystal gems
	{ThievesGuild, 0, 1, {750, 5, 0, 0, 0, 0, 0}},
	{Tavern, 0, 1, {500, 5, 0, 0, 0, 0, 0}},
	{Shipyard, 0, 1, {2000, 20, 0, 0, 0, 0, 0}},
	{Well, 0, 1, {500, 0, 0, 0, 0, 0, 0}},
	{Statue, 0, 1, {1250, 0, 0, 5, 0, 0, 0}},
	{LeftTurret, 0, 1, {1500, 0, 0, 5, 0, 0, 0}},
	{RightTurret, 0, 1, {1500, 0, 0, 5, 0, 0, 0}},
	{MarketPlace, 0, 1, {500, 5, 0, 0, 0, 0, 0}},
	{Moat, 0, 1, {750, 0, 0, 0, 0, 0, 0}},
	{CastleInTown, 0, 1, {5000, 20, 0, 20, 0, 0, 0}},
	{Captain, 0, 1, {500, 0, 0, 0, 0, 0, 0}},
	{MageGuild, 0, 1, {2000, 5, 0, 5, 0, 0, 0}},
	//
	{Well2, Knight, 1, {1000, 0, 0, 0, 0, 0, 0}},
	{Well2, Barbarian, 1, {1000, 0, 0, 0, 0, 0, 0}},
	{Well2, Sorcerer, 1, {1000, 0, 0, 0, 0, 0, 0}},
	{Well2, Warlock, 1, {1000, 0, 0, 0, 0, 0, 0}},
	{Well2, Wizard, 1, {1000, 0, 0, 0, 0, 0, 0}},
	{Well2, Necromancer, 1, {1000, 0, 0, 0, 0, 0, 0}},
	//
	{SpecialBuilding, Knight, 1, {1500, 5, 0, 15, 0, 0, 0}},
	{SpecialBuilding, Barbarian, 1, {2000, 10, 0, 10, 0, 0, 0}},
	{SpecialBuilding, Sorcerer, 1, {1500, 0, 0, 0, 0, 10, 0}},
	{SpecialBuilding, Warlock, 1, {3000, 5, 0, 10, 0, 0, 0}},
	{SpecialBuilding, Wizard, 1, {1500, 5, 5, 5, 5, 5, 5}},
	{SpecialBuilding, Necromancer, 1, {1000, 0, 10, 0, 10, 0, 0}},
	//
	{Dwelving1, Knight, 1, {200, 0, 0, 0, 0, 0, 0}},
	{Dwelving2, Knight, 1, {1000, 0, 0, 0, 0, 0, 0}},
	{Dwelving2, Knight, 2, {1500, 5, 0, 0, 0, 0, 0}},
	{Dwelving3, Knight, 1, {1000, 0, 0, 5, 0, 0, 0}},
	{Dwelving3, Knight, 2, {1500, 0, 0, 5, 0, 0, 0}},
	{Dwelving4, Knight, 1, {2000, 10, 0, 10, 0, 0, 0}},
	{Dwelving4, Knight, 2, {2000, 5, 0, 5, 0, 0, 0}},
	{Dwelving5, Knight, 1, {3000, 20, 0, 0, 0, 0, 0}},
	{Dwelving5, Knight, 2, {3000, 10, 0, 0, 0, 0, 0}},
	{Dwelving6, Knight, 1, {5000, 20, 0, 0, 0, 20, 0}},
	{Dwelving6, Knight, 2, {5000, 10, 0, 0, 0, 10, 0}},
	//
	{Dwelving1, Barbarian, 1, {300, 0, 0, 0, 0, 0, 0}},
	{Dwelving2, Barbarian, 1, {800, 5, 0, 0, 0, 0, 0}},
	{Dwelving2, Barbarian, 2, {1200, 5, 0, 0, 0, 0, 0}},
	{Dwelving3, Barbarian, 1, {1000, 0, 0, 0, 0, 0, 0}},
	{Dwelving4, Barbarian, 1, {2000, 10, 0, 10, 0, 0, 0}},
	{Dwelving4, Barbarian, 2, {3000, 5, 0, 5, 0, 0, 0}},
	{Dwelving5, Barbarian, 1, {4000, 0, 0, 20, 0, 0, 0}},
	{Dwelving5, Barbarian, 2, {2000, 0, 0, 10, 0, 0, 0}},
	{Dwelving6, Barbarian, 1, {6000, 0, 0, 20, 0, 20, 0}},
	//
	{Dwelving1, Sorcerer, 1, {500, 5, 0, 0, 0, 0, 0}},
	{Dwelving2, Sorcerer, 1, {1000, 5, 0, 0, 0, 0, 0}},
	{Dwelving2, Sorcerer, 2, {1500, 5, 0, 0, 0, 0, 0}},
	{Dwelving3, Sorcerer, 1, {1500, 0, 0, 0, 0, 0, 0}},
	{Dwelving3, Sorcerer, 2, {1500, 5, 0, 0, 0, 0, 0}},
	{Dwelving4, Sorcerer, 1, {1500, 0, 0, 10, 0, 0, 0}},
	{Dwelving4, Sorcerer, 2, {1500, 0, 5, 0, 0, 0, 0}},
	{Dwelving5, Sorcerer, 1, {3000, 10, 0, 0, 0, 0, 10}},
	{Dwelving6, Sorcerer, 1, {10000, 0, 20, 30, 0, 0, 0}},
	//
	{Dwelving1, Warlock, 1, {500, 0, 0, 0, 0, 0, 0}},
	{Dwelving2, Warlock, 1, {1000, 0, 0, 10, 0, 0, 0}},
	{Dwelving3, Warlock, 1, {2000, 0, 0, 0, 0, 0, 0}},
	{Dwelving4, Warlock, 1, {3000, 0, 0, 0, 0, 0, 10}},
	{Dwelving4, Warlock, 2, {2000, 0, 0, 0, 0, 0, 5}},
	{Dwelving5, Warlock, 1, {4000, 0, 0, 0, 10, 0, 0}},
	{Dwelving6, Warlock, 1, {15000, 0, 0, 30, 20, 0, 0}},
	{Dwelving6, Warlock, 2, {5000, 0, 0, 5, 10, 0, 0}},
	{Dwelving6, Warlock, 3, {5000, 0, 0, 5, 10, 0, 0}},
	//
	{Dwelving1, Wizard, 1, {400, 0, 0, 0, 0, 0, 0}},
	{Dwelving2, Wizard, 1, {800, 0, 0, 0, 0, 0, 0}},
	{Dwelving3, Wizard, 1, {1500, 5, 0, 5, 0, 0, 0}},
	{Dwelving3, Wizard, 2, {1500, 0, 5, 0, 0, 0, 0}},
	{Dwelving4, Wizard, 1, {3000, 5, 0, 0, 0, 0, 0}},
	{Dwelving5, Wizard, 1, {3500, 5, 5, 5, 5, 5, 5}},
	{Dwelving5, Wizard, 2, {4000, 5, 0, 5, 0, 0, 0}},
	{Dwelving6, Wizard, 1, {12500, 5, 0, 5, 0, 0, 20}},
	{Dwelving6, Wizard, 2, {20000, 5, 0, 5, 0, 0, 20}},
	//
	{Dwelving1, Necromancer, 1, {400, 0, 0, 0, 0, 0, 0}},
	{Dwelving2, Necromancer, 1, {1000, 0, 0, 0, 0, 0, 0}},
	{Dwelving2, Necromancer, 2, {1000, 0, 0, 0, 0, 0, 0}},
	{Dwelving3, Necromancer, 1, {1500, 0, 0, 10, 0, 0, 0}},
	{Dwelving3, Necromancer, 2, {1500, 0, 0, 5, 0, 0, 0}},
	{Dwelving4, Necromancer, 1, {3000, 10, 0, 0, 0, 0, 0}},
	{Dwelving4, Necromancer, 2, {4000, 5, 0, 0, 0, 10, 10}},
	{Dwelving5, Necromancer, 1, {4000, 10, 0, 0, 10, 0, 0}},
	{Dwelving5, Necromancer, 2, {3000, 0, 0, 5, 0, 5, 0}},
	{Dwelving6, Necromancer, 1, {10000, 10, 5, 10, 5, 5, 5}},
};

static int nocost[LastResource - FirstResource + 1];

const void* game::getbuildingcost(int race, int building, int level)
{
	for(auto& e : data)
	{
		if(e.id != building)
			continue;
		if(e.race && e.race != race)
			continue;
		if(e.level != level)
			continue;
		return e.cost;
	}
	return nocost;
}

bool game::isrequipment(int race, int building, int level, int req, int req_level)
{
	switch(race)
	{
	case Knight:
		switch(building)
		{
		case Dwelving2:
			return req == Dwelving1;
		case Dwelving3:
			return req == Dwelving1
				|| req == Well;
		case Dwelving4:
			return req == Dwelving1
				|| req == Dwelving2
				|| req == Tavern;
		case Dwelving5:
			return req == Dwelving3
				|| req == Dwelving4;
		case Dwelving6:
			return req == Dwelving3
				|| req == Dwelving2;
		}
	case Barbarian:
		switch(building)
		{
		case Dwelving2:
		case Dwelving3:
		case Dwelving4:
			return req == Dwelving1;
		case Dwelving5:
			return req == Dwelving4;
		case Dwelving6:
			return req == Dwelving5;
		}
		break;
	case Sorcerer:
		switch(building)
		{
		case Dwelving2:
			if(level == 2)
				return req == Well;
			return req == Dwelving1
				|| req == Tavern;
		case Dwelving3:
			if(level == 2)
				return req == Dwelving4;
			return req == Dwelving1;
		case Dwelving4:
			return req == Dwelving3
				|| req == MageGuild;
		case Dwelving5:
			return req == Dwelving4;
		case Dwelving6:
			return req == Dwelving5;
		}
	case Necromancer:
		switch(building)
		{
		case Dwelving2:
			return req == Dwelving1;
		case Dwelving3:
			return req == Dwelving1;
		case Dwelving4:
			return req == Dwelving3
				|| req == ThievesGuild;
		case Dwelving5:
			if(level == 2)
				return req == MageGuild && req_level == 2;
			return req == Dwelving2
				|| req == MageGuild;
		case Dwelving6:
			return req == Dwelving5;
		}
		break;
	case Warlock:
		switch(building)
		{
		case Dwelving2:
			return req == Dwelving1;
		case Dwelving3:
			return req == Dwelving1;
		case Dwelving4:
			return req == Dwelving2;
		case Dwelving5:
			return req == Dwelving3;
		case Dwelving6:
			return req == Dwelving4
				|| req == Dwelving5;
		}
	case Wizard:
		switch(building)
		{
		case Dwelving2:
			if(level == 2)
				return req == Well;
			return req == Dwelving1;
		case Dwelving4:
			return req == Dwelving2;
		case Dwelving5:
			if(level == 2)
				return req == Well2;
			return req == Dwelving3
				|| req == MageGuild;
		case Dwelving6:
			return req == Dwelving4
				|| req == Dwelving5;
		}
		break;
	}
	return false;
}

int game::getunit(int race, int building, int level)
{
	switch(race)
	{
	case Knight:
		switch(building)
		{
		case Dwelving1: return Peasant;
		case Dwelving2: return (level == 2) ? Ranger : Archer;
		case Dwelving3: return (level == 2) ? VeteranPikeman : Pikeman;
		case Dwelving4: return (level == 2) ? MasterSwordsman : Swordsman;
		case Dwelving5: return (level == 2) ? Champion : Cavalry;
		case Dwelving6: return (level == 2) ? Crusader : Paladin;
		}
		break;
	case Barbarian:
		switch(building)
		{
		case Dwelving1: return Goblin;
		case Dwelving2: return (level == 2) ? OrcChief : Orc;
		case Dwelving3: return Wolf;
		case Dwelving4: return (level == 2) ? OgreLord : Ogre;
		case Dwelving5: return (level == 2) ? WarTroll : Troll;
		case Dwelving6: return Cyclop;
		}
		break;
	case Sorcerer:
		switch(building)
		{
		case Dwelving1: return Sprite;
		case Dwelving2: return (level == 2) ? BattleDwarf : Dwarf;
		case Dwelving3: return (level == 2) ? GrandElf : Elf;
		case Dwelving4: return (level == 2) ? GreaterDruid : Druid;
		case Dwelving5: return Unicorn;
		case Dwelving6: return Phoenix;
		}
		break;
	case Necromancer:
		switch(building)
		{
		case Dwelving1: return Skeleton;
		case Dwelving2: return (level == 2) ? MutantZombie : Zombie;
		case Dwelving3: return (level == 2) ? RoyalMummy : Mummy;
		case Dwelving4: return (level == 2) ? VampireLord : Vampire;
		case Dwelving5: return (level == 2) ? PowerLich : Lich;
		case Dwelving6: return BoneDragon;
		}
		break;
	case Wizard:
		switch(building)
		{
		case Dwelving1: return Halfling;
		case Dwelving2: return Boar;
		case Dwelving3: return (level == 2) ? SteelGolem : IronGolem;
		case Dwelving4: return Roc;
		case Dwelving5: return (level == 2) ? ArchMage : Mage;
		case Dwelving6: return (level == 2) ? Titan : Giant;
		}
		break;
	case Warlock:
		switch(building)
		{
		case Dwelving1: return Centaur;
		case Dwelving2: return Gargoyle;
		case Dwelving3: return Griffin;
		case Dwelving4: return (level == 2) ? MinotaurKing : Minotaur;
		case Dwelving5: return Hydra;
		case Dwelving6:
			if(level == 3)
				return BlackDragon;
			else if(level == 2)
				return RedDragon;
			return GreenDragon;
		}
		break;
	}
	return 0;
}

const char* game::getbuildingname(int race, int building, int level)
{
	static char temp[64];
	static const char* buildings[][2] = {
		{"Castle"},
		{"Thieves Guild"},
		{"Tavern"},
		{"Shipyard"},
		{"Well"},
		{"Statue"},
		{"Left Turret"},
		{"Right Turret"},
		{"Moat"},
		{"Marketplace"},
		{"Captain quarter"},
	};
	static const char* well2[][2] = {
		{"Garbage Heap", "Куча мусора"},
		{"Farm", "Ферма"},
		{"Skull Pile", "Груда черепов"},
		{"Crystal Garden", "Кристальный сад"},
		{"Waterfall", "Водопад"},
		{"Orchard", "Газон"},
	};
	static const char* special[][2] = {
		{"Collesium", "Коллизей"},
		{"Fortification", "Укрепление"},
		{"Storm", "Буря"},
		{"Rainbow", "Радуга"},
		{"Dungeon", "Подземелье"},
		{"Library", "Библиотека"},
	};
	static const char* mageguild[][2] = {
		{"Mage Guild 1", "Гильдия магов 1"},
		{"Mage Guild 2", "Гильдия магов 2"},
		{"Mage Guild 3", "Гильдия магов 3"},
		{"Mage Guild 4", "Гильдия магов 4"},
		{"Mage Guild 5", "Гильдия магов 5"},
	};
	static const char* dwellings[][2] =
	{
		{"Hut", "Хижина"}, {"Thatched Hut", "Соломенная Хижина"}, {"Excavation", "Захоронения"}, {"Treehouse", "Дома на деревьях"}, {"Cave", "Пещера"}, {"Habitat", "Норы"},
		{"Stick Hut", "Хижина"}, {"Archery Range", "Полигон"}, {"Graveyard", "Кладбище"}, {"Cottage", "Коттедж"}, {"Crypt", "Крипта"}, {"Pen", "Загон"},
		{"Den", "Логово"}, {"Blacksmith", "Кузница"}, {"Pyramid", "Пирамида"}, {"Archery Range", "Полигон"}, {"Nest", "Гнездо"}, {"Foundry", "Фабрика"},
		{"Adobe", "Логово"}, {"Armory", "Оружейная"}, {"Mansion", "Особняк"}, {"Stonehenge", "Камни"}, {"Maze", "Лабиринт"}, {"Cliff Nest", "Гнездо на утесе"},
		{"Bridge", "Мост"}, {"Jousting Arena", "Турнирная Арена"}, {"Mausoleum", "Мавзолей"}, {"Fenced Meadow", "Загон"}, {"Swamp", "Болото"}, {"Ivory Tower", "Башня магов"},
		{"Pyramid", "Пирамида"}, {"Cathedral", "Собор"}, {"Laboratory", "Лаборатория"}, {"Red Tower", "Красная Башня"}, {"Green Tower", "Зеленая Башня"}, {"Cloud Castle", "Небесный замок"}
	};
	static const char* warlock_dwelling6[][2] = {
		{"Red Tower", "Красная башня"},
		{"Black Tower", "Черная башня"},
	};
	if(building >= CastleInTown && building <= Captain)
		return buildings[building - CastleInTown][locale];
	if(building >= Dwelving1 && building <= Dwelving6)
	{
		if(level<=1)
			return dwellings[race - Barbarian + (building - Dwelving1) * 6][locale];
		if(level>=2 && race==Warlock && building==Dwelving6)
			return warlock_dwelling6[level-2][locale];
		szprint(temp, "%1.%2", szt("Upg", "Ул"), dwellings[race - Barbarian + (building - Dwelving1) * 6][locale]);
		return temp;
	}
	if(building == Well2)
		return well2[race - Barbarian][locale];
	if(building == SpecialBuilding)
		return special[race - Barbarian][locale];
	if(building == MageGuild)
		return mageguild[level][locale];
	return "";
}

const char* game::getbuildinginfo(int type, int building, int level)
{
	static char temp[260];
	const char* buildings[][2] = {
		{"The Castle improves town defense and increases income to 1000 gold per day.", ""},
		{"The Thieves' Guild provides information on enemy players. Thieves' Guilds can also provide scouting information on enemy towns.", ""},
		{"The Tavern increases morale for troops defending the castle.", ""},
		{"The Shipyard allows ships to be built.", ""},
		{"The Well increases the growth rate of all dwellings by 2 creatures per week.", ""},
		{"The Statue increases your town's income by 250 per day.", ""},
		{"The Left Turret provides extra firepower during castle combat.", ""},
		{"The Right Turret provides extra firepower during castle combat."},
		{"The Moat slows attacking units. Any unit entering the moat must end its turn there and becomes more vulnerable to attack."},
		{"The Marketplace can be used to convert one type of resource into another. The more marketplaces you control, the better the exchange rate."},
		{"The Captain's Quarters provides a captain to assist in the castle's defense when no hero is present."},
	};
	const char* mageguild[2] = {"The Mage Guild allows heroes to learn spells and replenish their spell points.", ""};
	const char* dwelwings[2] = {"Allow to recruit %1.", "Позволяет нанимать %1."};
	if(building >= CastleInTown && building <= Captain)
		return buildings[building - CastleInTown][locale];
	if(building == MageGuild)
		return mageguild[locale];
	if(building >= Dwelving1 && building <= Dwelving6)
	{
		szprint(temp, dwelwings[locale], bsgets(game::getunit(type, building, level), NameMulti));
		return temp;
	}
	return "";
}

char* game::getbuilding(char* result, tokens race, int building, int level)
{
	szprint(result, "\n$(%1i/%2i)", building, race);
	zcat(result, game::getbuildinginfo(race, building, level));
	return result;
}

int game::getbuildingmaxlevel(int race, int building)
{
	switch(building)
	{
	case Dwelving2:
		switch(race)
		{
		case Knight:
		case Barbarian:
		case Sorcerer:
		case Necromancer:
			return 2;
		default:
			return 1;
		}
	case Dwelving3:
		switch(race)
		{
		case Knight:
		case Sorcerer:
		case Necromancer:
		case Wizard:
			return 2;
		default:
			return 1;
		}
	case Dwelving4:
		switch(race)
		{
		case Barbarian:
		case Knight:
		case Necromancer:
		case Sorcerer:
		case Warlock:
			return 2;
		default:
			return 1;
		}
	case Dwelving5:
		switch(race)
		{
		case Barbarian:
		case Knight:
		case Necromancer:
		case Wizard:
			return 2;
		default:
			return 1;
		}
	case Dwelving6:
		switch(race)
		{
		case Knight:
		case Wizard:
			return 2;
		case Warlock:
			return 3;
		default:
			return 1;
		}
	case MageGuild:
		return 5;
	default:
		return 1;
	}
}