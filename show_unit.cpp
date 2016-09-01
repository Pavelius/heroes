#include "main.h"

static const int ox = 10;
static const int oy = 18;

static int field(int x, int y, int id, int rec, int side, const char* name)
{
	char temp[260];
	if(!name)
		name = bsgets(id, Name);
	szprint(temp, "%1:", name);
	draw::text(x - draw::textw(temp), y, temp);
	int mt = rec;
	if(mt >= FirstCombatant)
		mt = bsget(mt, Type);
	if(id == HitPoints || id==HitPointsMax)
	{
		int dm1 = game::get(rec, id);
		szprint(temp, "%1i", dm1);
	}
	else if(id == DamageMin)
	{
		int dm1 = game::get(rec, DamageMin);
		int dm2 = game::get(rec, DamageMax);
		if(dm1 != dm2)
			szprint(temp, "%1i - %2i", dm1, dm2);
		else
			szprint(temp, "%1i", dm1);
	}
	else
	{
		int dm1 = bsget(mt, id);
		int dm2 = game::getsummary(rec, id, side);
		if(dm1 != dm2)
			szprint(temp, "%1i (%2i)", dm2, dm1);
		else
			szprint(temp, "%1i", dm1);
	}
	draw::text(x + ox, y, temp);
	return oy;
}

static int fieldt(int x, int y, int id, int base, int rec, int side)
{
	char temp[260];
	int value = game::getsummary(rec, id, side) + base;
	szprint(temp, "%1:", bsgets(id, Name));
	draw::text(x - draw::textw(temp), y, temp);
	draw::text(x + ox, y, bsgets(value, Name));
	return oy;
}

static int status(int x, int y, int rec, int side)
{
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
	y += fieldt(x, y, Speed, 0, rec, side);
	y += fieldt(x, y, Morale, MoraleNormal, rec, side);
	y += fieldt(x, y, Luck, LuckNormal, rec, side);
	return y - y1;
}

static void effects(int x, int y, int rec)
{
	char temp[32];
	static const int modes[] =
	{
		SpellBloodLust, SpellBless, SpellHaste, SpellShield, SpellStoneSkin,
		SpellDragonSlayer, SpellSteelSkin, SpellAntimagic, SpellCurse, SpellSlow,
		SpellBerserker, SpellHypnotize, SpellBlind, SpellParalyze, SpellStone
	};
	draw::fontsm push;
	int dx = 0;
	for(auto sid : modes)
	{
		if(!bsget(rec, sid))
			continue;
		int f = bsget(sid, FrameStatus);
		dx += res::width(res::SPELLINF, f);
	}
	x = x - dx / 2;
	for(auto sid : modes)
	{
		int value = bsget(rec, sid);
		if(!value)
			continue;
		sznum(temp, value);
		int f = bsget(sid, FrameStatus);
		draw::image(x, y, res::SPELLINF, f, AFNoOffset);
		draw::text(x + (res::width(res::SPELLINF, f) - draw::textw(temp)) / 2,
			y + res::height(res::SPELLINF, f) + 2,
			temp);
		x += res::width(res::SPELLINF, f);
	}
}

void show::unit(int rec, int side)
{
	if(!rec)
		return;
	draw::screenshoot surface;
	res::tokens back = draw::isevil(res::VIEWARME, res::VIEWARMY);
	int w1 = res::width(back, 0);
	int h1 = res::height(back, 0);
	int x = (draw::width - w1) / 2 - 16;
	int y = (draw::height - h1) / 2;
	int mt = rec;
	if(mt >= FirstCombatant && mt <= LastCombatant)
		mt = bsget(mt, Type);
	animation mon(tokens(mt), Move);
	while(true)
	{
		surface.restore();
		draw::image(x, y, back, 7);
		draw::image(x, y, back, 0);
		int x1 = x + 24;
		int y1 = y;
		status(x1 + 386, y1 + 40, rec, side);
		effects(x1 + 140, y1 + 188, rec);
		// name
		const char* p = bsgets(rec, Name);
		draw::text(x1 + 140 - draw::textw(p) / 2, y1 + 40, p);
		// avatar
		res::tokens icn = res::tokens(res::MONH0000 + mt - FirstMonster);
		draw::image(x1 + 150 - res::width(icn, 0) / 2,
			y1 + 140 - res::height(icn, 0) / 2,
			icn, 0, AFNoOffset);
		// count
		if(bsget(rec, Count))
		{
			char temp[32];
			sznum(temp, bsget(rec, Count));
			draw::text(x1 + 140 - draw::textw(temp) / 2, y1 + 227, temp);
		}
		int id = draw::input();
		switch(id)
		{
		case 0:
		case Cancel:
		case KeyEscape:
			hot::clear();
			return;
		case MouseLeft:
		case MouseRight:
			hot::clear();
			return;
		}
	}
}

bool show::recruit(int rec, int& count, int maximum)
{
	draw::screenshoot surface;
	res::tokens back = res::RECRBKG;
	int w1 = res::width(back, 0);
	int h1 = res::height(back, 0);
	int x = (draw::width - w1) / 2 - 8;
	int y = (draw::height - h1) / 2 - 16;
	int mt = rec;
	while(true)
	{
		surface.restore();
		draw::image(x, y, back, 1);
		draw::image(x, y, back, 0);
		int x1 = x + 16;
		int y1 = y;
		// name
		const char* name = bsgets(rec, Name);
		draw::text(x1 + 180 - draw::textw(name), y1 + 30, name);
		// count
		draw::edit(x1 + 168, y1 + 163, count, maximum);
		res::tokens icn = res::tokens(res::MONH0000 + mt - FirstMonster);
		draw::image(x1 + 80 - res::width(icn, 0) / 2,
			y1 + 100 - res::height(icn, 0) / 2,
			icn, 0, AFNoOffset);
		if(draw::area(x1 + 80 - 40, y1 + 100 - 50, x1 + 80 + 40, y1 + 100 + 50))
		{
			if(hot::key == MouseRight && hot::pressed)
				draw::execute(Information);
		}
		draw::button(x1 + 34, y1 + 249, res::RECRUIT, Accept, 8, 8, 9, KeyEnter);
		draw::button(x1 + 187, y1 + 249, res::RECRUIT, Cancel, 6, 6, 7, KeyEscape);
		draw::button(x1 + 230, y1 + 155, res::RECRUIT, Maximus, 4, 4, 5, Alpha + 'M');
		draw::button(x1 + 205, y1 + 154, res::RECRUIT, KeyUp, 0, 0, 1);
		draw::button(x1 + 205, y1 + 169, res::RECRUIT, KeyDown, 2, 2, 3);
		draw::cursor(res::ADVMCO, 0);
		int id = draw::input();
		switch(id)
		{
		case 0:
		case Cancel:
			return false;
		case Accept:
			return true;
		case Maximus:
			count = maximum;
			break;
		case KeyUp:
			count++;
			break;
		case KeyDown:
			count--;
			break;
		case Information:
			show::unit(rec);
			break;
		}
	}
}