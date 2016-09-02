#include "main.h"

static struct token
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
	{{"Neutral"}}, {{"Blue"}}, {{"Green"}}, {{"Red"}}, {{"Yellow"}}, {{"Orange"}}, {{"Purple"}},
	//
	{{"Basic"}}, {{"Advanced"}}, {{"Expert"}},
};
static bsmeta::field fields[] = {
	BSREQ(token, name, Name, Text),
};
BSMETA(token, "Tokens", "Токены", 0);

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