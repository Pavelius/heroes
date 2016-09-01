#include "main.h"

static int			attacker, defender;
static int			combat_order[LastCombatant - FirstCombatant + 2];
unsigned char		combat::movements[combat::ahd*combat::awd];
int					combat::rounds;
static int			casting[2];

bool combat::isattacker(int rec)
{
	if(rec >= FirstCombatant && rec <= LastCombatant)
		rec = bsget(rec, Side);
	return attacker == rec;
}

int	combat::opposition(int side)
{
	return isattacker(side) ? defender : attacker;
}

static int correct_damage(int result)
{
	if(result < 1)
		result = 1;
	return result;
}

int combat::attack(int att, int def)
{
	int dm1 = game::get(att, DamageMin);
	int dm2 = game::get(att, DamageMax);
	int count = game::get(att, Count);
	int result = xrand(dm1*count, dm2*count);
	int a1 = game::get(att, Attack);
	int d1 = game::get(def, Defence);
	int r = a1 - d1;
	if(r > 0)
		result += (result*imin(r, 30)) / 10;
	else if(r < 0)
		result -= (result*imin(-r, 12) * 5) / 100;
	return correct_damage(result);
}

void combat::melee(int att, int def, bool interactive)
{
	int result = attack(att, def);
	if(bsget(att, Shoots) && !game::ismeleearcher(att))
	{
		result /= 2;
		result = correct_damage(result);
	}
	if(interactive)
		show::battle::attack(att, def, result);
	else
		applydamage(def, result);
	if(!game::get(def, Count))
		return;
	if(bsget(def, AlreadyDefended)==0
		&& !game::ishideattack(att)
		&& (bsget(def, AllAttackAnswer) || !bsget(def, DefendThisTurn)))
	{
		bsadd(def, AlreadyDefended, 1);
		result = attack(def, att);
		if(bsget(def, Shoots) && !bsget(def, MeleeArcher))
		{
			result /= 2;
			result = correct_damage(result);
		}
		if(interactive)
			show::battle::attack(def, att, result);
		else
			applydamage(att, result);
		if(!game::get(att, Count))
			return;
	}
	if(bsget(att, TwiceAttack) && !bsget(att, Shoots))
	{
		result = attack(att, def);
		if(interactive)
			show::battle::attack(att, def, result);
		else
			applydamage(att, result);
	}
}

void combat::shoot(int att, int def)
{
	int result = attack(att, def);
	// RULE: Spell Shield implementation.
	if(bsget(def, SpellShield))
		result = correct_damage(result / 2);
	applydamage(def, result);
	bsadd(att, Shoots, -1, 0);
}

int combat::combatant(int index)
{
	return bsfind(FirstCombatant, Index, index);
}

static bool enemypos(int index, int rec)
{
	int e = combat::combatant(index);
	if(e == -1)
		return false;
	return combat::isenemy(rec, e);
}

bool combat::canshoot(int rec, int target)
{
	if(!combat::isenemy(rec, target))
		return false;
	if(!bsget(rec, Shoots))
		return false;
	int i = bsget(rec, Index);
	if(enemypos(combat::moveto(i, HexLeft), rec))
		return false;
	if(enemypos(combat::moveto(i, HexRight), rec))
		return false;
	if(enemypos(combat::moveto(i, HexLeftUp), rec))
		return false;
	if(enemypos(combat::moveto(i, HexLeftDown), rec))
		return false;
	if(enemypos(combat::moveto(i, HexRightUp), rec))
		return false;
	if(enemypos(combat::moveto(i, HexRightDown), rec))
		return false;
	return true;
}

bool combat::canattack(int rec, int target, tokens direction)
{
	if(!combat::isenemy(rec, target))
		return false;
	int i0 = bsget(rec, Index);
	int i1 = bsget(target, Index);
	int i2 = combat::moveto(i1, direction);
	if(i2 == -1)
		return false;
	int speed = game::get(rec, Speed) - SpeedCrawling + 2;
	return i2 == i0
		|| (combat::movements[i2] != 0 && combat::movements[i2] <= speed);
}

