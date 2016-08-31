#include "main.h"

static struct spell_t
{
    const char*     name;
    int             spellpoints;
    int             mp;
    int             spr;
    int             value;
    int             cost;
    const char*     description[2];
} objects[LastSpell-FirstSpell+1] =
{
    //  name sp mp spr  value  cost description
    {"Fireball", 9, 0, 8, 10, 0, {"Causes a giant fireball to strike the selected area, damaging all nearby creatures."}},
    {"Fireblast", 15, 0, 9, 10, 0, {"An improved version of fireball, fireblast affects two hexes around the center point of the spell, rather than one."}},
    {"Lightning Bolt", 7, 0, 4, 25, 0, {"Causes a bolt of electrical energy to strike the selected creature."}},
    {"Chain Lightning", 15, 0, 5, 40, 0, {"Causes a bolt of electrical energy to strike a selected creature, then strike the nearest creature with half damage, then strike the NEXT nearest creature with half again damage, and so on, until it becomes too weak to be harmful.  Warning:  This spell can hit your own creatures!"}},
    {"Teleport", 9, 0, 10, 0, 0, {"Teleports the creature you select to any open position on the battlefield."}},
    {"Cure", 6, 0, 6, 5, 0, {"Removes all negative spells cast upon one of your units, and restores up to %{count} HP per level of spell power."}},
    {"Mass Cure", 15, 0, 2, 5, 0, {"Removes all negative spells cast upon your forces, and restores up to %{count} HP per level of spell power, per creature."}},
    {"Resurrect", 12, 0, 13, 50, 0, {"Resurrects creatures from a damaged or dead unit until end of combat."}},
    {"Resurrect True", 15, 0, 12, 50, 0, {"Resurrects creatures from a damaged or dead unit permanently."}},
    {"Haste", 3, 0, 14, 0, 0, {"Increases the speed of any creature by %{count}."}},
    {"Mass Haste", 10, 0, 14, 0, 0, {"Increases the speed of all of your creatures by %{count}."}},
    {"Slow", 3, 0, 1, 0, 0, {"Slows target to half movement rate."}},
    {"Mass Slow", 15, 0, 1, 0, 0, {"Slows all enemies to half movement rate."}},
    //
    {"Blind", 6, 0, 21, 0, 0, {"Clouds the affected creatures' eyes, preventing them from moving."}},
    {"Bless", 3, 0, 7, 0, 0, {"Causes the selected creatures to inflict maximum damage."}},
    {"Mass Bless", 12, 0, 7, 0, 0, {"Causes all of your units to inflict maximum damage."}},
    {"Stoneskin", 3, 0, 31, 3, 0, {"Magically increases the defense skill of the selected creatures."}},
    {"Steelskin", 6, 0, 30, 5, 0, {"Increases the defense skill of the targeted creatures.  This is an improved version of Stoneskin."}},
    {"Curse", 3, 0, 3, 0, 0, {"Causes the selected creatures to inflict minimum damage."}},
    {"Mass Curse", 12, 0, 3, 0, 0, {"Causes all enemy troops to inflict minimum damage."}},
    {"Holy Word", 9, 0, 22, 10, 0, {"Damages all undead in the battle.", "Наносит повреждения всем мертвецам в битве."}},
    {"Holy Shout", 12, 0, 23, 20, 0, {"Damages all undead in the battle.  This is an improved version of Holy Word.", "Наносит повреждения всем мертвецам в битве. Это улучшенная версия Святого слова."}},
    {"Anti-Magic", 7, 0, 17, 0, 0, {"Prevents harmful magic against the selected creatures."}},
    {"Dispel Magic", 5, 0, 18, 0, 0, {"Removes all magic spells from a single target."}},
    {"Mass Dispel", 12, 0, 18, 0, 0, {"Removes all magic spells from all creatures."}},
    {"Magic Arrow", 3, 0, 38, 10, 0, {"Causes a magic arrow to strike the selected target."}},
    {"Berserker", 12, 0, 19, 0, 0, {"Causes a creature to attack its nearest neighbor."}},
    {"Armageddon", 20, 0, 16, 50, 0, {"Holy terror strikes the battlefield, causing severe damage to all creatures."}},
    {"Elemental Storm", 15, 0, 11, 25, 0, {"Magical elements pour down on the battlefield, damaging all creatures."}},
    {"Meteor Shower", 15, 0, 24, 25, 0, {"A rain of rocks strikes an area of the battlefield, damaging all nearby creatures."}},
    {"Paralyze", 9, 0, 20, 0, 0, {"The targeted creatures are paralyzed, unable to move or retaliate."}},
    {"Hypnotize", 15, 0, 37, 25, 0, {"Brings a single enemy unit under your control for one combat round if its hits are less than %{count} times the caster's spell power."}},
    {"Cold Ray", 6, 0, 36, 20, 0, {"Drains body heat from a single enemy unit."}},
    {"Cold Ring", 9, 0, 35, 10, 0, {"Drains body heat from all units surrounding the center point, but not including the center point."}},
    {"Disrupting Ray", 7, 0, 34, 3, 0, {"Reduces the defense rating of an enemy unit by three."}},
	{"Death Ripple", 6, 0, 28, 5, 0, {"Damages all living (non-undead) units in the battle."}},
	{"Death Wave", 10, 0, 29, 10, 0, {"Damages all living (non-undead) units in the battle.  This spell is an improved version of Death Ripple."}},
    {"Dragon Slayer", 6, 0, 32, 5, 0, {"Greatly increases a unit's attack skill vs. Dragons."}},
    {"Blood Lust", 3, 0, 27, 3, 0, {"Increases a unit's attack skill.","Увеличивает атаку отряда."}},
    {"Animate Dead", 10, 0, 25, 50, 0, {"Resurrects creatures from a damaged or dead undead unit permanently."}},
	{"Mirror Image", 25, 0, 26, 0, 0, {"Creates an illusionary unit that duplicates one of your existing units.  This illusionary unit does the same damages as the original, but will vanish if it takes any damage."}},
    {"Shield", 3, 0, 15, 2, 0, {"Halves damage received from ranged attacks for a single unit."}},
    {"Mass Shield", 7, 0, 15, 0, 0, {"Halves damage received from ranged attacks for all of your units."}},
    {"Summon Earth Elemental", 30, 0, 56, 3, 0, {"Summons Earth Elementals to fight for your army."}},
    {"Summon Air Elemental", 30, 0, 57, 3, 0, {"Summons Air Elementals to fight for your army."}},
    {"Summon Fire Elemental", 30, 0, 58, 3, 0, {"Summons Fire Elementals to fight for your army."}},
    {"Summon Water Elemental", 30, 0, 59, 3, 0, {"Summons Water Elementals to fight for your army."}},
    {"Earthquake", 15, 0, 33, 0, 0, {"Damages castle walls."}},
    // Adventure spells
    {"View Mines", 1, 0, 39, 0, 0, {"Causes all mines across the land to become visible."}},
    {"View Resources", 1, 0, 40, 0, 0, {"Causes all res::tokens across the land to become visible."}},
    {"View Artifacts", 2, 0, 41, 0, 0, {"Causes all artifacts across the land to become visible."}},
    {"View Towns", 2, 0, 42, 0, 0, {"Causes all towns and castles across the land to become visible."}},
    {"View Heroes", 2, 0, 43, 0, 0, {"Causes all Heroes across the land to become visible."}},
    {"View All", 3, 0, 44, 0, 0, {"Causes the entire land to become visible."}},
    {"Identify Hero", 3, 0, 45, 0, 0, {"Allows the caster to view detailed information on enemy Heroes."}},
    {"Summon Boat", 5, 0, 46, 0, 0, {"Summons the nearest unoccupied, friendly boat to an adjacent shore location.  A friendly boat is one which you just built or were the most recent player to occupy."}},
    {"Dimension Door", 10, 0, 47, 0, 0, {"Allows the caster to magically transport to a nearby location."}},
    {"Town Gate", 10, 0, 48, 0, 0, {"Returns the caster to any town or castle currently owned."}},
    {"Town Portal", 20, 0, 49, 0, 0, {"Returns the hero to the town or castle of choice, provided it is controlled by you."}},
    {"Visions", 6, 0, 50, 3, 0, {"Visions predicts the likely outcome of an encounter with a neutral army camp."}},
    {"Haunt", 8, 0, 51, 4, 0, {"Haunts a mine you control with Ghosts.  This mine stops producing res::tokens.  (If I can't keep it, nobody will!)"}},
    {"Set Earth Guardian", 15, 0, 52, 4, 0, {"Sets Earth Elementals to guard a mine against enemy armies."}},
    {"Set Air Guardian", 15, 0, 53, 4, 0, {"Sets Air Elementals to guard a mine against enemy armies."}},
    {"Set Fire Guardian", 15, 0, 54, 4, 0, {"Sets Fire Elementals to guard a mine against enemy armies."}},
    {"Set Water Guardian", 15, 0, 55, 4, 0, {"Sets Water Elementals to guard a mine against enemy armies."}},
    {"Stoning", 0, 0, 0, 0, 0, {"Stone spell from Medusa."}},
};

