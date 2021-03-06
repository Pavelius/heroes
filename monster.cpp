#include "main.h"

static struct monster
{
	unsigned char	level;
	unsigned char	animtype;
	unsigned char	attack;
	unsigned char	defence;
	unsigned char	damageMin;
	unsigned char	damageMax;
	unsigned short	hp;
	unsigned char	speed;
	unsigned char	grown;
	unsigned char	shoots;
	const char*		name[2];
	const char*		multiname[2];
	int				cost[LastResource - FirstResource + 1];
	int				rating;
} objects[LastMonster - FirstMonster + 1] =
{
	{1, 0, 1, 1, 1, 1, 1, 1, 12, 0, {"Peasant", "����������"}, {"Peasants", "��������"}, {20, 0, 0, 0, 0, 0, 0}},
	{1, 0, 5, 3, 2, 3, 10, 1, 8, 12, {"Archer", "������"}, {"Archers", "��������"}, {150, 0, 0, 0, 0, 0, 0}},
	{2, 0, 5, 3, 2, 3, 10, 3, 8, 24, {"Ranger", "��������"}, {"Rangers", "����������"}, {200, 0, 0, 0, 0, 0, 0}},
	{2, 0, 5, 9, 3, 4, 15, 3, 5, 0, {"Pikeman", "��������"}, {"Pikemen", "����������"}, {200, 0, 0, 0, 0, 0, 0}},
	{2, 0, 5, 9, 3, 4, 20, 4, 5, 0, {"Veteran Pikeman", "������� ��������"}, {"Veteran Pikemen", "��������� ����������"}, {250, 0, 0, 0, 0, 0, 0}},
	{3, 0, 7, 9, 4, 6, 25, 3, 4, 0, {"Swordsman", "������"}, {"Swordsmen", "��������"}, {250, 0, 0, 0, 0, 0, 0}},
	{3, 0, 7, 9, 4, 6, 30, 4, 4, 0, {"Master Swordsman", "������ ������"}, {"Master Swordsmen", "�������� ��������"}, {300, 0, 0, 0, 0, 0, 0}},
	{3, 0, 10, 9, 5, 10, 30, 5, 3, 0, {"Cavalry", "����������"}, {"Cavalries", "���������"}, {300, 0, 0, 0, 0, 0, 0}},
	{3, 0, 10, 9, 5, 10, 40, 6, 3, 0, {"Champion", "�������"}, {"Champions", "���������"}, {375, 0, 0, 0, 0, 0, 0}},
	{4, 0, 11, 12, 10, 20, 50, 4, 2, 0, {"Paladin", "�������"}, {"Paladins", "���������"}, {600, 0, 0, 0, 0, 0, 0}},
	{4, 0, 11, 12, 10, 20, 65, 5, 2, 0, {"Crusader", "�����������"}, {"Crusaders", "������������"}, {1000, 0, 0, 0, 0, 0, 0}},
	//
	{1, 0, 3, 1, 1, 2, 3, 3, 10, 0, {"Goblin", "������"}, {"Goblins", "��������"}, {40, 0, 0, 0, 0, 0, 0}},
	{1, 0, 3, 4, 2, 3, 10, 1, 8, 8, {"Orc", "���"}, {"Orcs", "�����"}, {140, 0, 0, 0, 0, 0, 0}},
	{2, 0, 3, 4, 3, 4, 15, 2, 8, 16, {"Orc Chief", "������ ���"}, {"Orc Chiefs", "������ �����"}, {175, 0, 0, 0, 0, 0, 0}},
	{2, 0, 6, 2, 3, 5, 20, 5, 5, 0, {"Wolf", "����"}, {"Wolves", "������"}, {200, 0, 0, 0, 0, 0, 0}},
	{3, 0, 9, 5, 4, 6, 40, 1, 4, 0, {"Ogre", "���"}, {"Ogres", "�����"}, {300, 0, 0, 0, 0, 0, 0}},
	{3, 0, 9, 5, 5, 7, 60, 3, 4, 0, {"Ogre Lord", "������� ���"}, {"Ogre Lords", "������� �����"}, {500, 0, 0, 0, 0, 0, 0}},
	{3, 0, 10, 5, 5, 7, 40, 3, 3, 8, {"Troll", "������"}, {"Trolls", "�������"}, {600, 0, 0, 0, 0, 0, 0}},
	{3, 0, 10, 5, 7, 9, 40, 4, 3, 16, {"War Troll", "������ ������"}, {"War Trolls", "������ �������"}, {700, 0, 0, 0, 0, 0, 0}},
	{4, 0, 12, 9, 12, 24, 80, 4, 2, 0, {"Cyclops", "������"}, {"Cyclopes", "��������"}, {750, 0, 0, 0, 0, 1, 0}},
	//
	{1, 0, 4, 2, 1, 2, 2, 3, 8, 0, {"Sprite", "���"}, {"Sprites", "���"}, {50, 0, 0, 0, 0, 0, 0}},
	{2, 0, 6, 5, 2, 4, 20, 1, 6, 0, {"Dwarf", "�����"}, {"Dwarves", "�������"}, {200, 0, 0, 0, 0, 0, 0}},
	{2, 0, 6, 6, 2, 4, 20, 3, 6, 0, {"Battle Dwarf", "������ �����"}, {"Battle Dwarves", "������ �������"}, {250, 0, 0, 0, 0, 0, 0}},
	{2, 0, 4, 3, 2, 3, 15, 3, 4, 24, {"Elf", "����"}, {"Elves", "������"}, {250, 0, 0, 0, 0, 0, 0}},
	{2, 0, 5, 5, 2, 3, 15, 5, 4, 24, {"Grand Elf", "������� ����"}, {"Grand Elves", "������� ������"}, {300, 0, 0, 0, 0, 0, 0}},
	{3, 0, 7, 5, 5, 8, 25, 4, 3, 8, {"Druid", "�����"}, {"Druids", "�������"}, {350, 0, 0, 0, 0, 0, 0}},
	{3, 0, 7, 7, 5, 8, 25, 5, 3, 16, {"Greater Druid", "������� �����"}, {"Greater Druids", "������� �������"}, {400, 0, 0, 0, 0, 0, 0}},
	{4, 0, 10, 9, 7, 14, 40, 4, 2, 0, {"Unicorn", "��������"}, {"Unicorns", "����������"}, {500, 0, 0, 0, 0, 0, 0}},
	{4, 0, 12, 10, 20, 40, 100, 6, 1, 0, {"Phoenix", "������"}, {"Phoenix", "��������"}, {1500, 0, 1, 0, 0, 0, 0}},
	//
	{1, 0, 3, 1, 1, 2, 5, 3, 8, 8, {"Centaur", "�������"}, {"Centaurs", "���������"}, {60, 0, 0, 0, 0, 0, 0}},
	{2, 0, 4, 7, 2, 3, 15, 5, 6, 0, {"Gargoyle", "��������"}, {"Gargoyles", "��������"}, {200, 0, 0, 0, 0, 0, 0}},
	{3, 0, 6, 6, 3, 5, 25, 3, 4, 0, {"Griffin", "������"}, {"Griffins", "��������"}, {300, 0, 0, 0, 0, 0, 0}},
	{3, 0, 9, 8, 5, 10, 35, 3, 3, 0, {"Minotaur", "��������"}, {"Minotaurs", "����������"}, {400, 0, 0, 0, 0, 0, 0}},
	{3, 0, 9, 8, 5, 10, 45, 5, 3, 0, {"Minotaur King", "������ ��������"}, {"Minotaur Kings", "������ ����������"}, {500, 0, 0, 0, 0, 0, 0}},
	{4, 0, 8, 9, 6, 12, 75, 1, 2, 0, {"Hydra", "�����"}, {"Hydras", "����"}, {800, 0, 0, 0, 0, 0, 0}},
	{4, 0, 12, 12, 25, 50, 200, 3, 1, 0, {"Green Dragon", "������� ������"}, {"Green Dragons", "������� ��������"}, {3000, 0, 0, 0, 1, 0, 0}},
	{4, 0, 13, 13, 25, 50, 250, 4, 1, 0, {"Red Dragon", "������� ������"}, {"Red Dragons", "������� ��������"}, {3500, 0, 0, 0, 1, 0, 0}},
	{4, 0, 14, 14, 25, 50, 300, 5, 1, 0, {"Black Dragon", "������ ������"}, {"Black Dragons", "������ ��������"}, {4000, 0, 0, 0, 2, 0, 0}},
	//
	{1, 0, 2, 1, 1, 3, 3, 2, 8, 12, {"Halfling", "����������"}, {"Halflings", "������������"}, {50, 0, 0, 0, 0, 0, 0}},
	{2, 0, 5, 4, 2, 3, 15, 5, 6, 0, {"Boar", "�����"}, {"Boars", "�������"}, {150, 0, 0, 0, 0, 0, 0}},
	{2, 0, 5, 10, 4, 5, 30, 1, 4, 0, {"Iron Golem", "�������� �����"}, {"Iron Golems", "�������� �������"}, {300, 0, 0, 0, 0, 0, 0}},
	{3, 0, 7, 10, 4, 5, 35, 2, 4, 0, {"Steel Golem", "�������� �����"}, {"Steel Golems", "�������� �������"}, {350, 0, 0, 0, 0, 0, 0}},
	{3, 0, 7, 7, 4, 8, 40, 3, 3, 0, {"Roc", "����"}, {"Rocs", "�����"}, {400, 0, 0, 0, 0, 0, 0}},
	{3, 0, 11, 7, 7, 9, 30, 4, 2, 12, {"Mage", "���"}, {"Magi", "�����"}, {600, 0, 0, 0, 0, 0, 0}},
	{3, 0, 12, 8, 7, 9, 35, 5, 2, 24, {"Archmage", "�������"}, {"Archmagi", "���������"}, {700, 0, 0, 0, 0, 0, 0}},
	{4, 0, 13, 10, 20, 30, 150, 3, 1, 0, {"Giant", "������"}, {"Giants", "��������"}, {2000, 0, 0, 0, 0, 0, 1}},
	{4, 0, 15, 15, 20, 30, 300, 5, 1, 24, {"Titan", "�����"}, {"Titans", "�������"}, {5000, 0, 0, 0, 0, 0, 2}},
	//
	{1, 0, 4, 3, 2, 3, 4, 3, 8, 0, {"Skeleton", "������"}, {"Skeletons", "��������"}, {75, 0, 0, 0, 0, 0, 0}},
	{1, 1, 5, 2, 2, 3, 15, 1, 6, 0, {"Zombie", "�����"}, {"Zombies", "�����"}, {150, 0, 0, 0, 0, 0, 0}},
	{2, 1, 5, 2, 2, 3, 25, 3, 6, 0, {"Mutant Zombie", "����� ������"}, {"Mutant Zombies", "����� ��������"}, {200, 0, 0, 0, 0, 0, 0}},
	{2, 0, 6, 6, 3, 4, 25, 3, 4, 0, {"Mummy", "�����"}, {"Mummies", "�����"}, {250, 0, 0, 0, 0, 0, 0}},
	{3, 0, 6, 6, 3, 4, 30, 4, 4, 0, {"Royal Mummy", "���������� �����"}, {"Royal Mummies", "����������� �����"}, {300, 0, 0, 0, 0, 0, 0}},
	{3, 0, 8, 6, 5, 7, 30, 3, 3, 0, {"Vampire", "������"}, {"Vampires", "��������"}, {500, 0, 0, 0, 0, 0, 0}},
	{3, 0, 8, 6, 5, 7, 40, 4, 3, 0, {"Vampire Lord", "������-����"}, {"Vampire Lords", "������ ��������"}, {650, 0, 0, 0, 0, 0, 0}},
	{3, 0, 7, 12, 8, 10, 25, 4, 2, 12, {"Lich", "���"}, {"Liches", "�����"}, {750, 0, 0, 0, 0, 0, 0}},
	{4, 0, 7, 13, 8, 10, 35, 5, 2, 24, {"Power Lich", "������� ���"}, {"Power Liches", "������� �����"}, {900, 0, 0, 0, 0, 0, 0}},
	{4, 0, 11, 9, 25, 45, 150, 3, 1, 0, {"Bone Dragon", "�������� ������"}, {"Bone Dragons", "�������� ��������"}, {1500, 0, 0, 0, 0, 0, 0}},
	//
	{1, 0, 6, 1, 1, 2, 4, 4, 4, 0, {"Rogue", "������"}, {"Rogues", "��������"}, {50, 0, 0, 0, 0, 0, 0}},
	{2, 0, 7, 6, 2, 5, 20, 5, 4, 0, {"Nomad", "��������"}, {"Nomads", "����������"}, {200, 0, 0, 0, 0, 0, 0}},
	{3, 0, 8, 7, 4, 6, 20, 4, 4, 0, {"Ghost", "�������"}, {"Ghosts", "���������"}, {1000, 0, 0, 0, 0, 0, 0}},
	{4, 0, 10, 9, 20, 30, 50, 5, 4, 0, {"Genie", "����"}, {"Genies", "������"}, {650, 0, 0, 0, 0, 0, 1}},
	{3, 0, 8, 9, 6, 10, 35, 3, 4, 0, {"Medusa", "������"}, {"Medusas", "�����"}, {500, 0, 0, 0, 0, 0, 0}},
	{3, 0, 8, 8, 4, 5, 50, 2, 4, 0, {"Earth Elemental", "�������� ����������"}, {"Earth Elementals", "�������� �����������"}, {500, 0, 0, 0, 0, 0, 0}},
	{3, 0, 7, 7, 2, 8, 35, 5, 4, 0, {"Air Elemental", "��������� ����������"}, {"Air Elementals", "��������� �����������"}, {500, 0, 0, 0, 0, 0, 0}},
	{3, 0, 8, 6, 4, 6, 40, 4, 4, 0, {"Fire Elemental", "�������� ����������"}, {"Fire Elementals", "�������� �����������"}, {500, 0, 0, 0, 0, 0, 0}},
	{3, 0, 6, 8, 3, 7, 45, 3, 4, 0, {"Water Elemental", "������ ����������"}, {"Water Elementals", "������ �����������"}, {500, 0, 0, 0, 0, 0, 0}},
	//
	{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, {"Random Monster"}, {"Random Monsters"}, {0, 0, 0, 0, 0, 0, 0}},
	{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, {"Random Monster 1"}, {"Random Monsters 1"}, {0, 0, 0, 0, 0, 0, 0}},
	{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, {"Random Monster 2"}, {"Random Monsters 2"}, {0, 0, 0, 0, 0, 0, 0}},
	{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, {"Random Monster 3"}, {"Random Monsters 3"}, {0, 0, 0, 0, 0, 0, 0}},
	{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, {"Random Monster 4"}, {"Random Monsters 4"}, {0, 0, 0, 0, 0, 0, 0}},
};
static bsmeta::field fields[] = {
	BSREQ(monster, name, Name, Text),
	BSREQ(monster, multiname, NameMulti, Text),
	BSREQ(monster, attack, Attack, Number),
	BSREQ(monster, defence, Defence, Number),
	BSREQ(monster, damageMin, DamageMin, Number),
	BSREQ(monster, damageMax, DamageMax, Number),
	BSREQ(monster, hp, HitPointsMax, Number),
	BSREQ(monster, shoots, Shoots, Number),
	BSREQ(monster, speed, Speed, Number),
	BSREQ(monster, level, Level, Number),
	BSINT(monster, cost, FirstResource, Number),
	BSREQ(monster, rating, Rating, Number),
};
BSMETA(monster, "Monsters", "�������", FirstMonster);

