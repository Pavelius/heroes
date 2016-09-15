#include "main.h"

static void statelack(const void* c1, const void* c2)
{
	char temp[260];
	szprint(temp, "%1 ", szt("Lack", "Не хватает"));
	auto p = zend(temp);
	auto c1i = (int*)c1;
	auto c2i = (int*)c2;
	for(int i = FirstResource; i <= LastResource; i++)
	{
		int d = c1i[i - FirstResource] - c2i[i - FirstResource];
		if(d <= 0)
			continue;
		if(p[0])
			zcat(p, ", ");
		szprint(zend(p), "%1i %2", d, bsgets(i, Name));
	}
	draw::status(temp);
}

static void building_control(int x, int y, int w, int h, int building, int rec, bool blank)
{
	auto race = bsget(rec, Type);
	auto player = bsget(rec, Player);
	auto c2 = (int*)bsptr(player, FirstResource);
	auto ab = bsget(rec, AlreadyMoved);
	bool hilite = draw::area(x, y, x + 132, y + 64);
	auto max_level = game::getbuildingmaxlevel(race, building);
	auto level = bsget(rec, building);
	auto next_level = imin(level+1, max_level);
	auto c1 = game::getbuildingcost(race, building, next_level);
	const char* name = game::getbuildingname(race, building, next_level);
	if(blank)
		draw::image(x + 1, y + 1, res::buildings(race), indexes::buildings(building, next_level));
	if(hilite && hot::key == MouseRight && hot::pressed)
	{
		hot::level = next_level;
		draw::execute(Information, building, race);
	}
	if(level >= max_level)
	{
		draw::image(x + w, y + h, res::TOWNWIND, 11);
		if(hilite)
			draw::status(szt("%1 is already build", "%1 уже построено"), name);
	}
	else if(ab)
	{
		draw::image(x + w, y + h, res::TOWNWIND, 12);
		if(blank)
			draw::image(x, y + h + 18, res::CASLXTRA, 2);
	}
	else if(!game::passrequipment(rec, building, next_level))
	{
		draw::image(x + w, y + h, res::TOWNWIND, 12);
		if(blank)
			draw::image(x, y + h + 18, res::CASLXTRA, 2);
	}
	else if(game::ismatch(c2, c1))
	{
		if(blank)
			draw::image(x, y + h + 18, res::CASLXTRA, 1);
		if(hilite)
		{
			draw::status(szt("Build %1", "Построить %1"), name);
			if(hot::key == MouseLeft && hot::pressed)
				draw::execute(building);
		}
	}
	else
	{
		// lack resurces
		draw::image(x + w, y + h, res::TOWNWIND, 13);
		if(blank)
			draw::image(x, y + h + 18, res::CASLXTRA, 2);
		if(hilite)
			statelack(c1, c2);
	}
	if(blank)
	{
		draw::state push;
		draw::font = res::SMALFONT;
		draw::text(x + (132 - draw::textw(name)) / 2, y + h + 21, name);
	}
}

static void building(int x, int y, int building, int rec)
{
	building_control(x, y, 115, 40, building, rec, true);
}

static res::tokens captainicn(int race)
{
	switch(race)
	{
	case Barbarian: return res::CSTLCAPB;
	case Knight: return res::CSTLCAPK;
	case Necromancer: return res::CSTLCAPN;
	case Sorcerer: return res::CSTLCAPS;
	case Warlock: return res::CSTLCAPW;
	case Wizard: return res::CSTLCAPZ;
	default: return res::Empthy;
	}
}

static tokens race2captain(tokens race)
{
	switch(race)
	{
	case Barbarian: return BarbarianCaptain;
	case Warlock: return WarlockCaptain;
	case Wizard: return WizardCaptain;
	case Sorcerer: return SorcererCaptain;
	case Necromancer: return NecromancerCaptain;
	default: return KnightCaptain;
	}
}

static void captain(int x, int y, int rec)
{
	auto race = (tokens)bsget(rec, Type);
	auto present = bsget(rec, Captain) != 0;
	auto already_moved = bsget(rec, AlreadyMoved) != 0;
	res::tokens icn = captainicn(race);
	int sx = res::width(icn, 0);
	int sy = res::height(icn, 0);
	if(!present)
	{
		draw::image(x + 85, y - 4, res::CAPTCOVR, 0);
		draw::image(x, y, icn, 0);
		building_control(x, y, 62, 58, Captain, rec, false);
	}
	else
	{
		draw::state push;
		draw::font = res::SMALFONT;
		int x1 = x + 85;
		int y1 = y + 16;
		for(int i = Attack; i <= Wisdow; i++)
		{
			char temp[32];
			zcpy(temp, bsgets(i, Name));
			zcat(temp, ":");
			draw::text(x1 + 80 - draw::textw(temp), y1, temp);
			sznum(temp, game::get(race2captain(race), i));
			draw::text(x1 + 84, y1, temp);
			y1 += draw::texth() + 2;
		}
		draw::image(x, y, icn, 1);
	}
}

