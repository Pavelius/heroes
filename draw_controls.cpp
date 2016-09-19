#include "main.h"

const int	padding = 16;

static int index_by_type(int id)
{
	switch(id)
	{
	case Necromancer:
		return 9;
	case Wizard:
		return 8;
	case Warlock:
		return 7;
	case Sorcerer:
		return 6;
	case Barbarian:
		return 5;
	case Knight:
		return 4;
	default:
		return 10;
	}
}

void draw::splitter(int x, int y, int id, res::tokens icn, int from, int to, int& cur)
{
	static int drag_id;
	int body = 1, left = 3, right = 5, bar = 2;
	int right_offset = 0, left_offset = 6, bar_offset = 4, pressed = 1;
	if(cur < from)
		cur = from;
	if(cur > to)
		cur = to;
	if(from == to)
		return;
	int bar_width = res::width(icn, bar);
	int body_width = res::width(icn, body);
	int height = res::height(icn, body);
	int mx1 = body_width - left_offset - res::width(icn, left) - res::width(icn, right) - bar_width - bar_offset * 2;
	if(mx1 < 0)
		return;
	int mx = to - from;
	if(mx <= 0)
		return;
	int p0 = res::width(icn, left) + left_offset + bar_offset;
	int px = x + p0 + cur * mx1 / mx;
	int h1 = res::height(icn, body);
	image(x, y, icn, body);
	image(px, y + bar_offset, icn, bar);
	if(drag_id)
	{
		if(hot::key == MouseLeft)
		{
			if(!hot::pressed)
				drag_id = 0;
		}
		else if(drag_id == id && hot::key == MouseMove)
		{
			if(hot::mouse.x < px)
				draw::execute(id, KeyLeft);
			else if(hot::mouse.x > px + (mx1 + mx - 1) / mx)
				draw::execute(id, KeyRight);
		}
	}
	else
	{
		if(hot::key == MouseLeft && hot::pressed)
		{
			if(mousein(x + p0, y, px, y + height))
				execute(id, KeyLeft);
			else if(mousein(px + bar_width, y, x + p0 + mx1 + bar_width, y + height))
				execute(id, KeyRight);
			else if(mousein(px, y, px + bar_width, y + height))
				drag_id = id;
		}
		else if(hot::key == MouseWheelUp || hot::key == MouseWheelDown)
		{
			if(mousein(x, y, x + body_width, y + height))
			{
				if(hot::key==MouseWheelUp)
					execute(id, KeyLeft);
				else
					execute(id, KeyRight);
			}
		}
	}
	button(x + left_offset, y + 1, icn, id, left, left, left + pressed, 0, 0, 0, KeyLeft);
	button(x + res::width(icn, body) - res::width(icn, right) - 1, y + 1, icn, id, right, right, right + pressed, 0, 0, 0, KeyRight);
}

struct picture
{

	int			id, count, frame;
	res::tokens	icn;

