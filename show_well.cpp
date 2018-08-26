#include "main.h"

static int field(int x, int y, int rec, int id, const char* short_name = 0) {
	char temp[32];
	int v = bsget(rec, id);
	if(!v)
		return 0;
	szprint(temp, "%1:", short_name ? short_name : bsgets(id, Name));
	draw::textm(x, y, 50, draw::Right, temp);
	if(id == DamageMin) {
		int v1 = bsget(rec, DamageMax);
		szprint(temp, (v1 != v) ? "%1i-%2i" : "%1i", v, v1);
	} else
		sznum(temp, v);
	draw::text(x + 52, y, temp);
	return draw::texth();
}

static int field(int x, int y, int rec, int id, int base_min, int base_max, const char* short_name = 0) {
	int y1 = y;
	char temp[32];
	int v = bsget(rec, id);
	if(!v)
		return 0;
	szprint(temp, "%1:", short_name ? short_name : bsgets(id, Name));
	y += draw::textm(x, y, 80, draw::Center, temp);
	if(v < 0)
		v = 0;
	v += base_min;
	if(v > base_max)
		v = base_max;
	y += draw::textm(x, y, 80, draw::Center, bsgets(v, Name));
	return y - y1;
}

static int field_grow(int x, int y, int rec, int level, const char* short_name) {
	int y1 = y;
	char temp[32];
	int v = game::getgrowth(rec, Dwelving1 + level - 1);
	if(!v)
		return 0;
	szprint(temp, "%1:", short_name);
	y += draw::textm(x, y, 80, draw::Center, temp);
	szprint(temp, szt("+%1i per week", "+%1i в неделю"), v);
	y += draw::textm(x, y, 80, draw::Center, temp);
	return y - y1;
}

static void paint_monster(int x, int y, int height, int width, int rec, int level) {
	char temp[32];
	draw::state push;
	draw::font = res::SMALFONT;
	auto type = bsget(rec, Type);
	auto dwl = Dwelving1 + level - 1;
	auto dwn = bsget(rec, dwl);
	auto monster = (tokens)game::getunit(type, dwl, dwn);
	auto icn = (res::tokens)(res::MONH0000 + monster - FirstMonster);
	auto name = bsgets(monster, Name);
	auto bld = res::buildings(type);
	auto frame = indexes::buildings(dwl, dwn);
	draw::textm(x, y + 2, 200, draw::Center, name);
	draw::image(x + 1, y + 18, bld, frame);
	name = game::getbuildingname(type, dwl, dwn);
	draw::textm(x, y + 85, 138, draw::Center, name);
	if(dwn) {
		draw::textm(x, y + 102, 70, draw::Right, szt("Available:", "Доступно:"));
		draw::state push;
		draw::font = res::FONT;
		sznum(temp, bsget(rec, FirstCreatureCount + level - 1));
		draw::text(x + 80, y + 100, temp);
	}
	if((level & 1) == 0)
		draw::image(x + 130 + 85, y + 20, icn, 0, AFMirror);
	else
		draw::image(x + 130 + 2, y + 20, icn, 0);
	y += 4;
	y += field(x + 210, y, monster, Attack);
	y += field(x + 210, y, monster, Defence);
	y += field(x + 210, y, monster, DamageMin, szt("Damage", "Урон"));
	y += field(x + 210, y, monster, HitPointsMax, szt("Hits", "Жизнь"));
	//y += field(x + 210, y, monster, Rating, szt("Rate", "Рейт"));
	y += draw::texth();
	y += field(x + 210, y, monster, Speed, SpeedCrawling, SpeedUltraFast);
	y += draw::texth();
	y += field_grow(x + 210, y, rec, level, szt("Grows", "Прирост"));
}

void show::well(int rec) {
	auto type = bsget(rec, Type);
	auto player = bsget(rec, Player);
	while(true) {
		draw::image(0, 0, res::WELLBKG, 0);
		draw::status(21, draw::height - 16, 21 + res::width(res::SMALLBAR, 0), draw::height - 1);
		paint_monster(20, 18, 288, 124, rec, 1);
		paint_monster(20, 168, 288, 124, rec, 3);
		paint_monster(20, 318, 288, 124, rec, 5);
		paint_monster(334, 18, 288, 124, rec, 2);
		paint_monster(334, 168, 288, 124, rec, 4);
		paint_monster(334, 318, 288, 124, rec, 6);
		draw::button(578, 461, res::WELLXTRA, Cancel, 0, 0, 1, KeyEscape);
		draw::cursor(res::ADVMCO, 0);
		int id = draw::input();
		switch(id) {
		case Cancel:
			return;
		default:
			draw::definput(id);
			break;
		}
	}
}