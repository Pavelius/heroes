#include "main.h"

static struct combat_unit : public animation
{
	int		index;
	int		side;
	int		hits;
	int		moved;
	int		defended;
	int		shoots;
	tokens	action;

	int getid() const override;

	static int getbarframe(int rec)
	{
		bool isboosted = game::isboosted(rec);
		bool ispenalized = game::ispenalized(rec);
		if(isboosted && ispenalized)
			return 13;
		else if(isboosted)
			return 12;
		else if(ispenalized)
			return 14;
		return 10;
	}

	void painting(point pt) const override
	{
		draw::image(pt.x + pos.x, pt.y + pos.y, icn, frame, flags);
		// Draw count
		int rec = getid();
		int count = game::get(rec, Count);
		if(count && action != Move && action!=Fly)
		{
			draw::state push;
			draw::font = res::SMALFONT;
			char temp[32];
			int x = pos.x + pt.x;
			int y = pos.y + pt.y;
			sznum(temp, count);
			int frame = getbarframe(rec);
			int x1 = x;
			int y1 = y;
			if(combat::isattacker(rec))
			{
				x1 += 12;
				y1 -= res::height(res::TEXTBAR, frame);
			}
			else
			{
				x1 -= 32;
				y1 -= res::height(res::TEXTBAR, frame) * 2;
			}
			draw::image(x1, y1, res::TEXTBAR, frame);
			draw::text(x1 + (res::width(res::TEXTBAR, frame) - draw::textw(temp)) / 2, y1 + 2, temp);
		}
	}

	void setaction(tokens action, int param = 0) override
	{
		set(rec, action, param);
		this->action = action;
	}

	bool incframe() override
	{
		if(wait)
		{
			wait--;
			return false;
		}
		if(++frame >= start + count)
		{
			frame = start;
			if(action == Killed)
				frame = start + count - 1;
			else
			{
				set(rec, ActorWarn);
				int d = d100();
				switch(count)
				{
				case 4: // Peasant, Archer,
					if(d < 40)
						frame += xrand(1, count-1);
					else
						wait = xrand(5, 10);
					break;
				case 6: // Swordsman
					if(d < 20)
						frame += xrand(3, count - 1);
					else if(d < 20)
						count = 2;
					else
					{
						frame += 2;
						wait = xrand(5, 10);
					}
					break;
				case 7:
					if(icn == res::TROLL || icn == res::TROLL2)
					{
						if(d100()<25)
							count = 4;
						else
						{
							frame += 4;
							wait = xrand(4, 10);
						}
					}
					else
					{
						// Goblin, Roc
						if(d100()<25)
							count = 2;
						else
						{
							frame += 2;
							wait = xrand(4, 10);
						}
					}
					break;
				}
			}
			return true;
		}
		return false;
	}

	point getzpos() const
	{
		point pt = pos;
		if(action == Killed && frame >= start + count - 1)
			pt.y -= 500;
		else if(action == Move)
			pt.y -= res::oy(icn, start);
		return pt;
	}

} objects[LastCombatant - FirstCombatant + 1];
static bsmeta::field fields[] = {
	BSREQ(combat_unit, rec, Type, Number),
	BSREQ(combat_unit, index, Index, Number),
	BSREQ(combat_unit, side, Side, Number),
	BSREQ(combat_unit, hits, HitPoints, Number),
	BSREQ(combat_unit, moved, AlreadyMoved, Number),
	BSREQ(combat_unit, defended, AlreadyDefended, Number),
	BSREQ(combat_unit, shoots, Shoots, Number),
	{0}
};
BSMETA(combat_unit, "Combatants", "Участники боя", FirstCombatant);

int combat_unit::getid() const
{
	return this - objects + FirstCombatant;
}

void combat::setaction(int rec, tokens action)
{
	objects[rec - FirstCombatant].setaction(action);
}

void combat::setindex(int rec, int index)
{
	auto& e = objects[rec - FirstCombatant];
	e.index = index;
	e.pos = combat::i2h(index);
	e.pos.y += 12;
	if(!combat::isattacker(e.side))
		e.flags = AFMirror;
	else
		e.flags = 0;
	e.action = ActorWarn;
	e.frame = 0;
	e.start = 0;
	e.count = 0;
	e.wait = 0;
	e.incframe();
}

static struct creature_drawable_plugin : public drawable::plugin
{
	void selecting(drawable** result, rect screen, unsigned flags) override
	{
		if((flags & DFCombat) == 0)
			return;
		auto p = result;
		for(int i = 0; i < combat_units.count; i++)
		{
			if(!objects[i].rec)
				continue;
			*p++ = objects + i;
		}
		*p = 0;
	}
} drawable_plugin;