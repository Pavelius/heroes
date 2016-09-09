#include "main.h"

// animate::heroshad(x1, y1 + 26, id, 0);
// animate::hero(x1, y1 + 26, id, 0);
// animate::heroflag(x1, y1 + 26, id, 0);

struct hero
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