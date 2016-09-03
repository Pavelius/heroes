#include "main.h"

static int		selected_object;
static int		information_mode;
static int      cashe_index;

static void handle_input(int x, int y, tokens object)
{
	if(object && tokens(selected_object) == object)
		draw::rectb(x, y, x + 54, y + 31, 0x56);
	if(draw::area(x, y, x + 54, y + 31))
	{
		//hot::object = object;
		if(hot::key == MouseLeft && hot::pressed)
			draw::execute(InputChoose);
		else if(hot::key == MouseLeftDBL)
			draw::execute(Change);
	}
}

static struct castles_list : public list
{

	bool	need_update;
	int		data[LastCastle - FirstCastle + 1];

	void prerender()
	{
		if(!need_update)
			return;
		maximum = 0;
		origin = 0;
		memset(data, 0, sizeof(data));
		tokens player = (tokens)game::getplayer();
		if(player==Empthy)
			return;
		bsselect(data, FirstCastle, -1, Player, player);
		maximum = zlen(data);
		need_update = false;
	}

	void row(int x, int y, int index) const
	{
		if(data[index])
		{
			int rec = data[index];
			int index_sprite = 1;
			bool iscastle = bsget(rec, CastleInTown) != 0;
			res::tokens icn = draw::isevil(res::LOCATORE, res::LOCATORS);
			switch(bsget(rec, Type))
			{
			case Knight:
				index_sprite = iscastle ? 9 : 15;
				break;
			case Barbarian:
				index_sprite = iscastle ? 10 : 16;
				break;
			case Sorcerer:
				index_sprite = iscastle ? 11 : 17;
				break;
			case Warlock:
				index_sprite = iscastle ? 12 : 18;
				break;
			case Wizard:
				index_sprite = iscastle ? 13 : 19;
				break;
			case Necromancer:
				index_sprite = iscastle ? 14 : 20;
				break;
			}
			draw::image(x - 1, y, icn, index_sprite);
			if(bsget(rec, BuildThisTurn))
				draw::image(x - 1, y + 1, icn, 24);
		}
		else
			draw::image(x - 1, y, draw::isevil(res::LOCATORE, res::LOCATORS), 1 + index);
		handle_input(x, y, tokens(data[index]));
	}

} castles;

static struct heroes_list : public list
{

	bool	need_update;
	int		data[LastHero - FirstHero + 1];

	void prerender()
	{
		if(!need_update)
			return;
		maximum = 0;
		origin = 0;
		memset(data, 0, sizeof(data));
		tokens player = (tokens)game::getplayer();
		if(player == Empthy)
			return;
		bsselect(data, FirstHero, -1, Player, player);
		maximum = zlen(data);
		need_update = false;
	}

	void row(int x, int y, int index) const
	{
		if(data[index])
		{
			draw::image(x + 4, y + 5, res::PORTXTRA, 0);
			draw::image(x - 1, y, res::MINIPORT, bsget(data[index], Portrait));
			draw::image(x + 4, y + 5, res::MOBILITY, imin(bsget(data[index], MovePoints) / 100, 25));
			draw::image(x + 43, y + 5, res::MANA, imin(bsget(data[index], SpellPoints) / 5, 25));
			if(selected_object == data[index])
				draw::rectb(x, y, x + 54, y + 31, 0x56);
			if(draw::area(x, y, x + 54, y + 31))
			{
				//hot::object = tokens(data[index]);
				if(hot::key == MouseLeft && hot::pressed)
					draw::execute(InputChoose);
				else if(hot::key == MouseLeftDBL)
					draw::execute(Change);
			}
		}
		else
			draw::image(x - 1, y, draw::isevil(res::LOCATORE, res::LOCATORS), 1 + index);
	}

} heroes;

static void minimap(int x, int y, int mode)
{
	if(!mode)
		draw::image(x, y, draw::isevil(res::HEROLOGE, res::HEROLOGO), 0, 0);
	else
	{
	}
}

static void information_resource(int x, int y, int id)
{
	char temp[16];
	auto cost = (int*)bsptr(game::getplayer(), FirstResource);
	if(!cost)
		return;
	sznum(temp, cost[id-FirstResource]);
	draw::text(x - draw::textw(temp) / 2, y, temp);
}

