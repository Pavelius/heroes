#include "main.h"

const int padding = 16;

struct picture
{
	int id;
	int	count;

	int width() const
	{
		if(id >= (int)FirstHero && id <= (int)LastHero)
			return 101;
		else if(id >= (int)FirstResource && id <= (int)LastResource)
			return 64;
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
			return 72;
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

int draw::clipart(int x, int y, int id, int param, int param2)
{
	char temp[32];
	if(id >= FirstHero && id <= LastHero)
	{
		const int w = 101;
		const int h = 93;
		switch(param)
		{
		case LargeSize:
			image(x - w / 2, y, res::tokens(res::PORT0000 + id - FirstHero), 0, 0);
			return h;
		}
	}
	else if(id >= FirstResource && id <= LastResource)
	{
		int id1 = id - FirstResource;
		int w = res::width(res::RESOURCE, id1);
		image(x - w / 2, y + 30 - res::height(res::RESOURCE, id1), res::RESOURCE, id1, AFNoOffset);
		draw::state push;
		draw::font = res::SMALFONT;
		sznum(temp, param);
		text(x - textw(temp) / 2, y + 33, temp);
		return 50;
	}
	else if(id >= FirstBuilding && id <= LastBuilding)
	{
		int type = bsget(param, Type);
		const int h = 72;
		const int w = 137;
		image(x - w / 2, y, res::BLDGXTRA, 0);
		image(x - w / 2 + 1, y + 1, res::buildings(type), indexes::buildings(id, 0));
		const char* name = bsgets(param, id);
		if(name)
		{
			draw::state push;
			draw::font = res::SMALFONT;
			text(x - textw(name) / 2, y + h - texth() - 1, name);
		}
		return h;
	}
	else if(id >= FirstMonster && id <= LastMonster)
	{
		res::tokens tk = res::tokens(res::MONH0000 + id - FirstMonster);
		int type = game::get(id, Type);
		const int w = 80;
		const int h = 93;
		image(x - w / 2, y, res::STRIP, index_by_type(type));
		image(x - w / 2, y, tk, 0);
		if(param)
		{
			sznum(temp, param);
			draw::state push;
			draw::font = res::SMALFONT;
			text(x + w / 2 - 4 - draw::textw(temp), y + h - draw::texth(), temp);
		}
		if(draw::area(x - w / 2, y, x + w / 2, y + h))
		{
			if(hot::key == MouseRight && hot::pressed)
				draw::execute(Tooltips, id);
		}
		return h;
	}
	else if(id >= FirstSkill && id <= LastSkill)
	{
		const int w = 80;
		const int h = res::height(res::SECSKILL, 0);
		const char* p = bsgets(id, Name);
		draw::image(x - 40, y, res::SECSKILL, id - FirstSkill + 1, 0);
		draw::text(x - 43 + (w - draw::textw(p)) / 2, y + 3, p);
		p = bsgets(param, Name);
		draw::text(x - 43 + (w - draw::textw(p)) / 2, y + 3 + 52, p);
		return h;
	}
	else if(id >= FirstArtifact && id <= LastArtifact)
	{
		const int w = 80;
		const int h = res::height(res::ARTIFACT, 0);
		draw::image(x - w / 2 + 1, y, res::ARTIFACT, id - FirstArtifact + 1, 0);
		return h;
	}
	return 0;
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

static int paint_icons(int x, int y, int width, picture* icons, int count, bool calculate)
{
	const int pad = 4;
	if(!count)
		return 0;
	int y1 = y;
	while(count > 0)
	{
		int c = (count < 3) ? count : 3;
		int h = icons[0].height();
		int w1 = icons[0].width();
		int w = w1*c + (c - 1)*pad;
		int x1 = x + (width - w) / 2 + w1 / 2;
		if(!calculate)
		{
			for(int i = 0; i < c; i++)
			{
				draw::clipart(x1, y, icons[i].id, icons[i].count);
				x1 += w1 + pad;
			}
		}
		y += h;
		icons += c;
		count -= c;
	}
	return y - y1;
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
		clipart(x + a.pos.x, y + a.pos.y, a.id, cost[a.id-FirstResource], 0);
}

int draw::textf(int x, int y, int width, const char* p, bool calculate)
{
	char temp[260];
	picture params[16];
	int y4 = y;
	while(*p)
	{
		if(*p == '%' && p[1] != '%')
		{
			int count = 0;
			char a = p[1];
			while(*p == '%' && a == p[1])
			{
				p++;
				params[count].id = 0;
				params[count].count = 0;
				switch(*p++)
				{
				case 'b':
					params[count].id = FirstBuilding;
					break;
				case 'h':
					params[count].id = FirstHero;
					params[count].count = LargeSize;
					break;
				case 'r':
					params[count].id = FirstResource;
					break;
				case 'm':
					params[count].id = FirstMonster;
					break;
				default:
					break;
				}
				params[count].id += sz2num(p, &p);
				if(*p == '(')
				{
					params[count].count = sz2num(zskipsp(p + 1), &p);
					if(*p == ')')
						p++;
				}
				p = zskipsp(p);
				count++;
			}
			if(count)
			{
				if(y != y4)
					y += 16;
				y += paint_icons(x, y, width, params, count, calculate);
			}
		}
		else
		{
			char* w = temp;
			while(*p != '%' && *p)
				*w++ = *p++;
			*w++ = 0;
			if(y != y4)
				y += 16;
			if(calculate)
				y += texth(temp, width);
			else
				y += textm(x, y, width, draw::Center, temp);
			p = zskipspcr(p);
		}
	}
	return y - y4;
}

bool dlgask(const char* title, const char* text)
{
	draw::screenshoot surface;
	draw::state push;
	draw::font = res::FONT;
	res::tokens ic1 = draw::isevil(res::SYSTEME, res::SYSTEM);
	int tw = 304 - 54 - 16;
	int th = draw::textf(0, 0, tw, text, true) + padding + res::height(ic1, 5);
	int x = (draw::width - tw) / 2;
	while(true)
	{
		surface.restore();
		int y1 = draw::dialog(th);
		//draw::rectb(x, y1, x+tw, y1+th, 0x40);
		y1 += draw::textf(x, y1, tw, text, false) + padding; // message text
		draw::button(x + 16, y1, ic1, 1, 5, 5, 6, KeyEnter);
		draw::button(x + tw - 16 - res::width(ic1, 7), y1, ic1, 2, 7, 7, 8, KeyEscape);
		draw::cursor(res::ADVMCO, 0);
		switch(draw::input())
		{
		case 0:
		case 2:
			return false;
		case 1:
			return true;
		}
	}
}

void dlgerr(const char* title, const char* format, ...)
{
	char temp[4096];
	szprintv(temp, format, xva_start(format));
	dlgmsg(title, temp);
}

void dlgmsg(const char* title, const char* text)
{
	draw::screenshoot surface;
	draw::state push;
	draw::font = res::FONT;
	res::tokens ic1 = draw::isevil(res::SYSTEME, res::SYSTEM);
	int tw = 304 - 54 - 16;
	int th = draw::texth(text, tw);
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
	int th = draw::textf(0, 0, tw, text, true);
	int x = (draw::width - tw) / 2;
	while(true)
	{
		surface.restore();
		int y1 = draw::dialog(th);
		y1 += draw::textf(x, y1, tw, text, false) + padding; // message text
		draw::cursor(res::ADVMCO, 0);
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

void draw::debug()
{
	char temp[64];
	draw::state push;
	draw::font = res::SMALFONT;
	szprint(temp, "Mouse %1i, %2i", hot::mouse.x, hot::mouse.y);
	text(10, 10, temp);
}

void draw::troopsinput(int id)
{
	static int source_rec;
	if(id >= FirstTroopsIndex && id <= LastTroopsIndex)
	{
		if(!current::focus)
		{
			current::focus = id;
			current::param = hot::param;
		}
		else if(current::focus == id && current::param == hot::param)
		{
			show::unit(bsget(hot::param, id), hot::param);
			current::focus = 0;
		}
		else
		{
			int dest = id;
			int dest_rec = hot::param;
			int source = current::focus;
			int source_rec = current::param;
			int v11 = bsget(dest_rec, dest);
			int v12 = bsget(dest_rec, dest + 1);
			int v21 = bsget(source_rec, source);
			int v22 = bsget(source_rec, source + 1);
			bsset(source_rec, source, v11);
			bsset(source_rec, source + 1, v12);
			bsset(dest_rec, dest, v21);
			bsset(dest_rec, dest + 1, v22);
			current::focus = 0;
		}
	}
}

void draw::troops(int x, int y, int rec, int index)
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
		if(i == index)
			image(x, y, res::STRIP, 1);
		if(area(x, y, x + w, y + h))
		{
			if((hot::key == MouseLeft && hot::pressed) || (hot::key == MouseLeftDBL))
				execute(i, rec);
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