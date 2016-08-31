#include "main.h"

static struct combatant : public animation
{
	int		index;
	int		side;
	int		hits;
	int		moved;
	int		defended;
	int		shooted;

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
		animation::painting(pt);
		// Draw count
		int rec = getid();
		int count = game::get(rec, Count);
		if(count)
		{
			draw::fontsm push;
			char temp[32];
			int x = pos.x + pt.x;
			int y = pos.y + pt.y;
			sznum(temp, count);
			int frame = getbarframe(rec);
			draw::image(x, y, res::TEXTBAR, frame);
			draw::text(x + (res::width(res::TEXTBAR, frame) - draw::textw(temp)) / 2, y + 2, temp);
		}
	}

} objects[LastCombatant - FirstCombatant + 1];
static bsmeta::field fields[] = {
	BSREQ(combatant, rec, Type, Number),
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

void combat::setaction(int rec, tokens action)
{
	auto side = bsget(rec, Side);
	auto& e = objects[rec - FirstCombatant];
	if(!isattacker(side))
		e.flags = AFMirror;
	e.set(tokens(e.rec), action, 0);
}

void combat::setpos(int rec, int index)
{
	auto& e = objects[rec - FirstCombatant];
	e.index = index;
	e.pos = combat::i2h(index);
	setaction(rec, ActorWait);
}

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