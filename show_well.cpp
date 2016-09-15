#include "main.h"

void show::well(int rec)
{
	auto type = bsget(rec, Type);
	auto player = bsget(rec, Player);
	while(true)
	{
		draw::image(0, 0, res::STONEBAK, 0);
		draw::image(0, 461, res::WELLXTRA, 2);
		draw::status(21, draw::height - 16, 21 + res::width(res::SMALLBAR, 0), draw::height - 1);
		draw::button(578, 461, res::WELLXTRA, Cancel, 0, 0, 1, KeyEscape);
		draw::cursor(res::ADVMCO, 0);
		int id = draw::input();
		switch(id)
		{
		case Cancel:
			return;
		default:
			draw::definput(id);
			break;
		}
	}
}