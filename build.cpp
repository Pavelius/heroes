#include "main.h"

static bool meet_requipment(int rec, int building)
{
	int race = bsget(rec, Type);
	for(int i = ThievesGuild; i<=(int)Dwelving6; i++)
	{
		if(buildings::requipment(race, building, i))
		{
			if(!bsget(rec, i))
				return false;
		}
	}
	int level = 0;
	if(buildings::requipment(race, building, MageGuild, 1))
		level = 1;
	else if(buildings::requipment(race, building, MageGuild, 2))
		level = 2;
	if(bsget(rec, MageGuild)<level)
		return false;
	return true;
}

static void building(int x, int y, int building, int rec)
{
	int race = bsget(rec, Type);
	int player = bsget(rec, Player);
    res::tokens ic1 = res::buildings(race);
	int id1 = indexes::buildings(building, 0);
	draw::image(x+1, y+1, ic1, id1);
	cost& c1 = buildings::gcost(race, building, 0);
	cost& c2 = player::gcost(player);
	bool hilite = hot::mouse.x>=x && hot::mouse.y>=y && hot::mouse.x<=x+132 && hot::mouse.y<=y+64;
	const char* name = bsgets(rec, building);
    if(hilite && hot::key==MouseRight && hot::pressed)
        draw::execute(Tooltips, building);
	if(bsget(rec, building))
	{
		draw::image(x+115, y+40, res::TOWNWIND, 11);
		if(hilite)
			draw::status(szt("%1 is already build", "%1 уже построено"), (int)name);
	}
	else if(bsget(rec, BuildThisTurn))
	{
		draw::image(x+115, y+40, res::TOWNWIND, 12);
		draw::image(x, y+58, res::CASLXTRA, 2);
	}
	else if(!meet_requipment(rec, building))
	{
		draw::image(x+115, y+40, res::TOWNWIND, 12);
		draw::image(x, y+58, res::CASLXTRA, 2);
	}
	else if(c1<=c2)
	{
		draw::image(x, y+58, res::CASLXTRA, 1);
		if(hilite)
		{
			draw::status(szt("Build %1", "Построить %1"), (int)name);
			if(hot::key==MouseLeft && hot::pressed)
				draw::execute(building);
		}
	}
	else
	{
		// lack resurces
		draw::image(x+115, y+40, res::TOWNWIND, 13);
		draw::image(x, y+58, res::CASLXTRA, 2);
		if(hilite)
		{
			char temp[260];
			szprint(temp, "%1 ", szt("Lack", "Не хватает"));
			int count = 0;
			for(int i = Wood; i<=(int)Gold; i++)
			{
				int d = c1.get(i) - c2.get(i);
				if(d<=0)
					continue;
				if(count++)
					zcat(temp, ", ");
				szprint(zend(temp), "%1i %2", d, bsget(i, Name));
			}
			draw::status(temp);
		}
	}
	draw::fontsm push;
	draw::text(x+(132-draw::textw(name))/2, y+61, name);
}

static bool build_structure(int mid, int building, cost& e, bool tips = false)
{
	if(!tips && bsget(mid, building))
		return false;
    char temp[260];
    szprint(temp, "%%b%1i(%2i)", building-FirstBuilding, mid);
    const char* p = bsgets(mid, Information+building-CastleInTown);
    if(p)
        zcat(temp, p);
    e.tostring(zend(temp));
    if(!tips)
        return show::question(temp);
    show::tips(temp);
    return false;
}

