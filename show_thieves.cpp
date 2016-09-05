#include "main.h"

static void paint_heroes(int x, int y, int width, int* players, int count, int* source)
{
	if(!count)
		return;
	draw::state push;
	draw::font = res::SMALFONT;
	const int chunk = width / count;
	for(auto i = 0; i < count; i++)
	{
		auto px = x + chunk / 2 + i * chunk;
		draw::clipart(px, y, source[i], 1);
		draw::image(px - res::width(res::LOCATORS,22)/2, y + 1, res::LOCATORS, 22, AFNoOffset);
		auto p = bsgets(source[i], Name);
		draw::text(px - draw::textw(p)/2, y - draw::texth() - 2, p);
	}
}

static void paint_heroes_stat(int x, int y, int width, int* players, int count, int* source)
{
	static const char* attr_names[][2] = {
		{"At", "Ат"},
		{"Df", "Зщ"},
		{"Sp", "Сл"},
		{"Kn", "Зн"},
	};
	if(!count)
		return;
	draw::state push;
	draw::font = res::SMALFONT;
	const int chunk = width / count;
	for(auto i = 0; i < count; i++)
	{
		char temp[64];
		auto hero = source[i];
		auto px = x + chunk / 2 + i * chunk;
		for(int j = Attack; j <= Wisdow; j++)
		{
			szprint(temp, "%1: %2i", attr_names[j-Attack][locale], bsget(hero, j));
			draw::text(px - draw::textw(temp) / 2, y + (j-Attack)*draw::texth() - 2, temp);
		}
	}
}

static void paint_header(int x, int y, int width, int* players, int count)
{
	if(!count)
		return;
	const int chunk = width / count;
	for(auto i = 0; i < count; i++)
	{
		auto px = x + chunk / 2 + i * chunk;
		auto p = bsgets(players[i], Name);
		draw::text(px - draw::textw(p) / 2, 270, p);
	}
}

int show::thieves(int player)
{
	// Select active players
	int players[6];
	int count = 0;
	for(int i = FirstPlayer; i <= LastPlayer; i++)
	{
		if(!bsget(i, Type))
			continue;
		players[count++] = i;
	}
	if(!count)
		return 0;
	// Select cities and towns
	int cities[LastPlayer - FirstPlayer + 1];
	int towns[LastPlayer - FirstPlayer + 1];
	int heroes[LastPlayer - FirstPlayer + 1];
	for(int i = 0; i < count; i++)
		cities[i] = game::getcastles(players[i], 2);
	for(int i = 0; i < count; i++)
		towns[i] = game::getcastles(players[i], 1);
	for(int i = 0; i < count; i++)
		heroes[i] = game::getbesthero(players[i]);
	// Constants
	const int textx = 185;
	const int startx = 210;
	const int maxw = 430;
	while(true)
	{
		draw::image(0, 0, res::STONEBAK, 0);
		draw::image(0, 461, res::WELLXTRA, 2);
		int x = 4;
		draw::text(x, 28, szt("Number of towns:", "Количество городов:"));
		draw::text(x, 54, szt("Number of castles:", "Количество замков:"));
		draw::text(x, 80, szt("Number of heroes:", "Количество героев:"));
		draw::text(x, 106, szt("Gold in Treasury:", "Золота в казне:"));
		draw::text(x, 132, szt("Wood & Ore:", "Дерево и руда:"));
		draw::text(x, 158, szt("Other resources:", "Другие ресурсы:"));
		draw::text(x, 184, szt("Obelisks Found:", "Найдено обелисков:"));
		draw::text(x, 210, szt("Total Army Strength:", "Общая сила армии:"));
		draw::text(x, 238, szt("Income:", "Доход:"));
		// Часть 2
		paint_header(x + startx, 270, maxw, players, count);
		draw::text(x, 306, szt("Best Hero:", "Лучший герой:"));
		paint_heroes(x + startx, 306, maxw, players, count, heroes);
		draw::text(x, 347, szt("Best Hero Stats:", "Атрибуты лучшего героя:"));
		paint_heroes_stat(x + startx, 347, maxw, players, count, heroes);
		draw::text(x, 388, szt("Personality:", "Характер:"));
		draw::text(x, 429, szt("Best Monster:", "Лучший монстер:"));
		draw::button(578, 461, res::WELLXTRA, Cancel, 0, 0, 1, KeyEscape);
		draw::cursor(res::ADVMCO, 0);
		int id = draw::input();
		switch(id)
		{
		case 0:
			return 0;
		case Cancel:
			return Cancel;
		default:
			draw::definput(id);
			break;
		}
	}
}