	void initialize()
	{
		icn = res::Empthy;
		frame = 0;
		if(id >= FirstHero && id <= LastHero)
		{
			if(count == 1)
			{
				icn = res::MINIPORT;
				frame = id - FirstHero;
			}
			else
			{
				icn = (res::tokens)(res::PORT0000 + id - FirstHero);
				frame = 0;
			}
		}
		else if(id >= FirstCaptain && id <=LastCaptain)
		{
			static int indicies[] = {1, 0, 5, 2, 3, 4};
			icn = res::tokens(res::PORT0090 + indicies[id - FirstCaptain]);
			frame = 0;
		}
		else if(id >= FirstResource && id <= LastResource)
		{
			static unsigned char frames[] = {6, 0, 1, 2, 3, 4, 5};
			icn = res::RESOURCE;
			frame = frames[id - FirstResource];
		}
		else if(id >= FirstBuilding && id <= LastBuilding)
		{
			icn = res::buildings(count);
			frame = indexes::buildings(id, 0);
		}
		else if(id >= FirstMonster && id <= LastMonster)
		{
			icn = res::STRIP;
			frame = index_by_type(game::get(id, Type));
		}
		else if(id >= FirstSkill && id <= LastSkill)
		{
			icn = res::SECSKILL;
			frame = id - FirstSkill + 1;
		}
		else if(id >= FirstArtifact && id <= LastArtifact)
		{
			icn = res::ARTIFACT;
			frame = id - FirstArtifact + 1;
		}
		else if(id >= FirstSpell && id <= LastSpell)
		{
			icn = res::SPELLS;
			frame = bsget(id, Portrait);
		}
		else if(id == Experience)
		{
			icn = res::EXPMRL;
			frame = 4;
		}
		else if(id == Morale)
		{
			icn = res::EXPMRL;
			if(count<0)
				frame = 3;
			else
				frame = 2;
		}
		else if(id == Yes)
		{
			icn = draw::isevil(res::SYSTEME, res::SYSTEM);
			frame = 5;
		}
		else if(id == No)
		{
			icn = draw::isevil(res::SYSTEME, res::SYSTEM);
			frame = 7;
		}
		else if(id == Accept)
		{
			icn = draw::isevil(res::SYSTEME, res::SYSTEM);
			frame = 1;
		}
		else if(id == Cancel)
		{
			icn = draw::isevil(res::SYSTEME, res::SYSTEM);
			frame = 3;
		}
		else if(id == Learn)
		{
			icn = draw::isevil(res::SYSTEME, res::SYSTEM);
			frame = 9;
		}
	}

	int getheight() const
	{
		if(id >= FirstResource && id <= LastResource)
			return 45;
		else if(id >= FirstSpell && id <= LastSpell)
			return 64;
		else if(id == Experience)
			return res::height(icn, frame) + res::height(res::SMALFONT, 'I' - 0x20) + 2 + 2;
		return res::height(icn, frame);
	}

	int getwidth() const
	{
		if(id >= FirstSpell && id <= LastSpell)
			return 64;
		return res::width(icn, frame);
	}

	void paint(int x, int y, bool perday) const
	{
		char temp[32];
		auto w = getwidth();
		auto h = getheight();
		auto paint_count = 0;
		bool paint_name = false;
		if(id >= FirstResource && id <= LastResource)
		{
			draw::image(x - w / 2, y + h - draw::texth() - res::height(icn, frame), icn, frame, AFNoOffset);
			paint_count = 1;
		}
		else if(id >= FirstSkill && id <= LastSkill)
		{
			draw::image(x - w / 2, y, icn, frame, AFNoOffset);
			const char* p = bsgets(id, Name);
			draw::text(x - w / 2 + (w - draw::textw(p)) / 2, y + 3, p);
			p = bsgets(count, Name);
			draw::text(x - w / 2 + (w - draw::textw(p)) / 2, y + 3 + 52, p);
		}
		else if(id >= FirstCaptain && id <= LastCaptain)
		{
			draw::image(x - w / 2, y, icn, frame, AFNoOffset);
			if(count)
				draw::image(x - w / 2 + 6, y - 2, res::CFLGSMAL, count - FirstPlayer);
		}
		else if(id >= FirstMonster && id <= LastMonster)
		{
			draw::image(x - w / 2 + 1, y, icn, frame, AFNoOffset);
			draw::image(x - w / 2, y, res::tokens(res::MONH0000 + id - FirstMonster), 0);
			paint_count = 2;
		}
		else if(id >= FirstSpell && id <= LastSpell)
		{
			draw::image(x - res::width(icn,frame) / 2, y, icn, frame, AFNoOffset);
			paint_name = true;
		}
		else if(id == Experience)
		{
			draw::image(x - w / 2, y, icn, frame, AFNoOffset);
			paint_count = 1;
		}
		else
			draw::image(x - w / 2, y, icn, frame, AFNoOffset);
		if(paint_count)
		{
			if(paint_count == 2 && count == 0)
				return;
			if(perday)
				szprint(temp, "%1i / %2", count, szt("day", "день"));
			else
				sznum(temp, count);
			draw::state push;
			draw::font = res::SMALFONT;
			int w1 = draw::textw(temp);
			int x1 = x + w / 2 - 4 - w1;
			if(paint_count == 1)
				x1 = x - w1 / 2;
			draw::text(x1, y + h - draw::texth(), temp);
		}
		if(paint_name)
		{
			const char* p = bsgets(id, Name);
			if(p)
			{
				draw::state push;
				draw::font = res::SMALFONT;
				auto th = draw::texth(p, w);
				auto ih = res::height(icn, frame);
				draw::textm(x-w/2+2, y + ih + (h - ih - th)/2, w, draw::Center, p);
			}
		}
	}

};

