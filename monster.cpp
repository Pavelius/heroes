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
	{1, 0, 1, 1, 1, 1, 1, 1, 12, 0, {"Peasant", "Крестьянин"}, {"Peasants", "Крестьян"}, {20, 0, 0, 0, 0, 0, 0}},
	{1, 0, 5, 3, 2, 3, 10, 1, 8, 12, {"Archer", "Лучник"}, {"Archers", "Лучников"}, {150, 0, 0, 0, 0, 0, 0}},
	{2, 0, 5, 3, 2, 3, 10, 3, 8, 24, {"Ranger", "Рейнджер"}, {"Rangers", "Рейнджеров"}, {200, 0, 0, 0, 0, 0, 0}},
	{2, 0, 5, 9, 3, 4, 15, 3, 5, 0, {"Pikeman", "Копейщик"}, {"Pikemen", "Копейщиков"}, {200, 0, 0, 0, 0, 0, 0}},
	{2, 0, 5, 9, 3, 4, 20, 4, 5, 0, {"Veteran Pikeman", "Ветеран копейщик"}, {"Veteran Pikemen", "Ветеранов копейщиков"}, {250, 0, 0, 0, 0, 0, 0}},
	{3, 0, 7, 9, 4, 6, 25, 3, 4, 0, {"Swordsman", "Мечник"}, {"Swordsmen", "Мечников"}, {250, 0, 0, 0, 0, 0, 0}},
	{3, 0, 7, 9, 4, 6, 30, 4, 4, 0, {"Master Swordsman", "Мастер мечник"}, {"Master Swordsmen", "Мастеров мечников"}, {300, 0, 0, 0, 0, 0, 0}},
	{3, 0, 10, 9, 5, 10, 30, 5, 3, 0, {"Cavalry", "Кавалерист"}, {"Cavalries", "Кавалерии"}, {300, 0, 0, 0, 0, 0, 0}},
	{3, 0, 10, 9, 5, 10, 40, 6, 3, 0, {"Champion", "Чемпион"}, {"Champions", "Чемпионов"}, {375, 0, 0, 0, 0, 0, 0}},
	{4, 0, 11, 12, 10, 20, 50, 4, 2, 0, {"Paladin", "Паладин"}, {"Paladins", "Паладинов"}, {600, 0, 0, 0, 0, 0, 0}},
	{4, 0, 11, 12, 10, 20, 65, 5, 2, 0, {"Crusader", "Крестоносец"}, {"Crusaders", "Крестоносцев"}, {1000, 0, 0, 0, 0, 0, 0}},
	//
	{1, 0, 3, 1, 1, 2, 3, 3, 10, 0, {"Goblin", "Гоблин"}, {"Goblins", "Гоблинов"}, {40, 0, 0, 0, 0, 0, 0}},
	{1, 0, 3, 4, 2, 3, 10, 1, 8, 8, {"Orc", "Орк"}, {"Orcs", "Орков"}, {140, 0, 0, 0, 0, 0, 0}},
	{2, 0, 3, 4, 3, 4, 15, 2, 8, 16, {"Orc Chief", "Боевой орк"}, {"Orc Chiefs", "Боевых орков"}, {175, 0, 0, 0, 0, 0, 0}},
	{2, 0, 6, 2, 3, 5, 20, 5, 5, 0, {"Wolf", "Волк"}, {"Wolves", "Волков"}, {200, 0, 0, 0, 0, 0, 0}},
	{3, 0, 9, 5, 4, 6, 40, 1, 4, 0, {"Ogre", "Огр"}, {"Ogres", "Огров"}, {300, 0, 0, 0, 0, 0, 0}},
	{3, 0, 9, 5, 5, 7, 60, 3, 4, 0, {"Ogre Lord", "Военный огр"}, {"Ogre Lords", "Военных огров"}, {500, 0, 0, 0, 0, 0, 0}},
	{3, 0, 10, 5, 5, 7, 40, 3, 3, 8, {"Troll", "Тролль"}, {"Trolls", "Троллей"}, {600, 0, 0, 0, 0, 0, 0}},
	{3, 0, 10, 5, 7, 9, 40, 4, 3, 16, {"War Troll", "Боевой тролль"}, {"War Trolls", "Боевых троллей"}, {700, 0, 0, 0, 0, 0, 0}},
	{4, 0, 12, 9, 12, 24, 80, 4, 2, 0, {"Cyclops", "Циклоп"}, {"Cyclopes", "Циклопов"}, {750, 0, 0, 0, 0, 1, 0}},
	//
	{1, 0, 4, 2, 1, 2, 2, 3, 8, 0, {"Sprite", "Фея"}, {"Sprites", "Фей"}, {50, 0, 0, 0, 0, 0, 0}},
	{2, 0, 6, 5, 2, 4, 20, 1, 6, 0, {"Dwarf", "Дварф"}, {"Dwarves", "Дварфов"}, {200, 0, 0, 0, 0, 0, 0}},
	{2, 0, 6, 6, 2, 4, 20, 3, 6, 0, {"Battle Dwarf", "Боевой дварф"}, {"Battle Dwarves", "Боевых дварфов"}, {250, 0, 0, 0, 0, 0, 0}},
	{2, 0, 4, 3, 2, 3, 15, 3, 4, 24, {"Elf", "Эльф"}, {"Elves", "Эльфов"}, {250, 0, 0, 0, 0, 0, 0}},
	{2, 0, 5, 5, 2, 3, 15, 5, 4, 24, {"Grand Elf", "Великий эльф"}, {"Grand Elves", "Великих эльфов"}, {300, 0, 0, 0, 0, 0, 0}},
	{3, 0, 7, 5, 5, 8, 25, 4, 3, 8, {"Druid", "Друид"}, {"Druids", "Друидов"}, {350, 0, 0, 0, 0, 0, 0}},
	{3, 0, 7, 7, 5, 8, 25, 5, 3, 16, {"Greater Druid", "Великий друид"}, {"Greater Druids", "Великих друидов"}, {400, 0, 0, 0, 0, 0, 0}},
	{4, 0, 10, 9, 7, 14, 40, 4, 2, 0, {"Unicorn", "Единорог"}, {"Unicorns", "Единорогов"}, {500, 0, 0, 0, 0, 0, 0}},
	{4, 0, 12, 10, 20, 40, 100, 6, 1, 0, {"Phoenix", "Феникс"}, {"Phoenix", "Фениксов"}, {1500, 0, 1, 0, 0, 0, 0}},
	//
	{1, 0, 3, 1, 1, 2, 5, 3, 8, 8, {"Centaur", "Кентавр"}, {"Centaurs", "Кентавров"}, {60, 0, 0, 0, 0, 0, 0}},
	{2, 0, 4, 7, 2, 3, 15, 5, 6, 0, {"Gargoyle", "Гаргулья"}, {"Gargoyles", "Гаргулей"}, {200, 0, 0, 0, 0, 0, 0}},
	{3, 0, 6, 6, 3, 5, 25, 3, 4, 0, {"Griffin", "Грифон"}, {"Griffins", "Грифонов"}, {300, 0, 0, 0, 0, 0, 0}},
	{3, 0, 9, 8, 5, 10, 35, 3, 3, 0, {"Minotaur", "Минотавр"}, {"Minotaurs", "Минотавров"}, {400, 0, 0, 0, 0, 0, 0}},
	{3, 0, 9, 8, 5, 10, 45, 5, 3, 0, {"Minotaur King", "Боевой минотавр"}, {"Minotaur Kings", "Боевых минотавров"}, {500, 0, 0, 0, 0, 0, 0}},
	{4, 0, 8, 9, 6, 12, 75, 1, 2, 0, {"Hydra", "Гидра"}, {"Hydras", "Гидр"}, {800, 0, 0, 0, 0, 0, 0}},
	{4, 0, 12, 12, 25, 50, 200, 3, 1, 0, {"Green Dragon", "Зеленый дракон"}, {"Green Dragons", "Зеленых драконов"}, {3000, 0, 0, 0, 1, 0, 0}},
	{4, 0, 13, 13, 25, 50, 250, 4, 1, 0, {"Red Dragon", "Красный дракон"}, {"Red Dragons", "Красных драконов"}, {3500, 0, 0, 0, 1, 0, 0}},
	{4, 0, 14, 14, 25, 50, 300, 5, 1, 0, {"Black Dragon", "Черный дракон"}, {"Black Dragons", "Черных драконов"}, {4000, 0, 0, 0, 2, 0, 0}},
	//
	{1, 0, 2, 1, 1, 3, 3, 2, 8, 12, {"Halfling", "Полурослик"}, {"Halflings", "Полуросликов"}, {50, 0, 0, 0, 0, 0, 0}},
	{2, 0, 5, 4, 2, 3, 15, 5, 6, 0, {"Boar", "Кабан"}, {"Boars", "Кабанов"}, {150, 0, 0, 0, 0, 0, 0}},
	{2, 0, 5, 10, 4, 5, 30, 1, 4, 0, {"Iron Golem", "Железный голем"}, {"Iron Golems", "Железных големов"}, {300, 0, 0, 0, 0, 0, 0}},
	{3, 0, 7, 10, 4, 5, 35, 2, 4, 0, {"Steel Golem", "Стальной голем"}, {"Steel Golems", "Стальных големов"}, {350, 0, 0, 0, 0, 0, 0}},
	{3, 0, 7, 7, 4, 8, 40, 3, 3, 0, {"Roc", "Орел"}, {"Rocs", "Орлов"}, {400, 0, 0, 0, 0, 0, 0}},
	{3, 0, 11, 7, 7, 9, 30, 4, 2, 12, {"Mage", "Маг"}, {"Magi", "Магов"}, {600, 0, 0, 0, 0, 0, 0}},
	{3, 0, 12, 8, 7, 9, 35, 5, 2, 24, {"Archmage", "Архимаг"}, {"Archmagi", "Архимагов"}, {700, 0, 0, 0, 0, 0, 0}},
	{4, 0, 13, 10, 20, 30, 150, 3, 1, 0, {"Giant", "Гигант"}, {"Giants", "Гигантов"}, {2000, 0, 0, 0, 0, 0, 1}},
	{4, 0, 15, 15, 20, 30, 300, 5, 1, 24, {"Titan", "Титан"}, {"Titans", "Титанов"}, {5000, 0, 0, 0, 0, 0, 2}},
	//
	{1, 0, 4, 3, 2, 3, 4, 3, 8, 0, {"Skeleton", "Скелет"}, {"Skeletons", "Скелетов"}, {75, 0, 0, 0, 0, 0, 0}},
	{1, 1, 5, 2, 2, 3, 15, 1, 6, 0, {"Zombie", "Зомби"}, {"Zombies", "Зомби"}, {150, 0, 0, 0, 0, 0, 0}},
	{2, 1, 5, 2, 2, 3, 25, 3, 6, 0, {"Mutant Zombie", "Зомби мутант"}, {"Mutant Zombies", "Зомби мутантов"}, {200, 0, 0, 0, 0, 0, 0}},
	{2, 0, 6, 6, 3, 4, 25, 3, 4, 0, {"Mummy", "Мумия"}, {"Mummies", "Мумий"}, {250, 0, 0, 0, 0, 0, 0}},
	{3, 0, 6, 6, 3, 4, 30, 4, 4, 0, {"Royal Mummy", "Королеская мумия"}, {"Royal Mummies", "Королевских мумий"}, {300, 0, 0, 0, 0, 0, 0}},
	{3, 0, 8, 6, 5, 7, 30, 3, 3, 0, {"Vampire", "Вампир"}, {"Vampires", "Вампиров"}, {500, 0, 0, 0, 0, 0, 0}},
	{3, 0, 8, 6, 5, 7, 40, 4, 3, 0, {"Vampire Lord", "Вампир-лорд"}, {"Vampire Lords", "Лордов вампиров"}, {650, 0, 0, 0, 0, 0, 0}},
	{3, 0, 7, 12, 8, 10, 25, 4, 2, 12, {"Lich", "Лич"}, {"Liches", "Личей"}, {750, 0, 0, 0, 0, 0, 0}},
	{4, 0, 7, 13, 8, 10, 35, 5, 2, 24, {"Power Lich", "Могучий лич"}, {"Power Liches", "Могучих личей"}, {900, 0, 0, 0, 0, 0, 0}},
	{4, 0, 11, 9, 25, 45, 150, 3, 1, 0, {"Bone Dragon", "Костяной дракон"}, {"Bone Dragons", "Костяных драконов"}, {1500, 0, 0, 0, 0, 0, 0}},
	//
	{1, 0, 6, 1, 1, 2, 4, 4, 4, 0, {"Rogue", "Бандит"}, {"Rogues", "Бандитов"}, {50, 0, 0, 0, 0, 0, 0}},
	{2, 0, 7, 6, 2, 5, 20, 5, 4, 0, {"Nomad", "Кочевник"}, {"Nomads", "Кочевников"}, {200, 0, 0, 0, 0, 0, 0}},
	{3, 0, 8, 7, 4, 6, 20, 4, 4, 0, {"Ghost", "Призрак"}, {"Ghosts", "Призраков"}, {1000, 0, 0, 0, 0, 0, 0}},
	{4, 0, 10, 9, 20, 30, 50, 5, 4, 0, {"Genie", "Джин"}, {"Genies", "Джинов"}, {650, 0, 0, 0, 0, 0, 1}},
	{3, 0, 8, 9, 6, 10, 35, 3, 4, 0, {"Medusa", "Медуза"}, {"Medusas", "Медуз"}, {500, 0, 0, 0, 0, 0, 0}},
	{3, 0, 8, 8, 4, 5, 50, 2, 4, 0, {"Earth Elemental", "Земляной элементаль"}, {"Earth Elementals", "Земляных элементалей"}, {500, 0, 0, 0, 0, 0, 0}},
	{3, 0, 7, 7, 2, 8, 35, 5, 4, 0, {"Air Elemental", "Воздушный элементаль"}, {"Air Elementals", "Воздушных элементалей"}, {500, 0, 0, 0, 0, 0, 0}},
	{3, 0, 8, 6, 4, 6, 40, 4, 4, 0, {"Fire Elemental", "Огненный элементаль"}, {"Fire Elementals", "Огненных элементалей"}, {500, 0, 0, 0, 0, 0, 0}},
	{3, 0, 6, 8, 3, 7, 45, 3, 4, 0, {"Water Elemental", "Водный элементаль"}, {"Water Elementals", "Водных элементалей"}, {500, 0, 0, 0, 0, 0, 0}},
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
BSMETA(monster, "Monsters", "Монстры", FirstMonster);

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