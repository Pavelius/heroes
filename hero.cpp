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
	unsigned short		artifacts[14];
	unsigned			state;
	struct army			army;
} objects[LastHero - FirstHero + 1] = {
	{{"Lord Killburn", "Лорд Килбурн"}}, {{"Sir Gallanth", "Сэр Галлант"}}, {{"Ector", "Эктор"}}, {{"Gvenneth", "Гвиннет"}}, {{"Tyro", "Тиро"}},
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
	BSINT(hero, spells, FirstSpell, Number),
	BSINT(hero, artifacts, FirstArtifactIndex, Number),
};
BSMETA(hero, "Heroes", "Герои", FirstHero);

static void create_army(int rec)
{
	auto p = game::getarmy(rec);
	p->clear();
	switch(bsget(rec, Type))
	{
	case Knight:
		p->add(Archer, xrand(3, 5));
		p->add(Peasant, xrand(30, 50));
		break;
	case Barbarian:
		p->add(Orc, xrand(3, 5));
		p->add(Goblin, xrand(10, 20));
		p->add(Wolf, xrand(0, 2));
		break;
	case Necromancer:
		p->add(Skeleton, xrand(12, 22));
		p->add(Zombie, xrand(4, 8));
		break;
	case Sorcerer:
		p->add(Sprite, xrand(10, 20));
		p->add(Dwarf, xrand(3, 7));
		break;
	case Wizard:
		p->add(Halfling, xrand(10, 20));
		p->add(Boar, xrand(2, 4));
		break;
	case Warlock:
		p->add(Centaur, xrand(8, 16));
		p->add(Gargoyle, xrand(2, 4));
		break;
	}
}

static void game_initialize()
{
	for(int rec = (int)FirstHero; rec<=(int)LastHero; rec++)
	{
		bsset(rec, Position, -1);
		bsset(rec, Player, PlayerNeutral); // neutral player
		bsset(rec, Portrait, rec-FirstHero);
		bsset(rec, Direction, map::Up);
		objects[rec-FirstHero].state = 0;
		objects[rec-FirstHero].army.clear();
		memset(objects[rec-FirstHero].skills, 0, sizeof(objects[rec-FirstHero].skills));
		memset(objects[rec - FirstHero].spells, 0, sizeof(objects[rec - FirstHero].spells));
		memset(objects[rec-FirstHero].artifacts, 0, sizeof(objects[rec-FirstHero].artifacts));
		if(rec<=(int)Bax)
			objects[rec-FirstHero].state |= InGame;
		switch(bsget(rec, Type))
		{
		case Knight:
			objects[rec-FirstHero].attack = 2;
			objects[rec-FirstHero].defence = 2;
			objects[rec-FirstHero].spellpower = 1;
			objects[rec-FirstHero].wisdow = 1;
			bsset(rec, SkillLeadership, 1);
			bsset(rec, SkillBallistics, 1);
			break;
		case Barbarian:
			objects[rec-FirstHero].attack = 3;
			objects[rec-FirstHero].defence = 1;
			objects[rec-FirstHero].spellpower = 1;
			objects[rec-FirstHero].wisdow = 1;
			bsset(rec, SkillPathfinding, 2);
			break;
		case Warlock:
			objects[rec-FirstHero].attack = 0;
			objects[rec-FirstHero].defence = 0;
			objects[rec-FirstHero].spellpower = 4;
			objects[rec-FirstHero].wisdow = 2;
			bsset(rec, SkillScounting, 2);
			bsset(rec, SkillWisdom, 1);
			bsset(rec, SpellCurse, 1);
			break;
		case Wizard:
			objects[rec-FirstHero].attack = 0;
			objects[rec-FirstHero].defence = 1;
			objects[rec-FirstHero].spellpower = 3;
			objects[rec-FirstHero].wisdow = 3;
			bsset(rec, SkillWisdom, 2);
			bsset(rec, SpellStoneSkin, 1);
			break;
		case Sorcerer:
			objects[rec-FirstHero].attack = 0;
			objects[rec-FirstHero].defence = 0;
			objects[rec-FirstHero].spellpower = 3;
			objects[rec-FirstHero].wisdow = 4;
			bsset(rec, SkillNavigation, 2);
			bsset(rec, SkillWisdom, 1);
			bsset(rec, SpellBless, 1);
			break;
		case Necromancer:
			objects[rec-FirstHero].attack = 1;
			objects[rec-FirstHero].defence = 0;
			objects[rec-FirstHero].spellpower = 3;
			objects[rec-FirstHero].wisdow = 3;
			bsset(rec, SkillWisdom, 1);
			bsset(rec, SkillNecromancy, 1);
			bsset(rec, SpellHaste, 1);
			break;
		}
		create_army(rec);
		objects[rec-FirstHero].spell_points = bsget(rec, SpellPointsMax);
		objects[rec-FirstHero].move_points = bsget(rec, MovePointsMax);
		objects[rec-FirstHero].index = -1;
	}
}

