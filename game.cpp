#include "main.h"

const int castle_income_well = 2;
const int castle_income_well2 = 10;

static int		day;
static tokens	week_of, month_of;
static tokens	game_difficult = EasyDifficulty;
static tokens	week_monsters[] =
{
	Goblin, Orc, Wolf, Ogre, Troll, Cyclop,
	Peasant, Archer, Cavalry,
	Centaur, Gargoyle, Griffin,
	Sprite, Dwarf, Elf,
	Halfling, Boar, Roc,
};
static struct hero_data
{
	const char*		name[2];
} heroes_data[LastHero - FirstHero + 1] = {
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

int artifacts_bonuses(int rec, int id)
{
	short unsigned* p = (short unsigned*)bsptr(rec, FirstArtifactIndex);
	if(!p)
		return 0;
	int result = 0;
	for(int i = 0; i <= LastArtifactIndex - FirstArtifactIndex; i++)
	{
		if(!p[i])
			continue;
		result += game::getartifact(p[i], id);
	}
	return result;
}

static int get_free_hero(int type)
{
	int temp[LastHero - FirstHero + 2];
	bsselect(temp, FirstHero, LastHero);
	zshuffle(temp, zlen(temp));
	for(int* p = temp; *p; p++)
	{
		if(bsget(*p, Recruit))
			continue;
		if(bsget(*p, Player))
			continue;
		if(type && game::get(*p, Type) != type)
			continue;
		return *p;
	}
	return 0;
}

bool game::isboosted(int rec)
{
	for(int eid = FirstEffect; eid <= LastEffect; eid++)
	{
		if(bsget(eid, Parent) != rec)
			continue;
		switch(bsget(eid, Type))
		{
		case SpellBless:
		case SpellStoneSkin:
		case SpellSteelSkin:
		case SpellAntimagic:
		case SpellBloodLust:
		case SpellHaste:
		case SpellShield:
			return true;
		}
	}
	return false;
}

bool game::ispenalized(int rec)
{
	for(int eid = FirstEffect; eid <= LastEffect; eid++)
	{
		if(bsget(eid, Parent) != rec)
			continue;
		switch(bsget(eid, Type))
		{
		case SpellBerserker:
		case SpellBlind:
		case SpellCurse:
		case SpellHypnotize:
		case SpellSlow:
		case SpellParalyze:
		case SpellStone:
			return true;
		}
	}
	return false;
}

int game::getartifact(int rec, int id)
{
	int type = bsget(rec, Type);
	int effect = bsget(rec, Value);
	switch(id)
	{
	case Attack:
	case Defence:
		if(type == id
			|| type == AttackDefence
			|| type == All)
			return effect;
		return 0;
	case Wisdow:
	case SpellPower:
		if(type == id
			|| type == SpellPowerWisdow
			|| type == All)
			return effect;
		return 0;
	default:
		if(type == id)
			return effect;
		return 0;
	}
}

void game::cleararmy(int rec)
{
	for(int i = FirstTroopsIndex; i <= LastTroopsIndex; i += 2)
	{
		bsset(rec, i, 0);
		bsset(rec, i + 1, 0);
	}
}

bool game::addunit(int rec, int type, int count)
{
	if(type >= FirstMonster && type <= LastMonster)
	{
		for(int i = FirstTroopsIndex; i <= LastTroopsIndex; i += 2)
		{
			if(bsget(rec, i) == type)
			{
				count += bsget(rec, i + 1);
				if(count < 0)
					count = 0;
				bsset(rec, i + 1, count);
				if(!count)
					bsset(rec, i, 0);
				return true;
			}
		}
		// Add new one
		for(int i = FirstTroopsIndex; i <= LastTroopsIndex; i += 2)
		{
			if(!bsget(rec, i))
			{
				bsset(rec, i, type);
				bsset(rec, i + 1, count);
				return true;
			}
		}
	}
	return false;
}

bool game::additem(int rec, int type)
{
	for(int i = FirstArtifactIndex; i <= LastArtifactIndex; i++)
	{
		if(bsget(rec, i))
			continue;
		bsset(rec, i, type);
		return true;
	}
	return false;
}

static int get_morale_troops(int i1, int i2, int side)
{
	int troops[SandsWarriors - Barbarian + 1];
	memset(troops, 0, sizeof(troops));
	for(int i = i1; i <= i2; i++)
	{
		int rec = i;
		if(rec >= FirstCombatant && rec <= LastCombatant)
		{
			if(!bsget(rec, Type))
				continue;
			if(bsget(rec, Side) != side)
				continue;
			rec = bsget(rec, Type);
		}
		else if(rec >= FirstTroopsIndex && rec <= LastTroopsIndex)
		{
			rec = bsget(side, rec);
			i++;
			if(!rec)
				continue;
		}
		int type = game::get(rec, Type);
		if(type >= Barbarian && type <= SandsWarriors)
			troops[type - Barbarian] = 1;
	}
	int m = 0;
	for(auto a : troops)
		m += a;
	if(m == 2)
		return -1;
	else if(m >= 3)
		return -2;
	return 0;
}

int game::getsummary(int rec, int id, int side)
{
	int t = rec;
	if(t >= FirstCombatant && t <= LastCombatant)
		t = bsget(t, Type);
	int m = game::get(t, id);
	if(side)
		m += game::get(side, id);
	switch(id)
	{
	case Attack:
		// RULE: Boold Lust
		if(combat::geteffect(rec, SpellBloodLust))
			m += 3;
		break;
	case Defence:
		// RULE: Stone skin.
		if(combat::geteffect(rec, SpellStone))
			m += 3;
		// RULE: Steel skin.
		else if(combat::geteffect(rec, SpellSteelSkin))
			m += 5;
		break;
	case Morale:
		if(rec >= FirstCombatant && rec <= LastCombatant)
		{
			if(combat::isattacker(rec))
				m += combat::enviroment::morale;
			else
				m -= combat::enviroment::morale;
		}
		break;
	case Luck:
		if(m > 3)
			m = 3;
		else if(m < -3)
			m = -3;
		break;
	case Speed:
		if(m < 0)
			m = 0;
		else if(m > 6)
			m = 6;
		break;
	case DamageMin:
		// RULE: Bless
		if(combat::geteffect(rec, SpellBless))
			m = bsget(t, DamageMax);
		break;
	case DamageMax:
		// RULE: Curse
		if(combat::geteffect(rec, SpellCurse))
			m = bsget(t, DamageMin);
		break;
	}
	return m;
}

int game::get(int rec, int id)
{
	int m, k;
	switch(id)
	{
	case Speed:
	case DamageMin:
	case DamageMax:
		if(rec >= FirstHero && rec <= LastHero)
			return bsget(rec, id) + artifacts_bonuses(rec, id);
		else if(rec >= FirstCombatant && rec <= LastCombatant)
			return getsummary(rec, id, bsget(rec, Side));
		return bsget(rec, id);
	case Attack:
		if(rec >= FirstHero && rec <= LastHero)
			return bsget(rec, id) + artifacts_bonuses(rec, id);
		else if(rec >= BarbarianCaptain && rec <= WizardCaptain)
		{
			switch(rec)
			{
			case BarbarianCaptain: return 3;
			case KnightCaptain: return 2;
			case NecromancerCaptain: return 1;
			default: return 0;
			}
		}
		else if(rec >= FirstCombatant && rec <= LastCombatant)
			return getsummary(rec, id, bsget(rec, Side));
		return bsget(rec, id);
	case Defence:
		if(rec >= FirstHero && rec <= LastHero)
			return bsget(rec, id) + artifacts_bonuses(rec, id);
		else if(rec >= BarbarianCaptain && rec <= WizardCaptain)
		{
			switch(rec)
			{
			case KnightCaptain: return 2;
			case BarbarianCaptain:
			case WizardCaptain: return 1;
			default: return 0;
			}
		}
		else if(rec >= FirstCombatant && rec <= LastCombatant)
			return getsummary(rec, id, bsget(rec, Side));
		return bsget(rec, id);
	case Wisdow:
		if(rec >= FirstHero && rec <= LastHero)
			return bsget(rec, id) + artifacts_bonuses(rec, id);
		else if(rec >= BarbarianCaptain && rec <= WizardCaptain)
		{
			switch(rec)
			{
			case SorcererCaptain: return 4;
			case WizardCaptain:
			case WarlockCaptain:
			case NecromancerCaptain: return 3;
			default: return 1;
			}
		}
		return bsget(rec, id);
	case SpellPower:
		if(rec >= FirstHero && rec <= LastHero)
			return bsget(rec, id) + artifacts_bonuses(rec, id);
		else if(rec >= BarbarianCaptain && rec <= WizardCaptain)
		{
			switch(rec)
			{
			case WarlockCaptain: return 4;
			case SorcererCaptain:
			case WizardCaptain:
			case NecromancerCaptain: return 3;
			default: return 1;
			}
		}
		return bsget(rec, id);
	case Morale:
		if(rec >= FirstHero && rec <= LastHero)
			return bsget(rec, SkillLeadership)
			+ artifacts_bonuses(rec, id)
			+ get_morale_troops(FirstTroopsIndex, LastTroopsIndex, rec);
		else if(rec >= FirstCombatant && rec <= LastCombatant)
			return getsummary(rec, id, bsget(rec, Side));
		else
		{
			switch(rec)
			{
			case MinotaurKing:
			case BattleDwarf:
				return 1;
			default:
				return 0;
			}
		}
	case Luck:
		if(rec >= FirstHero && rec <= LastHero)
			return bsget(rec, SkillLuck) + artifacts_bonuses(rec, id);
		else if(rec >= FirstCombatant && rec <= LastCombatant)
			return getsummary(rec, id, bsget(rec, Side));
		else
		{
			switch(rec)
			{
			case Sprite:
				return 1;
			default:
				return 0;
			}
		}
	case Count:
		if(rec >= FirstCombatant && rec <= LastCombatant)
		{
			int t = bsget(rec, Type);
			int m = bsget(t, HitPointsMax);
			int c = bsget(rec, HitPoints);
			return (c + m - 1) / m;
		}
		return 0;
	case HitPoints:
		if(rec >= FirstCombatant && rec <= LastCombatant)
		{
			int t = bsget(rec, Type);
			int m = bsget(t, HitPointsMax);
			return 1 + (bsget(rec, HitPoints) - 1) % m;
		}
		return 0;
	case HitPointsMax:
		if(rec >= FirstCombatant && rec <= LastCombatant)
			rec = bsget(rec, Type);
		return bsget(rec, id);
	case SpellPointsMax:
		return get(rec, Wisdow) * 10;
	case MovePointsMax:
		m = 0;
		// start point
		if(get(rec, ShipMaster))
		{
			m = 1500;
			// skill navigation
			m += m * 30 * get(rec, SkillNavigation) / 100;
			// artifact bonus
			k = artifacts_bonuses(rec, MovePointsSea);
		}
		else
		{
			m = 1000;
			// slowest monster speed
			for(int i = FirstTroopsIndex; i <= LastTroopsIndex; i += 2)
			{
				int unit = bsget(rec, i);
				if(!unit)
					continue;
				int ms = 1000;
				switch(bsget(unit, Speed))
				{
				case SpeedSlow: ms = 1100; break;
				case SpeedAverage: ms = 1200; break;
				case SpeedFast: ms = 1300; break;
				case SpeedVeryFast: ms = 1400; break;
				case SpeedUltraFast: ms = 1500; break;
				}
				if(m > ms)
					m = ms;
			}
			// skill logistics
			m += m*get(rec, SkillLogistics) * 10 / 100;
			k = artifacts_bonuses(rec, MovePointsLand);
		}
		if(k)
			m += k * 100;
		return m;
	case Type:
		if(rec >= FirstHero && rec <= LastHero)
		{
			if(rec <= Dimitry || rec == Corlagon)
				return Knight;
			if(rec <= Atlas || rec == Halton)
				return Barbarian;
			if(rec <= Luna || rec == Eliza)
				return Sorcerer;
			if(rec <= Wrathmont || rec == Archibald)
				return Warlock;
			if(rec <= Mandigal || rec == Roland)
				return Wizard;
			return Necromancer;
		}
		else if(rec >= FirstMonster && rec <= LastMonster)
		{
			if(rec >= Peasant && rec <= Crusader)
				return Knight;
			else if(rec >= Goblin && rec <= Cyclop)
				return Barbarian;
			else if(rec >= Centaur && rec <= BlackDragon)
				return Warlock;
			else if(rec >= Skeleton && rec <= BoneDragon)
				return Necromancer;
			else if(rec >= Sprite && rec <= Phoenix)
				return Sorcerer;
			else if(rec >= Halfling && rec <= Titan)
				return Wizard;
			return SandsWarriors;
		}
		return bsget(rec, id);
		//case AllAttackAnswer:
		//	if(rec >= FirstCombatant && rec <= LastCombatant)
		//		rec = bsget(rec, Type);
		//	switch(rec)
		//	{
		//	case Griffin:
		//		return 1;
		//	default:
		//		return 0;
		//	}
	default:
		return bsget(rec, id);
	}
}

bool game::hasspellbook(int rec)
{
	for(int i = FirstArtifactIndex; i <= LastArtifactIndex; i++)
	{
		if(bsget(rec, i) == MagicBook)
			return true;
	}
	return false;
}

int game::getmoralechance(int value)
{
	return getmorale(value) * 10;
}

int game::getmorale(int value)
{
	if(value < -3)
		value = -3;
	else if(value > 3)
		value = 3;
	return value;
}

int game::getspeed(int value)
{
	if(value < 0)
		value = 0;
	else if(value > (SpeedUltraFast - SpeedCrawling))
		value = (SpeedUltraFast - SpeedCrawling);
	return value;
}

int game::getday()
{
	return (day % 7) + 1;
}

int game::getweek()
{
	return ((day / 7) % 4) + 1;
}

int game::getmonth()
{
	return (day / (7 * 4) + 1);
}

int game::turn()
{
	for(int rec = FirstPlayer; rec <= LastPlayer; rec++)
	{
		if(!bsget(rec, Type))
			continue;
		switch(bsget(rec, PlayerType))
		{
		case Human:
			switch(show::game(rec))
			{
			case 0:
				return 0;
			}
			break;
		case Computer:
			break;
		}
	}
	return EndTurn;
}

int game::getincome(int rec)
{
	int result = 1000;
	if(bsget(rec, Castle) < 2)
		result = result / 2;
	if(bsget(rec, Statue))
		result += 250;
	if(bsget(rec, Type) == Warlock && bsget(rec, SpecialBuilding))
		result += 250 * 2;
	return result;
}

static tokens get_oppose(tokens value)
{
	switch(value)
	{
	case EasyDifficulty:
		return ImpossibleDifficulty;
	case NormalDifficulty:
		return VeryHardDifficulty;
	case VeryHardDifficulty:
		return NormalDifficulty;
	case ImpossibleDifficulty:
		return EasyDifficulty;
	default:
		return HardDifficulty;
	}
}

static void game_set_difficult(int rec, tokens value)
{
	switch(value)
	{
	case EasyDifficulty:
		bsset(rec, Ore, 20);
		bsset(rec, Wood, 20);
		bsset(rec, Crystal, 10);
		bsset(rec, Sulfur, 10);
		bsset(rec, Gems, 10);
		bsset(rec, Mercury, 10);
		bsset(rec, Gold, 20000);
		break;
	case NormalDifficulty:
		bsset(rec, Ore, 12);
		bsset(rec, Wood, 12);
		bsset(rec, Crystal, 6);
		bsset(rec, Sulfur, 6);
		bsset(rec, Gems, 6);
		bsset(rec, Mercury, 6);
		bsset(rec, Gold, 10000);
		break;
	case HardDifficulty:
		bsset(rec, Ore, 5);
		bsset(rec, Wood, 5);
		bsset(rec, Crystal, 0);
		bsset(rec, Sulfur, 0);
		bsset(rec, Gems, 0);
		bsset(rec, Mercury, 0);
		bsset(rec, Gold, 2500);
		break;
	}
}

void game::initialize()
{
	day = 0;
	week_of = Empthy;
	month_of = Empthy;
	// Setup random generator
	bsmeta::getbase("hero")->count = LastHero - FirstHero + 1;
	// Clear all players
	for(int rec = FirstPlayer; rec <= LastPlayer; rec++)
	{
		bsset(rec, Type, 0);
		bsset(rec, PlayerType, 0);
		bsset(rec, Recruit, 0);
		bsset(rec, RecruitLast, 0);
		for(int i = FirstResource; i <= LastResource; i++)
			bsset(rec, i, 0);
	}
	// Clear all heroes and create army
	for(int rec = FirstHero; rec <= LastHero; rec++)
	{
		bsset(rec, Player, 0);
		bsset(rec, Recruit, 0);
		bsset(rec, Portrait, rec - FirstHero);
		bsset(rec, Direction, map::Right);
		bsset(rec, SpellPoints, game::get(rec, SpellPointsMax));
		bsset(rec, MovePoints, game::get(rec, MovePointsMax));
		bsset(rec, Index, -1);
		bsset(rec, MoveTo, -1);
		bsset(rec, Name, heroes_data[rec - FirstHero].name[locale]);
		game::cleararmy(rec);
		for(int i = FirstSkill; i <= LastSkill; i++)
			bsset(rec, i, 0);
		for(int i = FirstSpell; i <= LastSpell; i++)
			bsset(rec, i, 0);
		for(int i = FirstArtifactIndex; i <= LastArtifactIndex; i++)
			bsset(rec, i, 0);
		switch(game::get(rec, Type))
		{
		case Knight:
			bsset(rec, Attack, 2);
			bsset(rec, Defence, 2);
			bsset(rec, SpellPower, 1);
			bsset(rec, Wisdow, 1);
			bsset(rec, SkillLeadership, 1);
			bsset(rec, SkillBallistics, 1);
			//
			game::addunit(rec, Archer, xrand(3, 5));
			game::addunit(rec, Peasant, xrand(30, 50));
			break;
		case Barbarian:
			bsset(rec, Attack, 3);
			bsset(rec, Defence, 1);
			bsset(rec, SpellPower, 1);
			bsset(rec, Wisdow, 1);
			bsset(rec, SkillPathfinding, 2);
			//
			game::addunit(rec, Orc, xrand(3, 5));
			game::addunit(rec, Goblin, xrand(10, 20));
			game::addunit(rec, Wolf, xrand(0, 2));
			break;
		case Warlock:
			bsset(rec, Attack, 0);
			bsset(rec, Defence, 0);
			bsset(rec, SpellPower, 4);
			bsset(rec, Wisdow, 3);
			bsset(rec, SkillScounting, 2);
			bsset(rec, SkillWisdom, 1);
			bsset(rec, SpellCurse, 1);
			//
			game::addunit(rec, Centaur, xrand(8, 16));
			game::addunit(rec, Gargoyle, xrand(2, 4));
			game::additem(rec, MagicBook);
			break;
		case Wizard:
			bsset(rec, Attack, 0);
			bsset(rec, Defence, 1);
			bsset(rec, SpellPower, 3);
			bsset(rec, Wisdow, 3);
			bsset(rec, SkillWisdom, 2);
			bsset(rec, SpellStoneSkin, 1);
			//
			game::addunit(rec, Halfling, xrand(10, 20));
			game::addunit(rec, Boar, xrand(2, 4));
			game::additem(rec, MagicBook);
			break;
		case Sorcerer:
			bsset(rec, Attack, 0);
			bsset(rec, Defence, 0);
			bsset(rec, SpellPower, 3);
			bsset(rec, Wisdow, 4);
			bsset(rec, SkillNavigation, 2);
			bsset(rec, SkillWisdom, 1);
			bsset(rec, SpellBless, 1);
			//
			game::addunit(rec, Sprite, xrand(10, 20));
			game::addunit(rec, Dwarf, xrand(3, 7));
			game::additem(rec, MagicBook);
			break;
		case Necromancer:
			bsset(rec, Attack, 1);
			bsset(rec, Defence, 0);
			bsset(rec, SpellPower, 3);
			bsset(rec, Wisdow, 3);
			bsset(rec, SkillNecromancy, 1);
			bsset(rec, SkillWisdom, 1);
			bsset(rec, SpellHaste, 1);
			//
			game::addunit(rec, Skeleton, xrand(12, 22));
			game::addunit(rec, Zombie, xrand(4, 8));
			game::additem(rec, MagicBook);
			break;
		}
	}
	// Calculate monater ratin
	for(int i = FirstMonster; i <= LastMonster; i++)
	{
		auto res = ((double)bsget(i, DamageMin) + (double)bsget(i, DamageMax)) / 2.0;
		auto hal = res * 0.5;
		// increase strength
		if(game::isfly(i))
			res += hal;
		else if(game::isarcher(i))
			res += hal;
		if(game::istwiceattack(i))
			res += hal;
		if(game::isstealth(i))
			res += hal;
		// slowly: decrease strength
		if((!game::isfly(i) && !game::isarcher(i))
			&& (bsget(i, Speed) + SpeedCrawling) < SpeedAverage)
			res -= hal;
		if(res == Ghost)
			res += hal * 4;
		bsset(i, Rating, (int)res);
	}
	command::execute("game_initialize");
}

static void game_endweek()
{
	int m_base = bsget(week_of, Base);
	int m_type = bsget(m_base, Type);
	//int m_dwll = bsget(m_base, Dwelve);
	for(int rec = FirstCastle; rec <= (int)LastCastle; rec++)
	{
		for(int i = Dwelving1; i <= Dwelving6; i++)
		{
			if(!bsget(rec, i))
				continue;
			bsadd(rec, i - Dwelving1 + FirstCreatureCount, game::getgrowth(rec, i));
		}
		// Week monster growth fast
		//if(m_type == bsget(rec, Type) && bsget(rec, m_dwll))
		//	bsadd(rec, FirstRecruit + m_dwll - Dwelving1, 5);
	}
}

void game::build(int rec, int id)
{
	auto race = (tokens)bsget(rec, Type);
	auto player = bsget(rec, Player);
	auto next_level = bsget(rec, id) + 1;
	auto cost = getbuildingcost(race, id, next_level);
	auto resources = bsptr(player, FirstResource);
	addresources(resources, resources, cost, true);
	bsset(rec, id, next_level);
	if(next_level == 1 && id >= Dwelving1 && id <= Dwelving6)
		bsadd(rec, id - Dwelving1 + FirstCreatureCount, getgrowth(rec, id));
	bsadd(rec, AlreadyMoved, 1);
	if(race == Wizard && id == SpecialBuilding)
	{
		game::random::spell(rec, 1);
		game::random::spell(rec, 2);
		game::random::spell(rec, 3);
		game::random::spell(rec, 4);
		game::random::spell(rec, 5);
	}
}

void game::prepare()
{
	int hero;
	for(int rec = FirstPlayer; rec <= LastPlayer; rec++)
	{
		int type = bsget(rec, Type);
		if(!type)
			continue;
		// Setup starting resource
		if(bsget(rec, PlayerType) == Human)
			game_set_difficult(rec, game_difficult);
		else
			game_set_difficult(rec, get_oppose(game_difficult));
		// Recruit heroes
		hero = game::random::hero(type);
		if(!hero)
			hero = game::random::hero(0);
		bsset(rec, Recruit, hero);
		hero = game::random::hero(0);
		bsset(rec, RecruitLast, hero);
	}
	command::execute("game_prepare");
	game_endweek();
}

static void game_endturn()
{
	// All heroes refresh their values
	for(int rec = FirstHero; rec <= LastHero; rec++)
	{
		if(!bsget(rec, Player))
			continue;
		// Spell points
		int sp = bsget(rec, SpellPoints);
		int spm = game::get(rec, SpellPointsMax);
		sp = imin(sp + 1 + bsget(rec, SkillMysticism), spm);
		bsset(rec, SpellPoints, sp);
		// Move points
		bsset(rec, MovePoints, game::get(rec, MovePointsMax));
	}
	// All castles give income and refresh state
	for(int rec = FirstCastle; rec <= LastCastle; rec++)
	{
		// Update build flags
		bsset(rec, AlreadyMoved, 0);
		// Get income for all player exept neutral
		int p = bsget(rec, Player);
		if(p)
		{
			int m = game::getincome(rec);
			bsadd(p, Gold, m);
		}
	}
}

int game::getgrowth(int rec, int dwelling)
{
	static int		growth_per_week[][6] = {
		{10, 8, 5, 4, 3, 2},
		{12, 8, 5, 4, 3, 2},
		{8, 6, 4, 3, 2, 1},
		{6, 6, 4, 3, 2, 1},
		{8, 6, 4, 3, 2, 1},
		{8, 6, 4, 3, 2, 1},
	};
	if(!bsget(rec, dwelling))
		return 0;
	int type = bsget(rec, Type);
	int result = growth_per_week[type - Barbarian][dwelling - Dwelving1];
	if(bsget(rec, Well))
		result += castle_income_well;
	if(dwelling == Dwelving1 && bsget(rec, Well2))
		result += castle_income_well2;
	return result;
}

int game::play(gamefile& game)
{
	game.validate();
	game::initialize();
	if(!map::load(game))
		return Cancel;
	game::prepare();
	while(true)
	{
		int result = turn();
		if(!result)
			return 0;
		game_endturn();
		day++;
		if((day % 7) == 0)
		{
			week_of = week_monsters[rand() % (sizeof(week_monsters) / sizeof(week_monsters[0]))];
			game_endweek();
		}
	}
}

void game::addresources(void* result_void, const void* v1, const void* v2, bool negative)
{
	auto result = (int*)result_void;
	auto e1 = (int*)v1;
	auto e2 = (int*)v2;
	if(negative)
	{
		for(int i = 0; i <= (LastResource - FirstResource); i++)
			result[i] = e1[i] - e2[i];
	}
	else
	{
		for(int i = 0; i <= (LastResource - FirstResource); i++)
			result[i] = e1[i] + e2[i];
	}
	// Normalize resource
	for(int i = 0; i <= (LastResource - FirstResource); i++)
	{
		if(result[i] < 0)
			result[i] = 0;
	}
}

void game::mulresource(int* result, const void* source_void, int value)
{
	int* source = (int*)source_void;
	for(int i = 0; i <= (LastResource - FirstResource); i++)
		result[i] = source[i] * value;
}

int game::divresource(const void* source_void, const void* divider_void)
{
	auto source = (int*)source_void;
	auto divider = (int*)divider_void;
	int result = 0x7FFFFFFF;
	for(int i = 0; i <= (LastResource - FirstResource); i++)
	{
		if(!divider[i])
			continue;
		auto v = source[i] / divider[i];
		if(result > v)
			result = v;
	}
	return result;
}

const int* game::gethirecost(int rec)
{
	static int cost[LastResource - FirstResource + 1] = {2500};
	return cost;
}

bool game::ismatch(const void* c1_void, const void* c2_void)
{
	auto c1 = (int*)c1_void;
	auto c2 = (int*)c2_void;
	for(int i = 0; i <= (LastResource - FirstResource); i++)
	{
		if(c1[i] < c2[i])
			return false;
	}
	return true;
}

bool game::hire(int hero, int player, int index)
{
	if(bsfind(FirstHero, Index, index))
		return false;
	int type = bsget(player, Type);
	if(!hero)
	{
		hero = game::random::hero(type);
		if(!hero)
			hero = game::random::hero(0);
		if(!hero)
			return false;
	}
	bsset(hero, Player, player);
	bsset(hero, Index, index);
	if(bsget(player, Recruit) == hero)
	{
		bsset(player, Recruit, 0);
		auto new_hero = game::random::hero(type);
		if(!new_hero)
			new_hero = game::random::hero(0);
		bsset(player, Recruit, new_hero);
	}
	if(bsget(player, RecruitLast) == hero)
	{
		bsset(player, RecruitLast, 0);
		auto new_hero = game::random::hero(0);
		bsset(player, RecruitLast, new_hero);
	}
	int object = bsfind(FirstCastle, Index, index);
	if(object)
		game::interact(index, object, hero, player);
	return true;
}

int game::getupgrade(int rec)
{
	switch(rec)
	{
	case Archer: return Ranger;
	case Pikeman: return VeteranPikeman;
	case Swordsman: return MasterSwordsman;
	case Cavalry: return Champion;
	case Paladin: return Crusader;
	case Orc: return OrcChief;
	case Ogre: return OgreLord;
	case Troll: return WarTroll;
	case Dwarf: return BattleDwarf;
	case Elf: return GrandElf;
	case Druid: return GreaterDruid;
	case Minotaur: return MinotaurKing;
	case GreenDragon: return RedDragon;
	case RedDragon: return BlackDragon;
	case IronGolem: return SteelGolem;
	case Mage: return ArchMage;
	case Giant: return Titan;
	case Zombie: return MutantZombie;
	case Mummy: return RoyalMummy;
	case Vampire: return VampireLord;
	case Lich: return PowerLich;
	default: return rec;
	}
}

int game::getdowngrade(int rec)
{
	switch(rec)
	{
	case Ranger: return Archer;
	case VeteranPikeman: return Pikeman;
	case MasterSwordsman: return Swordsman;
	case Champion: return Cavalry;
	case Crusader: return Paladin;
	case OrcChief: return Orc;
	case OgreLord: return Ogre;
	case WarTroll: return Troll;
	case BattleDwarf: return Dwarf;
	case GrandElf: return Elf;
	case GreaterDruid: return Druid;
	case MinotaurKing: return Minotaur;
	case RedDragon: return GreenDragon;
	case BlackDragon: return RedDragon;
	case SteelGolem: return IronGolem;
	case ArchMage: return Mage;
	case Titan: return Giant;
	case MutantZombie: return Zombie;
	case RoyalMummy: return Mummy;
	case VampireLord: return Vampire;
	case PowerLich: return Lich;
	default: return rec;
	}
}

int game::getdwelving(int rec)
{
	switch(rec)
	{
	case Goblin:
	case Centaur:
	case Halfling:
	case Sprite:
	case Skeleton:
	case Peasant:
		return Dwelving1;
	case Orc:
	case Gargoyle:
	case Boar:
	case Dwarf:
	case Zombie:
	case Archer:
		return Dwelving2;
	case Wolf:
	case Griffin:
	case IronGolem:
	case Elf:
	case Mummy:
	case Pikeman:
		return Dwelving3;
	case Ogre:
	case Minotaur:
	case Roc:
	case Druid:
	case Vampire:
	case Swordsman:
		return Dwelving4;
	case Troll:
	case Hydra:
	case Mage:
	case Unicorn:
	case Lich:
	case Cavalry:
		return Dwelving5;
	case Cyclop:
	case GreenDragon:
	case Giant:
	case Phoenix:
	case BoneDragon:
	case Paladin:
		return Dwelving6;
	case OrcChief:
	case BattleDwarf:
	case Ranger:
	case MutantZombie:
		return Dwelving2;
	case SteelGolem:
	case GrandElf:
	case RoyalMummy:
	case VeteranPikeman:
		return Dwelving3;
	case OgreLord:
	case MinotaurKing:
	case GreaterDruid:
	case VampireLord:
	case MasterSwordsman:
		return Dwelving4;
	case WarTroll:
	case ArchMage:
	case PowerLich:
	case Champion:
		return Dwelving5;
	case BlackDragon:
	case RedDragon:
	case Titan:
	case Crusader:
		return Dwelving6;
	default:
		return Empthy;
	}
}

int game::getcastle(int index)
{
	return bsfind(FirstCastle, Index, index);
}

bool game::canupgrade(int monster, int side)
{
	if(game::getupgrade(monster) == monster)
		return false;
	int castle;
	if(side >= FirstCastle && side <= LastCastle)
		castle = side;
	else
		castle = game::getcastle(bsget(side, Index));
	if(!castle)
		return false;
	int type = game::get(monster, Type);
	if(bsget(castle, Type) != type)
		return false;
	int dwelve = game::getdwelving(monster);
	if(bsget(castle, dwelve) < 2)
		return false;
	return true;
}

bool game::upgrade(int side, int index, bool interactive)
{
	char temp[260];
	auto monster = bsget(side, index);
	auto monster_new = game::getupgrade(monster);
	auto player = bsget(side, Player);
	auto count = bsget(side, index + 1);
	if(monster == monster_new)
		return true;
	auto pl = bsptr(player, FirstResource);
	auto c1 = bsptr(monster, FirstResource);
	auto c2 = bsptr(monster_new, FirstResource);
	int t1[LastResource - FirstResource + 1];
	int t2[LastResource - FirstResource + 1];
	mulresource(t1, c1, count);
	mulresource(t2, c2, count);
	addresources(t2, t2, t1, true);
	if(interactive)
	{
		szprint(temp, szt("Upgrade", "Улучшить"));
		szprint(zend(temp), "\n$(%1i, %2i)\n", monster, monster_new);
		game::addicon(zend(temp), t2);
	}
	if(ismatch(pl, t2))
	{
		if(interactive)
		{
			if(!dlgask(0, temp))
				return false;
		}
		addresources(pl, pl, t2, true);
		bsset(side, index, monster_new);
	}
	else
	{
		if(interactive)
		{
			szprint(zend(temp), szt("You don't have resources.", "У вас не хватает ресурсов."));
			dlgmsg(0, temp);
		}
		return false;
	}
	return true;
}

int game::gettrade(tokens resf, tokens rest, int markets)
{
	const int max_count = 9;
	static int sale_uncostly[max_count+1] = {0, 25, 37, 50, 62, 74, 87, 100, 112, 124};
	static int sale_costly[max_count + 1] = {0, 50, 74, 100, 124, 149, 175, 200, 224, 249};
	static int costly_costly[max_count+1] = {0, 10, 7, 5, 4, 4, 3, 3, 3, 2};
	static int uncostly_costly[max_count + 1] = {0, 20, 14, 10, 8, 7, 6, 5, 5, 4};
	static int costly_uncostly[max_count + 1] = {0, 5, 4, 3, 2, 2, 2, 2, 2, 1};
	static int buy_costly[max_count + 1] = {0, 5000, 3334, 2500, 2000, 1667, 1429, 1250, 1112, 1000};
	static int buy_uncostly[max_count + 1] = {0, 2500, 1667, 1250, 1000, 834, 715, 625, 556, 500};
	if(!markets)
		return 0;
	if(resf == rest)
		return 0;
	if(markets > max_count)
		markets = max_count;
	switch(resf)
	{
	case Wood:
	case Ore:
		switch(rest)
		{
		case Gold:
			return sale_uncostly[markets];
		case Mercury:
		case Sulfur:
		case Crystal:
		case Gems:
			return uncostly_costly[markets];
		case Wood:
		case Ore:
			return costly_costly[markets];
		default:
			return 0;
		}
		break;
	case Mercury:
	case Sulfur:
	case Crystal:
	case Gems:
		switch(rest)
		{
		case Gold:
			return sale_costly[markets];
		case Mercury:
		case Sulfur:
		case Crystal:
		case Gems:
			return costly_costly[markets];
		case Wood:
		case Ore:
			return costly_uncostly[markets];
		default:
			return 0;
		}
	case Gold:
		switch(rest)
		{
		case Mercury:
		case Sulfur:
		case Crystal:
		case Gems:
			return buy_costly[markets];
		case Wood:
		case Ore:
			return buy_uncostly[markets];
		default:
			return 0;
		}
	default:
		return 0;
	}
}

bool game::trade(int player, tokens from, tokens to, int count, int discount)
{
	int rate = gettrade(from, to, getbuildings(player, MarketPlace) + discount);
	auto resources = (int*)bsptr(player, FirstResource);
	if(!resources || !rate)
		return false;
	int resource_sub[LastResource - FirstResource + 1] = {0};
	int resource_add[LastResource - FirstResource + 1] = {0};
	if(to == Gold)
	{
		if(count > resources[from - FirstResource])
			count = resources[from - FirstResource];
		resource_sub[from - FirstResource] = count;
		resource_add[to - FirstResource] = count*rate;
	}
	else
	{
		if(count > resources[from - FirstResource]/rate)
			count = resources[from - FirstResource]/rate;
		resource_sub[from - FirstResource] = count*rate;
		resource_add[to - FirstResource] = count;
	}
	addresources(resources, resources, resource_sub, true);
	addresources(resources, resources, resource_add);
	return true;
}

bool game::isfullartifacts(int hero)
{
	for(int i = FirstArtifactIndex; i <= LastArtifactIndex; i++)
	{
		if(!bsget(hero, i))
			return false;
	}
	return true;
}

char* game::addicon(char* result, int type, int count)
{
	szprint(result, "\n$(%1i/%2i)", type, count);
	return result;
}

char* game::addicon(char* result, int t1, int c1, int t2, int c2)
{
	szprint(result, "\n$(p%1i/%2i, %3i/%4i)", t1, c1, t2, c2);
	return result;
}

char* game::addicon(char* result, const void* cost_void)
{
	char* p = result;
	auto cost = (int*)cost_void;
	p[0] = 0;
	for(int id = FirstResource; id <= LastResource; id++)
	{
		int value = cost[id - FirstResource];
		if(!value)
			continue;
		if(p == result)
			zcpy(p, "\n$(");
		else
			zcpy(p, ",");
		szprint(zend(p), "%1i/%2i", id, value);
		p = zend(p);
	}
	if(p != result)
		zcat(p, ")");
	return result;
}

int game::getunitscount(int side)
{
	int count = 0;
	for(int i = FirstTroopsIndex; i <= LastTroopsIndex; i += 2)
	{
		if(bsget(side, i))
			count++;
	}
	return count;
}
