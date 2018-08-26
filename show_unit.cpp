#include "main.h"

static const int ox = 10;
static const int oy = 18;

static int field(int x, int y, int id, int rec, int side, const char* name) {
	char temp[260];
	if(!name)
		name = bsgets(id, Name);
	szprint(temp, "%1:", name);
	draw::text(x - draw::textw(temp), y, temp);
	int mt = rec;
	if(mt >= FirstCombatant)
		mt = bsget(mt, Type);
	if(id == HitPoints || id == HitPointsMax || id == Shoots) {
		int dm1 = game::get(rec, id);
		szprint(temp, "%1i", dm1);
	} else if(id == DamageMin) {
		int dm1 = game::get(rec, DamageMin);
		int dm2 = game::get(rec, DamageMax);
		if(dm1 != dm2)
			szprint(temp, "%1i - %2i", dm1, dm2);
		else
			szprint(temp, "%1i", dm1);
	} else {
		int dm1 = bsget(mt, id);
		int dm2 = game::getsummary(rec, id, side);
		if(dm1 != dm2)
			szprint(temp, "%1i (%2i)", dm1, dm2);
		else
			szprint(temp, "%1i", dm1);
	}
	draw::text(x + ox, y, temp);
	return oy;
}

static int fieldt(int x, int y, int id, int base, int min, int max, int rec, int side) {
	char temp[260];
	int value = game::getsummary(rec, id, side) + base;
	if(value < min)
		value = min;
	else if(value > max)
		value = max;
	szprint(temp, "%1:", bsgets(id, Name));
	draw::text(x - draw::textw(temp), y, temp);
	draw::text(x + ox, y, bsgets(value, Name));
	return oy;
}

static int status(int x, int y, int rec, int side) {
	int	 y1 = y;
	// attack
	y += field(x, y, Attack, rec, side, 0);
	// defence
	y += field(x, y, Defence, rec, side, 0);
	// shoots
	if(bsget(rec, Shoots))
		y += field(x, y, Shoots, rec, side, szt("Shoots Left", "Выстрелов"));
	// damage
	y += field(x, y, DamageMin, rec, side, szt("Damage", "Урон"));
	// hits
	y += field(x, y, HitPointsMax, rec, side, szt("Hit Points", "Жизнь"));
	if(rec >= FirstCombatant && rec <= LastCombatant)
		y += field(x, y, HitPoints, rec, side, szt("Hits Left", "Жизнь ост."));
	// speed
	y += fieldt(x, y, Speed, SpeedCrawling, SpeedCrawling, SpeedUltraFast, rec, side);
	y += fieldt(x, y, Morale, MoraleNormal, MoraleTreason, MoraleBlood, rec, side);
	y += fieldt(x, y, Luck, LuckNormal, LuckCursed, LuckIrish, rec, side);
	return y - y1;
}

static void effects(int x, int y, int rec) {
	char temp[32];
	static const int modes[] = {
		SpellBloodLust, SpellBless, SpellHaste, SpellShield, SpellStoneSkin,
		SpellDragonSlayer, SpellSteelSkin, SpellAntimagic, SpellCurse, SpellSlow,
		SpellBerserker, SpellHypnotize, SpellBlind, SpellParalyze, SpellStone
	};
	draw::state push;
	draw::font = res::SMALFONT;
	int dx = 0;
	for(auto sid : modes) {
		if(!bsget(rec, sid))
			continue;
		int f = bsget(sid, Frame);
		dx += res::width(res::SPELLINF, f);
	}
	x = x - dx / 2;
	for(auto sid : modes) {
		int value = bsget(rec, sid);
		if(!value)
			continue;
		sznum(temp, value);
		int f = bsget(sid, Frame);
		draw::image(x, y, res::SPELLINF, f, AFNoOffset);
		draw::text(x + (res::width(res::SPELLINF, f) - draw::textw(temp)) / 2,
			y + res::height(res::SPELLINF, f) + 2,
			temp);
		x += res::width(res::SPELLINF, f);
	}
}

