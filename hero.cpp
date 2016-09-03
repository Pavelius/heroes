#include "main.h"

struct hero
{
	const char*			name[2];
	int					attack;
	int					defence;
	int					spellpower;
	int					wisdow;
	short int			spell_points;
	short int			move_points;
	int					index;
	int					index_move;
	unsigned char		direction;
	unsigned char		portrait;
	int					player, recruit;
	unsigned			experience;
	unsigned char		skills[LastSkill - FirstSkill + 1];
	unsigned char		spells[LastSpell - FirstSpell + 1];
	unsigned short		params[LastTroopsIndex - FirstArtifactIndex + 1];
} objects[LastHero - FirstHero + 1] = {
	{{"Lord Killburn", "Лорд Килбурн"}},{{"Sir Gallanth", "Сэр Галлант"}}, {{"Ector", "Эктор"}}, {{"Gvenneth", "Гвиннет"}}, {{"Tyro", "Тиро"}},
	{{"Ambrose", "Амброуз"}}, {{"Ruby", "Руби"}}, {{"Maximus", "Максимус"}}, {{"Dimitry", "Димитрий"}},
	//
	{{"Thundax", "Громотопор"}}, {{"Fineous", "Прекраснейший"}}, {{"Jojosh", "Джоджош"}}, {{"Crag Hack", "Крэг Хек"}}, {{"Jezebel", "Жезебель"}},
	{{"Jaclyn", "Жаклин"}}, {{"Ergon", "Эргон"}}, {{"Tsabu", "Тсабу"}}, {{"Atlas", "Атлас"}},
	//
	{{"Astra", "Астра"}}, {{"Natasha", "Наташа"}}, {{"Troyan", "Троян"}}, {{"Vatawna", "Ваттавна"}}, {{"Rebecca", "Ребека"}},
	{{"Gem", "Джем"}}, {{"Ariel", "Ариэль"}}, {{"Carlawn", "Карлавн"}}, {{"Luna", "Луна"}},
	//
	{{"Arie", "Ари"}}, {{"Alamar", "Аламар"}}, {{"Vesper", "Веспер"}}, {{"Crodo", "Кродо"}}, {{"Barok", "Барок"}},
	{{"Kastore", "Кастор"}}, {{"Agar", "Агар"}}, {{"Falagar", "Фалагар"}}, {{"Wrathmont", "Врасмонт"}},
	//
	{{"Myra", "Мира"}}, {{"Flint", "Флинт"}}, {{"Dawn", "Давн"}}, {{"Halon", "Халон"}}, {{"Myrini", "Мирини"}},
	{{"Wilfrey", "Вилфрей"}}, {{"Sarakin", "Саракин"}}, {{"Kalindra", "Калиндра"}}, {{"Mandigal", "Мандигал"}},
	//
	{{"Zom", "Зом"}}, {{"Darlana", "Дарлана"}}, {{"Zam", "Зам"}}, {{"Ranloo", "Ранлу"}}, {{"Charity", "Чарити"}},
	{{"Rialdo", "Риалдо"}}, {{"Roxana", "Роксана"}}, {{"Sandro", "Сандро"}}, {{"Celia", "Целиа"}},
};
static bsmeta::field fields[] = {
	BSREQ(hero, name, Name, Text),
	BSREQ(hero, attack, Attack, Number),
	BSREQ(hero, defence, Defence, Number),
	BSREQ(hero, spellpower, SpellPower, Number),
	BSREQ(hero, wisdow, Wisdow, Number),
	BSREQ(hero, spell_points, SpellPoints, Number),
	BSREQ(hero, move_points, MovePoints, Number),
	BSREQ(hero, index, Index, Number),
	BSREQ(hero, direction, Direction, Number),
	BSREQ(hero, portrait, Portrait, Number),
	BSREQ(hero, player, Player, Number),
	BSREQ(hero, recruit, Recruit, Number),
	BSREQ(hero, experience, Experience, Number),
	BSINT(hero, skills, FirstSkill, Number),
	BSINT(hero, spells, FirstSpell, Number),
	BSINT(hero, params, FirstArtifactIndex, Number),
};
BSMETA(hero, "Heroes", "Герои", FirstHero);

