#include "main.h"

static void paint_resource(int x, int y, res::tokens icn, int frame, tokens id, const void* resources_void, tokens select, tokens resf, int markets) {
	char temp[32];
	auto resources = (int*)resources_void;
	draw::image(x, y, icn, frame);
	if(markets == 0) {
		int value = resources[id - FirstResource];
		sznum(temp, value);
	} else {
		int value = game::gettrade(resf, id, markets);
		if(value)
			szprint(temp, "1/%1i", value);
		else
			szprint(temp, szt("n/a", "н/д"));
	}
	draw::text(x + (34 - draw::textw(temp)) / 2, y + 23, temp);
	if(select == id)
		draw::image(x - 2, y - 2, icn, 14);
	if(draw::mousein(x - 2, y - 2, x + 37, y + 37)) {
		if(hot::key == MouseLeft && hot::pressed)
			draw::execute(markets ? TargetIndex : SourceIndex, id);
	}
}

static void paint_resources(int x, int y, res::tokens icn, const void* resources, tokens select, tokens resf, int markets) {
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

void show::marketplace(int player) {
	draw::screenshoot surface;
	const int width = 230;
	const int height = 280;
	auto market_count = game::getbuildings(player, MarketPlace);
	if(!market_count)
		return;
	auto icn = draw::isevil(res::TRADPOSE, res::TRADPOST);
	auto x = (draw::width - width) / 2;
	auto resf = Wood;
	auto rest = Gold;
	auto ratf = 1;
	auto ratt = 1;
	auto count = 1;
	auto resources = (int*)bsptr(player, FirstResource);
	char temp[260];
	while(true) {
		surface.restore();
		int y = draw::dialog(height);
		int rate = game::gettrade(resf, rest, market_count);
		if(rate) {
			if(rest == Gold) {
				szprint(temp, szt(
					"For one %1 you can trade %3i %2.",
					"За один ресурс %1 можно выторговать %3i ресурса %2."),
					bsgets(resf, Name), bsgets(rest, Name), rate);
			} else {
				szprint(temp, szt(
					"For %3i units of %1 you can trade one %2.",
					"За %3i ресурсов %1 можно выторговать один ресурс %2."),
					bsgets(resf, Name), bsgets(rest, Name), rate);
			}
			draw::textf(x, y, width, temp);
			int x2 = x + (width / 3) / 2;
			int x3 = x + (width / 3) * 2 + (width / 3) / 2;
			int maximum;
			if(rest == Gold)
				maximum = resources[resf - FirstResource];
			else
				maximum = resources[resf - FirstResource] / rate;
			draw::splitter(x - 4, y + 86, MarketPlace, icn, 0, maximum, count);
			draw::image(x + width / 2, y + 86 - 32, icn, 0, AFCenter);
			draw::clipart(x2, y + 41, resf, rest == Gold ? count : count * rate);
			draw::clipart(x3, y + 41, rest, rest == Gold ? count * rate : count);
			if(true) {
				draw::state push;
				draw::font = res::SMALFONT;
				draw::text(x, y + 86 - draw::texth() - 2, width, draw::Center, szt("Count", "Количество"));
			}
			draw::button(x + (width - res::width(icn, 15)) / 2, y + 110, icn, MarketPlace, 15, 15, 16, KeyEnter);
		} else
			draw::textf(x, y, width, szt("This exchange is not supported.", "Этот обмен не возможен."));
		paint_resources(x, y + 140, icn, resources, resf, Empthy, 0);
		paint_resources(x + 120, y + 140, icn, resources, rest, resf, market_count);
		draw::button(x + (width - res::width(icn, 17)) / 2, y + height - res::height(icn, 17), icn, Cancel, 17, 17, 18, KeyEscape);
		draw::cursor(res::ADVMCO, 0);
		int id = draw::input();
		switch(id) {
		case Cancel:
			return;
		case SourceIndex:
			resf = (tokens)hot::param;
			break;
		case TargetIndex:
			rest = (tokens)hot::param;
			break;
		case MarketPlace:
			if(hot::param == KeyLeft)
				count--;
			else if(hot::param == KeyRight)
				count++;
			else if(hot::param == 0) {
				game::trade(player, resf, rest, count);
				return;
			}
			break;
		default:
			draw::definput(id);
			break;
		}
	}
}