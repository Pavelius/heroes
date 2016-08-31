#include "main.h"

int artifacts_bonuses(int rec, int id)
{
	short unsigned* p = (short unsigned*)bsptr(rec, FirstArtifactIndex);
	if(!p)
		return 0;
	int result = 0;
	for(int i = 0; i < LastArtifactIndex - FirstArtifactIndex + 1; i++)
	{
		if(!p[i])
			continue;
		result += game::getartifact(p[i], id);
	}
	return result;
}

int game::geteffect(int rec, int id)
{
	return 0;
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

bool game::candefend(int rec)
{
	if(bsget(rec, HitPoints) == 0)
		return false;
	if(geteffect(rec, SpellParalyze) != 0)
		return false;
	return true;
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
	// Add in existing troops
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

int game::get(int rec, int id)
{
	int m, k;
	switch(id)
	{
	case Attack:
		if(rec >= FirstHero && rec <= LastHero)
			return bsget(rec, id) + artifacts_bonuses(rec, id);
		else if(rec >= FirstCombatant && rec <= LastCombatant)
		{
			m = get(bsget(rec, Type), id);
			m += get(bsget(rec, Side), id);
			// RULE: Spell Boold Lust.
			if(get(rec, SpellBloodLust))
				m += 3;
			return m;
		}
		return bsget(rec, id);
	case Defence:
		if(rec >= FirstHero && rec <= LastHero)
			return bsget(rec, id) + artifacts_bonuses(rec, id);
		else if(rec >= FirstCombatant && rec <= LastCombatant)
		{
			m = get(bsget(rec, Type), id);
			m += get(bsget(rec, Side), id);
			// RULE: Spell Stone skin.
			if(get(rec, SpellStone))
				m += 3;
			// RULE: spell Steel skin.
			else if(get(rec, SpellSteelSkin))
				m += 5;
			return m;
		}
		return bsget(rec, id);
	case Wisdow:
	case SpellPower:
		if(rec >= FirstHero && rec <= LastHero)
			return bsget(rec, id) + artifacts_bonuses(rec, id);
		return bsget(rec, id);
	case Morale:
		if(rec >= FirstHero && rec <= LastHero)
			return MoraleNormal + artifacts_bonuses(rec, id);
		return bsget(rec, id);
	case Luck:
		if(rec >= FirstHero && rec <= LastHero)
			return LuckNormal + artifacts_bonuses(rec, id);
		return bsget(rec, id);
	case Speed:
		if(rec >= FirstCombatant && rec <= LastCombatant)
		{
			// RULE: spell Haste implementation
			m = 0;
			if(get(rec, SpellHaste))
				m = 2;
			return imin(get(bsget(rec, Type), Speed) + m, (int)UltraFast);
		}
		return bsget(rec, id);
	case Count:
		if(rec >= FirstCombatant && rec <= LastCombatant)
		{
			int t = bsget(rec, Type);
			int m = bsget(t, HitPointsMax);
			return (bsget(rec, HitPointsMax) + m - 1) / m;
		}
		return 0;
	case HitPoints:
		if(rec >= FirstCombatant && rec <= LastCombatant)
		{
			int t = bsget(rec, Type);
			int m = bsget(t, HitPointsMax);
			return 1 + (bsget(rec, HitPointsMax) - 1) % m;
		}
		return 0;
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
				case Slow: ms = 1100; break;
				case Average: ms = 1200; break;
				case Fast: ms = 1300; break;
				case VeryFast: ms = 1400; break;
				case UltraFast: ms = 1500; break;
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
		if(rec <= (int)Dimitry || rec == Corlagon)
			return Knight;
		if(rec <= (int)Atlas || rec == Halton)
			return Barbarian;
		if(rec <= (int)Luna || rec == Eliza)
			return Sorcerer;
		if(rec <= (int)Wrathmont || rec == Archibald)
			return Warlock;
		if(rec <= (int)Mandigal || rec == Roland)
			return Wizard;
		return Necromancer;
	case ArmyCost:
		m = 0;
		for(int i = FirstTroopsIndex; i <= LastTroopsIndex; i += 2) 
		{
			int u = bsget(rec, i);
			if(!u)
				continue;
			m += get(u, Gold) * bsget(rec, i+1);
		}
		return m;
	default:
		return bsget(rec, id);
	}
}