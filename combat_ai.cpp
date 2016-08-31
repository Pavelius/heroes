#include "main.h"

static int			attacker, defender;
static int			combat_order[LastCombatant - FirstCombatant + 2];
int					combat::damage;
int					combat::killed;
unsigned char		combat::movements[combat::ahd*combat::awd];
int					combat::rounds;
static int			casting[2];

bool combat::isattacker(int rec)
{
	return attacker == rec;
}

int	combat::opposition(int side)
{
	return isattacker(side) ? defender : attacker;
}

static void correct_damage()
{
	if(combat::damage < 1)
		combat::damage = 1;
}

void combat::attack(int att, int def)
{
	int dm1 = bsget(att, DamageMin);
	int dm2 = bsget(att, DamageMax);
	if(bsget(att, SpellBless))
		dm1 = dm2; // RULE: apply spell Bless
	else if(bsget(att, SpellCurse))
		dm2 = dm1; // RULE: apply spell Curse
	int count = bsget(att, Count);
	damage = xrand(dm1*count, dm2*count);
	int a1 = bsget(att, Attack);
	int d1 = bsget(def, Defence);
	int r = a1 - d1;
	if(r > 0)
		damage += (damage*imin(r, 30)) / 10;
	else if(r < 0)
		damage -= (damage*imin(-r, 12) * 5) / 100;
	correct_damage();
}

void combat::melee(int att, int def)
{
	attack(att, def);
	if(bsget(att, Shoots) && !bsget(att, MeleeArcher))
	{
		damage /= 2;
		correct_damage();
	}
	bsset(def, Damage, 1);
	if(bsget(def, CanDefend) && !bsget(att, HideAttack)
		&& (bsget(def, AllAttackAnswer) || !bsget(def, DefendThisTurn)))
	{
		attack(def, att);
		bsset(def, DefendThisTurn, 1);
		if(bsget(def, Shoots) && !bsget(def, MeleeArcher))
		{
			damage /= 2;
			correct_damage();
		}
		bsset(att, Damage, 1);
		if(bsget(att, Dead))
			return;
	}
	if(bsget(att, TwiceAttack) && !bsget(att, Shoots))
	{
		attack(att, def);
		bsset(def, Damage, 2);
	}
}

void combat::shoot(int att, int def)
{
	attack(att, def);
	// RULE: Spell Shield implementation.
	if(bsget(def, SpellShield))
		combat::damage = imax(1, combat::damage / 2);
	bsset(def, Damage, 1);
	bsadd(def, Shoots, -1, 0);
}

int combat::combatant(int index)
{
	return bsfind(FirstCombatant, Position, index);
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
	int i = bsget(rec, Position);
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
	int i0 = bsget(rec, Position);
	int i1 = bsget(target, Position);
	int i2 = combat::moveto(i1, direction);
	if(i2 == -1)
		return false;
	int speed = bsget(rec, Speed) - Crawling + 2;
	return i2 == i0
		|| (combat::movements[i2] != 0 && combat::movements[i2] <= speed);
}

bool combat::isenemy(int rec, int object)
{
	if(bsget(object, Count) == 0)
		return false;
	return bsget(rec, Side) != bsget(object, Side);
}

static void prepare_index()
{
	int attacker_index = 0;
	int defender_index = 10;
	for(unsigned rec = FirstCombatant; rec <= LastCombatant; rec++)
	{
		if(!bsget(rec, Valid))
			continue;
		if(combat::isattacker(bsget(rec, Side)))
		{
			bsset(rec, Position, attacker_index);
			attacker_index += combat::awd * 2;
		}
		else
		{
			bsset(rec, Position, defender_index);
			defender_index += combat::awd * 2;
		}
	}
}

static int add_unit(int id, int count, int side)
{
	int rec = bscreate(FirstCombatant);
	if(!rec)
		return 0;
	bsset(rec, Type, id);
	bsset(rec, Count, count);
	bsset(rec, Side, side);
	return rec;
}

static void add_army(int id, int count, int side)
{
	int mcount = imax(count / 5, 1);
	for(int i = 0; i < 5; i++)
	{
		if(mcount > count)
			mcount = count;
		add_unit(id, mcount, side);
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
			add_unit(u, bsget(rec, i + 1), rec);
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
		if(!bsget(rec, Valid))
			continue;
		if(bsget(rec, Side) != side && bsget(rec, Count))
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
		if(!bsget(i, Valid))
			continue;
		*p++ = i;
	}
	*p++ = 0;
	qsort(combat_order, p - combat_order - 1, sizeof(combat_order[0]), compare);
}

static void prepare_turn()
{
	for(unsigned i = FirstCombatant; i <= LastCombatant; i++)
	{
		if(!bsget(i, Valid))
			continue;
		bsset(i, AlreadyMoved, 0);
	}
	combat::rounds++;
	for(unsigned i = FirstEffect; i <= LastEffect; i++)
	{
		if(!effect::get(i, Valid))
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
	for(unsigned rec = FirstCombatant; rec <= LastCombatant; rec++)
	{
	}
	return -1;
}

static int make_turn(bool interactive)
{
	for(unsigned i = UltraFast; i >= Crawling; i--)
	{
		for(unsigned rec = FirstCombatant; rec <= LastCombatant; rec++)
		{
			if(!bsget(rec, Valid))
				continue;
			if(bsget(rec, Speed) != (int)i)
				continue;
			if(bsget(rec, AlreadyMoved))
				continue;
			if(!bsget(rec, Count))
				continue;
			combat::wave(bsget(rec, Position),
				bsget(rec, Wide) != 0,
				bsget(rec, Fly) != 0);
			int id = 0;
			// RULE: spell Berserker implementation
			if(bsget(rec, SpellBerserker))
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
					if(bsget(id, First) == FirstSpell)
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
				if(interactive)
				{
					if(bsget(rec, Fly))
						show::battle::fly(rec, hot::param);
					else
						show::battle::move(rec, hot::param);
				}
				bsset(rec, Position, hot::param);
				break;
			case Shoot:
				if(interactive)
					show::battle::shoot(rec, hot::param);
				combat::shoot(rec, hot::param);
				break;
			case Attack:
				// TODO: Animation for moving and attack.
				bsset(rec, Position, hot::param2);
				combat::melee(rec, hot::param);
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