#include "main.h"

static struct spell
{
	const char*     name;
	int             spellpoints;
	int             level;
	int             portrait;
	int             value;
	const char*     description[2];
} objects[LastSpell - FirstSpell + 1] =
{
	//  name sp mp spr  value  cost description
	{"Fireball", 9, 3, 8, 1, {"Causes a giant fireball to strike the selected area, damaging all nearby creatures."}},
	{"Fireblast", 15, 4, 9, 10, {"An improved version of fireball, fireblast affects two hexes around the center point of the spell, rather than one."}},
	{"Lightning Bolt", 7, 2, 4, 25, {"Causes a bolt of electrical energy to strike the selected creature."}},
	{"Chain Lightning", 15, 4, 5, 40, {"Causes a bolt of electrical energy to strike a selected creature, then strike the nearest creature with half damage, then strike the NEXT nearest creature with half again damage, and so on, until it becomes too weak to be harmful.  Warning:  This spell can hit your own creatures!"}},
	{"Teleport", 9, 3, 10, 0, {"Teleports the creature you select to any open position on the battlefield."}},
	{"Cure", 6, 1, 6, 5, {"Removes all negative spells cast upon one of your units, and restores up to %1 HP per level of spell power."}},
	{"Mass Cure", 15, 3, 2, 5, {"Removes all negative spells cast upon your forces, and restores up to %1 HP per level of spell power, per creature."}},
	{"Resurrect", 12, 4, 13, 50, {"Resurrects creatures from a damaged or dead unit until end of combat."}},
	{"Resurrect True", 15, 5, 12, 50, {"Resurrects creatures from a damaged or dead unit permanently."}},
	{"Haste", 3, 1, 14, 0, {"Increases the speed of any creature by %1."}},
	{"Mass Haste", 10, 3, 14, 0, {"Increases the speed of all of your creatures by %1."}},
	{"Slow", 3, 1, 1, 0, {"Slows target to half movement rate."}},
	{"Mass Slow", 15, 4, 1, 0, {"Slows all enemies to half movement rate."}},
	//
	{"Blind", 6, 2, 21, 0, {"Clouds the affected creatures' eyes, preventing them from moving."}},
	{"Bless", 3, 1, 7, 0, {"Causes the selected creatures to inflict maximum damage."}},
	{"Mass Bless", 12, 3, 7, 0, {"Causes all of your units to inflict maximum damage."}},
	{"Stoneskin", 3, 1, 31, 3, {"Magically increases the defense skill of the selected creatures."}},
	{"Steelskin", 6, 2, 30, 5, {"Increases the defense skill of the targeted creatures.  This is an improved version of Stoneskin."}},
	{"Curse", 3, 1, 3, 0, {"Causes the selected creatures to inflict minimum damage."}},
	{"Mass Curse", 12, 3, 3, 0, {"Causes all enemy troops to inflict minimum damage."}},
	{"Holy Word", 9, 3, 22, 10, {"Damages all undead in the battle.", "Наносит повреждения всем мертвецам в битве."}},
	{"Holy Shout", 12, 4, 23, 20, {"Damages all undead in the battle.  This is an improved version of Holy Word.", "Наносит повреждения всем мертвецам в битве. Это улучшенная версия Святого слова."}},
	{"Anti-Magic", 7, 3, 17, 0, {"Prevents harmful magic against the selected creatures."}},
	{"Dispel Magic", 5, 1, 18, 0, {"Removes all magic spells from a single target."}},
	{"Mass Dispel", 12, 3, 18, 0, {"Removes all magic spells from all creatures."}},
	{"Magic Arrow", 3, 1, 38, 10, {"Causes a magic arrow to strike the selected target."}},
	{"Berserker", 12, 3, 19, 0, {"Causes a creature to attack its nearest neighbor."}},
	{"Armageddon", 20, 5, 16, 50, {"Holy terror strikes the battlefield, causing severe damage to all creatures."}},
	{"Elemental Storm", 15, 4, 11, 25, {"Magical elements pour down on the battlefield, damaging all creatures."}},
	{"Meteor Shower", 15, 4, 24, 25, {"A rain of rocks strikes an area of the battlefield, damaging all nearby creatures."}},
	{"Paralyze", 9, 3, 20, 0, {"The targeted creatures are paralyzed, unable to move or retaliate."}},
	{"Hypnotize", 15, 5, 37, 25, {"Brings a single enemy unit under your control for one combat round if its hits are less than %1 times the caster's spell power."}},
	{"Cold Ray", 6, 2, 36, 20, {"Drains body heat from a single enemy unit."}},
	{"Cold Ring", 9, 3, 35, 10, {"Drains body heat from all units surrounding the center point, but not including the center point."}},
	{"Disrupting Ray", 7, 2, 34, 3, {"Reduces the defense rating of an enemy unit by three."}},
	{"Death Ripple", 6, 2, 28, 5, {"Damages all living (non-undead) units in the battle."}},
	{"Death Wave", 10, 3, 29, 10, {"Damages all living (non-undead) units in the battle.  This spell is an improved version of Death Ripple."}},
	{"Dragon Slayer", 6, 2, 32, 5, {"Greatly increases a unit's attack skill vs. Dragons."}},
	{"Blood Lust", 3, 1, 27, 3, {"Increases a unit's attack skill.", "Увеличивает атаку отряда."}},
	{"Animate Dead", 10, 3, 25, 50, {"Resurrects creatures from a damaged or dead undead unit permanently."}},
	{"Mirror Image", 25, 5, 26, 0, {"Creates an illusionary unit that duplicates one of your existing units.  This illusionary unit does the same damages as the original, but will vanish if it takes any damage."}},
	{"Shield", 3, 1, 15, 2, {"Halves damage received from ranged attacks for a single unit."}},
	{"Mass Shield", 7, 4, 15, 0, {"Halves damage received from ranged attacks for all of your units."}},
	{"Summon Earth Elemental", 30, 5, 56, 3, {"Summons Earth Elementals to fight for your army."}},
	{"Summon Air Elemental", 30, 5, 57, 3, {"Summons Air Elementals to fight for your army."}},
	{"Summon Fire Elemental", 30, 5, 58, 3, {"Summons Fire Elementals to fight for your army."}},
	{"Summon Water Elemental", 30, 5, 59, 3, {"Summons Water Elementals to fight for your army."}},
	{"Earthquake", 15, 4, 33, 0, {"Damages castle walls."}},
	// Adventure spells
	{"View Mines", 1, 1, 39, 0, {"Causes all mines across the land to become visible."}},
	{"View Resources", 1, 2, 40, 0, {"Causes all res::tokens across the land to become visible."}},
	{"View Artifacts", 2, 2, 41, 0, {"Causes all artifacts across the land to become visible."}},
	{"View Towns", 2, 3, 42, 0, {"Causes all towns and castles across the land to become visible."}},
	{"View Heroes", 2, 3, 43, 0, {"Causes all Heroes across the land to become visible."}},
	{"View All", 3, 4, 44, 0, {"Causes the entire land to become visible."}},
	{"Identify Hero", 3, 4, 45, 0, {"Allows the caster to view detailed information on enemy Heroes."}},
	{"Summon Boat", 5, 2, 46, 0, {"Summons the nearest unoccupied, friendly boat to an adjacent shore location.  A friendly boat is one which you just built or were the most recent player to occupy."}},
	{"Dimension Door", 10, 5, 47, 0, {"Allows the caster to magically transport to a nearby location."}},
	{"Town Gate", 10, 4, 48, 0, {"Returns the caster to any town or castle currently owned."}},
	{"Town Portal", 20, 5, 49, 0, {"Returns the hero to the town or castle of choice, provided it is controlled by you."}},
	{"Visions", 6, 2, 50, 3, {"Visions predicts the likely outcome of an encounter with a neutral army camp."}},
	{"Haunt", 8, 2, 51, 4, {"Haunts a mine you control with Ghosts.  This mine stops producing res::tokens.  (If I can't keep it, nobody will!)"}},
	{"Set Earth Guardian", 15, 4, 52, 4, {"Sets Earth Elementals to guard a mine against enemy armies."}},
	{"Set Air Guardian", 15, 4, 53, 4, {"Sets Air Elementals to guard a mine against enemy armies."}},
	{"Set Fire Guardian", 15, 4, 54, 4, {"Sets Fire Elementals to guard a mine against enemy armies."}},
	{"Set Water Guardian", 15, 4, 55, 4, {"Sets Water Elementals to guard a mine against enemy armies."}},
	{"Stoning", 0, 0, 0, 0, {"Stone spell from Medusa."}},
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
//    case RequiredTarget:
//        break;
//    }
//}