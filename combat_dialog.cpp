#include "main.h"

static int control(int x, int y, int side, int id)
{
	draw::fontsm push;
	char temp[128];
	szprint(temp, "%1: %2i", bsgets(id, Name), game::get(side, id));
	draw::text(x-draw::textw(temp)/2, y, temp);
	return draw::texth();
}

static int control(int x, int y, int side, int id, int id2)
{
	draw::fontsm push;
	char temp[128];
	szprint(temp, "%1: %2i/%3i", bsgets(id, Name), game::get(side, id), game::get(side, id2));
	draw::text(x-draw::textw(temp)/2, y, temp);
	return draw::texth();
}

static int controt(int x, int y, int side, int id)
{
	draw::fontsm push;
	char temp[128];
	szprint(temp, "%1: %2", bsgets(id, Name), bsgets(game::get(side, id), Name));
	draw::text(x-draw::textw(temp)/2, y, temp);
	return draw::texth();
}

int show::battle::dialog(int side)
{
	char temp[128];
	draw::screenshoot surface;
	res::tokens back = draw::isevil(res::VGENBKGE, res::VGENBKG);
	int w1 = res::width(back, 0);
	int h1 = res::height(back, 0);
	int x = (draw::width - w1) / 2;
	int y = (draw::height - h1) / 2 - 32;
    while(true)
    {
    	surface.restore();
    	draw::image(x, y, back, 1);
    	draw::image(x, y, back, 0);
    	draw::clipart(x+27+50, y+42, side, LargeSize, 0);
		int col = 1;
		draw::image(x + 148, y + 36, res::VIEWGEN, col);
		szprint(temp, "%1 - %2", bsgets(side,Name), bsgets(bsget(side, Type),Name));
		draw::text((draw::width-draw::textw(temp))/2, y + 12, temp);
		int y1 = y + 40 + 8;
		y1 += control(x+205, y1, side, Attack);
		y1 += control(x+205, y1, side, Defence);
		y1 += control(x+205, y1, side, SpellPower);
		y1 += control(x+205, y1, side, Wisdow)+8;
		y1 += controt(x+205, y1, side, Morale);
		y1 += controt(x+205, y1, side, Luck);
		y1 += control(x+205, y1, side, SpellPoints, SpellPointsMax);
		draw::button(x + 30, y + 148, res::VIEWGEN, Spells, 9, 9, 10, 0);
		draw::button(x + 89, y + 148, res::VIEWGEN, RunAway, 11, 11, 12, 0);
		draw::button(x + 148, y + 148, res::VIEWGEN, Surrender, 13, 13, 14, 0);
		draw::button(x + 207, y + 148, res::VIEWGEN, Cancel, 15, 15, 16, KeyEscape);
        draw::cursor(res::ADVMCO, 0);
        int id = draw::input();
        switch(id)
        {
        case Cancel:
        case 0:
            return 0;
		case Spells:
		case Surrender:
			return id;
		case RunAway:
			if(!show::question(
					szt("Do you really want to leave your army and flee away?", "Вы действительно хотите покинуть армию и бежать прочь?")))
					break;
			return id;
        }
    }
}