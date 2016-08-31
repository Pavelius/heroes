#include "main.h"

static struct combatant : public animation
{
	tokens	id;
	int		index;
	int		side;
	int		hits;
	int		moved;
	int		defended;
	int		shooted;

	int getid() const override;

	void painting(point pt) const override
	{
		animation::painting(pt);
		// Draw count
		int count = game::get(getid(), Count);
		if(count)
		{
			draw::fontsm push;
			char temp[32];
			int x = pos.x + pt.x;
			int y = pos.y + pt.y;
			sznum(temp, count);
			draw::image(x, y, res::TEXTBAR, frame);
			draw::text(x + (res::width(res::TEXTBAR, frame) - draw::textw(temp)) / 2, y + 2, temp);
		}
	}

} objects[LastCombatant - FirstCombatant + 1];
static bsmeta::field fields[] = {
	BSREQ(combatant, index, Position, Number),
	BSREQ(combatant, side, Side, Number),
	BSREQ(combatant, hits, HitPoints, Number),
	BSREQ(combatant, moved, AlreadyMoved, Number),
	BSREQ(combatant, defended, Defence, Number),
	BSREQ(combatant, shooted, Shoot, Number),
	{0}
};
BSMETA(combatant, "Combatants", "Участники боя", FirstCombatant);

int combatant::getid() const
{
	return objects - this + FirstCombatant;
}

static res::tokens		back;
static res::tokens		frng;
static unsigned char	hexagon_color;
bool					combat::setting::movement = true;
bool					combat::setting::cursor = true;
bool					combat::setting::grid;
bool					combat::setting::index;
int						combat::setting::speed;
int						combat::setting::info;
bool					combat::setting::shadow = true;
bool					combat::setting::spells;
bool					combat::setting::distance;
static int				hilite_index;
static int				hilite_combatant;
static animation		attacker_leader;
static animation		defender_leader;
static animation		overlay;

void combat::setaction(int rec, tokens action)
{
	auto& e = objects[rec - FirstCombatant];
	e.set(e.id, action, 0);
}

void combat::setpos(int rec, int value)
{
	bsset(rec, Position, value);
	setaction(rec, ActorWait);
}

static int i2x(int index)
{
	return 20 + 88 - ((index / combat::awd) % 2 ? cell_wd / 2 : 0) + (cell_wd - 1) * (index % combat::awd);
}

static int i2y(int index)
{
	return 20 + 85 + ((cell_hd / 4) * 3 - 1) * (index / combat::awd);
}

void combat::applydamage(int rec, int value)
{
	combat::killed = 0;
	if(!value)
		return;
	// Calculate killed
	int m = game::get(rec, Count);
	bsadd(rec, HitPoints, -value, 0);
	combat::killed = m - game::get(rec, Count);
	// RULE: clean all mind-affect spells when damaged
	bsset(rec, SpellBlind, 0);
	bsset(rec, SpellParalyze, 0);
	bsset(rec, SpellStone, 0);
	bsset(rec, SpellBerserker, 0);
	bsset(rec, SpellHypnotize, 0);
	// Make visible effect
	if(!bsget(rec, HitPoints))
	{
		if(m)
			setaction(rec, Killed);
		else
			setaction(rec, Dead);
	}
	else
		setaction(rec, Damage);
}

void combat::applyeffect(int rec, int effect, int value)
{
	if(value == 0)
	{
		int e = bsfind(FirstEffect, Type, effect, Target, rec);
		if(e)
			bsdelete(e);
		return;
	}
	int e = bsfind(FirstEffect, Type, effect, Target, rec);
	if(!e)
		e = bscreate(FirstEffect);
	bsset(e, Type, effect);
	bsset(e, Target, rec);
	bsset(e, Expire, combat::rounds + value);
}

//static void object_set(int rec, int id, int value)
//{
//	e.hits = bsget(rec, HitPointsMax) * value;
//}

static void prepare_leader(int rec, animation& e)
{
	e.clear();
	switch(bsget(rec, First))
	{
	case FirstHero:
		rec = bsget(rec, Type);
	case FirstCaptain:
		break;
	default:
		return;
	}
	e.set(tokens(rec), ActorWait);
	e.pos.x = 32;
	e.pos.y = 186;
}

