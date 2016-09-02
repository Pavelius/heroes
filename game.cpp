#include "main.h"

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

bool game::isboosted(int rec)
{
	for(int eid = FirstEffect; eid <= LastEffect; eid++)
	{
		if(bsget(eid, Target) != rec)
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
		if(bsget(eid, Target) != rec)
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
	for(int i = FirstTroopsIndex; i <= LastTroopsIndex; i++)
		bsset(rec, i, 0);
}

void game::addunit(int rec, int type, int count)
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
				return;
			}
		}
		// Add new one
		for(int i = FirstTroopsIndex; i <= LastTroopsIndex; i += 2)
		{
			if(!bsget(rec, i))
			{
				bsset(rec, i, type);
				bsset(rec, i + 1, count);
				return;
			}
		}
	}
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

int game::getsummary(int rec, int id, int side)
{
	int t = rec;
	if(t >= FirstCombatant && t <= LastCombatant)
		t = bsget(t, Type);
	int m = get(t, id);
	if(side)
		m += get(side, id);
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
	case Attack:
	case Defence:
	case Speed:
	case DamageMin:
	case DamageMax:
		if(rec >= FirstHero && rec <= LastHero)
			return bsget(rec, id) + artifacts_bonuses(rec, id);
		else if(rec >= FirstCombatant && rec <= LastCombatant)
			return getsummary(rec, id, bsget(rec, Side));
		return bsget(rec, id);
	case Wisdow:
	case SpellPower:
		if(rec >= FirstHero && rec <= LastHero)
			return bsget(rec, id) + artifacts_bonuses(rec, id);
		return bsget(rec, id);
	case Morale:
		if(rec >= FirstHero && rec <= LastHero)
			return bsget(rec, SkillLeadership) + artifacts_bonuses(rec, id);
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
	case AllAttackAnswer:
		if(rec >= FirstCombatant && rec <= LastCombatant)
			rec = bsget(rec, Type);
		switch(rec)
		{
		case Griffin:
			return 1;
		default:
			return 0;
		}
	case TwiceAttack:
		if(rec >= FirstCombatant && rec <= LastCombatant)
			rec = bsget(rec, Type);
		switch(rec)
		{
		case Ranger:
		case Crusader:
		case Paladin:
		case Wolf:
		case Elf:
		case GrandElf:
			return 1;
		default:
			return 0;
		}
	default:
		return bsget(rec, id);
	}
}

bool game::isstealth(int rec)
{
	if(rec >= FirstCombatant && rec <= LastCombatant)
		rec = bsget(rec, Type);
	switch(rec)
	{
	case Sprite:
	case Hydra:
	case Vampire:
	case VampireLord:
	case Rogue:
		return true;
	default:
		return false;
	}
}

bool game::ismeleearcher(int rec)
{
	if(rec >= FirstCombatant && rec <= LastCombatant)
		rec = bsget(rec, Type);
	switch(rec)
	{
	case Mage:
	case ArchMage:
	case Lich:
	case PowerLich:
		return true;
	default:
		return false;
	}
}

bool game::iswide(int rec)
{
	if(rec >= FirstCombatant && rec <= LastCombatant)
		rec = bsget(rec, Type);
	switch(rec)
	{
	case Cavalry:
	case Champion:
	case Wolf:
	case Unicorn:
	case Phoenix:
	case Centaur:
	case Griffin:
	case Hydra:
	case GreenDragon:
	case RedDragon:
	case BlackDragon:
	case Boar:
	case Roc:
	case BoneDragon:
	case Nomand:
	case Medusa:
		return true;
	default:
		return false;
	}
}

bool game::isfly(int rec)
{
	if(rec >= FirstCombatant && rec <= LastCombatant)
		rec = bsget(rec, Type);
	switch(rec)
	{
	case Sprite:
	case Phoenix:
	case Gargoyle:
	case Griffin:
	case GreenDragon:
	case RedDragon:
	case BlackDragon:
	case Roc:
	case Vampire:
	case VampireLord:
	case BoneDragon:
	case Ghost:
	case Genie:
		return true;
	default:
		return false;
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