bool combat::isenemy(int rec, int object)
{
	if(game::get(object, Count) == 0)
		return false;
	return bsget(rec, Side) != bsget(object, Side);
}

void combat::applydamage(int rec, int value)
{
	if(!value)
		return;
	// Calculate killed
	bsadd(rec, HitPoints, -value, 0);
	// RULE: clean all mind-affect spells when damaged
	bsset(rec, SpellBlind, 0);
	bsset(rec, SpellParalyze, 0);
	bsset(rec, SpellStone, 0);
	bsset(rec, SpellBerserker, 0);
	bsset(rec, SpellHypnotize, 0);
	// Make visible effect
	if(!bsget(rec, HitPoints))
		setaction(rec, Killed);
	else
		setaction(rec, Damage);
}

void combat::applyeffect(int rec, int effect, int value)
{
	if(value == 0)
	{
		int e = bsfind(FirstEffect, Type, effect, Target, rec);
		if(e)
			bsdelete(e);
		return;
	}
	int e = bsfind(FirstEffect, Type, effect, Target, rec);
	if(!e)
		e = bscreate(FirstEffect);
	bsset(e, Type, effect);
	bsset(e, Target, rec);
	bsset(e, Expire, combat::rounds + value);
}

static void prepare_index()
{
	int attacker_index = 0;
	int defender_index = 10;
	for(unsigned rec = FirstCombatant; rec <= LastCombatant; rec++)
	{
		if(!bsget(rec, Type))
			continue;
		if(combat::isattacker(bsget(rec, Side)))
		{
			combat::setindex(rec, attacker_index);
			attacker_index += combat::awd * 2;
		}
		else
		{
			combat::setindex(rec, defender_index);
			defender_index += combat::awd * 2;
		}
	}
}

void combat::add(int id, int count, int side)
{
	if(count <= 0)
		return;
	int rec = bscreate(FirstCombatant);
	if(!rec)
		return;
	bsset(rec, Type, id);
	bsset(rec, Count, count);
	bsset(rec, Side, side);
	bsset(rec, Shoots, bsget(id, Shoots));
	bsset(rec, HitPoints, count * bsget(id, HitPointsMax));
}

static void add_army(int id, int count, int side)
{
	int mcount = imax(count / 5, 1);
	for(int i = 0; i < 5; i++)
	{
		if(mcount > count)
			mcount = count;
		combat::add(id, mcount, side);
		count -= mcount;
		if(!count)
			break;
	}
	// RULE: if there is possible upgrade
	// make middle unit upgraded
	if((rand() % 100) < 30 && bsget(id, Upgrade) != id)
	{
	}
}

static void add_army(int rec)
{
	if(rec>=FirstHero && rec<=LastHero)
	{
		for(int i = FirstTroopsIndex; i <= LastTroopsIndex; i += 2)
		{
			int u = bsget(rec, i);
			if(!u)
				continue;
			combat::add(u, bsget(rec, i + 1), rec);
		}
	}
	else if(rec >= FirstMoveable && rec <= LastMoveable)
		add_army(bsget(rec, Type), bsget(rec, Count), rec);
}

static void prepare_army(int att, int def)
{
	attacker = att;
	defender = def;
	combat::rounds = 1;
	add_army(attacker);
	add_army(defender);
	prepare_index();
	effect::clear();
}

static bool test_victory(int side)
{
	for(unsigned rec = FirstCombatant; rec <= LastCombatant; rec++)
	{
		if(!bsget(rec, Type))
			continue;
		if(bsget(rec, Side) != side && game::get(rec, Count))
			return false;
	}
	return true;
}

static int compare(const void* p1, const void* p2)
{
	int s1 = bsget(*((int*)p1), Speed);
	int s2 = bsget(*((int*)p2), Speed);
	if(s1 != s2)
		return s1 - s2;
	s1 = bsget(*((int*)p1), Side);
	s2 = bsget(*((int*)p2), Side);
	if(s1 != s2)
		return combat::isattacker(s1) ? 1 : -1;
	return *((int*)p2) - *((int*)p1);
}