void combat::board(int attacker, int defender)
{
	combatants.clear();
	bool light = true;
	bool trees = false;
	tokens area = Beach;
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
	prepare_leader(attacker, attacker_leader);
	prepare_leader(defender, defender_leader);
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

static void paint_leader(int rec, animation& e)
{
	res::tokens icn = res::tokens(res::HEROFL00 + bsget(rec, Player) - PlayerBlue);
	e.painting({0, 0});
	draw::image(e.pos.x, e.pos.y, icn, draw::frame % 5);
}

static void paint_grid(int rec)
{
	if(rec != -1 && combat::setting::shadow)
	{
		for(auto& e : objects)
		{
			if(!e.id)
				continue;
			if(!e.hits)
				continue;
			int x = i2x(e.index);
			int y = i2y(e.index);
			draw::hexagonf(x, y, 0);
		}
	}
	if(rec != -1 && combat::setting::movement)
	{
		draw::fontsm push;
		int radius = bsget(rec, Speed) - Crawling + 2;
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
	if(rec != -1 && combat::setting::cursor)
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

static void timer_animation()
{
	for(auto& e : objects)
	{
		if(!e.id)
			continue;
		e.update();
		if(false)
		{
			if(!e.hits)
				e.set(e.id, Dead);
			else
			{
				if(d100() < 10)
					e.set(e.id, ActorWarn);
				else
					e.set(e.id, ActorWait);
			}
		}
	}
	attacker_leader.update();
	defender_leader.update();
}

static void action(animation& cursor, tokens id, int param, tokens idc = Empthy)
{
	cursor.set(CursorCombat, (idc == Empthy) ? id : idc);
	if(hot::key == MouseLeft && hot::pressed)
		draw::execute(id, param);
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
	//paint_leader(combat::attacker, attacker_leader);
	//paint_leader(combat::attacker, attacker_leader);
	//defender_leader.paint(300, 186, AFMirror);
	hittest_grid();
	paint_grid(rec);
	if(frng != res::Empthy)
		draw::image(0, 0, frng, 0);
}

static void flash()
{
	paint_field(-1);
	draw::inputex();
}

static int animate_speed()
{
	return 64;
}

static void animate_action(combatant& e, tokens a, int param, int wait = 0)
{
	e.set(e.id, a, param);
	while(true)
	{
		flash();
		bool last = e.islast();
		e.update();
		if(last)
			break;
		//if(wait)
		//	sleep(wait);
	}
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
		case InputTimer:
			timer_animation();
			break;
		}
	}
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
				int radius = bsget(rec, Speed) - Crawling + 2;
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
				draw::execute(Hero);
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
			show::unitinfo(hot::param, bsget(hot::param, Side));
			break;
		case Spells:
			if(casted)
				dlgmsg(0, szt("You can\'t cast spell twice per combat round.","Вы не можете создать заклинание дважды за ход."));
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
			timer_animation();
			break;
		}
	}
}

void show::battle::fly(int rec, int target)
{
	point data[128];
	auto& e = objects[rec - FirstCombatant];
	int count = animation::line(data, i2x(e.index), i2y(e.index), i2x(target), i2y(target), 40);
	// jump into the sky
	animate_action(e, Fly, 0, animate_speed());
	// fly over all units
	e.set(e.id, Fly, 1);
	for(int i = 0; i < count; i++)
	{
		e.pos.x = data[i].x;
		e.pos.y = data[i].y;
		flash();
		//sleep(animate_speed());
		e.update();
	}
	// sit down on ground
	animate_action(e, Fly, 2, animate_speed());
}

void show::battle::move(int rec, int target)
{
	int data[128];
	auto& e = objects[rec - FirstCombatant];
	int count = combat::move(data, bsget(rec, Position), target, bsget(rec, Speed));
	if(!count)
		return;
	e.set(e.id, Move, 0);
	int bx = res::ox(e.icn, e.frame);
	int by = res::oy(e.icn, e.frame);
	int x1 = e.pos.x;
	int y1 = e.pos.y;
	while(count--)
	{
		tokens d = hexagon_orient(e.pos.x, e.pos.y, i2x(data[count]), i2y(data[count]));
		while(true)
		{
			flash();
			//sleep(animate_speed());
			bool last = e.islast();
			e.update();
			if(last)
				break;
		}
		e.frame++;
		e.pos.x = i2x(data[count]);
		e.pos.y = i2y(data[count]);
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

void show::battle::shoot(int rec, int target)
{
	overlay.clear();
	overlay.icn = monster::getr(bsget(rec, Type), Shoot);
	if(overlay.icn == res::Empthy)
		return;
	point data[128];
	auto& e1 = objects[rec - FirstCombatant];
	auto& e2 = objects[target - FirstCombatant];
	int x1 = i2x(e1.index); int y1 = i2y(e1.index); y1 -= res::height(e1.icn, e1.frame) / 2;
	int x2 = i2x(e2.index); int y2 = i2y(e2.index); y2 -= res::height(e2.icn, e2.frame) / 2;
	int count = animation::line(data, x1, y1, x2, y2, 64);
	if(!count)
		return;
	// jump into the sky
	animate_action(e1, Shoot, 0, animate_speed());
	e1.set(e1.id, ActorWait, 0);
	overlay.start = missile_index(overlay.icn, x2 - x1, y2 - y1);
	overlay.frame = overlay.start;
	overlay.count = 1;
	for(int i = 0; i < count; i++)
	{
		overlay.pos.x = data[i].x;
		overlay.pos.y = data[i].y;
		flash();
		//sleep(animate_speed());
		overlay.update();
	}
	overlay.clear();
}

static void paint_board()
{
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
	paint_grid(-1);
	if(frng != res::Empthy)
		draw::image(0, 0, frng, 0);
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
		dwpaint(objects, {0,0,640,480}, {0, 0});
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