bool game::istwiceattack(int rec)
{
	if(rec >= FirstCombatant && rec <= LastCombatant)
		rec = bsget(rec, Type);
	switch(rec)
	{
	case Ranger:
	case Crusader:
	case Paladin:
	case Wolf:
	case Elf:
	case GrandElf:
		return true;
	default:
		return false;
	}
}

bool game::isstealth(int rec)
{
	if(rec >= FirstCombatant && rec <= LastCombatant)
		rec = bsget(rec, Type);
	switch(rec)
	{
	case Sprite:
	case Hydra:
	case Vampire:
	case VampireLord:
	case Rogue:
		return true;
	default:
		return false;
	}
}

bool game::ismeleearcher(int rec)
{
	if(rec >= FirstCombatant && rec <= LastCombatant)
		rec = bsget(rec, Type);
	switch(rec)
	{
	case Mage:
	case ArchMage:
	case Lich:
	case PowerLich:
		return true;
	default:
		return false;
	}
}

bool game::iswide(int rec)
{
	if(rec >= FirstCombatant && rec <= LastCombatant)
		rec = bsget(rec, Type);
	switch(rec)
	{
	case Cavalry:
	case Champion:
	case Wolf:
	case Unicorn:
	case Phoenix:
	case Centaur:
	case Griffin:
	case Hydra:
	case GreenDragon:
	case RedDragon:
	case BlackDragon:
	case Boar:
	case Roc:
	case BoneDragon:
	case Nomand:
	case Medusa:
		return true;
	default:
		return false;
	}
}

