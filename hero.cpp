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
	{{"Lord Killburn", "Лорд Килбурн"}},{{"Sir Gallanth", "Сэр Галлант"}}, {{"Ector", "Эктор"}}, {{"Gvenneth", "Гвиннет"}}, {{"Tyro", "Тиро"}},
	{{"Ambrose", "Амброуз"}}, {{"Ruby", "Руби"}}, {{"Maximus", "Максимус"}}, {{"Dimitry", "Димитрий"}},
	//
	{{"Thundax", "Громотопор"}}, {{"Fineous", "Прекраснейший"}}, {{"Jojosh", "Джоджош"}}, {{"Crag Hack", "Крэг Хек"}}, {{"Jezebel", "Жезебель"}},
	{{"Jaclyn", "Жаклин"}}, {{"Ergon", "Эргон"}}, {{"Tsabu", "Тсабу"}}, {{"Atlas", "Атлас"}},
	//
	{{"Astra", "Астра"}}, {{"Natasha", "Наташа"}}, {{"Troyan", "Троян"}}, {{"Vatawna", "Ваттавна"}}, {{"Rebecca", "Ребека"}},
	{{"Gem", "Джем"}}, {{"Ariel", "Ариэль"}}, {{"Carlawn", "Карлавн"}}, {{"Luna", "Луна"}},
	//
	{{"Arie", "Ари"}}, {{"Alamar", "Аламар"}}, {{"Vesper", "Веспер"}}, {{"Crodo", "Кродо"}}, {{"Barok", "Барок"}},
	{{"Kastore", "Кастор"}}, {{"Agar", "Агар"}}, {{"Falagar", "Фалагар"}}, {{"Wrathmont", "Врасмонт"}},
	//
	{{"Myra", "Мира"}}, {{"Flint", "Флинт"}}, {{"Dawn", "Давн"}}, {{"Halon", "Халон"}}, {{"Myrini", "Мирини"}},
	{{"Wilfrey", "Вилфрей"}}, {{"Sarakin", "Саракин"}}, {{"Kalindra", "Калиндра"}}, {{"Mandigal", "Мандигал"}},
	//
	{{"Zom", "Зом"}}, {{"Darlana", "Дарлана"}}, {{"Zam", "Зам"}}, {{"Ranloo", "Ранлу"}}, {{"Charity", "Чарити"}},
	{{"Rialdo", "Риалдо"}}, {{"Roxana", "Роксана"}}, {{"Sandro", "Сандро"}}, {{"Celia", "Целиа"}},
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
BSMETA(hero, "Heroes", "Герои", FirstHero);