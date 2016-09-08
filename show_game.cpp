#include "main.h"

static int selected_object;
static int information_mode;
static int cashe_index;

static void handle_input(int x, int y, int object)
{
	if(object && selected_object==object)
		draw::rectb(x, y, x + 54, y + 31, 0x56);
	if(draw::area(x, y, x + 54, y + 31))
	{
		if(hot::key == MouseLeft && hot::pressed)
			draw::execute(InputChoose, object);
		else if(hot::key == MouseLeftDBL)
			draw::execute(Change, object);
	}
}

struct castlelist : public list
{

	int	data[LastCastle - FirstCastle + 1];

	void setup(int player)
	{
		bsselect(data, FirstCastle, -1, Player, player);
		maximum = zlen(data);
	}

	void row(int x, int y, int index) const
	{
		if(index<maximum && data[index])
		{
			int rec = data[index];
			int index_sprite = 1;
			bool iscastle = bsget(rec, Castle) != 0;
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
			if(bsget(rec, AlreadyMoved))
				draw::image(x - 1, y + 1, icn, 24);
			handle_input(x, y, rec);
		}
		else
			draw::image(x - 1, y, draw::isevil(res::LOCATORE, res::LOCATORS), 1 + index);
	}

};

struct herolist : public list
{

	int	data[LastHero - FirstHero + 1];

	void setup(int player)
	{
		bsselect(data, FirstHero, -1, Player, player);
		maximum = zlen(data);
	}

	void row(int x, int y, int index) const
	{
		if(index<maximum && data[index])
		{
			draw::image(x + 4, y + 5, res::PORTXTRA, 0);
			draw::image(x - 1, y, res::MINIPORT, bsget(data[index], Portrait));
			draw::image(x + 4, y + 5, res::MOBILITY, imin(bsget(data[index], MovePoints) / 100, 25));
			draw::image(x + 43, y + 5, res::MANA, imin(bsget(data[index], SpellPoints) / 5, 25));
			handle_input(x, y, data[index]);
		}
		else
			draw::image(x - 1, y, draw::isevil(res::LOCATORE, res::LOCATORS), 1 + index);
	}

};

static void minimap(int x, int y, int mode)
{
	if(!mode)
		draw::image(x, y, draw::isevil(res::HEROLOGE, res::HEROLOGO), 0, 0);
	else
	{
	}
}

static void information_resource(int x, int y, int id, int player)
{
	char temp[16];
	auto cost = (int*)bsptr(player, FirstResource);
	if(!cost)
		return;
	sznum(temp, cost[id-FirstResource]);
	draw::text(x - draw::textw(temp) / 2, y, temp);
}

static void paint_information(int x, int y, int player)
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
				information_resource(x + 122, y + 31, Gold, player);
				information_resource(x + 14, y + 61, Wood, player);
				information_resource(x + 35, y + 61, Mercury, player);
				information_resource(x + 59, y + 61, Ore, player);
				information_resource(x + 82, y + 61, Sulfur, player);
				information_resource(x + 106, y + 61, Crystal, player);
				information_resource(x + 128, y + 61, Gems, player);
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

static void paint_cursor(rect screen, point camera)
{
	if(hot::command)
		return;
	animation m(res::ADVMCO, 0, 0);
	if(draw::area(screen.x1, screen.y1, screen.x2, screen.y2))
	{
		int i1 = map::m2i((camera.x + hot::mouse.x - screen.x1) / 32, (camera.y + hot::mouse.y - screen.y1) / 32);
		int mc = map::movecost(i1);
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
	}
	m.painting(hot::mouse);
}

static void paint_map(rect screen, point camera)
{
	for(int y = screen.y1; y < screen.y2; y += 32)
	{
		for(int x = screen.x1; x < screen.x2; x += 32)
		{
			int index = map::m2i((x+camera.x) / 32, (y + camera.y) / 32);
			draw::imager(x, y, res::TisGROUND32, map::show::tiles[index], map::show::flags[index] % 4);
			//for(auto e : map::show::objects[i])
			//{
			//	if(!e[0])
			//		break;
			//	int index = e[1];
			//	res::tokens res = res::map(e[0]);
			//	if(res::ishight(res, index))
			//		continue;
			//	image(x1, y1, res, index);
			//	index = indexes::animate(res, index, draw::counter + index*index, false);
			//	if(index)
			//		image(x1, y1, res, index);
			//}
		}
	}
	// Object is last layer in render
	//i = map::view();
	//for(int y1 = y; y1 < y2; y1 += dy)
	//{
	//	for(int x1 = x; x1 < x2; x1 += dx)
	//	{
	//		int ticket = i*i;
	//		int ix = map::i2x(i) - ix1;
	//		int iy = map::i2y(i) - iy1;
	//		int j = iy*viewx1 + ix;
	//		int id = objects[j];
	//		if(id)
	//		{
	//			//draw::rectb(x1,y1,x1+32,y1+32, 0xC4);
	//			if(id >= (int)FirstResource && id <= (int)LastResource)
	//			{
	//				draw::image(x1 - 32, y1, res::OBJNRSRC, (id - FirstResource) * 2);
	//				draw::image(x1, y1, res::OBJNRSRC, (id - FirstResource) * 2 + 1);
	//			}
	//			else if(id >= (int)FirstMonster && id <= (int)LastMonster)
	//				animate::monster(x1 + 16, y1 + 30, id, ticket);
	//			else if(id >= (int)FirstArtifact && id <= (int)LastArtifact)
	//				animate::artifact(x1, y1, id, ticket);
	//			else if(id == TreasureChest)
	//			{
	//				draw::image(x1 - 32, y1, res::OBJNRSRC, 18);
	//				draw::image(x1, y1, res::OBJNRSRC, 19);
	//			}
	//			else if(id >= (int)FirstHero && id <= (int)LastHero)
	//			{
	//				animate::heroshad(x1, y1 + 26, id, 0);
	//				animate::hero(x1, y1 + 26, id, 0);
	//				animate::heroflag(x1, y1 + 26, id, 0);
	//			}
	//		}
}

static void paint_objects(rect screen, point camera)
{

}

int show::game(int player)
{
	int selected_wave = -1;
	if(!information_mode)
		information_mode = Information;
	herolist heroes;
	castlelist castles;
	heroes.setup(player);
	castles.setup(player);
	if(!heroes.maximum && !castles.maximum)
		return 0;
	selected_object = 0;
	if(heroes.maximum)
		selected_object = heroes.data[0];
	else if(castles.maximum)
		selected_object = castles.data[0];
	int index = bsget(selected_object, Index);
	if(index > 0)
		map::jumpto(index);
	rect rcmap = {16, 16, 16 + 32 * 14, 16 + 32 * 14};
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
		hot::index = -1;
		draw::image(0, 0, draw::isevil(res::ADVBORDE, res::ADVBORD), 0, 0);
		minimap(480, 16, 0);
		heroes.draw(481, 176, 32, 32, 4);
		castles.draw(553, 176, 32, 32, 4);
		paint_information(480, 320, player);
		if(hot::key == KeyEnter)
			draw::execute(Change);
		paint_map(rcmap, map::camera);
		paint_cursor(rcmap, map::camera);
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
			selected_object = hot::param;
			index = bsget(selected_object, Index);
			if(index>0)
			    map::jumpto(index);
			break;
		case Change:
			if(selected_object>=FirstHero && selected_object<=LastHero)
				show::hero((tokens)selected_object);
			else if(selected_object >= FirstCastle && selected_object <= LastCastle)
				show::castle(selected_object);
			heroes.setup(player);
			castles.setup(player);
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