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