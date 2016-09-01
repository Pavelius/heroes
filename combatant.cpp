#include "main.h"

static struct combatant : public animation
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
		bool ispenalized = game::isboosted(rec);
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
		if(count && action != Move)
		{
			draw::fontsm push;
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
		if(++frame >= start + count)
		{
			frame = start;
			if(action == Killed)
				frame = start + count - 1;
			else if(action != ActorWarn)
				setaction(ActorWarn);
			else
				stamp += xrand(2, 5) * 1000;
			return true;
		}
		return false;
	}

	point getzpos() const
	{
		point pt = pos;
		if(action == Killed && frame >= start + count - 1)
			pt.y -= 500;
		return pt;
	}

} objects[LastCombatant - FirstCombatant + 1];
static bsmeta::field fields[] = {
	BSREQ(combatant, rec, Type, Number),
	BSREQ(combatant, index, Index, Number),
	BSREQ(combatant, side, Side, Number),
	BSREQ(combatant, hits, HitPoints, Number),
	BSREQ(combatant, moved, AlreadyMoved, Number),
	BSREQ(combatant, defended, AlreadyDefended, Number),
	BSREQ(combatant, shoots, Shoots, Number),
	{0}
};
BSMETA(combatant, "Combatants", "Участники боя", FirstCombatant);

int combatant::getid() const
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
	setaction(rec, ActorWarn);
}

static void battle_initialize()
{
	combatants.clear();
}

static command battle_commands[] = {
	{"initialize", battle_initialize},
	{0}
};
static command::plugin commands_plugin("battle", battle_commands);

static struct creature_drawable_plugin : public drawable::plugin
{
	void selecting(drawable** result, unsigned flags) override
	{
		if((flags & 1) == 0)
			return;
		auto p = result;
		for(int i = 0; i < combatants.count; i++)
		{
			if(!objects[i].rec)
				continue;
			*p++ = objects + i;
		}
		*p = 0;
	}
} drawable_plugin;