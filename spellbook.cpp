#include "main.h"

static int compare(const void* p1, const void* p2)
{
	int m1 = bsget(*(int*)p1, Level);
	int m2 = bsget(*(int*)p2, Level);
	if(m1==m2)
		return strcmp(bsgets(*(int*)p1, Name), bsgets(*(int*)p2, Name));
	return m1-m2;
}

static int select_spells(int* result, int rec, tokens mode)
{
	int* p = result;
	for(int i = FirstSpell; i <= (int)LastSpell; i++)
	{
		if(!bsget(rec, i))
			continue;
		if(i == SpellStone)
			continue;
		if(mode == CombatSpells && !game::iscombat(i))
			continue;
		if(mode == AdventureSpells && game::iscombat(i))
			continue;
		*p++ = i;
	}
	*p = 0;
	return p - result;
}

int show::spellbook(int rec, tokens mode)
{
	if(!rec)
		return 0;
	const int cw1 = 82;
	const int cw2 = 42;
	const int ch1 = 76;
	const int spell_per_page = 12;
	int names[LastSpell-FirstSpell+1];
	res::tokens back = res::BOOK;
	int w1 = res::width(back, 0);
	int h1 = res::height(back, 0);
	int x = draw::width / 2;
	int y = (draw::height - h1) / 2 - 32;
	int current_page = 0;
	// fill knowing spells
	int names_count = select_spells(names, rec, mode);
	qsort(names, names_count, sizeof(names[0]), compare);
	char temp[32];
	draw::screenshoot surface;
	draw::state push;
	draw::font = res::SMALFONT;
	while(true)
	{
		int x1 = x - w1 - 1;
		int y1 = y;
		surface.restore();
		// Open book
		draw::image(x, y, res::BOOK, 0);
		draw::image(x, y, res::BOOK, 0, AFMirror);
		draw::image(x1+125, y1+275, res::BOOK, 6); // information bookmark
		if(mode!=CombatSpells)
			draw::image(x1+270, y1+270, res::BOOK, 3); // adventure bookmark
		if(mode!=AdventureSpells)
			draw::image(x1+304, y1+284, res::BOOK, 4); // combat bookmark
		draw::button(x1+420, y1+284, res::BOOK, Cancel, 5, 5, 5, KeyEscape); // exit bookmark
		// Spell points
		int sp = bsget(rec, SpellPoints);
		sznum(temp, sp);
		draw::text(x1 + 125 + 17 - draw::textw(temp)/2, y1+308, temp);
		y1 += 23;
		x1 += 46;
		int ox = x1;
		int oy = y1;
		for(int i = 0; i + current_page*spell_per_page < names_count; i++)
		{
			if(i>=spell_per_page)
				break;
			int sid = names[i + current_page*spell_per_page];
			int side = i/(spell_per_page/2); // 0 - left, 1 - right
			int column = i%2; // 0 - left column, 1 - right column
			int row = (i/2)%3;
			int spr = bsget(sid, Portrait);
			rect rc;
			rc.x1 = ox;
			if(column)
				rc.x1 += cw1;
			if(side)
				rc.x1 += cw1*2 + 50;
			rc.y1 = oy;
			rc.y1 += row*ch1;
			if(side)
			{
				rc.x1 += 2*row;
				if(column)
					rc.y1 += 8;
			}
			else
			{
				rc.x1 -= 2*row;
				if(!column)
					rc.y1 += 8;
			}
			rc.x2 = rc.x1 + cw1 - 1;
			rc.y2 = rc.y1 + ch1 - 1;
			draw::image(rc.x1+(cw1-res::width(res::SPELLS, spr))/2, rc.y1+(cw2-res::height(res::SPELLS, spr))/2, res::SPELLS, spr, AFNoOffset);
			szprint(temp, "%1 (%2i)", bsgets(sid, Name), bsget(sid, SpellPoints));
			//draw::text(rc.x1+(cw1-draw::textw(temp))/2, rc.y1+cw2+1, temp);
			if(sp>=bsget(sid, SpellPoints))
			{
				if(hot::mouse.in(rc))
				{
					if(hot::key==MouseLeft && hot::pressed)
						draw::execute(names[i]);
				}
				draw::textm(rc.x1, rc.y1+cw2+1, cw1, draw::Center, temp);
			}
			else
			{
				//draw::fontgr push(sheme::gray_font);
				draw::textm(rc.x1, rc.y1+cw2+1, cw1, draw::Center, temp);
			}
		}
		draw::cursor(res::ADVMCO, 0);
		int id = draw::input();
		if(!id || id==Cancel)
			return 0;
		if(id>=(int)FirstSpell && id<=(int)LastSpell)
			return id;
	}
}