static void heroport(int x, int y, int player, int index, int rec)
{
	if(rec == -1)
		return;
	auto c1 = bsptr(player, FirstResource);
	auto c2 = game::gethirecost(rec);
	draw::clipart(x, y, rec, LargeSize);
	rect rc = {x - 50, y, x + 50, y + res::width(res::PORT0000, 0)};
	if(bsfind(FirstHero, Index, index))
	{
		draw::image(rc.x2 - 16, rc.y2 - 24, res::TOWNWIND, 12);
		if(hot::mouse.in(rc))
			draw::status(szt("Cannot hire another hero in town", "Нельзя нанять героя, когда друго герой в городе"));
	}
	else if(!game::ismatch(c1, c2))
	{
		draw::image(rc.x2 - 16, rc.y2 - 24, res::TOWNWIND, 13);
		if(hot::mouse.in(rc))
			statelack(c1, c2);
	}
	else
	{
		if(hot::mouse.in(rc))
		{
			draw::status(szt("Hire %1", "Нанять %1"), (int)bsget(rec, Name));
			if(hot::key == MouseLeft && hot::pressed)
				draw::execute(rec);
		}
	}
}

int show::build(int mid)
{
	auto race = tokens(bsget(mid, Type));
	auto player = bsget(mid, Player);
	auto index = bsget(mid, Index);
	auto h1 = bsget(player, Recruit);
	auto h2 = bsget(player, RecruitLast);
	char temp[260];
	while(true)
	{
		bool already_moved = bsget(mid, AlreadyMoved) != 0;
		draw::status(21, draw::height - 16, 21 + res::width(res::SMALLBAR, 0), draw::height - 1);
		draw::image(0, 0, res::CASLWIND, 0);
		// hide captain options
		draw::castle(460, 5, Dirt, race, false);
		draw::text(536, 2, bsgets(mid, Name), 0);
		draw::resource(552, 262, bsptr(player, FirstResource));
		//
		building(5, 2, Dwelving1, mid);
		building(149, 2, Dwelving2, mid);
		building(293, 2, Dwelving3, mid);
		building(5, 77, Dwelving4, mid);
		building(149, 77, Dwelving5, mid);
		building(293, 77, Dwelving6, mid);
		//
		building(5, 157, MageGuild, mid);
		if(race==Necromancer)
			draw::image(145, 157, res::STONBACK, 0);
		else
			building(149, 157, Tavern, mid);
		building(293, 157, ThievesGuild, mid);
		building(5, 232, Shipyard, mid);
		building(149, 232, Statue, mid);
		building(293, 232, MarketPlace, mid);
		building(5, 307, Well, mid);
		building(149, 307, Well2, mid);
		building(293, 307, SpecialBuilding, mid);
		building(5, 387, LeftTurret, mid);
		building(149, 387, RightTurret, mid);
		building(293, 387, Moat, mid);
		captain(444, 165, mid);
		//
		heroport(443 + 50, 260, player, index, h1);
		heroport(443 + 50, 362, player, index, h2);
		//
		draw::button(553, 428, res::SWAPBTN, Cancel, 0, 0, 1, KeyEscape, 0, szt("Back to castle view", "Назад к обзору замка"));
		draw::image(0, 461, res::CASLBAR, 0);
		//
		draw::cursor(res::ADVMCO, 0);
		int id = draw::input();
		switch(id)
		{
		case Cancel:
			return Cancel;
		case Information:
			if(hot::param >= FirstBuilding && hot::param <= LastBuilding)
			{
				auto e2 = game::getbuildingcost(race, hot::param, hot::level);
				game::getbuilding(temp, race, hot::param, hot::level);
				char* p = zend(temp);
				for(int i = ThievesGuild; i <= MageGuild; i++)
				{
					if(!game::isrequipment(race, hot::param, bsget(mid, hot::param), i, bsget(mid, i)))
						continue;
					if(bsget(mid, i))
						continue;
					if(!p[0])
						szprint(p, "\n%1: ", szt("Need", "Требует"));
					else
						zcat(p, ", ");
					zcat(p, game::getbuildingname(race, i, 0));
				}
				if(p[0])
					zcat(p, ".");
				game::addicon(zend(temp), e2);
				show::tips(temp);
			}
			break;
		default:
			if(id == Captain || (id >= FirstBuilding && id <= LastBuilding))
			{
				auto level = bsget(mid, id) + 1;
				auto e2 = game::getbuildingcost(race, id, level);
				game::getbuilding(temp, race, id, level);
				zcat(temp, " ");
				zcat(temp, szt("Do you want to build?", "Хотите построить?"));
				game::addicon(zend(temp), e2);
				if(dlgask(0, temp))
					return id;
			}
			else if(id >= FirstHero && id <= LastHero)
			{
				char temp[260];
				int player = bsget(mid, Player);
				auto cost = game::gethirecost(id);
				szprint(temp, "$(%1i)\n", id);
				szprint(zend(temp), szt("%1 %3i-level %2 ", "%1 %3i-уровня %2 "),
					bsgets(id, Name),
					bsgets(game::get(id, Type), Name),
					bsget(id, Level));
				if(bsget(id, ArtifactCount))
					szprint(zend(temp), szt(" with %1i artifacts", " с %1i артифактами"), bsget(id, ArtifactCount));
				szprint(zend(temp),
					szt("can work for you. Did you want to hire?", "может работать на вас. Хотите нанять?"));
				game::addicon(zend(temp), cost);
				if(dlgask(0, temp))
					return id;
			}
			else
				draw::definput(id);
			break;
		}
	}
}