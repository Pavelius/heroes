#include "main.h"

// animate::heroshad(x1, y1 + 26, id, 0);
// animate::hero(x1, y1 + 26, id, 0);
// animate::heroflag(x1, y1 + 26, id, 0);

static void paint_hero(int x, int y, int type, unsigned char direction, bool shipmaster)
{
	res::tokens icn = res::Empthy;
	int index = 0;
	if(shipmaster)
		icn = res::BOAT32;
	else switch(type)
	{
	case Knight: icn = res::KNGT32; break;
	case Barbarian: icn = res::BARB32; break;
	case Sorcerer: icn = res::SORC32; break;
	case Warlock: icn = res::WRLK32; break;
	case Wizard: icn = res::WZRD32; break;
	case Necromancer: icn = res::NECR32; break;
	default: return;
	}
	unsigned flags = 0;
	switch(direction)
	{
	case map::Up:          index = 0; break;
	case map::RightUp:     index = 9; break;
	case map::Right:       index = 18; break;
	case map::RightDown:   index = 27; break;
	case map::Down:        index = 36; break;
	case map::LeftDown:    index = 27; flags |= AFMirror; break;
	case map::Left:        index = 18; flags |= AFMirror; break;
	case map::LeftUp:      index = 9; flags |= AFMirror; break;
	default: break;
	}
	draw::image(x, y, icn, index, flags);
}

static void paint_flag(int x, int y, int player, unsigned char direction, int tick)
{
	res::tokens icn = res::Empthy;
	int index_sprite = 0;
	switch(player)
	{
	case PlayerBlue: icn = res::B_FLAG32; break;
	case PlayerGreen: icn = res::G_FLAG32; break;
	case PlayerRed: icn = res::R_FLAG32; break;
	case PlayerYellow: icn = res::Y_FLAG32; break;
	case PlayerOrange: icn = res::O_FLAG32; break;
	case PlayerPurple: icn = res::P_FLAG32; break;
	default: return;
	}
	unsigned flags = 0;
	switch(direction)
	{
	case map::Up:        index_sprite = 0; break;
	case map::RightUp:   index_sprite = 9; break;
	case map::Right:     index_sprite = 18; break;
	case map::RightDown: index_sprite = 27; break;
	case map::Down:      index_sprite = 36; break;
	case map::LeftDown:  index_sprite = 27; flags |= AFMirror; x += 8; break;
	case map::Left:      index_sprite = 18; flags |= AFMirror; x += 12; break;
	case map::LeftUp:    index_sprite = 9; flags |= AFMirror; x += 19; break;
	default: break;
	}
	draw::image(x, y, icn, index_sprite + (tick % 9), flags);
}

static void paint_shad(int x, int y, unsigned direction, int index)
{
	res::tokens icn = res::SHADOW32;
	int index_sprite = 0;
	switch(direction)
	{
	case map::Up:          index_sprite = 0; break;
	case map::RightUp:     index_sprite = 9; break;
	case map::Right:       index_sprite = 18; break;
	case map::RightDown:   index_sprite = 27; break;
	case map::Down:        index_sprite = 36; break;
	case map::LeftDown:    index_sprite = 45; break;
	case map::Left:        index_sprite = 54; break;
	case map::LeftUp:      index_sprite = 63; break;
	default: break;
	}
	draw::image(x, y, icn, index_sprite + (index % 9));
}

struct hero : public drawable
{
	char				name[32];
	unsigned char		level;
	unsigned char		attack;
	unsigned char		defence;
	unsigned char		spellpower;
	unsigned char		wisdow;
	short unsigned		spell_points;
	short unsigned		move_points;
	short unsigned		index;
	short unsigned		index_move;
	unsigned char		direction;
	unsigned char		portrait;
	int					player;
	unsigned			experience;
	unsigned char		skills[LastSkill - FirstSkill + 1];
	unsigned char		spells[LastSpell - FirstSpell + 1];
	unsigned short		army[LastTroopsIndex - FirstArtifactIndex + 1];

	int getid() const override;

	point getpos() const
	{
		return{(short)(map::i2x(index) * 32), (short)(map::i2y(index) * 32)};
	}

	// Fast clipping rect. For hit test use othe method.
	rect getrect() const override
	{
		auto x = map::i2x(index) * 32;
		auto y = map::i2y(index) * 32;
		return{x - 16, y - 16, x + 32 + 16, y + 32 + 16};
	}

	point getzpos() const override
	{
		return{(short)(map::i2x(index) * 32), (short)(map::i2y(index) * 32) + 32};
	}

	void painting(point camera, unsigned paint_flags) const override
	{
		auto pt = getpos() - camera;
		pt.y += 30;
		int rec = getid();
		paint_hero(pt.x, pt.y, game::get(rec, Type), direction, map::gettile(index)==Water);
		paint_flag(pt.x, pt.y, bsget(rec, Player), direction, 0);
		paint_shad(pt.x, pt.y, direction, 0);
	}

} objects[LastHero - FirstHero + 1];
static bsmeta::field fields[] = {
	BSREQ(hero, name, Name, Text),
	BSREQ(hero, attack, Attack, Number),
	BSREQ(hero, defence, Defence, Number),
	BSREQ(hero, spellpower, SpellPower, Number),
	BSREQ(hero, wisdow, Wisdow, Number),
	BSREQ(hero, spell_points, SpellPoints, Number),
	BSREQ(hero, move_points, MovePoints, Number),
	BSREQ(hero, index, Index, Number),
	BSREQ(hero, direction, Direction, Number),
	BSREQ(hero, portrait, Portrait, Number),
	BSREQ(hero, player, Player, Number),
	BSREQ(hero, experience, Experience, Number),
	BSINT(hero, skills, FirstSkill, Number),
	BSINT(hero, spells, FirstSpell, Number),
	BSINT(hero, army, FirstArtifactIndex, Number),
};
BSMETA(hero, "Heroes", "Герои", FirstHero);

int hero::getid() const
{
	return FirstHero + (this - objects);
}

static struct hero_drawable_plugin : public drawable::plugin
{
	void selecting(drawable** result, rect screen, unsigned flags) override
	{
		auto mode = flags&DWMask;
		if(mode != DWObjects)
			return;
		auto p = result;
		for(auto& e : objects)
		{
			if(!e.player)
				continue;
			if(!e.getrect().intersect(screen))
				continue;
			*p++ = &e;
		}
		*p = 0;
	}
} drawable_plugin;