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
	unsigned char	shots;
	const char*		name[2];
	const char*		multiname[2];
	struct cost		cost;
} objects[LastMonster - FirstMonster + 1] =
{
	{1, 0, 1, 1, 1, 1, 1, VerySlow, 12, 0, {"Peasant", "����������"}, {"Peasants", "��������"}, {20, 0, 0, 0, 0, 0, 0}},
	{1, 0, 5, 3, 2, 3, 10, VerySlow, 8, 12, {"Archer", "������"}, {"Archers", "��������"}, {150, 0, 0, 0, 0, 0, 0}},
	{2, 0, 5, 3, 2, 3, 10, Average, 8, 24, {"Ranger", "��������"}, {"Rangers", "����������"}, {200, 0, 0, 0, 0, 0, 0}},
	{2, 0, 5, 9, 3, 4, 15, Average, 5, 0, {"Pikeman", "��������"}, {"Pikemen", "����������"}, {200, 0, 0, 0, 0, 0, 0}},
	{2, 0, 5, 9, 3, 4, 20, Fast, 5, 0, {"Veteran Pikeman", "������� ��������"}, {"Veteran Pikemen", "��������� ����������"}, {250, 0, 0, 0, 0, 0, 0}},
	{3, 0, 7, 9, 4, 6, 25, Average, 4, 0, {"Swordsman", "������"}, {"Swordsmen", "��������"}, {250, 0, 0, 0, 0, 0, 0}},
	{3, 0, 7, 9, 4, 6, 30, Fast, 4, 0, {"Master Swordsman", "������ ������"}, {"Master Swordsmen", "�������� ��������"}, {300, 0, 0, 0, 0, 0, 0}},
	{3, 0, 10, 9, 5, 10, 30, VeryFast, 3, 0, {"Cavalry", "����������"}, {"Cavalries", "���������"}, {300, 0, 0, 0, 0, 0, 0}},
	{3, 0, 10, 9, 5, 10, 40, UltraFast, 3, 0, {"Champion", "�������"}, {"Champions", "���������"}, {375, 0, 0, 0, 0, 0, 0}},
	{4, 0, 11, 12, 10, 20, 50, Fast, 2, 0, {"Paladin", "�������"}, {"Paladins", "���������"}, {600, 0, 0, 0, 0, 0, 0}},
	{4, 0, 11, 12, 10, 20, 65, VeryFast, 2, 0, {"Crusader", "�����������"}, {"Crusaders", "������������"}, {1000, 0, 0, 0, 0, 0, 0}},
	//
	{1, 0, 3, 1, 1, 2, 3, Average, 10, 0, {"Goblin", "������"}, {"Goblins", "��������"}, {40, 0, 0, 0, 0, 0, 0}},
	{1, 0, 3, 4, 2, 3, 10, VerySlow, 8, 8, {"Orc", "���"}, {"Orcs"}, {140, 0, 0, 0, 0, 0, 0}},
	{2, 0, 3, 4, 3, 4, 15, Slow, 8, 16, {"Orc Chief", "������ ���"}, {"Orc Chiefs", "������ �����"}, {175, 0, 0, 0, 0, 0, 0}},
	{2, 0, 6, 2, 3, 5, 20, VeryFast, 5, 0, {"Wolf", "����"}, {"Wolves", "������"}, {200, 0, 0, 0, 0, 0, 0}},
	{3, 0, 9, 5, 4, 6, 40, VerySlow, 4, 0, {"Ogre", "���"}, {"Ogres", "�����"}, {300, 0, 0, 0, 0, 0, 0}},
	{3, 0, 9, 5, 5, 7, 60, Average, 4, 0, {"Ogre Lord"}, {"Ogre Lords"}, {500, 0, 0, 0, 0, 0, 0}},
	{3, 0, 10, 5, 5, 7, 40, Average, 3, 8, {"Troll", "������"}, {"Trolls", "�������"}, {600, 0, 0, 0, 0, 0, 0}},
	{3, 0, 10, 5, 7, 9, 40, Fast, 3, 16, {"War Troll"}, {"War Trolls"}, {700, 0, 0, 0, 0, 0, 0}},
	{4, 0, 12, 9, 12, 24, 80, Fast, 2, 0, {"Cyclops", "������"}, {"Cyclopes", "��������"}, {750, 0, 0, 0, 0, 1, 0}},
	//
	{1, 0, 4, 2, 1, 2, 2, Average, 8, 0, {"Sprite"}, {"Sprites"}, {50, 0, 0, 0, 0, 0, 0}},
	{2, 0, 6, 5, 2, 4, 20, VerySlow, 6, 0, {"Dwarf"}, {"Dwarves"}, {200, 0, 0, 0, 0, 0, 0}},
	{2, 0, 6, 6, 2, 4, 20, Average, 6, 0, {"Battle Dwarf"}, {"Battle Dwarves"}, {250, 0, 0, 0, 0, 0, 0}},
	{2, 0, 4, 3, 2, 3, 15, Average, 4, 24, {"Elf"}, {"Elves"}, {250, 0, 0, 0, 0, 0, 0}},
	{2, 0, 5, 5, 2, 3, 15, VeryFast, 4, 24, {"Grand Elf"}, {"Grand Elves"}, {300, 0, 0, 0, 0, 0, 0}},
	{3, 0, 7, 5, 5, 8, 25, Fast, 3, 8, {"Druid"}, {"Druids"}, {350, 0, 0, 0, 0, 0, 0}},
	{3, 0, 7, 7, 5, 8, 25, VeryFast, 3, 16, {"Greater Druid"}, {"Greater Druids"}, {400, 0, 0, 0, 0, 0, 0}},
	{4, 0, 10, 9, 7, 14, 40, Fast, 2, 0, {"Unicorn"}, {"Unicorns"}, {500, 0, 0, 0, 0, 0, 0}},
	{4, 0, 12, 10, 20, 40, 100, UltraFast, 1, 0, {"Phoenix"}, {"Phoenix"}, {1500, 0, 1, 0, 0, 0, 0}},
	//
	{1, 0, 3, 1, 1, 2, 5, Average, 8, 8, {"Centaur", "�������"}, {"Centaurs", "���������"}, {60, 0, 0, 0, 0, 0, 0}},
	{2, 0, 4, 7, 2, 3, 15, VeryFast, 6, 0, {"Gargoyle", "��������"}, {"Gargoyles", "��������"}, {200, 0, 0, 0, 0, 0, 0}},
	{3, 0, 6, 6, 3, 5, 25, Average, 4, 0, {"Griffin"}, {"Griffins"}, {300, 0, 0, 0, 0, 0, 0}},
	{3, 0, 9, 8, 5, 10, 35, Average, 3, 0, {"Minotaur"}, {"Minotaurs"}, {400, 0, 0, 0, 0, 0, 0}},
	{3, 0, 9, 8, 5, 10, 45, VeryFast, 3, 0, {"Minotaur King"}, {"Minotaur Kings"}, {500, 0, 0, 0, 0, 0, 0}},
	{4, 0, 8, 9, 6, 12, 75, VerySlow, 2, 0, {"Hydra"}, {"Hydras"}, {800, 0, 0, 0, 0, 0, 0}},
	{4, 0, 12, 12, 25, 50, 200, Average, 1, 0, {"Green Dragon"}, {"Green Dragons"}, {3000, 0, 0, 0, 1, 0, 0}},
	{4, 0, 13, 13, 25, 50, 250, Fast, 1, 0, {"Red Dragon"}, {"Red Dragons"}, {3500, 0, 0, 0, 1, 0, 0}},
	{4, 0, 14, 14, 25, 50, 300, VeryFast, 1, 0, {"Black Dragon"}, {"Black Dragons"}, {4000, 0, 0, 0, 2, 0, 0}},
	//
	{1, 0, 2, 1, 1, 3, 3, Slow, 8, 12, {"Halfling"}, {"Halflings"}, {50, 0, 0, 0, 0, 0, 0}},
	{2, 0, 5, 4, 2, 3, 15, VeryFast, 6, 0, {"Boar"}, {"Boars"}, {150, 0, 0, 0, 0, 0, 0}},
	{2, 0, 5, 10, 4, 5, 30, VerySlow, 4, 0, {"Iron Golem"}, {"Iron Golems"}, {300, 0, 0, 0, 0, 0, 0}},
	{3, 0, 7, 10, 4, 5, 35, Slow, 4, 0, {"Steel Golem"}, {"Steel Golems"}, {350, 0, 0, 0, 0, 0, 0}},
	{3, 0, 7, 7, 4, 8, 40, Average, 3, 0, {"Roc", "����"}, {"Rocs", "�����"}, {400, 0, 0, 0, 0, 0, 0}},
	{3, 0, 11, 7, 7, 9, 30, Fast, 2, 12, {"Mage"}, {"Magi"}, {600, 0, 0, 0, 0, 0, 0}},
	{3, 0, 12, 8, 7, 9, 35, VeryFast, 2, 24, {"Archmage"}, {"Archmagi"}, {700, 0, 0, 0, 0, 0, 0}},
	{4, 0, 13, 10, 20, 30, 150, Average, 1, 0, {"Giant"}, {"Giants"}, {2000, 0, 0, 0, 0, 0, 1}},
	{4, 0, 15, 15, 20, 30, 300, VeryFast, 1, 24, {"Titan"}, {"Titans"}, {5000, 0, 0, 0, 0, 0, 2}},
	//
	{1, 0, 4, 3, 2, 3, 4, Average, 8, 0, {"Skeleton", "������"}, {"Skeletons", "�������"}, {75, 0, 0, 0, 0, 0, 0}},
	{1, 1, 5, 2, 2, 3, 15, VerySlow, 6, 0, {"Zombie", "�����"}, {"Zombies", "�����"}, {150, 0, 0, 0, 0, 0, 0}},
	{2, 1, 5, 2, 2, 3, 25, Average, 6, 0, {"Mutant Zombie", "����� ������"}, {"Mutant Zombies", "����� �������"}, {200, 0, 0, 0, 0, 0, 0}},
	{2, 0, 6, 6, 3, 4, 25, Average, 4, 0, {"Mummy", "�����"}, {"Mummies", "�����"}, {250, 0, 0, 0, 0, 0, 0}},
	{3, 0, 6, 6, 3, 4, 30, Fast, 4, 0, {"Royal Mummy"}, {"Royal Mummies"}, {300, 0, 0, 0, 0, 0, 0}},
	{3, 0, 8, 6, 5, 7, 30, Average, 3, 0, {"Vampire"}, {"Vampires"}, {500, 0, 0, 0, 0, 0, 0}},
	{3, 0, 8, 6, 5, 7, 40, Fast, 3, 0, {"Vampire Lord"}, {"Vampire Lords"}, {650, 0, 0, 0, 0, 0, 0}},
	{3, 0, 7, 12, 8, 10, 25, Fast, 2, 12, {"Lich"}, {"Liches"}, {750, 0, 0, 0, 0, 0, 0}},
	{4, 0, 7, 13, 8, 10, 35, VeryFast, 2, 24, {"Power Lich"}, {"Power Liches"}, {900, 0, 0, 0, 0, 0, 0}},
	{4, 0, 11, 9, 25, 45, 150, Average, 1, 0, {"Bone Dragon"}, {"Bone Dragons"}, {1500, 0, 0, 0, 0, 0, 0}},
	//
	{1, 0, 6, 1, 1, 2, 4, Fast, 4, 0, {"Rogue", "������"}, {"Rogues", "��������"}, {50, 0, 0, 0, 0, 0, 0}},
	{2, 0, 7, 6, 2, 5, 20, VeryFast, 4, 0, {"Nomad", "��������"}, {"Nomads", "����������"}, {200, 0, 0, 0, 0, 0, 0}},
	{3, 0, 8, 7, 4, 6, 20, Fast, 4, 0, {"Ghost", "�������"}, {"Ghosts", "���������"}, {1000, 0, 0, 0, 0, 0, 0}},
	{4, 0, 10, 9, 20, 30, 50, VeryFast, 4, 0, {"Genie"}, {"Genies"}, {650, 0, 0, 0, 0, 0, 1}},
	{3, 0, 8, 9, 6, 10, 35, Average, 4, 0, {"Medusa"}, {"Medusas"}, {500, 0, 0, 0, 0, 0, 0}},
	{3, 0, 8, 8, 4, 5, 50, Slow, 4, 0, {"Earth Elemental"}, {"Earth Elementals"}, {500, 0, 0, 0, 0, 0, 0}},
	{3, 0, 7, 7, 2, 8, 35, VeryFast, 4, 0, {"Air Elemental"}, {"Air Elementals"}, {500, 0, 0, 0, 0, 0, 0}},
	{3, 0, 8, 6, 4, 6, 40, Fast, 4, 0, {"Fire Elemental"}, {"Fire Elementals"}, {500, 0, 0, 0, 0, 0, 0}},
	{3, 0, 6, 8, 3, 7, 45, Average, 4, 0, {"Water Elemental"}, {"Water Elementals"}, {500, 0, 0, 0, 0, 0, 0}},
	//
	{0, 0, 0, 0, 0, 0, 0, VerySlow, 0, 0, {"Random Monster"}, {"Random Monsters"}, {0, 0, 0, 0, 0, 0, 0}},
	{0, 0, 0, 0, 0, 0, 0, VerySlow, 0, 0, {"Random Monster 1"}, {"Random Monsters 1"}, {0, 0, 0, 0, 0, 0, 0}},
	{0, 0, 0, 0, 0, 0, 0, VerySlow, 0, 0, {"Random Monster 2"}, {"Random Monsters 2"}, {0, 0, 0, 0, 0, 0, 0}},
	{0, 0, 0, 0, 0, 0, 0, VerySlow, 0, 0, {"Random Monster 3"}, {"Random Monsters 3"}, {0, 0, 0, 0, 0, 0, 0}},
	{0, 0, 0, 0, 0, 0, 0, VerySlow, 0, 0, {"Random Monster 4"}, {"Random Monsters 4"}, {0, 0, 0, 0, 0, 0, 0}},
};
static bsmeta::field fields[] = {
	BSREQ(monster, name, Name, Text),
	BSREQ(monster, multiname, NameMulti, Text),
	BSREQ(monster, attack, Attack, Number),
	BSREQ(monster, defence, Defence, Number),
	BSREQ(monster, speed, Speed, Number),
	BSREQ(monster, level, Level, Number),
};
BSMETA(monster, "Monsters", "�������", FirstMonster);