static void prepare_order()
{
	int* p = combat_order;
	for(unsigned i = FirstCombatant; i <= LastCombatant; i++)
	{
		if(!bsget(i, Type))
			continue;
		*p++ = i;
	}
	*p++ = 0;
	qsort(combat_order, p - combat_order - 1, sizeof(combat_order[0]), compare);
}

static void prepare_turn()
{
	for(int i = FirstCombatant; i <= LastCombatant; i++)
	{
		if(!bsget(i, Type))
			continue;
		bsset(i, AlreadyMoved, 0);
	}
	combat::rounds++;
	for(unsigned i = FirstEffect; i <= LastEffect; i++)
	{
		if(!effect::get(i, Type))
			continue;
		if(effect::get(i, Expire) > combat::rounds)
			continue;
		effect::set(i, Type, 0);
	}
	memset(casting, 0, sizeof(casting));
}

static int side_index(int rec)
{
	if(combat::isattacker(rec))
		return 0;
	return 1;
}

static int closest_unit(int rec, int side = -1)
{
	for(int rec = FirstCombatant; rec <= LastCombatant; rec++)
	{
	}
	return -1;
}

void combat::move(int rec, int index, bool interactive)
{
	if(interactive)
	{
		if(bsget(rec, Fly))
			show::battle::fly(rec, index);
		else
			show::battle::move(rec, index);
	}
	combat::setindex(rec, index);
}

static void shoot_creature(int attacker, int defender, bool interactive)
{
	if(interactive)
		show::battle::shoot(attacker, defender);
}

static int make_turn(bool interactive)
{
	int enemy, index;
	for(int i = SpeedUltraFast; i >= SpeedCrawling; i--)
	{
		for(unsigned rec = FirstCombatant; rec <= LastCombatant; rec++)
		{
			if(!bsget(rec, Type))
				continue;
			auto speed = tokens(game::get(rec, Speed));
			if(speed != i)
				continue;
			if(bsget(rec, AlreadyMoved))
				continue;
			if(!game::get(rec, Count))
				continue;
			combat::wave(bsget(rec, Index),
				bsget(rec, Wide) != 0,
				bsget(rec, Fly) != 0);
			int id = 0;
			// RULE: spell Berserker implementation
			if(game::geteffect(rec, SpellBerserker))
			{
				int target = closest_unit(rec);
				if(target != -1)
				{
					combat::attack(rec, target);
				}
			}
			else
			{
				// TODO: decision mechanics
				while(true)
				{
					id = show::battle::unit(rec, casting[side_index(rec)]);
					if(id>=FirstSpell && id<=LastSpell)
					{
						int side = bsget(rec, Side);
						int target = -1;
						if(bsget(id, RequiredTarget))
						{
							target = show::battle::target(side, id);
							if(!target)
								continue;
						}
						combat::cast(side, id, target, 0, true, false);
						casting[side_index(rec)]++;
						continue;
					}
					break;
				}
			}
			switch(id)
			{
			case Cancel:
			case 0:
				return Cancel;
			case Skip:
				break;
			case Move:
				combat::move(rec, hot::param, interactive);
				break;
			case Shoot:
				shoot_creature(rec, hot::param, interactive);
				combat::shoot(rec, hot::param);
				break;
			case Attack:
				enemy = hot::param;
				index = hot::param2;
				combat::move(rec, index, interactive);
				combat::melee(rec, enemy, interactive);
				break;
			case RunAway:
				return RunAway;
			}
			bsset(rec, AlreadyMoved, 1);
			if(test_victory(attacker))
				return AttackerWin;
			if(test_victory(defender))
				return DefenderWin;
			return Accept;
		}
	}
	return EndTurn;
}

void combat::start(int attacker, int defender)
{
	command::execute("battle", "initialize");
	combat::board(attacker, defender);
	prepare_army(attacker, defender);
	bool interactive = true;
	while(true)
	{
		prepare_order();
		int id = make_turn(interactive);
		switch(id)
		{
		case Cancel:
			return;
		case AttackerWin:
		case DefenderWin:
			return;
		case EndTurn:
			prepare_turn();
			break;
		case RunAway:
			show::fadeback(6);
			dlgmsg(0, "Test");
			return;
		}
	}
}