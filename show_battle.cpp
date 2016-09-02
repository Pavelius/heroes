#include "main.h"

struct leader : public animation
{
	void painting(point screen) const override
	{
		animation::painting(screen);
		int player = bsget(rec, Player);
		if(player >= PlayerBlue && player <= LastPlayer)
		{
			res::tokens icn = res::tokens(res::HEROFL00 + player - PlayerBlue);
			draw::image(pos.x + screen.x, pos.y + screen.y, icn, draw::counter % 5, flags);
		}
	}
};

const unsigned			combat_timeout = 1000 / 24;
static res::tokens		back;
static res::tokens		frng;
static unsigned char	hexagon_color;
bool					combat::setting::movement = true;
bool					combat::setting::cursor = true;
bool					combat::setting::grid;
bool					combat::setting::index;
int						combat::setting::speed;
int						combat::setting::info;
bool					combat::setting::spells;
bool					combat::setting::distance;
static int				hilite_index;
static leader			attacker_leader;
static leader			defender_leader;

static int i2x(int index)
{
	return 20 + 88 - ((index / combat::awd) % 2 ? cell_wd / 2 : 0) + (cell_wd - 1) * (index % combat::awd);
}

static int i2y(int index)
{
	return 20 + 85 + ((cell_hd / 4) * 3 - 1) * (index / combat::awd);
}

inline int sin_a(int a)
{
	return a * 38 / 43;
}

inline int cos_a(int a)
{
	return a * 22 / 43;
}

point combat::i2h(int index)
{
	return{(short)i2x(index), (short)i2y(index)};
}

static int indicator_index(int rec)
{
	bool boosted = bsget(rec, Boosted) != 0;
	bool penalized = bsget(rec, Penalized) != 0;
	if(boosted && penalized)
		return 13; // yellow
	else if(boosted)
		return 12; // green
	else if(penalized)
		return 14; // red
	return 10;
}

static void action(animation& cursor, tokens id, int param, tokens idc = Empthy)
{
	cursor.set(CursorCombat, (idc == Empthy) ? id : idc);
	if(hot::key == MouseLeft && hot::pressed)
		draw::execute(id, param);
}

static void prepare_leader(int rec, animation& e, bool defender)
{
	e.clear();
	e.rec = rec;
	if(rec >= FirstHero && rec <= LastHero)
		rec = game::get(rec, Type);
	else
		return;
	e.set(tokens(rec), ActorWait);
	if(defender)
	{
		e.pos.x = 606;
		e.pos.y = 156;
		e.flags = AFMirror;
	}
	else
	{
		e.pos.x = 32;
		e.pos.y = 186;
	}
}

void combat::board(int attacker, int defender)
{
	bool light = true;
	bool trees = false;
	tokens area = Lava;
	if(defender >= (int)FirstMoveable && defender <= (int)LastMoveable)
	{
		int pos = bsget(defender, Index);
		area = map::gtile(pos);
	}
	switch(area)
	{
	case Desert:
		back = res::CBKGDSRT;
		frng = res::FRNG0004;
		break;
	case Snow:
		back = trees ? res::CBKGSNTR : res::CBKGSNMT;
		light = false;
		frng = trees ? res::FRNG0006 : res::FRNG0007;
		break;
	case Swamp:
		back = res::CBKGSWMP;
		frng = res::FRNG0008;
		break;
	case Wastelands:
		back = res::CBKGCRCK;
		light = false;
		frng = res::FRNG0003;
		break;
	case Beach:
		back = res::CBKGBEAC;
		light = false;
		frng = res::FRNG0002;
		break;
	case Lava:
		back = res::CBKGLAVA;
		frng = res::FRNG0005;
		break;
	case Dirt:
		back = trees ? res::CBKGDITR : res::CBKGDIMT;
		frng = trees ? res::FRNG0010 : res::FRNG0009;
		break;
	case Water:
		back = res::CBKGWATR;
		frng = res::FRNG0013;
		break;
	default:
		back = trees ? res::CBKGGRTR : res::CBKGGRMT;
		frng = trees ? res::FRNG0011 : res::FRNG0009;
		break;
	}
	if(false)
	{
		back = res::CBKGGRAV;
		light = true;
		frng = res::FRNG0001;
	}
	hexagon_color = light ? 0xE0 : 0xE5;
	prepare_leader(attacker, attacker_leader, false);
	prepare_leader(defender, defender_leader, true);
}