static void information(int x, int y)
{
	char temp[256];
	if(!information_mode)
		draw::image(x, y, draw::isevil(res::HEROLOGE, res::HEROLOGO), 0, 0);
	else
	{
		res::tokens icn = draw::isevil(res::ADVEBTNS, res::ADVBTNS);
		draw::button(x + 0 * 36, y + 0 * 36, icn, 1, 0, 0, 1, Alpha + 'H', 0, szt("Next hero", "Следующий герой"));
		draw::button(x + 1 * 36, y + 0 * 36, icn, Move, 2, 2, 3, Alpha + 'M', 0, szt("Continue moving", "Продолжить движение"));
		draw::button(x + 2 * 36, y + 0 * 36, icn, 1, 4, 4, 5);
		draw::button(x + 3 * 36, y + 0 * 36, icn, 1, 6, 6, 7);
		draw::button(x + 0 * 36, y + 1 * 36, icn, EndTurn, 8, 8, 9, Alpha + 'E', 0, szt("End turn", "Закончить ход"));
		draw::button(x + 1 * 36, y + 1 * 36, icn, 1, 10, 10, 11);
		draw::button(x + 2 * 36, y + 1 * 36, icn, 1, 12, 12, 13);
		draw::button(x + 3 * 36, y + 1 * 36, icn, 1, 14, 14, 15);
		y += 2 * 36;
		draw::image(x, y, draw::isevil(res::STONBAKE, res::STONBACK), 0);
		if(draw::area(x, y, x + 142, y + 4 * 36))
		{
			if(hot::key == MouseLeft && hot::pressed)
				draw::execute(ChangeMode);
		}
		switch(information_mode)
		{
		case Resource:
			if(true)
			{
				draw::state push;
				draw::font = res::SMALFONT;
				draw::image(x, y, res::RESSMALL, 0);
				draw::text(x + 26, y + 31, "1"); // castle
				draw::text(x + 78, y + 31, "0"); // town
				information_resource(x + 122, y + 31, Gold);
				information_resource(x + 14, y + 61, Wood);
				information_resource(x + 35, y + 61, Mercury);
				information_resource(x + 59, y + 61, Ore);
				information_resource(x + 82, y + 61, Sulfur);
				information_resource(x + 106, y + 61, Crystal);
				information_resource(x + 128, y + 61, Gems);
			}
			break;
		case Hero:
			//army(x + 4, y + 20, 132, world::hero->garmy()->units, 5, false);
			break;
		case Information:
			draw::image(x, y, draw::isevil(res::SUNMOONE, res::SUNMOON), 3 - ((game::getweek() - 1) % 4) + 1);
			if(true)
			{
				draw::state push;
				draw::font = res::SMALFONT;
				szprint(temp, "%1: %3i, %2: %4i",
					szt("Month", "Месяц"), szt("Week", "Неделя"),
					game::getmonth(), game::getweek());
				draw::textm(x, y + 34, 140, draw::Center, temp);
			}
			szprint(temp, "%1: %2i",
				szt("Day", "День"),
				game::getday());
			draw::textm(x, y + 50, 140, draw::Center, temp);
			break;
		}
	}
}

static void hero_load(int* rec, int index, int w, int h)
{
	int x1 = map::i2x(index);
	int y1 = map::i2y(index);
	int x2 = x1 + w;
	int y2 = y1 + h;
	for(int e = FirstHero; e <= (int)LastHero; e++)
	{
		int pos = bsget(e, Index);
		if(pos == -1)
			continue;
		int x = map::i2x(pos);
		int y = map::i2y(pos);
		if(x >= x1 && x <= x2 && y >= y1 && y <= y2)
		{
			int j = (y - y1)*w + (x - x1);
			rec[j] = e;
		}
	}
}

