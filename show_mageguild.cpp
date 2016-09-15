#include "main.h"

static void show_row(int x, int y, int rec, int level, int maximum, int guild_level)
{
	int source[5];
	int count = 0;
	auto icn = res::TOWNWIND;
	for(int i = FirstSpell; i <= LastSpell; i++)
	{
		if(!bsget(rec, i))
			continue;
		if(bsget(i, Level)!=level)
			continue;
		source[count++] = i;
		if(count >= 4)
			break;
	}
	source[count] = 0;
	int w1 = res::width(icn, 0);
	if(level > guild_level)
	{
		for(int i = 0; i < maximum; i++)
		{
			draw::image(x-4, y, icn, 1);
			x += w1 + 4;
		}
	}
	else
	{
		for(int i = 0; i < count; i++)
		{
			draw::image(x, y, icn, 0);
			draw::clipart(x + w1 / 2, y + 16, source[i], 0);
			x += w1 + 4;
		}
	}
}

void show::mageguild(int rec)
{
	auto type = bsget(rec, Type);
	auto player = bsget(rec, Player);
	auto level = bsget(rec, MageGuild);
	if(!level)
		return;
	while(true)
	{
		draw::image(0, 0, res::STONEBAK, 0);
		draw::image(0, 461, res::WELLXTRA, 2);
		draw::status(21, draw::height - 16, 21 + res::width(res::SMALLBAR, 0), draw::height - 1);
		// Show mage guild
		auto icn = res::Empthy;
		switch(type)
		{
		case Knight: icn = res::MAGEGLDK; break;
		case Barbarian: icn = res::MAGEGLDB; break;
		case Sorcerer: icn = res::MAGEGLDS; break;
		case Warlock: icn = res::MAGEGLDW; break;
		case Wizard: icn = res::MAGEGLDZ; break;
		case Necromancer: icn = res::MAGEGLDN; break;
		default: break;
		}
		int frame = level - 1;
		draw::image(64 - res::width(icn, frame) / 2, 190 - res::height(icn, frame), icn, frame);
		// Show spells
		int library = 0;
		if(type == Wizard && bsget(rec, SpecialBuilding))
			library = 1;
		int x = 180;
		show_row(x, 5, rec, 5, 1 + library, level);
		show_row(x, 95, rec, 4, 2 + library, level);
		show_row(x, 185, rec, 3, 2 + library, level);
		show_row(x, 275, rec, 2, 3 + library, level);
		show_row(x, 365, rec, 1, 3+library, level);
		//
		draw::status(szt("This spells allow here.", "”казанные заклинани€ доступны здесь"));
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