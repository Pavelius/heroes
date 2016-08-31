#include "main.h"

static struct artifact_t
{
	int				effect;
	int				type;
	int				level;
	const char*		name;
	const char*		description[2];
} objects[LastArtifact-FirstArtifact+1] =
{
	{12, All, 4, "Ultimate Book of Knowledge", {"The %1 increases your knowledge by %2i."}},
	{12, All, 4, "Ultimate Sword of Dominion", {"The %1 increases your attack skill by %2i."}},
	{12, All, 4, "Ultimate Cloak of Protection", {"The %1 increases your defense skill by %2i."}},
	{12, All, 4, "Ultimate Wand of Magic", {"The %1 increases your spell power by %2i."}},
	{6, All, 4, "Ultimate Shield", {"The %1 increases your attack and defense skills by %2i each."}},
	{6, All, 4, "Ultimate Staff", {"The %1 increases your spell power and knowledge by %2i each."}},
	{4, All, 4, "Ultimate Crown", {"The %1 increases each of your basic skills by %2i points."}},
	{5000, Gold, 4, "Golden Goose", {"The %1 brings in an income of %2i gold per turn."}},
	{4, SpellPower, 3, "Arcane Necklace of Magic", {"The %1 increases your spell power by %2i."}},
	{2, SpellPower, 2, "Caster's Bracelet of Magic", {"The %1 increases your spell power by %2i."}},
	{2, SpellPower, 2, "Mage's Ring of Power", {"The %1 increases your spell power by %2i."}},
	{3, SpellPower, 3, "Witch's Broach of Magic", {"The %1 increases your spell power by %2i."}},
	{1, Morale, 1, "Medal of Valor", {"The %1 increases your morale."}},
	{1, Morale, 1, "Medal of Courage", {"The %1 increases your morale."}},
	{1, Morale, 1, "Medal of Honor", {"The %1 increases your morale."}},
	{1, Morale, 1, "Medal of Distinction", {"The %1 increases your morale."}},
	{-2, Morale, -1, "Fizbin of Misfortune", {"The %1 greatly decreases your morale by %2i."}},
	{1, Attack, 1, "Thunder Mace of Dominion", {"The %1 increases your attack skill by %2i."}},
	{1, Defence, 1, "Armored Gauntlets of Protection", {"The %1 increase your defense skill by %2i."}},
	{1, Defence, 1, "Defender Helm of Protection", {"The %1 increases your defense skill by %2i."}},
	{1, Attack, 1, "Giant Flail of Dominion", {"The %1 increases your attack skill by %2i."}},
	{2, 0, 3, "Ballista of Quickness", {"The %1 lets your catapult fire twice per combat round."}},
	{2, Defence, 2, "Stealth Shield of Protection", {"The %1 increases your defense skill by %2i."}},
	{3, Attack, 3, "Dragon Sword of Dominion", {"The %1 increases your attack skill by %2i."}},
	{2, Attack, 2, "Power Axe of Dominion", {"The %1 increases your attack skill by %2i."}},
	{3, Defence, 3, "Divine Breastplate of Protection", {"The %1 increases your defense skill by %2i."}},
	{2, Wisdow, 2, "Minor Scroll of Knowledge", {"The %1 increases your knowledge by %2i."}},
	{3, Wisdow, 3, "Major Scroll of Knowledge", {"The %1 increases your knowledge by %2i."}},
	{4, Wisdow, 3, "Superior Scroll of Knowledge", {"The %1 increases your knowledge by %2i."}},
	{5, Wisdow, 3, "Foremost Scroll of Knowledge", {"The %1 increases your knowledge by %2i."}},
	{1000, Gold, 3, "Endless Sack of Gold", {"The %1 provides you with %2i gold per day."}},
	{500, Gold, 3, "Endless Bag of Gold", {"The %1 provides you with %2i gold per day."}},
	{100, Gold, 2, "Endless Purse of Gold", {"The %1 provides you with %2i gold per day."}},
	{0, 0, 3, "Nomad Boots of Mobility", {"The %1 increase your movement on land."}},
	{0, 0, 3, "Traveler's Boots of Mobility", {"The %1 increase your movement on land."}},
	{1, Luck, 1, "Lucky Rabbit's Foot", {"The %1 increases your luck in combat."}},
	{1, Luck, 1, "Golden Horseshoe", {"The %1 increases your luck in combat."}},
	{1, Luck, 1, "Gambler's Lucky Coin", {"The %1 increases your luck in combat."}},
	{1, Luck, 1, "Four-Leaf Clover", {"The %1 increases your luck in combat."}},
	{0, 0, 3, "True Compass of Mobility", {"The %1 increases your movement on land and sea."}},
	{0, 0, 2, "Sailor's Astrolabe of Mobility", {"The %1 increases your movement on sea."}},
	{0, 0, 2, "Evil Eye", {"The %1 reduces the casting cost of curse spells by half."}},
	{2, 0, 1, "Enchanted Hourglass", {"The %1 extends the duration of all your spells by %2i turns."}},
	{0, 0, 2, "Gold Watch", {"The %1 doubles the effectiveness of your hypnotize spells."}},
	{0, 0, 2, "Skullcap", {"The %1 halves the casting cost of all mind influencing spells."}},
	{0, 0, 1, "Ice Cloak", {"The %1 halves all damage your troops take from cold spells."}},
	{0, 0, 1, "Fire Cloak", {"The %1 halves all damage your troops take from fire spells."}},
	{0, 0, 1, "Lightning Helm", {"The %1 halves all damage your troops take from lightning spells."}},
	{50, SpellColdRay, 2, "Evercold Icicle", {"The %1 causes your cold spells to do %2i percent more damage to enemy troops."}},
	{50, SpellFireBall, 2, "Everhot Lava Rock", {"The %1 causes your fire spells to do %2i percent more damage to enemy troops."}},
	{50, SpellLightingBolt, 3, "Lightning Rod", {"The %1 causes your lightning spells to do %2i percent more damage to enemy troops."}},
	{0, SpellBless, 1, "Snake-Ring", {"The %1 halves the casting cost of all your bless spells."}},
	{0, 0, 2, "Ankh", {"The %1 doubles the effectiveness of all your resurrect and animate spells."}},
	{0, 0, 2, "Book of Elements", {"The %1 doubles the effectiveness of all your summoning spells."}},
	{0, 0, 2, "Elemental Ring", {"The %1 halves the casting cost of all summoning spells."}},
	{0, 0, 1, "Holy Pendant", {"The %1 makes all your troops immune to curse spells."}},
	{0, 0, 1, "Pendant of Free Will", {"The %1 makes all your troops immune to hypnotize spells."}},
	{0, 0, 1, "Pendant of Life", {"The %1 makes all your troops immune to death spells."}},
	{0, 0, 1, "Serenity Pendant", {"The %1 makes all your troops immune to berserk spells."}},
	{0, 0, 1, "Seeing-eye Pendant", {"The %1 makes all your troops immune to blindness spells."}},
	{0, 0, 1, "Kinetic Pendant", {"The %1 makes all your troops immune to paralyze spells."}},
	{0, 0, 1, "Pendant of Death", {"The %1 makes all your troops immune to holy spells."}},
	{0, 0, 3, "Wand of Negation", {"The %1 protects your troops from the Dispel Magic spell."}},
	{50, 0, 1, "Golden Bow", {"The %1 eliminates the %2i percent penalty for your troops shooting past obstacles. (e.g. castle walls)"}},
	{1, 0, 1, "Telescope", {"The %1 increases the amount of terrain your hero reveals when adventuring by %2i extra square."}},
	{10, 0, 1, "Statesman's Quill", {"The %1 reduces the cost of surrender to %2i percent of the total cost of troops you have in your army."}},
	{10, 0, 3, "Wizard's Hat", {"The %1 increases the duration of your spells by %2i turns."}},
	{2, SpellPoints, 2, "Power Ring", {"The %1 returns %2i extra power points/turn to your hero."}},
	{0, 0, 2, "Ammo Cart", {"The %1 provides endless ammunition for all your troops that shoot."}},
	{-250, Gold, -1, "Tax Lien", {"The %1 costs you %2i gold pieces/turn."}},
	{0, 0, -1, "Hideous Mask", {"The %1 prevents all 'wandering' armies from joining your hero."}},
	{1, Sulfur, 3, "Endless Pouch of Sulfur", {"The %1 provides %2i unit of sulfur per day."}},
	{1, Mercury, 3, "Endless Vial of Mercury", {"The %1 provides %2i unit of mercury per day."}},
	{1, Gems, 3, "Endless Pouch of Gems", {"The %1 provides %2i unit of gems per day."}},
	{1, Wood, 2, "Endless Cord of Wood", {"The %1 provides %2i unit of wood per day."}},
	{1, Ore, 2, "Endless Cart of Ore", {"The %1 provides %2i unit of ore per day."}},
	{1, Crystal, 3, "Endless Pouch of Crystal", {"The %1 provides %2i unit of crystal/day."}},
	{1, AttackDefence, 2, "Spiked Helm", {"The %1 increases your attack and defense skills by %2i each."}},
	{2, AttackDefence, 3, "Spiked Shield", {"The %1 increases your attack and defense skills by %2i each."}},
	{1, SpellPowerWisdow, 2, "White Pearl", {"The %1 increases your spell power and knowledge by %2i each."}},
	{2, SpellPowerWisdow, 3, "Black Pearl", {"The %1 increases your spell power and knowledge by %2i each."}},
	//
	{0, 0, 0, "Magic Book", {"The %1 enables you to cast spells."}},
};

static int object_get(int rec, int id)
{
	artifact_t& e = objects[rec-FirstArtifact];
	switch(id)
	{
	case Valid:
		return 1;
	case Level:
		return e.level;
	case Attack:
	case Defence:
		if(e.type==id
			|| e.type==AttackDefence
			|| e.type==All)
			return e.effect;
		return 0;
	case Wisdow:
	case SpellPower:
		if(e.type==id
			|| e.type==SpellPowerWisdow
			|| e.type==All)
			return e.effect;
		return 0;
	default:
		if(e.type==id)
			return e.effect;
		return 0;
	}
}
