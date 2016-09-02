#include "main.h"

static int select_squads(int* result, int side = -1, int undead = -1, int dragon = -1, int elements_immunity = -1, int exclude = -1, int sid = -1)
{
	int count = 0;
	for(unsigned i = FirstCombatant; i<=LastCombatant; i++)
    {
    	if(!bsget(i, Type))
			continue;
        if(bsget(i, MagicImmunity))
            continue;
		if(bsget(i, SpellAntimagic))
			continue;
		if(side!=-1 && bsget(i, Side)!=side)
			continue;
		if(undead!=-1 && bsget(i, Undead)!=undead)
			continue;
		if(dragon!=-1 && bsget(i, Dragon)!=dragon)
			continue;
		if(exclude!=-1 && bsget(i, Type)==exclude)
			continue;
		if(elements_immunity!=-1 && bsget(i, ElementsImmunity)!=elements_immunity)
			continue;
		if(result==0 && sid!=-1 && (bsget(i, sid) || !combat::cast(bsget(i,Side), sid, i, -1, false, false)))
			continue;
		if(result && sid!=-1 && !combat::cast(bsget(i,Side), sid, i, -1, false, false))
			continue;
		if(result)
			result[count] = i;
		count++;
	}
	if(result)
		result[count] = 0;
	return count;
}

bool combat::cast(int side, int sid, int cid, int pos, bool run, bool free)
{
	int m;
	int su[LastCombatant-FirstCombatant+1];
	if(!free)
	{
		// We need enought spell points to cast spell
		if(bsget(side, SpellPoints)<bsget(sid, SpellPoints))
			return false;
	}
	if(cid!=-1)
	{
		// Target has magic immunity (like Black Dragon)
		if(bsget(cid, MagicImmunity))
			return false;
		// Target has active anti-magic spell
		if(bsget(cid, SpellAntimagic))
			return false;
		// No stack spells
		if(bsget(cid, sid))
			return false;
		// Hostile spells required enemy unit as target
		if(bsget(sid, Hostile) && bsget(cid, Side)==side)
			return false;
		// Friendly spells required friendly unit as target
		if(bsget(sid, Friendly) && bsget(cid, Side)!=side)
			return false;
	}
	else
	{
		// Spell required target must be error
		if(bsget(sid, RequiredTarget)!=0)
			return false;
	}
	switch(sid)
	{
	case SpellMassHaste:
	case SpellMassBless:
	case SpellMassShield:
	case SpellMassCure:
	case SpellMassDispel:
	case SpellMassCurse:
	case SpellMassSlow:
		// All mass spell cast single version on each valid units
		if(!select_squads(0,-1,-1,-1,-1,-1,bsget(sid,SingleVersion)))
			return false;
		if(run)
		{
			select_squads(su,-1,-1,-1,-1,-1,bsget(sid,SingleVersion));
			for(auto e : su)
			{
				if(!e)
					break;
				cast(side, sid, bsget(sid,SingleVersion), pos, run, true);
			}
		}
		break;
	case SpellCure:
		m = bsget(cid, HitPointsMax);
		if(bsget(cid, HitPoints)>=m)
			return false;
		if(run)
			bsset(cid, HitPoints, m);
		break;
	case SpellAntimagic:
		break;
	case SpellStoneSkin:
	case SpellSteelSkin:
	case SpellBless:
	case SpellBloodLust:
	case SpellDragonSlayer:
	case SpellHaste:
	case SpellShield:
		// RULEX: This spell not cast on metal golem
		if(sid==SpellSteelSkin || sid==SpellStoneSkin)
		{
			switch(bsget(cid, Type))
			{
			case Titan:
			case Giant:
			case SteelGolem:
			case IronGolem:
				return false;
			}
		}
		if(run)
			bsset(cid, sid, bsget(side, SpellPower));
		break;
	case SpellDispel:
		if(run)
			bsset(cid, SpellDispel, 0);
		break;
	case SpellCurse:
	case SpellBlind:
	case SpellBerserker:
	case SpellParalyze:
		// RULE: Curse don't work on Titan and Giant
		if(sid==SpellCurse)
		{
			switch(bsget(cid, Type))
			{
			case Titan:
			case Giant:
				return false;
			}
		}
		if(run)
			bsset(cid, sid, bsget(side, SpellPower));
		break;
	case SpellDisruptingRay:
	case SpellSlow:
		if(run)
			bsset(cid, sid, bsget(side, SpellPower));
		break;
	case SpellHypnotize:
		break;
	case SpellTeleport:
		if(run)
			bsset(cid, Index, pos);
		break;
	case SpellResurrect:
		break;
	case SpellRessurectTrue:
		break;
	case SpellAnimateDead:
		break;
	case SpellMirrorImage:
		break;
	case SpellMagicArrow:
	case SpellLightingBolt:
	case SpellColdRay:
		if(run)
		{
			int result = bsget(sid, SpellPower)*bsget(side, SpellPower);
			combat::damage(cid, result);
		}
		break;
	case SpellColdRing:
		break;
	case SpellChainLighting:
		break;
	case SpellFireBall:
		if(pos==-1)
			return false;
		if(run)
		{
		}
		break;
	case SpellFireBlast:
		if(pos==-1)
			return false;
		if(run)
		{
		}
		break;
	case SpellMeteorShower:
		if(pos==-1)
			return false;
		if(run)
		{

		}
		break;
	case SpellDeathRipple:
	case SpellDeathWave:
		if(!select_squads(su,-1,0))
			return false;
		m = bsget(sid, SpellPower)*bsget(side, SpellPower);
		for(auto e : su)
		{
			if(!e)
				break;
			combat::damage(e, m);
		}
		break;
	case SpellHolyWord:
	case SpellHolyShout:
		if(!select_squads(su,-1,1))
			return false;
		m = bsget(sid, SpellPower)*bsget(side, SpellPower);
		for(auto e : su)
		{
			if(!e)
				break;
			combat::damage(e, m);
		}
		break;
	case SpellArmagedon:
		if(!select_squads(su))
			return false;
		m = bsget(sid, SpellPower)*bsget(side, SpellPower);
		for(auto e : su)
		{
			if(!e)
				break;
			combat::damage(e, m);
		}
		break;
	case SpellElementalStorm:
		if(!select_squads(su,-1,-1,-1,0,-1))
			return false;
		m = bsget(sid, SpellPower)*bsget(side, SpellPower);
		for(auto e : su)
		{
			if(!e)
				break;
			combat::damage(e, m);
		}
		break;
	default:
		return false;
	}
	if(run)
	{
		if(!free)
			bsset(side, SpellPoints, bsget(side, SpellPoints) - bsget(sid, SpellPoints));
		if(cid!=-1)
		{
			// RULE: Spells that cancel each other
			switch(sid)
			{
			case SpellBless:
			case SpellCure:
				bsset(cid, SpellCurse, 0);
				break;
			case SpellCurse:
				bsset(cid, SpellBless, 0);
				break;
			case SpellSteelSkin:
				bsset(cid, SpellStoneSkin, 0);
				break;
			case SpellStoneSkin:
				bsset(cid, SpellSteelSkin, 0);
				break;
			}
		}
	}
	return true;
}