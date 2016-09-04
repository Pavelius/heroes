#include "main.h"

struct buildings_stats
{
	int			id;
	int			race;
	int			cost[LastResource - FirstResource + 1];
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

const int* game::getcost(int race, int building)
{
	for(auto& e : data)
	{
		if(e.id != building)
			continue;
		if(e.race && e.race != race)
			continue;
		return e.cost;
	}
	return data[0].cost;
}

bool game::isrequipment(int race, int building, int req, int level)
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
			return req == Dwelving1
				|| req == Tavern;
		case Upgrade2:
			return req == Well;
		case Dwelving3:
			return req == Dwelving1;
		case Upgrade3:
			return req == Dwelving4;
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
			return req == Dwelving2
				|| req == MageGuild;
		case Upgrade5:
			return req == MageGuild && level == 2;
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
			return req == Dwelving1;
		case Upgrade3:
			return req == Well;
		case Dwelving4:
			return req == Dwelving2;
		case Dwelving5:
			return req == Dwelving3
				|| req == MageGuild;
		case Upgrade5:
			return req == Well2;
		case Dwelving6:
			return req == Dwelving4
				|| req == Dwelving5;
		}
		break;
	}
	return false;
}

int game::getunit(int race, int building)
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

const char* game::getbuildingname(int type, int building, int level)
{
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
	if(building >= CastleInTown && building <= Captain)
		return buildings[building - CastleInTown][locale];
	if(building >= Dwelving1 && building <= Dwelving6)
		return dwellings[type - Barbarian + (building - Dwelving1) * 6][locale];
	if(building == Well2)
		return well2[type - Barbarian][locale];
	if(building == SpecialBuilding)
		return special[type - Barbarian][locale];
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
		szprint(temp, dwelwings[locale], bsgets(game::getunit(type, building), NameMulti));
		return temp;
	}
	return "";
}

char* game::getbuilding(char* result, tokens race, int building)
{
	auto cost = game::getcost(race, building);
	szprint(result, "\n$(%1i/%2i)", building, race);
	zcat(result, game::getbuildinginfo(race, building, 1));
	return result;
}