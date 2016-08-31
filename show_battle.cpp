#include "main.h"

struct leader : public animation
{
	void painting(point screen) const override
	{
		animation::painting(screen);
		int player = bsget(rec, Player);
		if(player >= PlayerBlue && player <= LastPlayer)
		{
			res::tokens icn = res::tokens(res::HEROFL00 + player -PlayerBlue);
			draw::image(pos.x + screen.x, pos.y + screen.y, icn, (draw::frame/6) % 5, flags);
		}
	}
};

static res::tokens		back;
static res::tokens		frng;
static unsigned char	hexagon_color;
bool					combat::setting::movement = true;
bool					combat::setting::cursor = true;
bool					combat::setting::grid = true;
bool					combat::setting::index = true;
int						combat::setting::speed;
int						combat::setting::info;
bool					combat::setting::shadow = true;
bool					combat::setting::spells;
bool					combat::setting::distance;
static int				hilite_index;
static int				hilite_combatant;
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
	command::execute("battle", "initialize");
	bool light = true;
	bool trees = false;
	tokens area = Grass;
	if(defender >= (int)FirstMoveable && defender <= (int)LastMoveable)
	{
		int pos = bsget(defender, Position);
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
	if(rec != -1 && combat::setting::shadow)
	{
		for(int rec= FirstCombatant; rec<=LastCombatant; rec++)
		{
			if(!bsget(rec, Type))
				continue;
			if(!bsget(rec, HitPoints))
				continue;
			int index = bsget(rec, Position);
			int x = i2x(index);
			int y = i2y(index);
			draw::hexagonf(x, y, 0);
		}
	}
	if(rec != -1 && combat::setting::movement)
	{
		draw::fontsm push;
		int radius = bsget(rec, Speed) - SpeedCrawling + 2;
		for(int i = 0; i < combat::awd*combat::ahd; i++)
		{
			if(combat::movements[i])
			{
				int x = i2x(i);
				int y = i2y(i);
				if(combat::movements[i] <= radius)
					draw::hexagonf(x, y, 0);
				else if(combat::movements[i] == BlockSquad)
					draw::hexagonf(x, y, 1);
				if(combat::movements[i] < BlockSquad && combat::setting::distance)
				{
					char temp[32];
					sznum(temp, combat::movements[i] - 1);
					draw::text(x - draw::textw(temp) / 2, y - 5, temp);
				}
			}
		}
	}
	if(combat::setting::cursor)
	{
		if(hilite_index != -1)
		{
			int x = i2x(hilite_index);
			int y = i2y(hilite_index);
			draw::hexagonf(x, y, 0);
		}
	}
	if(combat::setting::grid)
	{
		for(int i = 0; i < combat::awd*combat::ahd; i++)
		{
			int x = i2x(i);
			int y = i2y(i);
			draw::hexagon(x, y, hexagon_color);
		}
	}
	if(combat::setting::index)
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

static void paint_field(int rec)
{
	int h1 = res::height(res::TEXTBAR, 4);
	int h2 = res::height(res::TEXTBAR, 6);
	int h3 = res::height(res::TEXTBAR, 0);
	int h8 = res::height(res::TEXTBAR, 8);
	int h9 = res::height(res::TEXTBAR, 9);
	int w3 = res::width(res::TEXTBAR, 0);
	hilite_index = -1;
	hilite_combatant = -1;
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
}

static void paint_board()
{
	hilite_index = -1;
	hilite_combatant = -1;
	int h1 = res::height(res::TEXTBAR, 4);
	int h2 = res::height(res::TEXTBAR, 6);
	int h3 = res::height(res::TEXTBAR, 0);
	int h8 = res::height(res::TEXTBAR, 8);
	int h9 = res::height(res::TEXTBAR, 9);
	int w3 = res::width(res::TEXTBAR, 0);
	draw::image(0, 0, back, 0);
	int x = res::width(res::TEXTBAR, 4);
	draw::image(x, draw::height - h8 - h9, res::TEXTBAR, 8);
	draw::image(x, draw::height - h9, res::TEXTBAR, 9);
	draw::status(x + 32, draw::height - h8 - h9 + 1, draw::width - w3, draw::height - 1);
	draw::button(0, draw::height - h1 - h2, res::TEXTBAR, AutoCombat, 4, 4, 5, Alpha + 'A', 0, szt("Run auto-combat", "Запустить автоматический бой"));
	draw::button(0, draw::height - h2, res::TEXTBAR, Setting, 6, 6, 7, KeyEscape, 0, szt("Open combat setting", "Открыть настройки"));
	draw::button(draw::width - w3, draw::height - h3, res::TEXTBAR, Skip, 0, 0, 1, KeySpace, 0, szt("Skip current turn", "Пропустить текущий ход"));
	hittest_grid();
	paint_grid(-1);
	if(frng != res::Empthy)
		draw::image(0, 0, frng, 0);
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

static int missile_index(res::tokens icn, int dx, int dy)
{
	switch(icn)
	{
	case res::KEEP:
	case res::ARCH_MSL:
	case res::ORC__MSL:
		return missile9(dx, dy);
	case res::ELF__MSL:
	case res::DRUIDMSL:
	case res::HALFLMSL:
	case res::TITANMSL:
		return missile7(dx, dy);
	case res::TROLLMSL:
	case res::LICH_MSL:
	default:
		break;
	}
	return 0;
}

int show::battle::target(int side, int sid)
{
	while(true)
	{
		paint_field(-1);
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

void show::battle::shoot(int rec, int target)
{
}

void show::battle::fly(int rec, int target)
{
}

void show::battle::move(int rec, int target)
{
}

int show::battle::unit(int rec, int casted)
{
	while(true)
	{
		paint_field(rec);
		animation cursor(CursorCombat, Cursor);
		if(hilite_index != -1)
		{
			hilite_combatant = combat::combatant(hilite_index);
			if(combat::movements[hilite_index] != 0)
			{
				int radius = bsget(rec, Speed) - SpeedCrawling + 2;
				if(combat::movements[hilite_index] < BlockSquad && combat::movements[hilite_index] <= radius)
					action(cursor, Move, hilite_index);
				else if(combat::movements[hilite_index] == BlockSquad)
				{
					if(combat::isenemy(rec, hilite_combatant))
					{
						if(combat::canshoot(rec, hilite_combatant))
							action(cursor, Shoot, hilite_combatant);
						else
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
					}
					else
						cursor.set(CursorCombat, Information);
				}
			}
			if(hilite_combatant != -1)
			{
				if(hot::key == MouseRight && hot::pressed)
					draw::execute(Information, hilite_combatant);
			}
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
		}
	}
}

void show::battle::area()
{
	animation cursor(CursorCombat, Cursor);
	drawable* objects[64];
	while(true)
	{
		paint_board();
		objects[0] = 0;
		zcat(objects, static_cast<drawable*>(&attacker_leader));
		zcat(objects, static_cast<drawable*>(&defender_leader));
		dworder(objects, zlen(objects));
		dwpaint(objects, {0, 0, 640, 480}, {0, 0});
		draw::debug();
		draw::cursor(cursor.icn, cursor.frame, cursor.pos.x, cursor.pos.y);
		int id = draw::input();
		switch(id)
		{
		case Cancel:
		case 0:
			return;
		case InputTimer:
			for(auto e : objects)
			{
				if(!e)
					break;
				e->update();
			}
			break;
		}
	}
}