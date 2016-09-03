#include "main.h"

static struct token_string
{
	const char*	name[2];
} objects[] = {
	{{"Empthy"}},
	// Basic ablity
	{{"Attack"}},
	{{"Defence"}},
	{{"Spell Power"}},
	{{"Knowledge"}},
	// Skills
	{{"Pathfinding", "Поиск пути"}},
	{{"Archery", "Лучник"}},
	{{"Logistics", "Логистика"}},
	{{"Scounting", "Разведка"}},
	{{"Diplomacy", "Дипломатия"}},
	{{"Navigation"}},
	{{"Leadership"}},
	{{"Wisdom", "Мудрость"}},
	{{"Mysticism", "Мистицизм"}},
	{{"Luck"}},
	{{"Ballistics", "Баллистика"}},
	{{"Eagle Eye", "Орлинный глаз"}},
	{{"Necromancy", "Некромантия"}},
	{{"Estates", "Доход"}},
	//
	{{"Luck"}},
	{{"Cursed"}},
	{{"Awful"}},
	{{"Bad"}},
	{{"Normal"}},
	{{"Good"}},
	{{"Great"}},
	{{"Irish"}},
	//
	{{"Morale"}},
	{{"Treason"}},
	{{"Awful"}},
	{{"Poor"}},
	{{"Normal"}},
	{{"Good"}},
	{{"Great"}},
	{{"Blood"}},
	//
	{{"Speed"}},
	{{"Crawling"}},
	{{"Very Slow"}},
	{{"Slow"}},
	{{"Average"}},
	{{"Fast"}},
	{{"Very Fast"}},
	{{"Ultra Fast"}},
	//
	{{"Easy"}}, {{"Normal"}}, {{"Hard"}}, {{"Very Hard"}}, {{"Impossible"}},
	{{"Beginner"}}, {{"Normal"}}, {{"Hard"}}, {{"Expert"}},
	{{"Barbarian"}}, {{"Knight"}}, {{"Necromancer"}}, {{"Sorcerer"}}, {{"Warlock"}}, {{"Wizard"}}, {{"Elementals"}},
	{{"Magic"}}, {{"Magic (max)"}},
	{{"Hits"}}, {{"Hits"}}, {{"Shoots"}},
	{{"Gold"}}, {{"Wood"}}, {{"Mercury"}}, {{"Ore"}}, {{"Sulfur"}}, {{"Crystal"}}, {{"Gems"}},
	//
	{{"Basic"}}, {{"Advanced"}}, {{"Expert"}},
};
static bsmeta::field fields[] = {
	BSREQ(token_string, name, Name, Text),
};
BSMETA(token_string, "Tokens", "Токены", 0);

static int object_get(int rec, int id)
{
	switch(id)
	{
	case Rating:
		switch(rec)
		{
		case EasyDifficulty: return 70;
		case NormalDifficulty: return 100;
		case HardDifficulty: return 120;
		case VeryHardDifficulty: return 160;
		case ImpossibleDifficulty: return 180;
		default: return 0;
		}
	case Attack:
		switch(rec)
		{
		case BarbarianCaptain: return 3;
		case KnightCaptain: return 2;
		case NecromancerCaptain: return 1;
		default: return 0;
		}
	case Defence:
		switch(rec)
		{
		case KnightCaptain: return 2;
		case BarbarianCaptain:
		case WizardCaptain: return 1;
		default: return 0;
		}
	case Wisdow:
		switch(rec)
		{
		case SorcererCaptain: return 4;
		case WizardCaptain:
		case WarlockCaptain:
		case NecromancerCaptain: return 3;
		default: return 1;
		}
	case SpellPower:
		switch(rec)
		{
		case WarlockCaptain: return 4;
		case SorcererCaptain:
		case WizardCaptain:
		case NecromancerCaptain: return 3;
		default: return 1;
		}
	default:
		return 0;
	}
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