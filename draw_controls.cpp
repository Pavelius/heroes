#include "main.h"

const int				padding = 16;
extern unsigned char	pallette[256][4];

struct picture
{
	int id;
	int	count;

	int width() const
	{
		if(id >= (int)FirstHero && id <= (int)LastHero)
			return 101;
		else if(id >= (int)FirstResource && id <= (int)LastResource)
			return 60;
		else if(id >= (int)FirstBuilding && id <= (int)LastBuilding)
			return 137;
		else if(id >= (int)FirstMonster && id <= (int)LastMonster)
			return 80;
		else if(id >= (int)FirstSkill && id <= (int)LastSkill)
			return 80;
		return 0;
	}

	int height() const
	{
		if(id >= (int)FirstHero && id <= (int)LastHero)
			return 93;
		else if(id >= (int)FirstResource && id <= (int)LastResource)
			return 50;
		else if(id >= (int)FirstBuilding && id <= (int)LastBuilding)
			return 58;
		else if(id >= (int)FirstMonster && id <= (int)LastMonster)
			return 93;
		return 0;
	}

};

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
			if(area(x + p0, y, px, y + height))
				execute(id, KeyLeft);
			else if(area(px + bar_width, y, x + p0 + mx1 + bar_width, y + height))
				execute(id, KeyRight);
			else if(area(px, y, px + bar_width, y + height))
				drag_id = id;
		}
		else if(hot::key == MouseWheelUp || hot::key == MouseWheelDown)
		{
			if(area(x, y, x + body_width, y + height))
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

int draw::clipart(int x, int y, int id, int param, int param2, bool border)
{
	int w = 0;
	int h = 0;
	char temp[32];
	if(id >= FirstHero && id <= LastHero)
	{
		if(param == 1)
		{
			w = 43;
			h = 27;
			image(x - res::width(res::MINIPORT, 0) / 2, y + res::oy(res::MINIPORT, 0), res::MINIPORT, id - FirstHero, AFNoOffset);
		}
		else
		{
			w = 101;
			h = 93;
			image(x - w / 2, y, res::tokens(res::PORT0000 + id - FirstHero), 0, 0);
		}
		if(border)
			rectb(x - w / 2, y, x + w / 2, y + h, 0x0C);
	}
	if(id >= FirstCaptain && id <= LastCaptain)
	{
		static int indicies[] = {1, 0, 5, 2, 3, 4};
		w = 101;
		h = 93;
		image(x - w / 2, y, res::tokens(res::PORT0090 + indicies[id - FirstCaptain]), 0, 0);
		if(param)
			draw::image(x - w / 2 + 6, y - 2, res::CFLGSMAL, param - FirstPlayer);
		if(border)
			rectb(x - w / 2, y, x + w / 2, y + h, 0x0C);
	}
	else if(id >= FirstResource && id <= LastResource)
	{
		static unsigned char frames[] = {6, 0, 1, 2, 3, 4, 5};
		int id1 = frames[id - FirstResource];
		h = 50;
		w = res::width(res::RESOURCE, id1);
		image(x - w / 2, y + 30 - res::height(res::RESOURCE, id1), res::RESOURCE, id1, AFNoOffset);
		draw::state push;
		draw::font = res::SMALFONT;
		sznum(temp, param);
		text(x - textw(temp) / 2, y + 33, temp);
	}
	else if(id >= FirstBuilding && id <= LastBuilding)
	{
		h = 58;
		w = 137;
		image(x - w / 2 + 1, y + 1, res::buildings(param), indexes::buildings(id, 0));
		if(border)
			rectb(x - w / 2, y, x + w / 2, y + h, 0x0C);
	}
	else if(id >= FirstMonster && id <= LastMonster)
	{
		res::tokens tk = res::tokens(res::MONH0000 + id - FirstMonster);
		int type = game::get(id, Type);
		w = 80;
		h = 93;
		image(x - w / 2, y, res::STRIP, index_by_type(type));
		image(x - w / 2, y, tk, 0);
		if(param)
		{
			sznum(temp, param);
			draw::state push;
			draw::font = res::SMALFONT;
			text(x + w / 2 - 4 - draw::textw(temp), y + h - draw::texth(), temp);
		}
		if(border)
			rectb(x - w / 2, y, x + w / 2 + 2, y + h, 0x0C);
		// Don't uncomment this code! If do - your tooltips in castle not counted hero side.
		//if(draw::area(x - w / 2, y, x + w / 2, y + h))
		//{
		//	if(hot::key == MouseRight && hot::pressed)
		//		execute(Information, id);
		//}
	}
	else if(id >= FirstSkill && id <= LastSkill)
	{
		w = 80;
		h = res::height(res::SECSKILL, 0);
		const char* p = bsgets(id, Name);
		draw::image(x - w / 2, y, res::SECSKILL, id - FirstSkill + 1, 0);
		draw::text(x - w / 2 - 3 + (w - draw::textw(p)) / 2, y + 3, p);
		p = bsgets(param, Name);
		draw::text(x - w / 2 - 3 + (w - draw::textw(p)) / 2, y + 3 + 52, p);
	}
	else if(id >= FirstArtifact && id <= LastArtifact)
	{
		w = 80;
		h = res::height(res::ARTIFACT, 0);
		draw::image(x - w / 2 + 1, y, res::ARTIFACT, id - FirstArtifact + 1, 0);
	}
	return h;
}

static int paint_icons(int x, int y, int width, picture* icons, int count)
{
	const int pad = 4;
	if(!count)
		return 0;
	int y1 = y;
	while(count > 0)
	{
		int c = (count < 3) ? count : 3;
		int h = icons->height();
		int w1 = icons->width();
		int w = w1*c + (c - 1)*pad;
		int x1 = x + (width - w) / 2;
		for(int i = 0; i < c; i++)
		{
			int h1 = draw::clipart(x1 + w1 / 2, y, icons[i].id, icons[i].count, 0, true);
			if(h < h1)
				h = h1;
			x1 += w1 + pad;
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
			while(*p)
			{
				params[count].id = sz2num(p, &p);
				params[count].count = 0;
				if(*p == '/')
					params[count].count = sz2num(zskipsp(p + 1), &p);
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
				y += paint_icons(x, y, width, params, count) + 16;
		}
		else
		{
			int c = draw::textbc(p, width);
			draw::text(x, y, width, draw::Center, p, c);
			y += draw::texth();
			p += c;
			if(p > start && p[-1] == '\n')
				y += 6;
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
		clipart(x + a.pos.x, y + a.pos.y, a.id, cost[a.id - FirstResource], 0);
}

bool dlgask(const char* title, const char* text)
{
	draw::screenshoot surface;
	draw::state push;
	draw::font = res::FONT;
	res::tokens ic1 = draw::isevil(res::SYSTEME, res::SYSTEM);
	int tw = 304 - 54 - 16;
	int th = draw::textf(tw, text) + padding + res::height(ic1, 5);
	int x = (draw::width - tw) / 2;
	while(true)
	{
		surface.restore();
		int y1 = draw::dialog(th);
		//draw::rectb(x, y1, x+tw, y1+th, 0x40);
		y1 += draw::textf(x, y1, tw, text) + padding; // message text
		draw::button(x + 16, y1, ic1, 1, 5, 5, 6, KeyEnter);
		draw::button(x + tw - 16 - res::width(ic1, 7), y1, ic1, 2, 7, 7, 8, KeyEscape);
		draw::cursor(res::ADVMCO, 0);
		int id = draw::input();
		switch(id)
		{
		case 0:
		case 2:
			return false;
		case 1:
			return true;
		default:
			draw::definput(id);
			break;
		}
	}
}

void dlgmsg(const char* title, const char* text)
{
	draw::screenshoot surface;
	draw::state push;
	draw::font = res::FONT;
	auto ic1 = draw::isevil(res::SYSTEME, res::SYSTEM);
	auto tw = 304 - 54 - 16;
	auto th = draw::texth(text, tw);
	while(true)
	{
		surface.restore();
		int y1 = draw::dialog(th + res::height(ic1, 1) + padding);
		y1 = y1 + draw::textm((draw::width - tw) / 2, y1, tw, draw::Center, text) + padding; // message text
		draw::button((draw::width - res::width(ic1, 1)) / 2, y1,
			ic1, 1, 1, 1, 2, KeyEnter);
		draw::cursor(res::ADVMCO, 0);
		switch(draw::input())
		{
		case 0:
		case 1:
			return;
		}
	}
}

void show::tips(const char* text)
{
	draw::screenshoot surface;
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
		if(hot::param >= FirstMonster && hot::param <= LastMonster)
			show::unit(hot::param, hot::param2, 0, 0);
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
			hot_troops_index = 0;
		}
	}
}

void draw::troops(int x, int y, int rec)
{
	int w = res::width(res::STRIP, 2);
	int h = res::width(res::STRIP, 2);
	for(int i = FirstTroopsIndex; i <= LastTroopsIndex; i += 2)
	{
		int unit = bsget(rec, i);
		int count = bsget(rec, i + 1);
		if(unit == 0)
			image(x, y, res::STRIP, 2);
		else
			clipart(x + 40, y, unit, count);
		if(i == hot_troops_index && hot_troops_owner == rec)
			image(x, y, res::STRIP, 1);
		if(area(x, y, x + w, y + h))
		{
			if(hot::key == MouseRight && hot::pressed)
				execute(Information, bsget(rec, i), rec);
			else if(hot::key == MouseLeft && hot::pressed)
				execute(i, rec);
			else if(hot::key == MouseLeftDBL)
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

void draw::screenshoot::blend(screenshoot& source)
{
	auto bits = ptr(0, 0);
	auto rgb_lookup = (unsigned char*)res::get(res::BinRGBLOOKP);
	if(!rgb_lookup || !bits)
		return;
	for(int j = 0; j<16; j++)
	{
		unsigned char a = j*16;
		for(int i = 0; i < 640 * 480; i++)
		{
			unsigned char res[4];
			unsigned char* c1 = pallette[source.bits[i]];
			unsigned char* c2 = pallette[this->bits[i]];
			res[2] = (((int)c1[0] * a + c2[0] * (255 - a))) >> (8 + 3);
			res[1] = (((int)c1[1] * a + c2[1] * (255 - a))) >> (8 + 3);
			res[0] = (((int)c1[2] * a + c2[2] * (255 - a))) >> (8 + 3);
			bits[i] = rgb_lookup[(res[0] << 10) + (res[1] << 5) + res[2]];
		}
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