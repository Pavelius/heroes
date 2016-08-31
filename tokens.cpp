#include "main.h"

static const char* tokens_names[] =
{
	"Empthy",
	"Luck", "Cursed", "Awful", "Bad", "Normal", "Good", "Great", "Irish",
	"Morale", "Treason", "Awful", "Poor", "Normal", "Good", "Great", "Blood",
	"Speed", "Crawling", "Very Slow", "Slow", "Average", "Fast", "Very Fast", "Ultra Fast",
	"Easy", "Normal", "Hard", "Very Hard", "Impossible",
	"Beginner", "Normal", "Hard", "Expert",
	"Barbarian", "Knight", "Necromancer", "Sorcerer", "Warlock", "Wizard", "Elementals",
	"Attack", "Defence", "Spell Power", "Wisdow", "Magic", "Magic (max)",
	"Hits", "Hits", "Shoots",
	"Wood", "Mercury", "Ore", "Sulfur", "Crystal", "Gems", "Gold",
	"Neutral", "Blue", "Green", "Red", "Yellow", "Orange", "Purple",
	// Skills
	"Pathfinding", "Archery", "Logistics", "Scounting",
	"Diplomacy", "Navigation",  "Leadership", "Wisdom",
	"Mysticism", "Luck", "Ballistics", "Eagle Eye",
	"Necromancy", "Estates",
	//
	"Basic", "Advanced", "Expert",
};

static const char* object_gets(int rec, int id)
{
	switch(id)
	{
	case Name:
		return tokens_names[rec];
	}
	return "";
}

static int object_get(int rec, int id)
{
    switch(id)
    {
    case Rating:
        switch(rec)
        {
        case EasyDifficulty: return 70;
        case NormalDifficulty: return 100;
        case HardDifficulty: return 120;
        case VeryHardDifficulty: return 160;
        case ImpossibleDifficulty: return 180;
        default: return 0;
        }
    case Attack:
        switch(rec)
        {
        case BarbarianCaptain: return 3;
        case KnightCaptain: return 2;
        case NecromancerCaptain: return 1;
        default: return 0;
        }
    case Defence:
        switch(rec)
        {
        case KnightCaptain: return 2;
        case BarbarianCaptain:
        case WizardCaptain: return 1;
        default: return 0;
        }
    case Wisdow:
        switch(rec)
        {
        case SorcererCaptain: return 4;
        case WizardCaptain:
        case WarlockCaptain:
        case NecromancerCaptain: return 3;
        default: return 1;
        }
    case SpellPower:
        switch(rec)
        {
        case WarlockCaptain: return 4;
        case SorcererCaptain:
        case WizardCaptain:
        case NecromancerCaptain: return 3;
        default: return 1;
        }
    default:
        return 0;
    }
}

const char* sztag(const char* name)
{
	return name;
}