int draw::clipart(int x, int y, int id, int param, int param2, bool border, bool clickable, bool informable)
{
	picture icon;
	icon.id = id;
	icon.count = param;
	icon.initialize();
	icon.paint(x, y, false);
	rect rc;
	rc.x2 = icon.getwidth();
	rc.y2 = icon.getheight();
	rc.x1 = x - rc.x2 / 2; rc.x2 += rc.x1;
	rc.y1 = y; rc.y2 += rc.y1;
	if(draw::mousein(rc))
	{
		if(clickable && hot::key == MouseLeft && hot::pressed)
			draw::execute(id);
		if(informable && hot::key == MouseRight && hot::pressed)
			draw::execute(Information, id);
	}
	return icon.getheight();
}

static int paint_icons(int x, int y, int width, picture* icons, int count, bool proportional, bool perday)
{
	if(!count)
		return 0;
	const int pad = 4;
	int y1 = y;
	while(count > 0)
	{
		int c = (count < 4) ? count : 4;
		int h = 0;
		int w = 0;
		for(int i = 0; i < c; i++)
		{
			auto h1 = icons[i].getheight();
			if(h1 > h)
				h = h1;
			w += icons[i].getwidth() + pad;
		}
		w -= pad;
		if(proportional)
			w = (width / c)*c;
		int x1 = x + (width - w) / 2;
		for(int i = 0; i < c; i++)
		{
			auto h1 = icons[i].getheight();
			auto w1 = proportional ? (w/c) : icons[i].getwidth();
			icons[i].paint(x1 + w1 / 2, y + h - h1, perday);
			x1 += w1;
			if(!proportional)
				x1 += pad;
		}
		y += h;
		icons += c;
		count -= c;
	}
	return y - y1;
}

int draw::textf(int x, int y, int width, const char* p)
{
	picture params[16];
	int y4 = y;
	p = zskipspcr(p);
	auto start = p;
	while(*p)
	{
		if(p[0] == '$' && p[1] == '(')
		{
			p += 2;
			int count = 0;
			bool proportional = false;
			bool dayly = false;
			while(*p)
			{
				if(*p == 'p')
				{
					p++;
					proportional = true;
					continue;
				}
				else if(*p == 'd')
				{
					p++;
					dayly = true;
					continue;
				}
				params[count].id = sz2num(p, &p);
				params[count].count = 0;
				if(*p == '/')
					params[count].count = sz2num(zskipsp(p + 1), &p);
				params[count].initialize();
				count++;
				if(*p == ')')
				{
					p = zskipspcr(p + 1);
					break;
				}
				if(*p == ',')
					p++;
			}
			if(count)
				y += paint_icons(x, y, width, params, count, proportional, dayly) + draw::texth();
		}
		else
		{
			int c = draw::textbc(p, width);
			draw::text(x, y, width, draw::Center, p, c);
			y += draw::texth();
			p += c;
			if(p > start && p[-1] == '\n')
				y += draw::texth()/2;
			p = zskipspcr(p);
		}
	}
	return y - y4;
}