static void hittest_grid()
{
	for(int i = 0; i < combat::awd*combat::ahd; i++)
	{
		int x = i2x(i);
		int y = i2y(i);
		rect rc = {x - cell_wd / 2, y - cell_hr, x + cell_wd / 2, y + cell_hr};
		point cooru[] =
		{
			{(short)(x - cell_wd / 2), (short)(y - cell_hr)},
			{(short)x, (short)(y - cell_hd / 2)},
			{(short)(x + cell_wd / 2), (short)(y - cell_hr)},
		};
		point coord[] =
		{
			{(short)(x + cell_wd / 2), (short)(y + cell_hr)},
			{(short)x, (short)(y + cell_hd / 2)},
			{(short)(x - cell_wd / 2), (short)(y + cell_hr)},
		};
		if(hot::mouse.in(rc)
			|| hot::mouse.in(cooru[0], cooru[1], cooru[2])
			|| hot::mouse.in(coord[0], coord[1], coord[2]))
		{
			hilite_index = i;
			return;
		}
	}
}

static void paint_grid(int rec)
{
	// Shadow movement indecies
	if(rec && combat::setting::movement)
	{
		draw::fontsm push;
		int radius = game::get(rec, Speed) - SpeedCrawling + 2;
		for(int i = 0; i < combat::awd*combat::ahd; i++)
		{
			if(combat::movements[i])
			{
				int x = i2x(i);
				int y = i2y(i);
				if(combat::movements[i] <= radius)
					draw::hexagonf(x, y, 0);
				if(combat::movements[i] < BlockSquad && combat::setting::distance)
				{
					char temp[32];
					sznum(temp, combat::movements[i] - 1);
					draw::text(x - draw::textw(temp) / 2, y - 5, temp);
				}
			}
		}
	}
	// Shadow cursor index
	if(rec && combat::setting::cursor)
	{
		if(hilite_index != -1)
		{
			int x = i2x(hilite_index);
			int y = i2y(hilite_index);
			draw::hexagonf(x, y, 0);
		}
	}
	// Show grid
	if(combat::setting::grid)
	{
		for(int i = 0; i < combat::awd*combat::ahd; i++)
		{
			int x = i2x(i);
			int y = i2y(i);
			draw::hexagon(x, y, hexagon_color);
		}
	}
	// Show index (only debug)
	if(rec && combat::setting::index)
	{
		draw::fontsm push;
		for(int i = 0; i < combat::awd*combat::ahd; i++)
		{
			char temp[8];
			int x = i2x(i);
			int y = i2y(i);
			sznum(temp, i);
			draw::text(x - 4, y - 4, temp);
		}
	}
}

static tokens hexagon_orient(int x1, int y1, int x2, int y2)
{
	const int INFL = 12;
	point pt = {(short)x2, (short)y2};
	point coord[7] =
	{
		{(short)x1, (short)y1},
		//
		{(short)x1, (short)(y1 - cell_hd*INFL / 2)}, // u
		{(short)(x1 + cell_wd*INFL / 2), (short)(y1 - cell_hr*INFL)}, // ru
		{(short)(x1 + cell_wd*INFL / 2), (short)(y1 + cell_hr*INFL)}, // rd
		{(short)x1, (short)(y1 + cell_hd*INFL / 2)}, // d
		{(short)(x1 - cell_wd*INFL / 2), (short)(y1 + cell_hr*INFL)}, // ld
		{(short)(x1 - cell_wd*INFL / 2), (short)(y1 - cell_hr*INFL)}, // lu
	};
	if(pt == coord[0])
		return HexCenter;
	else if(pt.in(coord[0], coord[1], coord[2]))
		return HexRightUp;
	else if(pt.in(coord[0], coord[2], coord[3]))
		return HexRight;
	else if(pt.in(coord[0], coord[3], coord[4]))
		return HexRightDown;
	else if(pt.in(coord[0], coord[4], coord[5]))
		return HexLeftDown;
	else if(pt.in(coord[0], coord[5], coord[6]))
		return HexLeft;
	else if(pt.in(coord[0], coord[1], coord[6]))
		return HexLeftUp;
	return Empthy;
}

static void select_animation(drawable** objects)
{
	objects[0] = 0;
	dwselect(zend(objects), 1);
	zcat(objects, static_cast<drawable*>(&attacker_leader));
	zcat(objects, static_cast<drawable*>(&defender_leader));
}

