#include "main.h"

static int combat_timeout_values[] = {1000/12, 1000/20, 1000/28, 0};

static void checkbox(int x, int y, res::tokens icn, bool& value, int off, int on, const char* text)
{
	if(draw::area(x, y, x + res::width(icn, off), y + res::height(icn, off)))
	{
		if(hot::key == MouseLeft && hot::pressed)
			value = !value;
		draw::status(text);
	}
	int i = value ? on : off;
	draw::image(x, y, icn, i);
}

static void radiobox(int x, int y, res::tokens icn, int& value, int min, int max, const char* text)
{
	if(draw::area(x, y, x + res::width(icn, min), y + res::height(icn, min)))
	{
		if(hot::key == MouseLeft && hot::pressed)
			value++;
		draw::status(text);
	}
	if(min + value > max)
		value = 0;
	int i = min + value;
	draw::image(x, y, icn, i);
}

void show::settings()
{
	draw::screenshoot surface;
	res::tokens icn = draw::isevil(res::CSPANBKE, res::CSPANBKG);
	int h8 = res::height(res::TEXTBAR, 8);
	int h9 = res::height(res::TEXTBAR, 9);
	int w3 = res::width(res::TEXTBAR, 0);
	int x1 = (draw::width - res::width(icn, 0)) / 2 + 16;
	int y1 = (draw::height - res::height(icn, 0)) / 2;
	while(true)
	{
		surface.restore();
		draw::image(x1 - 16, y1, icn, 1);
		draw::image(x1 - 16, y1, icn, 0);
		draw::status(32, draw::height - h8 - h9 + 1, draw::width - w3, draw::height - 1);
		radiobox(x1 + 36, y1 + 47, res::CSPANEL, combat::setting::speed, 0, 2, szt("Animation speed", "Скорость анимации"));
		radiobox(x1 + 128, y1 + 47, res::CSPANEL, combat::setting::info, 3, 5, szt("Units informations level", "Уровень информации об отряде"));
		checkbox(x1 + 220, y1 + 47, res::CSPANEL, combat::setting::spells, 6, 7, szt("Use spells in autocombat mode", "Использовать заклинания в автоматическом бою"));
		checkbox(x1 + 36, y1 + 157, res::CSPANEL, combat::setting::grid, 8, 9, szt("Show/Hide battle grid", "Показать/Спрятать сетку"));
		checkbox(x1 + 128, y1 + 157, res::CSPANEL, combat::setting::movement, 10, 11, szt("Show/Hide movement shadows", "Показать/Спрятать движение"));
		checkbox(x1 + 220, y1 + 157, res::CSPANEL, combat::setting::cursor, 12, 13, szt("Shadow cursor hexagon", "Подсвечивать текущий гекс"));
		draw::button(x1 + 113, y1 + 252, draw::isevil(res::CSPANBTE, res::CSPANBTN), Accept, 0, 0, 1, KeyEnter);
		if(hot::key == KeyEscape)
			draw::execute(Cancel);
		draw::cursor(res::ADVMCO, 0);
		int id = draw::input();
		switch(id)
		{
		case Cancel:
		case 0:
			return;
		case Accept:
			return;
		}
	}
}