int draw::textf(int width, const char* string)
{
	draw::state push;
	draw::clipping.clear();
	return textf(0, 0, width, string);
}

int draw::dialog(int height)
{
	const int p1 = 30;
	const int p2 = 30;
	res::tokens icn = draw::isevil(res::BUYBUILE, res::BUYBUILD);
	int h4 = res::height(icn, 4); // top
	int h5 = res::height(icn, 5); // middle
	int h6 = res::height(icn, 6); // bottom
	int w4 = res::width(icn, 4); // top
	int w0 = res::width(icn, 0); // middle
	int count = (height - p1 - p2 + h5 - 1) / h5;
	int x = (draw::width - (w4 + w0)) / 2 - 8;
	int y1 = (draw::height - height) / 2;
	int y = (draw::height - (h4 + h5*count + h6)) / 2 - 8;
	if(icn == res::BUYBUILD)
	{
		draw::image(x + 1, y, icn, 4, AFNoOffset);
		draw::image(x + w4 + 1, y, icn, 0, AFNoOffset);
	}
	else
	{
		draw::image(x, y, icn, 4, AFNoOffset);
		draw::image(x + w4, y, icn, 0, AFNoOffset);
	}
	//draw::line(x+4,y,x+wa-4,y,0x10);
	y += h4;
	for(int i = 0; i < count; i++)
	{
		draw::image(x, y, icn, 5, AFNoOffset);
		draw::image(x + res::width(icn, 5), y, icn, 1, AFNoOffset);
		//draw::line(x+4,y,x+wa-4,y,0x10);
		y += h5;
	}
	draw::image(x, y, icn, 6, AFNoOffset);
	draw::image(x + res::width(icn, 6), y, icn, 2, AFNoOffset);
	//draw::line(x+4,y,x+wa-4,y,0x13);
	//draw::rectb(x,y1,x+wa,y1+height,0x40);
	return y1;
}

void draw::resource(int x, int y, const void* cost_ptr)
{
	int* cost = (int*)cost_ptr;
	if(!cost)
		return;
	struct element
	{
		int     id;
		point   pos;
		int     dy;
	};
	static element elements[] =
	{
		{Wood, {1, 0}}, {Sulfur, {42, 0}},
		{Crystal, {1, 42}}, {Mercury, {42, 42}},
		{Ore, {1, 84}}, {Gems, {42, 84}},
		{Gold, {22, 123}}
	};
	rectf(x, y, x + 81, y + 191, 0x20);
	x += 20;
	for(auto& a : elements)
		clipart(x + a.pos.x, y + a.pos.y - 2, a.id, cost[a.id - FirstResource], 0);
}

bool dlgask(const char* title, const char* text)
{
	draw::screenshoot surface;
	draw::state push;
	draw::font = res::FONT;
	res::tokens ic1 = draw::isevil(res::SYSTEME, res::SYSTEM);
	int tw = 300 - 54 - 16;
	int th = draw::textf(tw, text) + padding/2 + res::height(ic1, 5);
	int x = (draw::width - tw) / 2;
	while(true)
	{
		surface.restore();
		int y1 = draw::dialog(th);
		//draw::rectb(x, y1, x+tw, y1+th, 0x40);
		y1 += draw::textf(x, y1, tw, text) + padding/2; // message text
		draw::button(x + 10, y1, ic1, Accept, 5, 5, 6, KeyEnter);
		draw::button(x + tw - 10 - res::width(ic1, 7), y1, ic1, Cancel, 7, 7, 8, KeyEscape);
		draw::cursor(res::ADVMCO, 0);
		int id = draw::input();
		switch(id)
		{
		case Cancel:
			return false;
		case Accept:
			return true;
		default:
			draw::definput(id);
			break;
		}
	}
}

bool show::ask(const char* format, const char* arguments)
{
	char temp[4096];
	szprintv(temp, format, arguments);
	return dlgask(0, temp);
}

