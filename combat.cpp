#include "main.h"

static int	attacker, defender;
static int	combat_order[LastCombatant - FirstCombatant + 2];
int			combat::rounds;
int			combat::enviroment::morale;
static int	casting[2];

static int correct_damage(int result) {
	if(result < 1)
		result = 1;
	return result;
}

static void change_morale_to_side(bool attacker) {
	if(attacker)
		combat::enviroment::morale++;
	else
		combat::enviroment::morale--;
	if(combat::enviroment::morale < -3)
		combat::enviroment::morale = -3;
	else if(combat::enviroment::morale > 3)
		combat::enviroment::morale = 3;
}

bool combat::isattacker(int rec) {
	if(rec >= FirstCombatant && rec <= LastCombatant)
		rec = bsget(rec, Side);
	return attacker == rec;
}

int	combat::opposition(int side) {
	return isattacker(side) ? defender : attacker;
}

int combat::attack(int att, int def) {
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

void combat::melee(int att, int def, bool interactive) {
	int result = attack(att, def);
	if(bsget(att, Shoots) && !game::ismeleearcher(att)) {
		result /= 2;
		result = correct_damage(result);
	}
	if(interactive)
		show::battle::attack(att, def, result);
	else
		damage(def, result);
	if(!game::get(def, Count))
		return;
	if(bsget(def, AlreadyDefended) == 0
		&& !game::isstealth(att)
		&& (/*game::get(def, AllAttackAnswer) || */!bsget(def, DefendThisTurn))) {
		bsadd(def, AlreadyDefended, 1);
		result = attack(def, att);
		if(bsget(def, Shoots) && !game::ismeleearcher(def)) {
			result /= 2;
			result = correct_damage(result);
		}
		if(interactive)
			show::battle::attack(def, att, result);
		else
			damage(att, result);
		if(!game::get(att, Count))
			return;
	}
	if(/*bsget(att, TwiceAttack) && */!bsget(att, Shoots)) {
		result = attack(att, def);
		if(interactive)
			show::battle::attack(att, def, result);
		else
			damage(att, result);
	}
}

void combat::shoot(int att, int def, bool interactive) {
	int result = attack(att, def);
	// RULE: Spell Shield implementation.
	if(bsget(def, SpellShield))
		result = correct_damage(result / 2);
	bsadd(att, Shoots, -1, 0);
	if(interactive)
		show::battle::shoot(att, def, result);
	else
		damage(def, result);
}

int combat::getcombatant(int index) {
	index = getindex(index);
	int last = bsget(FirstCombatant, Last);
	for(int rec = FirstCombatant; rec <= last; rec++) {
		if(bsget(rec, HitPoints) == 0)
			continue;
		if(bsget(rec, Index) == index)
			return rec;
	}
	return 0;
}

static bool enemypos(int index, int rec) {
	int e = combat::getcombatant(index);
	if(e == -1)
		return false;
	return combat::isenemy(rec, e);
}

bool combat::canshoot(int rec, int target) {
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
	i = combat::getwideindex(rec);
	if(i != -1) {
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
	}
	return true;
}

bool combat::candefend(int rec) {
	if(bsget(rec, HitPoints) == 0)
		return false;
	if(geteffect(rec, SpellParalyze))
		return false;
	return true;
}

bool combat::canattack(int rec, int target, int target_index, tokens from_direction) {
	if(!combat::isenemy(rec, target))
		return false;
	int i0 = combat::moveto(target_index, from_direction);
	if(i0 == -1)
		return false;
	if(getcombatant(i0) != rec) {
		if(combat::getpassable(i0) > (game::getspeed(game::get(rec, Speed)) + 2))
			return false;
	}
	return true;
}

bool combat::canmove(int rec) {
	if(!game::get(rec, Count))
		return false;
	if(combat::geteffect(rec, SpellBlind))
		return false;
	if(combat::geteffect(rec, SpellParalyze))
		return false;
	return true;
}

bool combat::isenemy(int rec, int object) {
	if(bsget(object, HitPoints) == 0)
		return false;
	return bsget(rec, Side) != bsget(object, Side);
}

void combat::damage(int rec, int value) {
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

void combat::applyeffect(int rec, int effect, int value) {
	if(value == 0) {
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

static void prepare_index() {
	int attacker_index = 0;
	int defender_index = 10;
	for(unsigned rec = FirstCombatant; rec <= LastCombatant; rec++) {
		if(!bsget(rec, Type))
			continue;
		if(combat::isattacker(bsget(rec, Side))) {
			combat::setindex(rec, attacker_index);
			attacker_index += combat::awd * 2;
		} else {
			combat::setindex(rec, defender_index);
			defender_index += combat::awd * 2;
		}
	}
}

void combat::add(int id, int count, int side) {
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

static void add_army(int id, int count, int side) {
	int mcount = imax(count / 5, 1);
	for(int i = 0; i < 5; i++) {
		if(mcount > count)
			mcount = count;
		combat::add(id, mcount, side);
		count -= mcount;
		if(!count)
			break;
	}
	// RULE: if there is possible upgrade
	// make middle unit upgraded
	if((rand() % 100) < 30 && bsget(id, Upgrade) != id) {
	}
}

static void add_army(int rec) {
	if(rec >= FirstHero && rec <= LastHero) {
		for(int i = FirstTroopsIndex; i <= LastTroopsIndex; i += 2) {
			int u = bsget(rec, i);
			if(!u)
				continue;
			combat::add(u, bsget(rec, i + 1), rec);
		}
	} else if(rec >= FirstMapObject && rec <= LastMapObject)
		add_army(bsget(rec, Type), bsget(rec, Count), rec);
}

static void prepare_army(int att, int def) {
	attacker = att;
	defender = def;
	combat::rounds = 1;
	add_army(attacker);
	add_army(defender);
	prepare_index();
}

static bool test_victory(int side) {
	for(unsigned rec = FirstCombatant; rec <= LastCombatant; rec++) {
		if(!bsget(rec, Type))
			continue;
		if(bsget(rec, Side) != side && game::get(rec, Count))
			return false;
	}
	return true;
}

static int compare(const void* p1, const void* p2) {
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

static void prepare_order() {
	int* p = combat_order;
	for(unsigned i = FirstCombatant; i <= LastCombatant; i++) {
		if(!bsget(i, Type))
			continue;
		*p++ = i;
	}
	*p++ = 0;
	qsort(combat_order, p - combat_order - 1, sizeof(combat_order[0]), compare);
}

static void prepare_turn() {
	for(int i = FirstCombatant; i <= LastCombatant; i++) {
		if(!bsget(i, Type))
			continue;
		bsset(i, AlreadyMoved, 0);
	}
	combat::rounds++;
	// Apply combat effects
	for(unsigned i = FirstEffect; i <= LastEffect; i++) {
		if(!bsget(i, Type))
			continue;
		if(bsget(i, Expire) > combat::rounds)
			continue;
		bsset(i, Type, 0);
	}
	// Reset casting operations
	memset(casting, 0, sizeof(casting));
}

static int side_index(int rec) {
	if(combat::isattacker(rec))
		return 0;
	return 1;
}

static int closest_unit(int rec, int side = -1) {
	for(int rec = FirstCombatant; rec <= LastCombatant; rec++) {
	}
	return -1;
}

int combat::getarmycost(int side) {
	int m = 0;
	for(int i = FirstCombatant; i <= LastCombatant; i++) {
		int mt = bsget(i, Type);
		if(!mt)
			continue;
		if(bsget(i, Side) != side)
			continue;
		m += game::get(mt, Gold) * game::get(i, Count);
	}
	// RULE: Diplomacy reduce bribe price
	m -= (m * bsget(side, SkillDiplomacy) * 20) / 100;
	return m;
}

void combat::move(int rec, int index, bool interactive) {
	if(interactive) {
		if(game::isfly(rec))
			show::battle::fly(rec, index);
		else
			show::battle::move(rec, index);
	}
	combat::setindex(rec, index);
}

int combat::geteffect(int rec, int id) {
	int effect = bsfind(FirstEffect, Type, id, Parent, rec);
	if(!effect)
		return 0;
	return bsget(effect, Value) - combat::rounds;
}

void combat::seteffect(int rec, int id, int rounds) {
	int effect = bscreate(FirstEffect);
	if(!effect)
		return;
	bsset(effect, Type, id);
	bsset(effect, Parent, rec);
	bsset(effect, Value, combat::rounds + rounds);
}

static int make_turn_unit(int rec, bool second, bool interactive) {
	int enemy, index;
	int speed = game::getspeed(game::get(rec, Speed));
	bool wide = game::iswide(rec);
	combat::wave(bsget(rec, Index),
		game::isfly(rec),
		wide ? (combat::isattacker(rec) ? HexRight : HexLeft) : Empthy,
		speed + 2);
	int id = 0;
	// RULE: Berserker implementation
	if(combat::geteffect(rec, SpellBerserker)) {
		int target = closest_unit(rec);
		if(target != -1) {
			combat::attack(rec, target);
		}
	} else {
		while(true) {
			// If player command this, he must specify action.
			// Or Computer may select action by itself.
			id = show::battle::unit(rec, casting[side_index(rec)]);
			if(id >= FirstSpell && id <= LastSpell) {
				int side = bsget(rec, Side);
				int target = game::gettarget(id);
				if(target) {
					target = show::battle::target(side, id, target);
					if(!target)
						continue;
				}
				if(interactive)
					show::battle::leader(side, CastSpell);
				combat::cast(side, id, target, true, false, interactive);
				casting[side_index(rec)]++;
				continue;
			} else if(id == Surrender) {
				int side = bsget(rec, Side);
				int cost = combat::getarmycost(side);
				if(interactive) {
					char temp[260];
					szprint(temp, szt("%1 leave you away if you pay bribe. Would you pay %2i golds to save your troops and go away?",
						"%1 оставит тебя в покое если вы заплатите взятку. Вы готовы заплатить %2i золотых, чтобы сохранить вашу армию и уйти прочь?"),
						bsgets(combat::opposition(side), Name), cost);
					if(!dlgask(0, temp))
						continue;
					// TODO: pay bribe to opposition hero
				}
			} else if(id == RunAway) {
				if(interactive) {
					if(!dlgask(0,
						szt("Do you really want to leave your army and flee away?", "Вы действительно хотите покинуть армию и бежать прочь?")))
						continue;
				}
			}
			break;
		}
	}
	switch(id) {
	case Cancel:
	case 0:
		return Cancel;
	case Skip:
		break;
	case Move:
		combat::move(rec, hot::param, interactive);
		break;
	case Shoot:
		combat::shoot(rec, hot::param, interactive);
		// RULEX: Each shoot change morale
		if(!second)
			change_morale_to_side(combat::isattacker(rec));
		break;
	case Attack:
		enemy = hot::param;
		index = hot::param2;
		combat::move(rec, index, interactive);
		combat::melee(rec, enemy, interactive);
		// RULEX: Each melee attack change morale to this side
		if(!second)
			change_morale_to_side(combat::isattacker(rec));
		break;
	case Surrender:
		return Surrender;
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

static int make_turn(bool interactive) {
	for(int i = 6; i >= 0; i--) {
		for(unsigned rec = FirstCombatant; rec <= LastCombatant; rec++) {
			if(!bsget(rec, Type))
				continue;
			auto speed = game::getspeed(game::get(rec, Speed));
			if(speed != i)
				continue;
			if(bsget(rec, AlreadyMoved))
				continue;
			if(!combat::canmove(rec))
				continue;
			int morale = game::get(rec, Morale);
			if(morale < 0) {
				// RULE: Bad Morale compel creature skip action
				int d = d100();
				if(d < game::getmoralechance(-morale)) {
					if(interactive)
						show::battle::effect(rec, Morale, 0);
					bsset(rec, AlreadyMoved, 1);
					return Accept;
				}
			}
			int id = make_turn_unit(rec, false, interactive);
			if(id == Accept) {
				morale = game::get(rec, Morale);
				if(morale > 0) {
					// RULE: Morale can boost creature to another action
					int d = d100();
					if(d < game::getmoralechance(morale)) {
						if(interactive)
							show::battle::effect(rec, Morale, 1);
						id = make_turn_unit(rec, true, interactive);
					}
				}
			}
			return id;
		}
	}
	return EndTurn;
}

void combat::start(int attacker, int defender) {
	bsmeta::getbase("combat_unit")->clear();
	bsmeta::getbase("combat_effect")->clear();
	combat::board(attacker, defender);
	prepare_army(attacker, defender);
	// RULEX: Scouting can increase start morale
	combat::enviroment::morale = bsget(attacker, SkillScounting) - bsget(defender, SkillScounting);
	bool interactive = true;
	while(true) {
		prepare_order();
		int id = make_turn(interactive);
		switch(id) {
		case Cancel:
			return;
		case AttackerWin:
		case DefenderWin:
			return;
		case EndTurn:
			prepare_turn();
			break;
		case RunAway:
			show::fadeback(3);
			dlgmsg(0, "Test");
			return;
		case Surrender:
			show::fadeback(3);
			dlgmsg(0, "Test");
			return;
		}
	}
}