static void cursor_adventure(int x, int y, int index, int* objects)
{
	if(hot::command)
		return;
	animation m(res::ADVMCO, 0, 0);
	if(draw::area(x, y, x + 32 * map::viewx, y + 32 * map::viewx))
	{
		int i1 = index + ((hot::mouse.y - y) / 32)*(map::width) + ((hot::mouse.x - x) / 32);
		int i2 = ((hot::mouse.y - y) / 32)*(map::viewx + 1) + ((hot::mouse.x - x) / 32);
		int id = objects[i2];
		int mc = map::movecost(i1);
		//        int xp = x+((hot::mouse.x-x)/32)*32;
		//        int yp = y+((hot::mouse.y-y)/32)*32;
		//        draw::rectb(xp, yp, xp+32, yp+32, 0xC4);
		switch(mc)
		{
		case map::Blocked:
			// Non passable area. Mountains, Tree and like this.
			// Use default cursor
			break;
		case map::Attack:
			// Monster area
			m.set(CursorAdventure, Attack, 0);
			if(hot::key == MouseLeft && hot::pressed)
			{
				if(bsget(selected_object, First) == FirstHero)
					draw::execute(MoveTo, i1);
			}
			break;
		case map::Action:
			// Action able area. Chest, resource, gazebo, mine and like this
			m.set(CursorAdventure, MakeAction, 0);
			if(hot::key == MouseLeft && hot::pressed)
			{
				if(bsget(selected_object, First) == FirstHero)
					draw::execute(MoveTo, i1);
			}
			break;
		default:
			if(mc != 0)
			{
				// Only reachable targets
				// If targets pleced in area where you can't go by obstacle - we call it temporary unreachable
				m.set(CursorAdventure, MoveTo, 0);
				if(hot::key == MouseLeft && hot::pressed)
				{
					if(bsget(selected_object, First) == FirstHero)
						draw::execute(MoveTo, i1);
				}
			}
			break;
		}
		if(id >= (int)FirstHero && id <= (int)LastHero)
		{
			if(bsget(id, Player) == game::getplayer())
				m.set(CursorAdventure, Hero, 0);
		}
	}
	m.painting(hot::mouse);
}

int show::game()
{
	int objects[(map::viewx + 1)*map::viewy];
	int route[(map::viewx + 1)*map::viewy];
	int selected_wave = -1;
	heroes.need_update = true;
	castles.need_update = true;
	if(!information_mode)
		information_mode = Information;
	selected_object = bsfind(FirstHero, Player, game::getplayer());
	if(selected_object == -1)
		selected_object = bsfind(FirstCastle, Player, game::getplayer());
	int index = bsget(selected_object, Index);
	if(index > 0)
		map::jumpto(index);
	while(true)
	{
		if(bsget(selected_object, First) == FirstHero && selected_wave != selected_object)
		{
			map::route::wave(bsget(selected_object, Index),
				bsget(selected_object, SkillPathfinding),
				0);
			if(bsget(selected_object, MoveTo) != -1)
				map::route::path(bsget(selected_object, Index),
					bsget(selected_object, MoveTo));
			selected_wave = selected_object;
			cashe_index = -1;
		}
		if(cashe_index != map::view())
		{
			cashe_index = map::view();
			memset(objects, 0, sizeof(objects));
			memset(route, -1, sizeof(route));
			map::moveable::load(objects, cashe_index, map::viewx + 1, map::viewy);
			hero_load(objects, cashe_index, map::viewx + 1, map::viewy);
			map::route::load(route, cashe_index, map::viewx + 1, map::viewy);
		}
		hot::index = -1;
		//hot::object = Empthy;
		draw::map(16, 16, objects, route);
		draw::image(0, 0, draw::isevil(res::ADVBORDE, res::ADVBORD), 0, 0);
		minimap(480, 16, 0);
		heroes.draw(481, 176, 32, 32, 4);
		castles.draw(553, 176, 32, 32, 4);
		information(480, 320);
		if(hot::key == KeyEnter)
			draw::execute(Change);
		cursor_adventure(16, 16, cashe_index, objects);
		int id = draw::input();
		switch(id)
		{
		case 0:
		case Cancel:
			return 0;
		case Move:
			hot::param = bsget(selected_object, MoveTo);
		case MoveTo:
			if(bsget(selected_object, MoveTo) == hot::param)
			{
				bsset(selected_object, MoveTo, hot::param);
				map::route::move(selected_object, 0);
			}
			else
				bsset(selected_object, MoveTo, hot::param);
			selected_wave = -1;
			break;
		case InputChoose:
			//selected_object = tokens(hot::object);
			//index = bsget(selected_object, Index);
			//if(index>0)
			//    map::jumpto(index);
			break;
		case Change:
			switch(bsget(selected_object, First))
			{
			case FirstHero:
				show::hero(tokens(selected_object));
				break;
			case FirstCastle:
				show::castle(selected_object);
				break;
			default:
				break;
			}
			break;
		case ChangeMode:
			switch(information_mode)
			{
			case Resource:
				information_mode = Hero;
				break;
			case Hero:
				information_mode = Information;
				break;
			default:
				information_mode = Resource;
				break;
			}
			break;
		case KeyRight:
			map::slide(map::Right);
			break;
		case KeyDown:
			map::slide(map::Down);
			break;
		case KeyUp:
			map::slide(map::Up);
			break;
		case KeyLeft:
			map::slide(map::Left);
			break;
		case EndTurn:
			return EndTurn;
		default:
			break;
		}
	}
}