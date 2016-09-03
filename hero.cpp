#include "main.h"

struct hero
{
	const char*			name[2];
	int					attack;
	int					defence;
	int					spellpower;
	int					wisdow;
	short int			spell_points;
	short int			move_points;
	int					index;
	int					index_move;
	unsigned char		direction;
	unsigned char		portrait;
	int					player, recruit;
	unsigned			experience;
	unsigned char		skills[LastSkill - FirstSkill + 1];
	unsigned char		spells[LastSpell - FirstSpell + 1];
	unsigned short		params[LastTroopsIndex - FirstArtifactIndex + 1];
} objects[LastHero - FirstHero + 1] = {
	{{"Lord Killburn", "���� �������"}},{{"Sir Gallanth", "��� �������"}}, {{"Ector", "�����"}}, {{"Gvenneth", "�������"}}, {{"Tyro", "����"}},
	{{"Ambrose", "�������"}}, {{"Ruby", "����"}}, {{"Maximus", "��������"}}, {{"Dimitry", "��������"}},
	//
	{{"Thundax", "����������"}}, {{"Fineous", "�������������"}}, {{"Jojosh", "�������"}}, {{"Crag Hack", "���� ���"}}, {{"Jezebel", "��������"}},
	{{"Jaclyn", "������"}}, {{"Ergon", "�����"}}, {{"Tsabu", "�����"}}, {{"Atlas", "�����"}},
	//
	{{"Astra", "�����"}}, {{"Natasha", "������"}}, {{"Troyan", "�����"}}, {{"Vatawna", "��������"}}, {{"Rebecca", "������"}},
	{{"Gem", "����"}}, {{"Ariel", "������"}}, {{"Carlawn", "�������"}}, {{"Luna", "����"}},
	//
	{{"Arie", "���"}}, {{"Alamar", "������"}}, {{"Vesper", "������"}}, {{"Crodo", "�����"}}, {{"Barok", "�����"}},
	{{"Kastore", "������"}}, {{"Agar", "����"}}, {{"Falagar", "�������"}}, {{"Wrathmont", "��������"}},
	//
	{{"Myra", "����"}}, {{"Flint", "�����"}}, {{"Dawn", "����"}}, {{"Halon", "�����"}}, {{"Myrini", "������"}},
	{{"Wilfrey", "�������"}}, {{"Sarakin", "�������"}}, {{"Kalindra", "��������"}}, {{"Mandigal", "��������"}},
	//
	{{"Zom", "���"}}, {{"Darlana", "�������"}}, {{"Zam", "���"}}, {{"Ranloo", "�����"}}, {{"Charity", "������"}},
	{{"Rialdo", "������"}}, {{"Roxana", "�������"}}, {{"Sandro", "������"}}, {{"Celia", "�����"}},
};
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
	BSREQ(hero, recruit, Recruit, Number),
	BSREQ(hero, experience, Experience, Number),
	BSINT(hero, skills, FirstSkill, Number),
	BSINT(hero, spells, FirstSpell, Number),
	BSINT(hero, params, FirstArtifactIndex, Number),
};
BSMETA(hero, "Heroes", "�����", FirstHero);