bool game::isarcher(int rec)
{
	if(rec >= FirstCombatant && rec <= LastCombatant)
		rec = bsget(rec, Type);
	return bsget(rec, Shoots) != 0;
}

bool game::isfly(int rec)
{
	if(rec >= FirstCombatant && rec <= LastCombatant)
		rec = bsget(rec, Type);
	switch(rec)
	{
	case Sprite:
	case Phoenix:
	case Gargoyle:
	case Griffin:
	case GreenDragon:
	case RedDragon:
	case BlackDragon:
	case Roc:
	case Vampire:
	case VampireLord:
	case BoneDragon:
	case Ghost:
	case Genie:
		return true;
	default:
		return false;
	}
}

//	case Undead:
//		switch(rec)
//		{
//		case Skeleton:
//		case Zombie:
//		case MutantZombie:
//		case Mummy:
//		case RoyalMummy:
//		case Vampire:
//		case VampireLord:
//		case Lich:
//		case PowerLich:
//		case BoneDragon:
//		case Ghost:
//			return 1;
//		default:
//			return 0;
//		}
//	case Dragon:
//		switch(rec)
//		{
//		case BoneDragon:
//		case BlackDragon:
//		case RedDragon:
//		case GreenDragon:
//			return 1;
//		default:
//			return 0;
//		}
//	case ElementsImmunity:
//		switch(rec)
//		{
//		case Phoenix:
//			return 1;
//		default:
//			return 0;
//		}