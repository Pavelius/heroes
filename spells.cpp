#include "main.h"

static struct spell
{
	const char*     name[2];
	int             spellpoints;
	int             level;
	int             portrait;
	int             value;
	const char*     description[2];
} objects[LastSpell - FirstSpell + 1] =
{
	//  name sp mp spr  value  cost description
	{{"Fireball", "Огненный шар"}, 9, 3, 8, 1, {"Causes a giant fireball to strike the selected area, damaging all nearby creatures.", "Заставляет огромный шар огня попасть в указанную область, поражая всех существ вокруг."}},
	{{"Fireblast", "Огненный взрыв"}, 15, 4, 9, 10, {"An improved version of fireball, fireblast affects two hexes around the center point of the spell, rather than one."}},
	{{"Lightning Bolt", "Молния"}, 7, 2, 4, 25, {"Causes a bolt of electrical energy to strike the selected creature."}},
	{{"Chain Lightning", "Цепь молний"}, 15, 4, 5, 40, {"Causes a bolt of electrical energy to strike a selected creature, then strike the nearest creature with half damage, then strike the NEXT nearest creature with half again damage, and so on, until it becomes too weak to be harmful.  Warning:  This spell can hit your own creatures!"}},
	{{"Teleport", "Телепорт"}, 9, 3, 10, 0, {"Teleports the creature you select to any open position on the battlefield."}},
	{{"Cure", "Лечение"}, 6, 1, 6, 5, {"Removes all negative spells cast upon one of your units, and restores up to %1 HP per level of spell power.", "Убирает все негативные заклинания с одного вашего юнита и восстанавливает вплоть до %1 хитов на уровень Силы Магии."}},
	{{"Mass Cure", "Общее лечение"}, 15, 3, 2, 5, {"Removes all negative spells cast upon your forces, and restores up to %1 HP per level of spell power, per creature.", "Убирает все негативные заклинания и восстанавливает вплоть до %1 хитов на уровень Силы Магии у всех ваших существ."}},
	{{"Resurrect", "Оживление"}, 12, 4, 13, 50, {"Resurrects creatures from a damaged or dead unit until end of combat.", "Оживляет поврежденные или мертвые существа до конца боя."}},
	{{"Resurrect True", "Истинное оживление"}, 15, 5, 12, 50, {"Resurrects creatures from a damaged or dead unit permanently.", "Навсегда оживляет поврежденные или мертвые существа."}},
	{{"Haste", "Ускорение"}, 3, 1, 14, 0, {"Increases the speed of any creature by %1.","Увеличивет скорость существа на %1."}},
	{{"Mass Haste", "Общее ускорение"}, 10, 3, 14, 0, {"Increases the speed of all of your creatures by %1.", "Увеличивет скорость всех ваших существа на %1."}},
	{{"Slow", "Замедление"}, 3, 1, 1, 0, {"Slows target to half movement rate.", "Замедляет цель до половины ее скорости."}},
	{{"Mass Slow", "Общее замедление"}, 15, 4, 1, 0, {"Slows all enemies to half movement rate.", "Замедляет всех врагов до половины их скорости."}},
	//
	{{"Blind", "Ослепление"}, 6, 2, 21, 0, {"Clouds the affected creatures' eyes, preventing them from moving.","Дым, поражающий глаза противника лишает возможности их ходить."}},
	{{"Bless", "Благословение"}, 3, 1, 7, 0, {"Causes the selected creatures to inflict maximum damage.","Заставляет выбранный отряд наносить максимальные повреждения."}},
	{{"Mass Bless", "Общее благословение"}, 12, 3, 7, 0, {"Causes all of your units to inflict maximum damage."}},
	{{"Stoneskin", "Каменная кожа"}, 3, 1, 31, 3, {"Magically increases the defense skill of the selected creatures."}},
	{{"Steelskin", "Стальная кожа"}, 6, 2, 30, 5, {"Increases the defense skill of the targeted creatures.  This is an improved version of Stoneskin."}},
	{{"Curse", "Проклятие"}, 3, 1, 3, 0, {"Causes the selected creatures to inflict minimum damage."}},
	{{"Mass Curse", "Общее проклятие"}, 12, 3, 3, 0, {"Causes all enemy troops to inflict minimum damage."}},
	{{"Holy Word", "Святое слово"}, 9, 3, 22, 10, {"Damages all undead in the battle.", "Наносит повреждения всем мертвецам в битве."}},
	{{"Holy Shout", "Святой крик"}, 12, 4, 23, 20, {"Damages all undead in the battle.  This is an improved version of Holy Word.", "Наносит повреждения всем мертвецам в битве. Это улучшенная версия Святого слова."}},
	{{"Anti-Magic", "Антимагия"}, 7, 3, 17, 0, {"Prevents harmful magic against the selected creatures."}},
	{{"Dispel Magic", "Рассеять магию"}, 5, 1, 18, 0, {"Removes all magic spells from a single target."}},
	{{"Mass Dispel", "Общее рассеивание"}, 12, 3, 18, 0, {"Removes all magic spells from all creatures."}},
	{{"Magic Arrow", "Волшебная стрела"}, 3, 1, 38, 10, {"Causes a magic arrow to strike the selected target."}},
	{{"Berserker", "Берсеркер"}, 12, 3, 19, 0, {"Causes a creature to attack its nearest neighbor."}},
	{{"Armageddon", "Армагедон"}, 20, 5, 16, 50, {"Holy terror strikes the battlefield, causing severe damage to all creatures."}},
	{{"Elemental Storm", "Буря элементов"}, 15, 4, 11, 25, {"Magical elements pour down on the battlefield, damaging all creatures."}},
	{{"Meteor Shower", "Метеоритный дождь"}, 15, 4, 24, 25, {"A rain of rocks strikes an area of the battlefield, damaging all nearby creatures."}},
	{{"Paralyze", "Паралич"}, 9, 3, 20, 0, {"The targeted creatures are paralyzed, unable to move or retaliate."}},
	{{"Hypnotize", "Гипноз"}, 15, 5, 37, 25, {"Brings a single enemy unit under your control for one combat round if its hits are less than %1 times the caster's spell power."}},
	{{"Cold Ray", "Луч холода"}, 6, 2, 36, 20, {"Drains body heat from a single enemy unit."}},
	{{"Cold Ring", "Кольцо холода"}, 9, 3, 35, 10, {"Drains body heat from all units surrounding the center point, but not including the center point."}},
	{{"Disrupting Ray", "Луч разрушения"}, 7, 2, 34, 3, {"Reduces the defense rating of an enemy unit by three."}},
	{{"Death Ripple", "Дрожь смерти"}, 6, 2, 28, 5, {"Damages all living (non-undead) units in the battle."}},
	{{"Death Wave", "Волна смерти"}, 10, 3, 29, 10, {"Damages all living (non-undead) units in the battle.  This spell is an improved version of Death Ripple."}},
	{{"Dragon Slayer", "Убийца драконов"}, 6, 2, 32, 5, {"Greatly increases a unit's attack skill vs. Dragons."}},
	{{"Blood Lust", "Жажда крови"}, 3, 1, 27, 3, {"Increases a unit's attack skill.", "Увеличивает атаку отряда."}},
	{{"Animate Dead", "Оживить мертвых"}, 10, 3, 25, 50, {"Resurrects creatures from a damaged or dead undead unit permanently."}},
	{{"Mirror Image", "Иллюзия"}, 25, 5, 26, 0, {"Creates an illusionary unit that duplicates one of your existing units.  This illusionary unit does the same damages as the original, but will vanish if it takes any damage."}},
	{{"Shield", "Щит"}, 3, 1, 15, 2, {"Halves damage received from ranged attacks for a single unit."}},
	{{"Mass Shield", "Общий щит"}, 7, 4, 15, 0, {"Halves damage received from ranged attacks for all of your units."}},
	{{"Summon Earth Elemental", "Вызов элементаля земли"}, 30, 5, 56, 3, {"Summons Earth Elementals to fight for your army."}},
	{{"Summon Air Elemental", "Вызов элементаля воздуха"}, 30, 5, 57, 3, {"Summons Air Elementals to fight for your army."}},
	{{"Summon Fire Elemental", "Вызов элементаля огня"}, 30, 5, 58, 3, {"Summons Fire Elementals to fight for your army."}},
	{{"Summon Water Elemental", "Вызов элементаля воды"}, 30, 5, 59, 3, {"Summons Water Elementals to fight for your army."}},
	{{"Earthquake", "Землятресение"}, 15, 4, 33, 0, {"Damages castle walls."}},
	// Adventure spells
	{{"View Mines", "Показать шахты"}, 1, 1, 39, 0, {"Causes all mines across the land to become visible."}},
	{{"View Resources", "Показать ресурсы"}, 1, 2, 40, 0, {"Causes all res::tokens across the land to become visible."}},
	{{"View Artifacts", "Показать артефакты"}, 2, 2, 41, 0, {"Causes all artifacts across the land to become visible."}},
	{{"View Towns", "Показать города"}, 2, 3, 42, 0, {"Causes all towns and castles across the land to become visible."}},
	{{"View Heroes", "Показать героев"}, 2, 3, 43, 0, {"Causes all Heroes across the land to become visible."}},
	{{"View All", "Показать все"}, 3, 4, 44, 0, {"Causes the entire land to become visible."}},
	{{"Identify Hero", "Опознать героя"}, 3, 4, 45, 0, {"Allows the caster to view detailed information on enemy Heroes."}},
	{{"Summon Boat", "Вызвать корабль"}, 5, 2, 46, 0, {"Summons the nearest unoccupied, friendly boat to an adjacent shore location.  A friendly boat is one which you just built or were the most recent player to occupy."}},
	{{"Dimension Door", "Портал"}, 10, 5, 47, 0, {"Allows the caster to magically transport to a nearby location."}},
	{{"Town Gate", "Городские врата"}, 10, 4, 48, 0, {"Returns the caster to any town or castle currently owned."}},
	{{"Town Portal", "Городской портал"}, 20, 5, 49, 0, {"Returns the hero to the town or castle of choice, provided it is controlled by you."}},
	{{"Visions", "Видения"}, 6, 2, 50, 3, {"Visions predicts the likely outcome of an encounter with a neutral army camp."}},
	{{"Haunt", "Запустение"}, 8, 2, 51, 4, {"Haunts a mine you control with Ghosts.  This mine stops producing res::tokens.  (If I can't keep it, nobody will!)"}},
	{{"Set Earth Guardian", "Стражи земли"}, 15, 4, 52, 4, {"Sets Earth Elementals to guard a mine against enemy armies."}},
	{{"Set Air Guardian", "Стражи воздуха"}, 15, 4, 53, 4, {"Sets Air Elementals to guard a mine against enemy armies."}},
	{{"Set Fire Guardian", "Стражи огненя"}, 15, 4, 54, 4, {"Sets Fire Elementals to guard a mine against enemy armies."}},
	{{"Set Water Guardian", "Стражи воды"}, 15, 4, 55, 4, {"Sets Water Elementals to guard a mine against enemy armies."}},
	{{"Stoning", "Окаменение"}, 0, 0, 0, 0, {"Stone spell from Medusa."}},
};
static bsmeta::field fields[] = {
	BSREQ(spell, name, Name, Text),
	BSREQ(spell, description, Text, Text),
	BSREQ(spell, spellpoints, SpellPoints, Number),
	BSREQ(spell, level, Level, Number),
	BSREQ(spell, portrait, Portrait, Number),
	BSREQ(spell, value, Value, Number),
	{0}
};
BSMETA(spell, "Spells", "Заклинания", FirstSpell);