static void create_army(int rec)
{
	game::cleararmy(rec);
	switch(game::get(rec, Type))
	{
	case Knight:
		game::addunit(rec, Archer, xrand(3, 5));
		game::addunit(rec, Peasant, xrand(30, 50));
		break;
	case Barbarian:
		game::addunit(rec, Orc, xrand(3, 5));
		game::addunit(rec, Goblin, xrand(10, 20));
		game::addunit(rec, Wolf, xrand(0, 2));
		break;
	case Necromancer:
		game::addunit(rec, Skeleton, xrand(12, 22));
		game::addunit(rec, Zombie, xrand(4, 8));
		break;
	case Sorcerer:
		game::addunit(rec, Sprite, xrand(10, 20));
		game::addunit(rec, Dwarf, xrand(3, 7));
		break;
	case Wizard:
		game::addunit(rec, Halfling, xrand(10, 20));
		game::addunit(rec, Boar, xrand(2, 4));
		break;
	case Warlock:
		game::addunit(rec, Centaur, xrand(8, 16));
		game::addunit(rec, Gargoyle, xrand(2, 4));
		break;
	}
}

static void game_initialize()
{
	for(int rec = FirstHero; rec <= LastHero; rec++)
	{
		bsset(rec, Index, -1);
		bsset(rec, Player, 0);
		bsset(rec, Recruit, 0);
		bsset(rec, Portrait, rec - FirstHero);
		bsset(rec, Direction, map::Up);
		memset(objects[rec - FirstHero].skills, 0, sizeof(objects[rec - FirstHero].skills));
		memset(objects[rec - FirstHero].spells, 0, sizeof(objects[rec - FirstHero].spells));
		switch(game::get(rec, Type))
		{
		case Knight:
			bsset(rec, Attack, 2);
			bsset(rec, Defence, 2);
			bsset(rec, SpellPower, 1);
			bsset(rec, Wisdow, 1);
			bsset(rec, SkillLeadership, 1);
			bsset(rec, SkillBallistics, 1);
			break;
		case Barbarian:
			bsset(rec, Attack, 3);
			bsset(rec, Defence, 1);
			bsset(rec, SpellPower, 1);
			bsset(rec, Wisdow, 1);
			bsset(rec, SkillPathfinding, 2);
			break;
		case Warlock:
			bsset(rec, Attack, 0);
			bsset(rec, Defence, 0);
			bsset(rec, SpellPower, 4);
			bsset(rec, Wisdow, 3);
			bsset(rec, SkillScounting, 2);
			bsset(rec, SkillWisdom, 1);
			bsset(rec, SpellCurse, 1);
			break;
		case Wizard:
			bsset(rec, Attack, 0);
			bsset(rec, Defence, 1);
			bsset(rec, SpellPower, 3);
			bsset(rec, Wisdow, 3);
			bsset(rec, SkillWisdom, 2);
			bsset(rec, SpellStoneSkin, 1);
			break;
		case Sorcerer:
			bsset(rec, Attack, 0);
			bsset(rec, Defence, 0);
			bsset(rec, SpellPower, 3);
			bsset(rec, Wisdow, 4);
			bsset(rec, SkillNavigation, 2);
			bsset(rec, SkillWisdom, 1);
			bsset(rec, SpellBless, 1);
			break;
		case Necromancer:
			bsset(rec, Attack, 1);
			bsset(rec, Defence, 0);
			bsset(rec, SpellPower, 3);
			bsset(rec, Wisdow, 3);
			bsset(rec, SkillNecromancy, 1);
			bsset(rec, SkillWisdom, 1);
			bsset(rec, SpellHaste, 1);
			break;
		}
		create_army(rec);
		bsset(rec, SpellPoints, game::get(rec, SpellPointsMax));
		bsset(rec, MovePoints, game::get(rec, MovePointsMax));
		bsset(rec, Index, -1);
	}
}

static command game_commands[] = {
	{"initialize", game_initialize},
	{0}
};
static command::plugin commands_plugin("game", game_commands);