static bool hire_hero(int rec, int player, const cost* e)
{
    char temp[260];
    zcpy(temp, bsgets(rec, Code));
    szprint(zend(temp), szt("%1 %3i-level %2 ", "%1 %3i-уровня %2 "),
            bsgets(rec, Name),
            bsget(bsget(rec,Type), Name),
            bsget(rec,Level));
    if(bsget(rec, ArtifactCount))
        szprint(zend(temp), szt(" with %1i artifacts", " с %1i артифактами"), bsget(rec, ArtifactCount));
    zcat(temp, szt("can work for you. Did you want to hire?","может работать на вас. Хотите его нанять?"));
    e->tostring(zend(temp));
    return show::question(temp);
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
    case Barbarian:
        return BarbarianCaptain;
    case Warlock:
        return WarlockCaptain;
    case Wizard:
        return WizardCaptain;
    case Sorcerer:
        return SorcererCaptain;
    case Necromancer:
        return NecromancerCaptain;
    default:
        return KnightCaptain;
    }
}

static void captain(int x, int y, tokens race, bool present)
{
	res::tokens icn = captainicn(race);
	int sx = res::width(icn, 0);
	int sy = res::height(icn, 0);
	if(!present)
        draw::image(x+85, y-4, res::CAPTCOVR, 0);
    else
    {
        draw::fontsm push;
        int x1 = x + 85;
        int y1 = y + 16;
        for(int i=Attack; i<=(int)Wisdow; i++)
        {
            char temp[32];
            zcpy(temp, bsgets(i,Name));
            zcat(temp, ":");
            draw::text(x1 + 80 - draw::textw(temp), y1, temp);
            sznum(temp, bsget(race2captain(race), i));
            draw::text(x1 + 84, y1, temp);
            y1 += draw::texth() + 2;
        }
    }
	draw::image(x, y, icn, present ? 1 : 0);
	if(hot::mouse.x>=x && hot::mouse.y>=y && hot::mouse.x<x+sx && hot::mouse.y<y+sy)
	{
		if(!present)
			draw::status(szt("Build captain quarter", "Построить коморку капитана"));
		if(hot::key==MouseLeft && hot::pressed)
			draw::execute(Captain);
	}
}

static void heroport(int x, int y, int rec)
{
	if(rec==-1)
		return;
    draw::clipart(x, y, rec, LargeSize);
    rect rc = {x-50,y,x+50,y+res::width(res::PORT0000,0)};
    if(hot::mouse.in(rc))
    {
        draw::status(szt("Hire %1", "Нанять %1"), (int)bsget(rec, Name));
        if(hot::key==MouseLeft && hot::pressed)
            draw::execute(rec);
    }
}

void show::build(int mid)
{
	int player = bsget(mid, Player);
	tokens race = tokens(bsget(mid, Type));
	int h1 = bsfind(FirstHero, Recruit, player);
	int h2 = h1!=-1?bsfind(h1+1, Recruit, player):-1;
	while(true)
	{
		draw::status(21, draw::height-16, 21+res::width(res::SMALLBAR, 0), draw::height-1);
		draw::image(0, 0, res::CASLWIND, 0);
		// hide captain options
		draw::castle(460, 5, Grass, race, false);
		draw::text(536, 2, bsgets(mid, Name), 0);
		draw::resource(552, 262, player::gcost(player));
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
		captain(444, 165, race, bsget(mid, Captain)!=0);
		//
		heroport(443+50, 260, h1);
		heroport(443+50, 362, h2);
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
        case Tooltips:
            if(hot::param)
            {
				cost& e2 = buildings::gcost(race, hot::param, 0);
				build_structure(mid, hot::param, e2, true);
            }
            break;
		default:
			if(id==Captain || (id>=(int)FirstBuilding && id<=(int)LastBuilding))
			{
				cost& e2 = buildings::gcost(race, id, 0);
				if(build_structure(mid, id, e2))
                {
					int p = bsget(mid, Player);
					cost& e1 = player::gcost(p);
					e1 -= e2;
                    bsset(mid, BuildThisTurn, 1);
                    bsset(mid, id, 1);
					return;
                }
			}
			else if(id>=(int)FirstHero && id<=(int)LastHero)
			{
				if(hire_hero(id, bsget(mid,Player), game::getcost(id)))
					return;
			}
			break;
		}
	}
}