static int object_get(int rec, int id)
{
	switch(id)
	{
	case Morale:
		switch(rec)
		{
		case MinotaurKing:
		case BattleDwarf:
			return MoraleGood;
		default:
			return MoraleNormal;
		}
	case Gold:
		return objects[rec - FirstMonster].cost.gold;
	case HitPointsMax:
		return objects[rec - FirstMonster].hp;
	case Shoots:
		return objects[rec - FirstMonster].shots;
	case AnimationType:
		return objects[rec - FirstMonster].animtype;
	case AllAttackAnswer:
		switch(rec)
		{
		case Griffin:
			return 1;
		default:
			return 0;
		}
	case Downgrade:
		switch(rec)
		{
		case Ranger: return Archer;
		case VeteranPikeman: return Pikeman;
		case MasterSwordsman: return Swordsman;
		case Champion: return Cavalry;
		case Crusader: return Paladin;
		case OrcChief: return Orc;
		case OgreLord: return Ogre;
		case WarTroll: return Troll;
		case BattleDwarf: return Dwarf;
		case GrandElf: return Elf;
		case GreaterDruid: return Druid;
		case MinotaurKing: return Minotaur;
		case RedDragon: return GreenDragon;
		case BlackDragon: return RedDragon;
		case SteelGolem: return IronGolem;
		case ArchMage: return Mage;
		case Titan: return Giant;
		case MutantZombie: return Zombie;
		case RoyalMummy: return Mummy;
		case VampireLord: return Vampire;
		case PowerLich: return Lich;
		default: return rec;
		}
	case Fly:
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
			return 1;
		default:
			return 0;
		}
	case HideAttack:
		switch(rec)
		{
		case Sprite:
		case Hydra:
		case Vampire:
		case VampireLord:
		case Rogue:
			return 1;
		default:
			return 0;
		}
	case MeleeArcher:
		switch(rec)
		{
		case Mage:
		case ArchMage:
		case Lich:
		case PowerLich:
			return 1;
		default:
			return 0;
		}
	case TwiceAttack:
		switch(rec)
		{
		case Ranger:
		case Crusader:
		case Paladin:
		case Wolf:
		case Elf:
		case GrandElf:
			return 1;
		default:
			return 0;
		}
	case Wide:
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
			return 1;
		default:
			return 0;
		}
	case Undead:
		switch(rec)
		{
		case Skeleton:
		case Zombie:
		case MutantZombie:
		case Mummy:
		case RoyalMummy:
		case Vampire:
		case VampireLord:
		case Lich:
		case PowerLich:
		case BoneDragon:
		case Ghost:
			return 1;
		default:
			return 0;
		}
	case Dragon:
		switch(rec)
		{
		case BoneDragon:
		case BlackDragon:
		case RedDragon:
		case GreenDragon:
			return 1;
		default:
			return 0;
		}
	case ElementsImmunity:
		switch(rec)
		{
		case Phoenix:
			return 1;
		default:
			return 0;
		}
	case Upgrade:
		switch(rec)
		{
		case Archer: return Ranger;
		case Pikeman: return VeteranPikeman;
		case Swordsman: return MasterSwordsman;
		case Cavalry: return Champion;
		case Paladin: return Crusader;
		case Orc: return OrcChief;
		case Ogre: return OgreLord;
		case Troll: return WarTroll;
		case Dwarf: return BattleDwarf;
		case Elf: return GrandElf;
		case Druid: return GreaterDruid;
		case Minotaur: return MinotaurKing;
		case GreenDragon: return RedDragon;
		case RedDragon: return BlackDragon;
		case IronGolem: return SteelGolem;
		case Mage: return ArchMage;
		case Giant: return Titan;
		case Zombie: return MutantZombie;
		case Mummy: return RoyalMummy;
		case Vampire: return VampireLord;
		case Lich: return PowerLich;
		default: return rec;
		}
	case Portrait:
		return rec - FirstMonster;
	case Base:
		return bsget(bsget(rec, Downgrade), Downgrade);
	case Dwelve:
		switch(rec)
		{
		case Goblin:
		case Centaur:
		case Halfling:
		case Sprite:
		case Skeleton:
		case Peasant:
			return Dwelving1;
		case Orc:
		case Gargoyle:
		case Boar:
		case Dwarf:
		case Zombie:
		case Archer:
			return Dwelving2;
		case Wolf:
		case Griffin:
		case IronGolem:
		case Elf:
		case Mummy:
		case Pikeman:
			return Dwelving3;
		case Ogre:
		case Minotaur:
		case Roc:
		case Druid:
		case Vampire:
		case Swordsman:
			return Dwelving4;
		case Troll:
		case Hydra:
		case Mage:
		case Unicorn:
		case Lich:
		case Cavalry:
			return Dwelving5;
		case Cyclop:
		case GreenDragon:
		case Giant:
		case Phoenix:
		case BoneDragon:
		case Paladin:
			return Dwelving6;
		case OrcChief:
		case BattleDwarf:
		case Ranger:
		case MutantZombie:
			return Upgrade2;
		case SteelGolem:
		case GrandElf:
		case RoyalMummy:
		case VeteranPikeman:
			return Upgrade3;
		case OgreLord:
		case MinotaurKing:
		case GreaterDruid:
		case VampireLord:
		case MasterSwordsman:
			return Upgrade4;
		case WarTroll:
		case ArchMage:
		case PowerLich:
		case Champion:
			return Upgrade5;
		case RedDragon:
		case Titan:
		case Crusader:
			return Dwelving6;
		default:
			return Empthy;
		}
	default:
		return 0;
	}
}

const char* monster_get_name(int rec)
{
	return objects[rec - FirstMonster].name[0];
}

const char* monster_get_name_multi(int rec, int id)
{
	return objects[rec - FirstMonster].multiname[0];
}

int random::monster(int level)
{
	int a[LastMonster - FirstMonster + 1];
	if(level == -1)
		return xrand(Peasant, WaterElement);
	int count = 0;
	for(auto& e : objects)
	{
		if(e.level != level)
			continue;
		a[count++] = &e - objects + FirstMonster;
	}
	if(count)
		return a[rand() % count];
	return 0;
}