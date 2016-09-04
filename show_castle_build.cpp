#include "main.h"

static bool meet_requipment(int rec, int building)
{
	int race = bsget(rec, Type);
	int level = bsget(rec, building);
	for(int i = ThievesGuild; i <= (int)Dwelving6; i++)
	{
		if(game::isrequipment(race, building, level, i, bsget(rec, i)))
		{
			if(!bsget(rec, i))
				return false;
		}
	}
	return true;
}

static void house(int x, int y, int w, int h, int building, int rec, bool blank)
{
	auto race = bsget(rec, Type);
	auto player = bsget(rec, Player);
	auto level = bsget(rec, building);
	auto c1 = game::getcost(race, building);
	auto c2 = (int*)bsptr(player, FirstResource);
	auto ab = bsget(rec, AlreadyMoved);
	bool hilite = draw::area(x, y, x + 132, y + 64);
	const char* name = game::getbuildingname(race, building, level);
	if(blank)
		draw::image(x + 1, y + 1, res::buildings(race), indexes::buildings(building, 0));
	if(hilite && hot::key == MouseRight && hot::pressed)
		draw::execute(Information, building, race);
	if(level >= 1)
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
	else if(!meet_requipment(rec, building))
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
		{
			char temp[260];
			szprint(temp, "%1 ", szt("Lack", "Не хватает"));
			auto p = zend(temp);
			for(int i = FirstResource; i <= LastResource; i++)
			{
				int d = c1[i - FirstResource] - c2[i - FirstResource];
				if(d <= 0)
					continue;
				if(p[0])
					zcat(p, ", ");
				szprint(zend(p), "%1i %2", d, bsgets(i, Name));
			}
			draw::status(temp);
		}
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
	house(x, y, 115, 40, building, rec, true);
}

static bool hire_hero(int rec, int player, const int* e)
{
	char temp[260];
	zcpy(temp, bsgets(rec, Code));
	szprint(zend(temp), szt("%1 %3i-level %2 ", "%1 %3i-уровня %2 "),
		bsgets(rec, Name),
		bsgets(game::get(rec, Type), Name),
		bsget(rec, Level));
	if(bsget(rec, ArtifactCount))
		szprint(zend(temp), szt(" with %1i artifacts", " с %1i артифактами"), bsget(rec, ArtifactCount));
	szprint(zend(temp),
		szt("can work for you. Did you want to hire?", "может работать на вас. Хотите нанять?"),
		e[Gold - FirstResource]);
	//e->tostring(zend(temp));
	return dlgask(0, temp);
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
	auto already_moved = bsget(rec, AlreadyMoved)!=0;
	res::tokens icn = captainicn(race);
	int sx = res::width(icn, 0);
	int sy = res::height(icn, 0);
	if(!present)
		draw::image(x + 85, y - 4, res::CAPTCOVR, 0);
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
	}
	draw::image(x, y, icn, present ? 1 : 0);
	if(!present)
		house(x, y, 62, 58, Captain, rec, false);
}

static void heroport(int x, int y, int rec)
{
	if(rec == -1)
		return;
	draw::clipart(x, y, rec, LargeSize);
	rect rc = {x - 50, y, x + 50, y + res::width(res::PORT0000, 0)};
	if(hot::mouse.in(rc))
	{
		draw::status(szt("Hire %1", "Нанять %1"), (int)bsget(rec, Name));
		if(hot::key == MouseLeft && hot::pressed)
			draw::execute(rec);
	}
}

void show::build(int mid)
{
	tokens race = tokens(bsget(mid, Type));
	int player = bsget(mid, Player);
	int h1 = bsget(player, Recruit);
	int h2 = bsget(player, RecruitLast);
	char temp[260];
	while(true)
	{
		bool already_moved = bsget(mid, AlreadyMoved) != 0;
		draw::status(21, draw::height - 16, 21 + res::width(res::SMALLBAR, 0), draw::height - 1);
		draw::image(0, 0, res::CASLWIND, 0);
		// hide captain options
		draw::castle(460, 5, Grass, race, false);
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
		heroport(443 + 50, 260, h1);
		heroport(443 + 50, 362, h2);
		//
		draw::button(553, 428, res::SWAPBTN, Cancel, 0, 0, 1, KeyEscape, 0, szt("Back to castle view", "Назад к обзору замка"));
		draw::image(0, 461, res::CASLBAR, 0);
		//
		draw::cursor(res::ADVMCO, 0);
		int id = draw::input();
		switch(id)
		{
		case 0:
		case Cancel:
			return;
		case Information:
			if(hot::param >= FirstBuilding && hot::param <= LastBuilding)
			{
				auto e2 = game::getcost(race, hot::param);
				game::getbuilding(temp, race, hot::param);
				char* p = zend(temp);
				for(int i = ThievesGuild; i <= (int)Dwelving6; i++)
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
				game::getcosttext(zend(temp), e2);
				show::tips(temp);
			}
			break;
		default:
			if(id == Captain || (id >= FirstBuilding && id <= LastBuilding))
			{
				auto e2 = game::getcost(race, id);
				game::getbuilding(temp, race, id);
				zcat(temp, " ");
				zcat(temp, szt("Do you want to build?", "Хотите построить?"));
				game::getcosttext(zend(temp), e2);
				if(dlgask(0, temp))
				{
					game::build(mid, id);
					return;
				}
			}
			else if(id >= FirstHero && id <= LastHero)
			{
				if(hire_hero(id, bsget(mid, Player), game::gethirecost(id)))
					return;
			}
			else
				draw::definput(id);
			break;
		}
	}
}