bool game::ishostile(int rec)
{
	switch(rec)
	{
	case SpellCurse:
	case SpellBlind:
	case SpellSlow:
	case SpellBerserker:
	case SpellParalyze:
	case SpellDisruptingRay:
	case SpellMagicArrow:
	case SpellLightingBolt:
	case SpellColdRay:
	case SpellColdRing:
	case SpellChainLighting:
	case SpellFireBall:
	case SpellFireBlast:
	case SpellMeteorShower:
	case SpellMassCurse:
	case SpellMassSlow:
		return true;
	default:
		return false;
	}
}

bool game::isfriendly(int rec)
{
	switch(rec)
	{
	case SpellStoneSkin:
	case SpellSteelSkin:
	case SpellBless:
	case SpellBloodLust:
	case SpellDragonSlayer:
	case SpellHaste:
	case SpellShield:
	case SpellMassHaste:
	case SpellMassBless:
	case SpellMassShield:
	case SpellMassCure:
		return true;
	default:
		return false;
	}
}

int game::gettarget(int rec)
{
	switch(rec)
	{
	case SpellLightingBolt:
	case SpellChainLighting:
	case SpellTeleport:
	case SpellCure:
	case SpellResurrect:
	case SpellRessurectTrue:
	case SpellHaste:
	case SpellSlow:
	case SpellBlind:
	case SpellBless:
	case SpellStoneSkin:
	case SpellSteelSkin:
	case SpellCurse:
	case SpellAntimagic:
	case SpellDispel:
	case SpellMagicArrow:
	case SpellBerserker:
	case SpellParalyze:
	case SpellHypnotize:
	case SpellColdRay:
	case SpellDisruptingRay:
	case SpellDragonSlayer:
	case SpellBloodLust:
	case SpellAnimateDead:
	case SpellMirrorImage:
	case SpellShield:
		return TargetUnit;
	default:
		return 0;
	}
}