void dlgmsg(const char* title, const char* text)
{
	draw::screenshoot surface;
	draw::state push;
	draw::font = res::FONT;
	auto ic1 = draw::isevil(res::SYSTEME, res::SYSTEM);
	auto tw = 304 - 54 - 16;
	auto th = draw::textf(tw, text);
	while(true)
	{
		surface.restore();
		int y1 = draw::dialog(th + res::height(ic1, 1) + padding/2);
		y1 = y1 + draw::textf((draw::width - tw) / 2, y1, tw, text) + padding/2; // message text
		draw::button((draw::width - res::width(ic1, 1)) / 2, y1,
			ic1, Accept, 1, 1, 2, KeyEnter);
		draw::cursor(res::ADVMCO, 0);
		switch(draw::input())
		{
		case Cancel:
		case Accept:
			return;
		}
	}
}

void show::message(const char* format, const char* arguments)
{
	char temp[4096];
	szprintv(temp, format, arguments);
	return dlgmsg(0, temp);
}

void show::tips(const char* text)
{
	draw::screenshoot surface;
	draw::state push;
	draw::font = res::FONT;
	int tw = 304 - 54 - 16;
	int th = draw::textf(tw, text);
	int x = (draw::width - tw) / 2;
	while(true)
	{
		surface.restore();
		int y1 = draw::dialog(th);
		y1 += draw::textf(x, y1, tw, text) + padding; // message text
		switch(draw::input())
		{
		case 0:
			return;
		case MouseLeft:
		case MouseRight:
			if(!hot::pressed)
				return;
			return;
		}
	}
}

void show::tipsf(const char* format, ...)
{
	char temp[4096];
	szprintv(temp, format, xva_start(format));
	tips(temp);
}

void draw::tiles(int x, int y, res::tokens icn, int* rec, int w, int h)
{
	const int dy = 32;
	const int dx = 32;
	int x2 = x + w*dx;
	int y2 = y + h*dy;
	int ri = 0;
	for(int y1 = y; y1 < y2; y1 += dy)
	{
		for(int x1 = x; x1 < x2; x1 += dx)
		{
			auto id = rec[ri];
			if(id != -1)
				draw::image(x1 - 12, y1, icn, id);
			ri++;
		}
	}
}

void draw::debug(int ox, int oy)
{
	static point pos;
	char temp[64];
	draw::state push;
	draw::font = res::SMALFONT;
	if(hot::key == MouseLeft && hot::pressed)
		pos = hot::mouse;
	if(hot::pressed)
	{
		szprint(temp, "Rect pos(%1i, %2i), size (%3i, %4i)", pos.x - ox, pos.y - oy, hot::mouse.x - pos.x, hot::mouse.y - pos.y);
		draw::rectb(pos.x, pos.y, hot::mouse.x, hot::mouse.y, 10);
	}
	else
		szprint(temp, "Mouse %1i, %2i", hot::mouse.x - ox, hot::mouse.y - oy);
	text(10, 10, temp);
}

static int hot_troops_index;
static int hot_troops_owner;

