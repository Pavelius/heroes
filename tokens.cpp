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
	default:
		return 0;
	}
}