void show::unit(int rec, int side, int count, int index) {
	if(!rec)
		return;
	char temp[260];
	draw::screenshoot surface;
	res::tokens back = draw::isevil(res::VIEWARME, res::VIEWARMY);
	int w1 = res::width(back, 0);
	int h1 = res::height(back, 0);
	int x = (draw::width - w1) / 2 - 16;
	int y = (draw::height - h1) / 2;
	int side_info = side;
	if(side >= FirstCastle && side <= LastCastle) {
		if(bsget(side, Captain))
			side_info = bsget(side_info, Type) - Barbarian + BarbarianCaptain;
		int hero = bsfind(FirstHero, Index, bsget(side, Index));
		if(hero)
			side_info = hero;
	}
	int mt = rec;
	if(mt >= FirstCombatant && mt <= LastCombatant)
		mt = bsget(mt, Type);
	bool allow_dismiss = !(index && side >= FirstHero && side <= LastHero && game::getunitscount(side) <= 1);
	animation mon(tokens(mt), ActorWarn);
	while(true) {
		surface.restore();
		draw::image(x, y, back, 7);
		draw::image(x, y, back, 0);
		int x1 = x + 24;
		int y1 = y;
		status(x1 + 386, y1 + 40, rec, side_info);
		effects(x1 + 140, y1 + 188, rec);
		// name
		const char* p = bsgets(rec, Name);
		draw::text(x1 + 140 - draw::textw(p) / 2, y1 + 40, p);
		// avatar
		res::tokens icn = res::tokens(res::MONH0000 + mt - FirstMonster);
		mon.painting({(short)(x1 + 146 - (game::iswide(mt) ? cell_wd / 2 : 0)), (short)(y1 + 170)});
		if(count) {
			char temp[32];
			sznum(temp, count);
			draw::text(x1 + 140 - draw::textw(temp) / 2, y1 + 227, temp);
		}
		if(index) {
			if(game::canupgrade(mt, side))
				draw::button(x + 435, y + 192, back, Upgrade, 5, 5, 6);
			if(allow_dismiss)
				draw::button(x + 310, y + 221, back, Dismiss, 1, 1, 2);
			draw::button(x + 435, y + 221, back, Cancel, 3, 3, 4);
			draw::cursor(res::ADVMCO, 0);
		}
		int id = draw::input();
		switch(id) {
		case 0:
		case Cancel:
		case KeyEscape:
			hot::clear();
			return;
		case MouseLeft:
		case MouseRight:
			if(!index) {
				hot::clear();
				return;
			}
			break;
		case InputTimer:
			if(index)
				mon.update();
			break;
		case Upgrade:
			if(game::upgrade(side, index, true))
				return;
			break;
		case Dismiss:
			szprint(temp, szt("You really want to dismiss %1i %2?", "Вы действительно хотите распустить %1i %2?"), count, bsgets(mt, NameMulti));
			if(dlgask(0, temp)) {
				bsset(side, index, 0);
				bsset(side, index + 1, 0);
				return;
			}
			break;
		}
	}
}

bool show::recruit(int rec, int& count, int maximum, void* available_resources) {
	char temp[260];
	draw::screenshoot surface;
	res::tokens back = res::RECRBKG;
	int w1 = res::width(back, 0);
	int h1 = res::height(back, 0);
	int x = (draw::width - w1) / 2 - 8;
	int y = (draw::height - h1) / 2 - 16;
	int mt = rec;
	int total[LastResource - FirstResource + 1];
	auto monster_cost = bsptr(mt, FirstResource);
	auto maximum_available = game::divresource(available_resources, monster_cost);
	if(maximum_available > maximum)
		maximum_available = maximum;
	while(true) {
		surface.restore();
		draw::image(x, y, back, 1);
		draw::image(x, y, back, 0);
		int x1 = x + 16;
		int y1 = y;
		// name
		draw::text(x1 + 23, y1 + 30, 273, draw::Center, bsgets(rec, Name));
		if(true) {
			draw::state push;
			draw::font = res::SMALFONT;
			// available
			draw::text(x1 + 32, y1 + 148, 90, draw::Center, szprint(temp, "%1: %2i", szt("Available", "Достпуно"), maximum));
			// recruit
			draw::text(x1 + 32, y1 + 164, 90, draw::Left, szt("Number to buy:", "Нанять кол-во:"));
		}
		// count
		draw::edit(x1 + 168, y1 + 163, count, maximum_available);
		res::tokens icn = res::tokens(res::MONH0000 + mt - FirstMonster);
		draw::image(x1 + 80 - res::width(icn, 0) / 2,
			y1 + 100 - res::height(icn, 0) / 2,
			icn, 0, AFNoOffset);
		if(draw::mousein(x1 + 80 - 40, y1 + 100 - 50, x1 + 80 + 40, y1 + 100 + 50)) {
			if(hot::key == MouseRight && hot::pressed)
				draw::execute(Information);
		}
		// Price
		draw::textf(x1 + 142, y1 + 72, 130, game::addicon(temp, monster_cost));
		// total
		game::mulresource(total, monster_cost, count);
		draw::textf(x1 + 33, y1 + 190, 250, game::addicon(temp, total));
		//
		draw::button(x1 + 34, y1 + 249, res::RECRUIT, Accept, 8, 8, 9, KeyEnter);
		draw::button(x1 + 187, y1 + 249, res::RECRUIT, Cancel, 6, 6, 7, KeyEscape);
		draw::button(x1 + 230, y1 + 155, res::RECRUIT, Maximus, 4, 4, 5, Alpha + 'M');
		draw::button(x1 + 205, y1 + 154, res::RECRUIT, KeyUp, 0, 0, 1);
		draw::button(x1 + 205, y1 + 169, res::RECRUIT, KeyDown, 2, 2, 3);
		draw::cursor(res::ADVMCO, 0);
		int id = draw::input();
		switch(id) {
		case 0:
		case Cancel:
			return false;
		case Accept:
			return true;
		case Maximus:
			count = maximum_available;
			break;
		case KeyUp:
			count++;
			break;
		case KeyDown:
			count--;
			break;
		case Information:
			show::unit(rec, 0, 0, 0);
			break;
		}
	}
}