static void paint_field(int rec, drawable** objects)
{
	int h1 = res::height(res::TEXTBAR, 4);
	int h2 = res::height(res::TEXTBAR, 6);
	int h3 = res::height(res::TEXTBAR, 0);
	int h8 = res::height(res::TEXTBAR, 8);
	int h9 = res::height(res::TEXTBAR, 9);
	int w3 = res::width(res::TEXTBAR, 0);
	hilite_index = -1;
	draw::image(0, 0, back, 0);
	int x = res::width(res::TEXTBAR, 4);
	draw::image(x, draw::height - h8 - h9, res::TEXTBAR, 8);
	draw::image(x, draw::height - h9, res::TEXTBAR, 9);
	draw::status(x + 32, draw::height - h8 - h9 + 1, draw::width - w3, draw::height - 1);
	draw::button(0, draw::height - h1 - h2, res::TEXTBAR, AutoCombat, 4, 4, 5, Alpha + 'A', 0, szt("Run auto-combat", "Запустить автоматический бой"));
	draw::button(0, draw::height - h2, res::TEXTBAR, Setting, 6, 6, 7, KeyEscape, 0, szt("Open combat setting", "Открыть настройки"));
	draw::button(draw::width - w3, draw::height - h3, res::TEXTBAR, Skip, 0, 0, 1, KeySpace, 0, szt("Skip current turn", "Пропустить текущий ход"));
	hittest_grid();
	paint_grid(rec);
	if(frng != res::Empthy)
		draw::image(0, 0, frng, 0);
	if(objects)
	{
		dworder(objects, zlen(objects));
		dwpaint(objects, {0, 0, 640, 480}, {0, 0});
	}
}

static int missile9(int dx, int dy)
{
	if(0 == dx)
		return dy > 0 ? 0 : 8;
	int tan = iabs(1000 * dy / dx);
	if(tan <= 60)
		return 4;
	else if(tan <= 577) // tan 30: 0 - 30
		return dy > 0 ? 3 : 5;
	else if(tan >= 1732) // tan 60: 90 - 60
		return dy > 0 ? 1 : 7;
	// tan 45: 30 - 60
	return dy > 0 ? 2 : 6;
}

static int missile7(int dx, int dy)
{
	if(0 == dx)
		return dy > 0 ? 0 : 6;
	else if(0 == dy)
		return 3;
	int tan = iabs(1000 * dy / dx);
	// tan 45: 0 - 45
	if(1000 >= tan)
		return dy > 0 ? 2 : 4;
	// 45 - 90
	return dy > 0 ? 1 : 5;
}

static int get_missile_index(res::tokens icn, int dx, int dy)
{
	switch(res::getcount(icn))
	{
	case 9:
		return missile9(dx, dy);
	case 7:
		return missile7(dx, dy);
	default:
		return 0;
	}
}

int show::battle::target(int side, int sid)
{
	drawable* objects[64];
	while(true)
	{
		int hilite_combatant = 0;
		select_animation(objects);
		paint_field(-1, objects);
		if(hot::key == KeyEscape)
			draw::execute(Cancel);
		int i = bsget(sid, Portrait);
		if(hilite_index != -1)
		{
			hilite_combatant = combat::combatant(hilite_index);
			if(!combat::cast(side, sid, hilite_combatant, hilite_index, false, false))
				i = -1;
		}
		if(hot::key == MouseLeft && hot::pressed)
		{
			if(i != -1)
				draw::execute(Spells);
			else
				draw::execute(Cancel);
		}
		if(i == -1)
			draw::cursor(res::SPELCO, 0, -res::width(res::SPELCO, 0) / 2, -res::height(res::SPELCO, 0) / 2);
		else
			draw::cursor(res::SPELCO, i, -res::width(res::SPELCO, i) / 2, -res::height(res::SPELCO, i) / 2);
		int id = draw::input();
		switch(id)
		{
		case Cancel:
		case 0:
			return 0;
		case Spells:
			return hilite_combatant;
		}
	}
}

