#include "main.h"

static void paint_resource(int x, int y, res::tokens icn, int frame, tokens id, const void* resources_void, tokens select, tokens resf, int markets)
{
	char temp[32];
	auto resources = (int*)resources_void;
	draw::image(x, y, icn, frame);
	if(markets == 0)
	{
		int value = resources[id - FirstResource];
		sznum(temp, value);
	}
	else
	{
		int value = game::gettrade(resf, id, markets);
		if(value)
			szprint(temp, "1/%1i", value);
		else
			szprint(temp, szt("n/a", "н/д"));
	}
	draw::text(x + (34 - draw::textw(temp)) / 2, y + 23, temp);
	if(select == id)
		draw::image(x-2, y-2, icn, 14);
	if(draw::area(x - 2, y - 2, x + 37, y + 37))
	{
		if(hot::key == MouseLeft && hot::pressed)
			draw::execute(markets ? TargetIndex : SourceIndex, id);
	}
}

static void paint_resources(int x, int y, res::tokens icn, const void* resources, tokens select, tokens resf, int markets)
{
	draw::state push;
	draw::font = res::SMALFONT;
	paint_resource(x, y, icn, 7, Wood, resources, select, resf, markets);
	paint_resource(x + 37, y, icn, 8, Mercury, resources, select, resf, markets);
	paint_resource(x + 74, y, icn, 9, Ore, resources, select, resf, markets);
	paint_resource(x, y + 37, icn, 10, Sulfur, resources, select, resf, markets);
	paint_resource(x + 37, y + 37, icn, 11, Crystal, resources, select, resf, markets);
	paint_resource(x + 74, y + 37, icn, 12, Gems, resources, select, resf, markets);
	paint_resource(x + 37, y + 74, icn, 13, Gold, resources, select, resf, markets);
}

void show::marketplace(int player)
{
	draw::screenshoot surface;
	const int width = 230;
	const int height = 260;
	auto market_count = game::getbuildings(player, MarketPlace);
	if(!market_count)
		return;
	auto icn = draw::isevil(res::TRADPOSE, res::TRADPOST);
	auto x = (draw::width - width) / 2;
	auto resf = Wood;
	auto rest = Gold;
	auto ratf = 1;
	auto ratt = 1;
	auto resources = bsptr(player, FirstResource);
	char temp[260];
	while(true)
	{
		surface.restore();
		int y1 = draw::dialog(260);
		int y = y1;
		draw::text(x, y - 20, width, draw::Center, game::getbuildingname(Knight, MarketPlace, 1));
		int rate = game::gettrade(resf, rest, market_count);
		if(rest == Gold)
		{
			szprint(temp, szt(
				"On my market for 1 %1 you get %3i %2.",
				"Ќа моем рынке за 1 ед. %1 можно получить %3i ед. %2."),
				bsgets(resf, Name), bsgets(rest, Name), rate);
		}
		else
		{
			szprint(temp, szt(
				"On my market for %3i %1 you get 1 %2.",
				"Ќа моем рынке за %3i ед. %1 можно получить 1 ед. %2."),
				bsgets(resf, Name), bsgets(rest, Name), rate);
		}
		draw::textf(x, y, width, temp);
		paint_resources(x, y + 120, icn, resources, resf, Empthy, 0);
		paint_resources(x+120, y + 120, icn, resources, rest, resf, market_count);
		draw::button(x + (width - res::width(icn, 17)) / 2, y1 + height - res::height(icn, 17), icn, Cancel, 17, 17, 18, KeyEscape);
		draw::cursor(res::ADVMCO, 0);
		int id = draw::input();
		switch(id)
		{
		case 0:
		case Cancel:
			return;
		case SourceIndex:
			resf = (tokens)hot::param;
			break;
		case TargetIndex:
			rest = (tokens)hot::param;
			break;
		default:
			draw::definput(id);
			break;
		}
	}
}