//static void object_set(int rec, int id, int value)
//{
//	hero_t& e = objects[rec-FirstHero];
//	switch(id)
//	{
//	case Player:
//		e.player = tokens(value);
//		break;
//	case Recruit:
//		e.recruit = tokens(value);
//		break;
//	case Position:
//		e.index = value;
//		if(value==objects[rec-FirstHero].index_move)
//            e.index_move = -1;
//		break;
//	case MoveTo:
//		e.index_move = value;
//		if(value==objects[rec-FirstHero].index)
//            e.index_move = -1;
//		break;
//	case Direction:
//		e.direction = value;
//		break;
//	case Portrait:
//		e.portrait = value;
//		break;
//	case PathTo:
//		break;
//	case MovePoints:
//		e.move_points = value;
//		break;
//	case SpellPoints:
//		e.spell_points = value;
//		break;
//	default:
//		if(id>=(int)FirstSkill && id<=(int)LastSkill)
//			e.skills[id-FirstSkill] = value;
//		else if(id>=(int)FirstArtifactIndex && id<=(int)LastArtifactIndex)
//			e.artifacts[id-FirstArtifactIndex] = value;
//		else if(id>=(int)FirstArtifact && id<=(int)LastArtifact)
//		{
//			for(int i = 0; i<(int)(sizeof(e.artifacts)/sizeof(e.artifacts[0])); i++)
//			{
//				if(e.artifacts[i])
//					continue;
//				e.artifacts[i] = id;
//				break;
//			}
//		}
//		else if(id>=(int)FirstSpell && id<=(int)LastSpell)
//			e.spells.set(id, value);
//		break;
//	}
//}
//
//static const char* object_gets(int rec, int id)
//{
//    static char code[8];
//	switch(id)
//	{
//	case Name:
//		return objects[rec-FirstHero].name;
//    case Code:
//        szprint(code, "%%h%1i", rec-FirstHero);
//        return code;
//	default:
//		return "...";
//	}
//}

army* game::getarmy(int rec)
{
	if(!heros.has(rec))
		return 0;
	return &objects[rec - FirstHero].army;
}

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
	int army_index = -1;
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
			bsget(rec, Name),
			bsget(bsget(rec, Type), Name),
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
			sznum(temp, bsget(rec, i));
			draw::text(x + (88 - draw::textw(temp)) / 2, y + 72, temp);
		}
		// Skills
		if(true)
		{
			draw::fontsm push;
			int x = 3 + 40;
			int y = 233;
			const int dx = res::width(res::SECSKILL, 0) + 5;
			for(int i = FirstSkill; i <= (int)LastSkill; i++)
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
			for(int i = (int)FirstArtifactIndex; i <= (int)LastArtifactIndex; i++)
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
		draw::troops(156, 130, game::getarmy(rec), FirstGarrisonIndex, army_index);
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
			if(id == Tooltips || (id >= (int)FirstGarrisonIndex && id <= (int)LastGarrisonIndex))
				game::getarmy(rec)->input(id, FirstGarrisonIndex, army_index, rec);
			break;
		}
	}
}