void show::battle::shoot(int rec, int enemy, int damage)
{
	drawable* objects[64];
	select_animation(objects);
	paint_field(0, 0);
	auto pa = animation::find(objects, rec);
	auto pe = animation::find(objects, enemy);
	if(!pa || !pe)
		return;
	draw::screenshoot screen;
	animation::state a1(pa);
	// Prepare shoot
	if(pa->hasanimation(Shoot, 2))
	{
		pa->setaction(Shoot, 0);
		screen.redraw(objects, combat_timeout, pa);
		pa->setaction(Shoot, 2);
		screen.redraw(objects, combat_timeout, pa, pa->start + pa->count - 1);
	}
	else
	{
		pa->setaction(Shoot, 0);
		screen.redraw(objects, combat_timeout, pa, pa->start + pa->count - 1);
	}
	// Projectile
	animation arrow; zcat(objects, static_cast<drawable*>(&arrow));
	int i1 = bsget(rec, Index);
	int i2 = bsget(enemy, Index);
	point p1 = combat::i2h(i1); p1.y -= 32;
	point p2 = combat::i2h(i2); p2.y -= 32;
	point points[256];
	int dx = p1.x - p2.x;
	int dy = p1.y - p2.y;
	arrow.icn = res::getshooting(rec);
	arrow.frame = get_missile_index(arrow.icn, dx, dy);
	arrow.start = arrow.frame;
	arrow.flags = pa->flags;
	arrow.count = 1;
	int count = animation::fly(points, p1, p2, 48);
	for(int i = 1; i < count - 1; i++)
	{
		arrow.pos = points[i];
		screen.redraw(objects, combat_timeout);
	}
	arrow.icn = res::Empthy;
	// Last phase
	combat::applydamage(enemy, damage);
	screen.redraw(objects, combat_timeout, pa, pe);
}

void show::battle::fly(int rec, int target)
{
	point points[256];
	drawable* objects[64];
	select_animation(objects);
	paint_field(0, 0);
	auto pa = animation::find(objects, rec);
	if(!pa)
		return;
	int i1 = bsget(rec, Index);
	int i2 = target;
	point p1 = combat::i2h(i1);
	point p2 = combat::i2h(i2);
	draw::screenshoot screen;
	animation::state a1(pa);
	// Flying Up
	pa->setaction(Fly, 0);
	screen.redraw(objects, combat_timeout, pa);
	// Main phase
	pa->setaction(Fly, 1);
	int count = animation::fly(points, p1, p2, 16);
	for(int i = 1; i < count - 1; i++)
	{
		pa->pos = points[i];
		screen.redraw(objects, combat_timeout);
		if(pa->incframe())
			pa->setaction(Fly, 1);
	}
	// Flying Down
	pa->pos = points[count - 1];
	pa->setaction(Fly, 2);
	screen.redraw(objects, combat_timeout, pa);
}

void show::battle::attack(int rec, int enemy, int damage)
{
	drawable* objects[64];
	select_animation(objects);
	paint_field(0, 0);
	auto pa = animation::find(objects, rec);
	auto pe = animation::find(objects, enemy);
	if(!pa || !pe)
		return;
	int i1 = bsget(rec, Index);
	int i2 = bsget(enemy, Index);
	auto d = combat::direction(i1, i2);
	draw::screenshoot screen;
	animation::state a1(pa);
	animation::state a2(pe);
	if(d == HexLeft || d == HexLeftUp || d == HexLeftDown)
	{
		pa->flags = AFMirror;
		pe->flags = 0;
	}
	else
	{
		pa->flags = 0;
		pe->flags = AFMirror;
	}
	pa->setaction(Attack, 0);
	if(pa->count)
		screen.redraw(objects, combat_timeout, pa);
	if(d == HexRightUp || d == HexLeftUp)
		pa->setaction(Attack, 1);
	else if(d == HexRightDown || d == HexLeftDown)
		pa->setaction(Attack, 3);
	else
		pa->setaction(Attack, 2);
	screen.redraw(objects, combat_timeout, pa);
	pa->setaction(ActorWarn);
	pe->setaction(ActorWarn);
	combat::applydamage(enemy, damage);
	screen.redraw(objects, combat_timeout, pe);
}