int game::getframe(int rec)
{
	if(rec >= FirstSpell && rec <= LastSpell)
	{
		switch(rec)
		{
		case SpellBloodLust:
			return 9;
		case SpellBless:
			return 3;
		case SpellHaste:
			return 0;
		case SpellShield:
			return 10;
		case SpellStoneSkin:
			return 13;
		case SpellDragonSlayer:
			return 8;
		case SpellSteelSkin:
			return 14;
		case SpellAntimagic:
			return 12;
		case SpellCurse:
			return 4;
		case SpellSlow:
			return 1;
		case SpellBerserker:
			return 5;
		case SpellHypnotize:
			return 7;
		case SpellBlind:
			return 2;
		case SpellParalyze:
			return 6;
		case SpellStone:
			return 11;
		default:
			return 100;
		}
	}
	return 0;
}

bool game::iscombat(int rec)
{
	return rec >= FirstSpell && rec <= SpellEarthquake;
}

//static int object_get(int rec, int id)
//{
//    switch(id)
//    {
//    case SingleVersion:
//        switch(rec)
//        {
//        case SpellMassHaste:
//            return SpellHaste;
//        case SpellMassBless:
//            return SpellBless;
//        case SpellMassDispel:
//            return SpellDispel;
//        case SpellMassShield:
//            return SpellShield;
//        case SpellMassCurse:
//            return SpellCurse;
//        case SpellMassCure:
//            return SpellCure;
//        case SpellMassSlow:
//            return SpellSlow;
//        default:
//            return rec;
//        }
//        break;
//    case MassEffect:
//        switch(rec)
//        {
//        case SpellMassHaste:
//        case SpellMassBless:
//        case SpellMassDispel:
//        case SpellEarthquake:
//        case SpellMassShield:
//        case SpellMassCurse:
//		case SpellMassCure:
//        case SpellMassSlow:
//        case SpellHolyWord:
//        case SpellHolyShout:
//        case SpellArmagedon:
//        case SpellElementalStorm:
//        case SpellDeathRipple:
//        case SpellDeathWave:
//            return 1;
//        default:
//            return 0;
//        }
//        break;