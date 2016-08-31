#include "main.h"

const int				InGame = 1;
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
	tokens				player, recruit;
	unsigned			experience;
	unsigned char		skills[LastSkill - FirstSkill + 1];
	unsigned char		spells[LastSpell - FirstSpell + 1];
	unsigned short		params[LastTroopsIndex - FirstArtifactIndex + 1];
	unsigned			state;
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
	BSREQ(hero, spell_points, SpellPoints, Number),
	BSREQ(hero, move_points, MovePoints, Number),
	BSREQ(hero, index, Position, Number),
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
		bsset(rec, Position, -1);
		bsset(rec, Player, PlayerNeutral); // neutral player
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
			bsset(rec, Wisdow, 2);
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
			objects[rec - FirstHero].attack = 1;
			objects[rec - FirstHero].defence = 0;
			objects[rec - FirstHero].spellpower = 3;
			objects[rec - FirstHero].wisdow = 3;
			bsset(rec, SkillWisdom, 1);
			bsset(rec, SkillNecromancy, 1);
			bsset(rec, SpellHaste, 1);
			break;
		}
		create_army(rec);
		bsset(rec, SpellPoints, game::get(rec, SpellPointsMax));
		bsset(rec, MovePoints, game::get(rec, MovePointsMax));
		bsset(rec, Position, -1);
	}
}

static command game_commands[] = {
	{"initialize", game_initialize},
	{0}
};
static command::plugin commands_plugin("game", game_commands);

const cost* game::getcost(int rec)
{
	if(heros.has(rec))
	{
		static cost e = {2500};
		return &e;
	}
	return 0;
}

void show::hero(tokens rec)
{
	char temp[260];
	draw::current::focus = -1;
	while(true)
	{
		draw::status(22, draw::height - 16, 22 + res::width(res::HSBTNS, 8), draw::height - 1);
		draw::button(0, draw::height - 20, res::HSBTNS, KeyLeft, 4, 4, 5, 0, 0, szt("Previous hero", "Предыдущий герой"));
		draw::image(22, draw::height - 20, res::HSBTNS, 8);
		draw::button(draw::width - 22, draw::height - 20, res::HSBTNS, KeyRight, 6, 6, 7, 0, 0, szt("Next hero", "Следующий герой"));
		draw::image(0, 0, res::HEROBKG, 0);
		draw::image(0, 0, draw::isevil(res::HEROEXTE, res::HEROEXTG), 0);
		draw::clipart(99, 31, rec, LargeSize);
		szprint(temp, szt("%1 the %2 ( Level %3i )", "%1 - %2 ( Уровень %3i )"),
			bsgets(rec, Name),
			bsgets(game::get(rec, Type), Name),
			bsget(rec, Level));
		draw::text((draw::width - draw::textw(temp)) / 2, 3, temp);
		// Abilities
		for(int i = (int)Attack; i <= (int)Wisdow; i++)
		{
			int x = 152 + (i - Attack) * 88;
			int y = 36;
			if(true)
			{
				draw::fontsm push;
				const char* p = bsgets(i, Name);
				draw::text(x + (88 - draw::textw(p)) / 2, y, p);
			}
			sznum(temp, game::get(rec, i));
			draw::text(x + (88 - draw::textw(temp)) / 2, y + 72, temp);
		}
		// Skills
		if(true)
		{
			draw::fontsm push;
			int x = 3 + 40;
			int y = 233;
			const int dx = res::width(res::SECSKILL, 0) + 5;
			for(int i = FirstSkill; i <= LastSkill; i++)
			{
				int n = bsget(rec, i);
				if(!n)
					continue;
				draw::clipart(x, y, i, n + SkillLevelBasic - 1);
				x += dx;
			}
			int x2 = 3 + 40 + 8 * dx;
			while(x < x2)
			{
				draw::image(x - 40, y, res::SECSKILL, 0, 0);
				x += dx;
			}
		}
		if(true)
		{
			const int dx = res::width(res::ARTIFACT, 0) + 15;
			const int dy = res::height(res::ARTIFACT, 0) + 15;
			int x = 51 + dx / 2;
			int y = 308;
			int x2 = x + 7 * dx;
			for(int i = FirstArtifactIndex; i <= LastArtifactIndex; i++)
			{
				int n = bsget(rec, i);
				if(n)
					draw::clipart(x, y, n, 0);
				else
					draw::image(x - dx / 2, y, res::ARTIFACT, 0);
				x += dx;
				if(x >= x2)
				{
					x = 51 + dx / 2;
					y += dy;
				}
			}
		}
		draw::image(49, 130, res::CREST, bsget(rec, Player) - FirstPlayer);
		draw::troops(156, 130, rec, draw::current::focus);
		draw::button(5, 318, res::HSBTNS, Dismiss, 0, 0, 1, Alpha + 'A', 0, szt("Dismiss hero", "Уволить героя"));
		draw::button(603, 318, res::HSBTNS, Cancel, 2, 2, 3, KeyEscape, 0, szt("Close hero window", "Закрыть окно"));
		draw::cursor(res::ADVMCO, 0);
		int id = draw::input();
		switch(id)
		{
		case Cancel:
		case 0:
			return;
		default:
			draw::troopsinput(id);
			break;
		}
	}
}