void show::battle::move(int rec, int target)
{
	int steps[64];
	drawable* objects[64];
	select_animation(objects);
	paint_field(0, 0);
	auto pa = animation::find(objects, rec);
	if(!pa)
		return;
	animation::state a1(pa);
	draw::screenshoot screen;
	int count = combat::move(steps, bsget(rec, Index), target, game::get(rec, Speed));
	for(int i = count - 1; i >= 0; i--)
	{
		int i1 = bsget(rec, Index);
		int i2 = steps[i];
		auto d = combat::direction(i1, i2);
		pa->setaction(Move, d);
		if(i != count - 1)
			pa->frame++;
		point move_base = res::offset(pa->icn, pa->frame);
		point move_start = pa->pos;
		if(d == HexLeft || d == HexLeftUp || d == HexLeftDown)
		{
			pa->flags = AFNoOffset | AFMirror;
			move_start.x -= move_base.x;
		}
		else
		{
			pa->flags = AFNoOffset;
			move_start.x += move_base.x;
		}
		move_start.y += move_base.y;
		while(true)
		{
			point pt = res::offset(pa->icn, pa->frame) - move_base;
			// cosA = 22/44 or 1/2
			// sinA = 38/44 or 19/22
			if(d == HexLeft)
			{
				pa->pos.x = move_start.x - pt.x;
				pa->pos.y = move_start.y + pt.y;
			}
			else if(d == HexRight)
			{
				pa->pos.x = move_start.x + pt.x;
				pa->pos.y = move_start.y + pt.y;
			}
			else if(d == HexRightUp)
			{
				pa->pos.x = move_start.x + (cos_a(pt.x) - sin_a(pt.y));
				pa->pos.y = move_start.y - (sin_a(pt.x) - cos_a(pt.y));
			}
			else if(d == HexRightDown)
			{
				pa->pos.x = move_start.x + (cos_a(pt.x) - sin_a(pt.y));
				pa->pos.y = move_start.y + (sin_a(pt.x) - cos_a(pt.y));
			}
			else if(d == HexLeftUp)
			{
				pa->pos.x = move_start.x - (cos_a(pt.x) - sin_a(pt.y));
				pa->pos.y = move_start.y - (sin_a(pt.x) - pt.y / 2);
			}
			else if(d == HexLeftDown)
			{
				pa->pos.x = move_start.x - (cos_a(pt.x) - sin_a(pt.y));
				pa->pos.y = move_start.y + (sin_a(pt.x) - pt.y / 2);
			}
			screen.redraw(objects, combat_timeout);
			if(pa->incframe())
				break;
		}
		combat::setindex(rec, i2);
	}
}

int show::battle::unit(int rec, int casted)
{
	drawable* objects[64];
	while(true)
	{
		animation cursor(CursorCombat, Cursor);
		select_animation(objects);
		paint_field(rec, objects);
		if(hilite_index != -1)
		{
			int radius = game::get(rec, Speed) - SpeedCrawling + 2;
			int hilite_combatant = combat::combatant(hilite_index);
			if(hilite_combatant && game::get(hilite_combatant, Count))
			{
				if(combat::canshoot(rec, hilite_combatant))
					action(cursor, Shoot, hilite_combatant);
				else if(combat::isenemy(rec, hilite_combatant))
				{
					int x = i2x(hilite_index);
					int y = i2y(hilite_index);
					tokens d = hexagon_orient(x, y, hot::mouse.x, hot::mouse.y);
					if(combat::canattack(rec, hilite_combatant, d))
					{
						hot::param2 = combat::moveto(hilite_index, d);
						action(cursor, Attack, hilite_combatant, d);
					}
				}
				if(hot::key == MouseRight && hot::pressed)
					draw::execute(Information, hilite_combatant);
			}
			else if(combat::movements[hilite_index] < BlockSquad && combat::movements[hilite_index] <= radius)
				action(cursor, Move, hilite_index);
		}
		else if(hot::mouse.in(attacker_leader.getrect()))
		{
			cursor.set(CursorCombat, Hero);
			if(hot::key == MouseLeft && hot::pressed)
				draw::execute(Hero, attacker_leader.getid());
		}
		if(hot::key == Alpha + 'C')
			draw::execute(Spells);
		draw::cursor(cursor.icn, cursor.frame, cursor.pos.x, cursor.pos.y);
		int id = draw::input();
		switch(id)
		{
		case Cancel:
		case 0:
		case Skip:
		case Shoot:
		case Attack:
			return id;
		case Move:
			return id;
		case Setting:
			show::settings();
			break;
		case Information:
			show::unit(hot::param, bsget(hot::param, Side));
			break;
		case Spells:
			if(casted)
				dlgmsg(0, szt("You can\'t cast spell twice per combat round.", "Вы не можете создать заклинание дважды за ход."));
			else
			{
				id = show::spellbook(bsget(rec, Side), CombatSpells);
				if(id)
				{
					draw::execute(0);
					return id;
				}
			}
			break;
		case Hero:
			id = show::battle::dialog(hot::param);
			if(!id)
				break;
			else if(id == Spells)
				draw::execute(Spells);
			return id;
		case InputTimer:
			if((draw::counter % 4) == 0)
			{
				for(auto e : objects)
				{
					if(!e)
						break;
					e->update();
				}
			}
			break;
		}
	}
}