void draw::definput(int id)
{
	static int source_rec;
	if(id == Information)
	{
		if(hot::param >= FirstBuilding && hot::param <= LastBuilding)
			show::tips(game::getbuildinginfo(hot::param2, hot::param, hot::level));
		else if(hot::param >= FirstMonster && hot::param <= LastMonster)
			show::unit(hot::param, hot::param2, 0, 0);
		else if(hot::param >= FirstSpell && hot::param <= LastSpell)
		{
			char temp[4096];
			auto p = bsgets(hot::param, Text);
			if(!p)
				return;
			szprint(temp, "$(%1i)\n", hot::param);
			szprint(zend(temp), p, bsget(hot::param, Value));
			show::tips(temp);
		}
	}
	else if(id >= FirstTroopsIndex && id <= LastTroopsIndex)
	{
		if(hot::param2)
		{
			int count = bsget(hot::param, id + 1);
			show::unit(bsget(hot::param, id), hot::param, count, id);
			hot_troops_index = 0;
		}
		else if(!hot_troops_index)
		{
			if(bsget(hot::param, id))
			{
				hot_troops_index = id;
				hot_troops_owner = hot::param;
			}
		}
		else if(hot_troops_index == id && hot_troops_owner == hot::param)
			hot_troops_index = 0;
		else
		{
			int dest = id;
			int dest_rec = hot::param;
			int source = hot_troops_index;
			int source_rec = hot_troops_owner;
			int v11 = bsget(dest_rec, dest);
			int v12 = bsget(dest_rec, dest + 1);
			int v21 = bsget(source_rec, source);
			int v22 = bsget(source_rec, source + 1);
			if(!(source_rec >= FirstHero && source_rec <= LastHero && source_rec!=dest_rec && game::getunitscount(source_rec) <= 1))
			{
				if(v11 == v21)
				{
					// Add total if same type
					bsset(source_rec, source, 0);
					bsset(source_rec, source + 1, 0);
					bsset(dest_rec, dest, v21);
					bsset(dest_rec, dest + 1, v12 + v22);
				}
				else
				{
					bsset(source_rec, source, v11);
					bsset(source_rec, source + 1, v12);
					bsset(dest_rec, dest, v21);
					bsset(dest_rec, dest + 1, v22);
				}
			}
			hot_troops_index = 0;
		}
	}
	else if(id >= FirstHero && id <= LastHero)
		show::hero((tokens)id);
}

void draw::troops(int x, int y, int rec)
{
	int w = res::width(res::STRIP, 2);
	int h = res::width(res::STRIP, 2);
	for(int i = FirstTroopsIndex; i <= LastTroopsIndex; i += 2)
	{
		int unit = bsget(rec, i);
		int count = bsget(rec, i + 1);
		int key = hot::key;
		bool pressed = hot::pressed;
		if(unit == 0)
			image(x, y, res::STRIP, 2);
		else
			clipart(x + 40, y, unit, count);
		if(i == hot_troops_index && hot_troops_owner == rec)
			image(x, y, res::STRIP, 1);
		if(mousein(x, y, x + w, y + h))
		{
			if(key == MouseRight && pressed)
				execute(Information, bsget(rec, i), rec);
			else if(key == MouseLeft && pressed)
				execute(i, rec);
			else if(key == MouseLeftDBL)
				execute(i, rec, i);
			if(unit)
				status("%1i %2", count, bsgets(unit, NameMulti));
		}
		x += 88;
	}
}

void show::fadeback(int count)
{
	for(int i = 0; i < count; i++)
	{
		draw::shadow(0, 0, draw::width - 1, draw::height - 1, 0);
		draw::input(false);
		sleep(50);
	}
}

void draw::screenshoot::redraw(drawable** objects, unsigned timeout)
{
	restore();
	dworder(objects, zlen(objects));
	dwpaint(objects, {0, 0, draw::width, draw::height}, {0, 0});
	draw::input(false);
	sleep(timeout);
}

void draw::screenshoot::redraw(drawable** objects, unsigned timeout, drawable* e1, int stop)
{
	auto a1 = static_cast<animation*>(e1);
	if(!a1->count)
		return;
	while(true)
	{
		redraw(objects, timeout);
		if(a1->incframe())
			break;
		if(stop && a1->frame >= stop)
			break;
	}
}

void draw::screenshoot::redraw(drawable** objects, unsigned timeout, drawable* e1, drawable* e2)
{
	auto a1 = static_cast<animation*>(e1);
	auto a2 = static_cast<animation*>(e2);
	bool a1_run = true;
	bool a2_run = true;
	while(a1_run || a2_run)
	{
		redraw(objects, timeout);
		if(a1->incframe())
			a1_run = false;
		if(a2->incframe())
			a2_run = false;
	}
}