static int object_get(int rec, int id)
{
    switch(id)
    {
    case Level:
        switch(rec)
        {
        case SpellBless:
        case SpellBloodLust:
        case SpellCure:
        case SpellCurse:
        case SpellDispel:
        case SpellHaste:
        case SpellMagicArrow:
        case SpellShield:
        case SpellSlow:
        case SpellStoneSkin:
        case SpellViewMines:
        case SpellViewResources:
            return 1;
        case SpellBlind:
        case SpellColdRay:
        case SpellDeathRipple:
        case SpellDisruptingRay:
        case SpellDragonSlayer:
        case SpellLightingBolt:
        case SpellSteelSkin:
        case SpellHaunt:
        case SpellSummonBoat:
        case SpellViewArtifacts:
        case SpellVisions:
            return 2;
        case SpellAnimateDead:
        case SpellAntimagic:
        case SpellColdRing:
        case SpellDeathWave:
        case SpellEarthquake:
        case SpellFireBall:
        case SpellHolyWord:
        case SpellMassBless:
        case SpellMassCurse:
        case SpellMassDispel:
        case SpellMassHaste:
        case SpellParalyze:
        case SpellTeleport:
        case SpellIdentifyHero:
        case SpellViewHeroes:
        case SpellViewTowns:
            return 3;
        case SpellBerserker:
        case SpellChainLighting:
        case SpellElementalStorm:
        case SpellFireBlast:
        case SpellHolyShout:
        case SpellMassCure:
        case SpellMassShield:
        case SpellMassSlow:
        case SpellMeteorShower:
        case SpellResurrect:
        case SpellSetAirGuadrdian:
        case SpellSetEartGuadrdian:
        case SpellSetFireGuadrdian:
        case SpellSetWaterGuadrdian:
        case SpellTownGate:
        case SpellViewAll:
            return 4;
        case SpellArmagedon:
        case SpellHypnotize:
        case SpellMirrorImage:
        case SpellRessurectTrue:
        case SpellSummonAirElemental:
        case SpellSummonFireElemental:
        case SpellSummonEarthElemental:
        case SpellSummonWaterElemental:
        case SpellDimensionDoor:
        case SpellTownPortal:
            return 5;
        default:
            return 0;
        }
        break;
    case SingleVersion:
        switch(rec)
        {
        case SpellMassHaste:
            return SpellHaste;
        case SpellMassBless:
            return SpellBless;
        case SpellMassDispel:
            return SpellDispel;
        case SpellMassShield:
            return SpellShield;
        case SpellMassCurse:
            return SpellCurse;
        case SpellMassCure:
            return SpellCure;
        case SpellMassSlow:
            return SpellSlow;
        default:
            return rec;
        }
        break;
    case MassEffect:
        switch(rec)
        {
        case SpellMassHaste:
        case SpellMassBless:
        case SpellMassDispel:
        case SpellEarthquake:
        case SpellMassShield:
        case SpellMassCurse:
		case SpellMassCure:
        case SpellMassSlow:
        case SpellHolyWord:
        case SpellHolyShout:
        case SpellArmagedon:
        case SpellElementalStorm:
        case SpellDeathRipple:
        case SpellDeathWave:
            return 1;
        default:
            return 0;
        }
        break;
    case Friendly:
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
            return 1;
        default:
            return 0;
        }
    case Hostile:
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
            return 1;
        default:
            return 0;
        }
    case RequiredTarget:
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
            return 1;
        default:
            return 0;
        }
        break;
    case FrameStatus:
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
    case Combat:
        if(rec>=(int)FirstSpell && rec<=(int)SpellEarthquake)
            return 1;
        return 0;
    case Portrait:
        return objects[rec-FirstSpell].spr;
    case SpellPoints:
        return objects[rec-FirstSpell].spellpoints;
    case SpellPower:
        return objects[rec-FirstSpell].value;
    default:
        return 0;
    }
}

static const char* object_gets(int rec, int id)
{
    switch(id)
    {
    case Name:
        return objects[rec-FirstSpell].name;
    default:
        return "";
    }
}