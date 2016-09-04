#include "main.h"

void show::hero(tokens rec)
{
	char temp[260];
	draw::current::focus = 0;
	while(true)
	{
		draw::status(22, draw::height - 16, 22 + res::width(res::HSBTNS, 8), draw::height - 1);
		draw::button(0, draw::height - 20, res::HSBTNS, KeyLeft, 4, 4, 5, 0, 0, szt("Previous hero", "Предыдущий герой"));
		draw::image(22, draw::height - 20, res::HSBTNS, 8);
		draw::button(draw::width - 22, draw::height - 20, res::HSBTNS, KeyRight, 6, 6, 7, 0, 0, szt("Next hero", "Следующий герой"));
		draw::image(0, 0, res::HEROBKG, 0);
		draw::image(0, 0, draw::isevil(res::HEROEXTE, res::HEROEXTG), 0);
		draw::clipart(99, 31, rec, LargeSize);
		szprint(temp, szt("%1 the %2 ( Level %3i )", "%1 - %2 ( Уровень %3i )"),
			bsgets(rec, Name),
			bsgets(game::get(rec, Type), Name),
			bsget(rec, Level));
		draw::text((draw::width - draw::textw(temp)) / 2, 3, temp);
		// Abilities
		for(int i = (int)Attack; i <= (int)Wisdow; i++)
		{
			int x = 152 + (i - Attack) * 88;
			int y = 36;
			if(true)
			{
				draw::state push;
				draw::font = res::SMALFONT;
				const char* p = bsgets(i, Name);
				draw::text(x + (88 - draw::textw(p)) / 2, y, p);
			}
			sznum(temp, game::get(rec, i));
			draw::text(x + (88 - draw::textw(temp)) / 2, y + 72, temp);
		}
		// Skills
		if(true)
		{
			draw::state push;
			draw::font = res::SMALFONT;
			int x = 3 + 40;
			int y = 233;
			const int dx = res::width(res::SECSKILL, 0) + 5;
			for(int i = FirstSkill; i <= LastSkill; i++)
			{
				int n = bsget(rec, i);
				if(!n)
					continue;
				draw::clipart(x, y, i, n + SkillLevelBasic - 1);
				x += dx;
			}
			int x2 = 3 + 40 + 8 * dx;
			while(x < x2)
			{
				draw::image(x - 40, y, res::SECSKILL, 0, 0);
				x += dx;
			}
		}
		if(true)
		{
			const int dx = res::width(res::ARTIFACT, 0) + 15;
			const int dy = res::height(res::ARTIFACT, 0) + 15;
			int x = 51 + dx / 2;
			int y = 308;
			int x2 = x + 7 * dx;
			for(int i = FirstArtifactIndex; i <= LastArtifactIndex; i++)
			{
				int n = bsget(rec, i);
				if(n)
					draw::clipart(x, y, n, 0);
				else
					draw::image(x - dx / 2, y, res::ARTIFACT, 0);
				x += dx;
				if(x >= x2)
				{
					x = 51 + dx / 2;
					y += dy;
				}
			}
		}
		draw::image(49, 130, res::CREST, bsget(rec, Player) - FirstPlayer);
		draw::troops(156, 130, rec, draw::current::focus);
		draw::button(5, 318, res::HSBTNS, Dismiss, 0, 0, 1, Alpha + 'A', 0, szt("Dismiss hero", "Уволить героя"));
		draw::button(603, 318, res::HSBTNS, Cancel, 2, 2, 3, KeyEscape, 0, szt("Close hero window", "Закрыть окно"));
		draw::cursor(res::ADVMCO, 0);
		int id = draw::input();
		switch(id)
		{
		case Cancel:
		case 0:
			return;
		default:
			draw::definput(id);
			break;
		}
	}
}
