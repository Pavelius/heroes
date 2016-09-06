#include "main.h"

const int textx = 185;
const int startx = 210;
const int maxw = 430;

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

static void paint_monsters(int x, int y, int width, int* players, int count, int* source)
{
	if(!count)
		return;
	draw::state push;
	draw::font = res::SMALFONT;
	const int chunk = width / count;
	for(auto i = 0; i < count; i++)
	{
		int id = source[i];
		int frame = (id - FirstMonster) * 9;
		auto px = x + chunk / 2 + i * chunk;
		draw::image(px, y + 12, res::MINIMON, frame);
		draw::image(px, y + 12, res::MINIMON, frame+1);
		auto p = bsgets(source[i], Name);
		draw::text(px - draw::textw(p) / 2, y + 15, p);
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
			szprint(temp, "%1: %2i", attr_names[j-Attack][locale], game::get(hero, j));
			draw::text(px - draw::textw(temp) / 2, y + (j-Attack)*draw::texth() - 2, temp);
		}
	}
}

static void paint_title(int x, int y, int width, int count)
{
	static const char* text_header[6][2] = {
		{"1st","1-ое"},
		{"2nd", "2-ое"},
		{"3rd", "3-ье"},
		{"4th", "4-ое"},
		{"5th", "5-ое"},
		{"6th", "6-ое"},
	};
	if(!count)
		return;
	const int chunk = width / count;
	for(auto i = 0; i < count; i++)
	{
		auto px = x + chunk / 2 + i * chunk;
		auto p = text_header[i][locale];
		draw::text(px - draw::textw(p) / 2, y, p);
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

static void paint_flags(int x, int y, int width, int* players, int count, int* source)
{
	int places[6] = {0};
	const auto chunk = width / count;

	for(int i = 0; i < count; ++i)
	{
		int players_source[6] = {0};
		for(int j = 0; j < count; j++)
		{
			if(places[j] == i)
				zcat(players_source, players[j]);
		}
		int players_count = zlen(players_source);
		if(!players_count)
			continue;
		auto sw = res::width(res::FLAG32, 1);
		auto px = x + chunk / 2 + i * chunk - (players_count * sw) / 2;
		for(int j = 0; j<players_count; j++)
		{
			auto frame = (players_source[j] - FirstPlayer) * 2 + 1;
			draw::image(px, y, res::FLAG32, frame, AFNoOffset);
			px += sw;
		}
	}
}

static void paint_summary(int x, int y, const char* text, int* players, int count, int* source)
{
	draw::text(x, y, text);
	paint_flags(x + startx, y, maxw, players, count, source);
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
	int monsters[LastPlayer - FirstPlayer + 1];
	for(int i = 0; i < count; i++)
		cities[i] = game::getcastles(players[i], 2);
	for(int i = 0; i < count; i++)
		towns[i] = game::getcastles(players[i], 1);
	for(int i = 0; i < count; i++)
		heroes[i] = game::getbesthero(players[i]);
	for(int i = 0; i < count; i++)
		monsters[i] = game::getbestmonster(players[i]);
	// Constants
	while(true)
	{
		draw::image(0, 0, res::STONEBAK, 0);
		draw::image(0, 461, res::WELLXTRA, 2);
		int x = 4;
		// Часть 1
		paint_title(x + startx, 5, maxw, count);
		paint_summary(x, 28, szt("Number of towns:", "Количество городов:"), players, count, towns);
		paint_summary(x, 54, szt("Number of castles:", "Количество замков:"), players, count, cities);
		paint_summary(x, 80, szt("Number of heroes:", "Количество героев:"), players, count, heroes);
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
		paint_monsters(x + startx, 429, maxw, players, count, monsters);
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