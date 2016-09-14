#include "array.h"
#include "base.h"
#include "command.h"
#include "crt.h"
#include "drawable.h"
#include "io.h"
#include "input.h"
#include "pointc.h"

enum tokens
{
	Empthy,
	// Basic ability
	Attack, Defence, SpellPower, Wisdow,
	// Skills
	SkillPathfinding, SkillArchery, SkillLogistics, SkillScounting, SkillDiplomacy,
	SkillNavigation, SkillLeadership, SkillWisdom, SkillMysticism, SkillLuck,
	SkillBallistics, SkillEagleEye, SkillNecromancy, SkillEstates,
	FirstSkill = SkillPathfinding, LastSkill = SkillEstates,
	//
	Luck, LuckCursed, LuckAwful, LuckBad, LuckNormal, LuckGood, LuckGreat, LuckIrish,
	Morale, MoraleTreason, MoraleAwful, MoralePoor, MoraleNormal, MoraleGood, MoraleGreat, MoraleBlood,
	Speed, SpeedCrawling, SpeedVerySlow, SpeedSlow, SpeedAverage, SpeedFast, SpeedVeryFast, SpeedUltraFast,
	// Difficulty
	EasyDifficulty, NormalDifficulty, HardDifficulty, VeryHardDifficulty, ImpossibleDifficulty,
	LevelEasy, LevelNormal, LevelHard, LevelExpert,
	// Heroes types
	Barbarian, Knight, Necromancer, Sorcerer, Warlock, Wizard, SandsWarriors,
	// Ability
	SpellPoints, SpellPointsMax,
	HitPoints, HitPointsMax, Shoots,
	// Resources
	Gold, Wood, Mercury, Ore, Sulfur, Crystal, Gems,
	FirstResource = Gold, LastResource = Gems,
	// Skills level
	SkillLevelBasic, SkillLevelAvanced, SkillLevelExpert,
	// Players
	PlayerBlue, PlayerGreen, PlayerRed, PlayerYellow, PlayerOrange, PlayerPurple,
	FirstPlayer = PlayerBlue, LastPlayer = PlayerPurple,
	// Main menu
	NewGame, LoadGame, Credits, HightScores,
	StandartGame, CampaignGame, MultiplayerGame,
	HotSeatGame,
	AutoCombat, Character, RunAway, Surrender, CastSpell, Recruit, RecruitLast, Index, MoveTo, MakeAction,
	Cursor, CursorAdventure, CursorCombat, All, AttackDefence, SpellPowerWisdow, Information,
	Accept, Learn, Count, Random, Portrait, Rating, ChangeMode, Dismiss, Yes, No,
	EndTurn, Damage, Block, Income,
	NameMulti, Level, Target, Side, Base, Expire,
	SingleVersion, RequiredTarget, MassEffect, DamageMin, DamageMax, Frame,
	Move, Fly, MagicImmunity, ElementsImmunity, Undead, Dragon,
	SourceIndex, TargetUnit, TargetIndex,
	Upgrade, Downgrade,
	OneTime,
	Difficulty, PlayerCount,
	ShipMaster, Direction, Player, PlayerType,
	AnimationType,
	DefendThisTurn, Squad, Experience,
	MovePoints, MovePointsMax, MovePointsSea, MovePointsLand, Moveable, Quality, LackResource,
	ArtifactCount, Code,
	AttackerWin, DefenderWin, AlreadyMoved, AlreadyDefended,
	// Map size
	SmallSize, MediumSize, LargeSize, XLargeSize,
	// Player type
	Human, Computer, ComputerOnly, MultiPlayers,
	// Grid directions
	HexRight, HexRightDown, HexLeftDown, HexLeft, HexLeftUp, HexRightUp, HexCenter,
	// Terrain (порядок важен!!!)
	Water, Grass, Snow, Swamp, Lava, Desert, Dirt, Wastelands, Beach,
	FirstTerrain = Water, LastTerrain = Beach,
	// spells mode
	CombatSpells, AdventureSpells, Spells,
	// Spells
	SpellFireBall, SpellFireBlast, SpellLightingBolt, SpellChainLighting, SpellTeleport,
	SpellCure, SpellMassCure, SpellResurrect, SpellRessurectTrue, SpellHaste, SpellMassHaste, SpellSlow, SpellMassSlow,
	SpellBlind, SpellBless, SpellMassBless, SpellStoneSkin, SpellSteelSkin, SpellCurse, SpellMassCurse,
	SpellHolyWord, SpellHolyShout, SpellAntimagic, SpellDispel, SpellMassDispel, SpellMagicArrow, SpellBerserker,
	SpellArmagedon, SpellElementalStorm, SpellMeteorShower, SpellParalyze, SpellHypnotize, SpellColdRay, SpellColdRing,
	SpellDisruptingRay, SpellDeathRipple, SpellDeathWave, SpellDragonSlayer, SpellBloodLust, SpellAnimateDead, SpellMirrorImage,
	SpellShield, SpellMassShield,
	SpellSummonEarthElemental, SpellSummonAirElemental, SpellSummonFireElemental, SpellSummonWaterElemental,
	SpellEarthquake,
	SpellViewMines, SpellViewResources, SpellViewArtifacts, SpellViewTowns, SpellViewHeroes, SpellViewAll,
	SpellIdentifyHero, SpellSummonBoat,
	SpellDimensionDoor, SpellTownGate, SpellTownPortal, SpellVisions,
	SpellHaunt, SpellSetEartGuadrdian, SpellSetAirGuadrdian, SpellSetFireGuadrdian, SpellSetWaterGuadrdian,
	SpellStone,
	FirstSpell = SpellFireBall, LastSpell = SpellStone,
	// Heroes
	LordKillburn, SirGallanth, Ector, Gvenneth, Tyro, Ambrose, Ruby, Maximus, Dimitry,
	Thundax, Fineous, Jojosh, CragHack, Jezebel, Jaclyn, Ergon, Tsabu, Atlas,
	Astra, Natasha, Troyan, Vatawna, Rebecca, Gem, Ariel, Carlawn, Luna,
	Arie, Alamar, Vesper, Crodo, Barok, Kastore, Agar, Falagar, Wrathmont,
	Myra, Flint, Dawn, Halon, Myrini, Wilfrey, Sarakin, Kalindra, Mandigal,
	Zom, Darlana, Zam, Ranloo, Charity, Rialdo, Roxana, Sandro, Celia,
	Roland, Corlagon, Eliza, Archibald, Halton, Bax,
	FirstHero = LordKillburn, LastGameHero = Celia, LastHero = Bax,
	// Henchmens
	BarbarianCaptain, KnightCaptain, NecromancerCaptain, SorcererCaptain, WarlockCaptain, WizardCaptain,
	FirstCaptain = BarbarianCaptain, LastCaptain = WizardCaptain,
	// Artifacts
	UltimateBook, UltimateSword, UltimateCloack, UltimateWand, UltimateShied, UltimateStaff, UltimateCrown, GoldenGoose,
	ArcaneNecklage, CasterBracers, MageRing, WitchesBroach,
	MedalValor, MedalCourage, MedalHonor, MedalDistinction, FizbinMesfortune,
	ThunderMace, ArmouredGauntlets, DefenderHelm, GiantFlail, Ballista, StealthShield,
	DragonSword, PowerAxe, DivineBreastPlate,
	MinorScroll, MajorScroll, SuperiorScroll, ForemostScroll,
	EndlessSackOfGold, EndlessBagOfGold, EndlessPurseOfGold,
	NomadBootsOfMobility, TravellerBootsOfMobility,
	RabbitFoot, GoldenHorseshoe, GamblerLuckCoin, fourLeafClover,
	TrueCompassMobility, SailorsAstrolableMobility,
	EvilEye, EnchantedHourglass, GoldenWatch, SkullCap, IceCloack, FireCloack, LightingHelm,
	EvercoldIcicle, EverhotLavaRock, LightingRod,
	SnakeRing,
	Ankh, BookElements, ElementalRing,
	PedantHoly, PendantFreeWill, PendantLife, PendantSerenity, PendantSeekingEye, PendantKinetic, PendantDeath,
	WandNegation, GoldenBow, Telescope, StatemanQuill, WizardHat, PowerRing, AmmoCart,
	TaxLien, HideousMask,
	EndlessPouchSulfur, EndlessPouchMercury, EndlessPouchGems, EndlessCordWood, EndlessCartOre, EndlessPouchCrystal,
	SpikedHelm, SpikedShield,
	WhitePearl, BlackPearl,
	MagicBook,
	FirstArtifact = UltimateBook, LastArtifact = MagicBook,
	// Action in combat
	ActorWait, ActorWarn, ActorPalmFace, Shoot, Dead, Killed,
	Skip,
	// Knight creatures
	Peasant,
	Archer, Ranger,
	Pikeman, VeteranPikeman,
	Swordsman, MasterSwordsman,
	Cavalry, Champion,
	Paladin, Crusader,
	// Barbarian creatures
	Goblin,
	Orc, OrcChief,
	Wolf,
	Ogre, OgreLord,
	Troll, WarTroll,
	Cyclop,
	//
	Sprite,
	Dwarf, BattleDwarf,
	Elf, GrandElf,
	Druid, GreaterDruid,
	Unicorn,
	Phoenix,
	//
	Centaur,
	Gargoyle,
	Griffin,
	Minotaur, MinotaurKing,
	Hydra,
	GreenDragon, RedDragon, BlackDragon,
	//
	Halfling,
	Boar,
	IronGolem, SteelGolem,
	Roc,
	Mage, ArchMage,
	Giant, Titan,
	// Necromancer creatures
	Skeleton,
	Zombie, MutantZombie,
	Mummy, RoyalMummy,
	Vampire, VampireLord,
	Lich, PowerLich,
	BoneDragon,
	// Neutral creatures
	Rogue,
	Nomand,
	Ghost,
	Genie,
	Medusa,
	EarthElement,
	AirElement,
	FireElement,
	WaterElement,
	// Random creatures
	MonsterRnd1,
	MonsterRnd2,
	MonsterRnd3,
	MonsterRnd4,
	MonsterRnd,
	FirstMonster = Peasant, LastMonster = MonsterRnd,
	// Buildings
	Castle, ThievesGuild, Tavern, Shipyard, Well, Statue,
	LeftTurret, RightTurret, Moat,
	MarketPlace, Captain, Well2, SpecialBuilding,
	Dwelving1, Dwelving2, Dwelving3, Dwelving4, Dwelving5, Dwelving6,
	MageGuild, Tent,
	FirstBuilding = Castle, LastBuilding = Tent,
	DisableCastleUpgrade,
	Rumor, Riddle,
	// Map objects (order is impotant)
	WaterChest, AlchemyLab, Sign, WaterBue, // 0x80-0x83
	DeadSkeleton, DemonCave, TreasureChest, FaerieRing, // 0x84-0x87
	CampFire, Fountain, Gazebo, AncientLamp, // 0x88-0x8B
	Graveyard, ArcherHouse, GoblinHut, DwarfCottage, // 0x8C-0x8F
	PeasantHut, Map0x91, Map0x92, Event, // 0x90-0x93
	DragonCity, LitghHouse, WaterWheel, Mines, // 0x95-0x97
	Monster, Obelisk, Oasis, Resource, // 0x98-0x9B
	Map0x9C, SawMill, Oracle, Shrine1, // 0x9C-0x9F
	ShipWreck, Map0xA1, DesertTent, CastleOnMap, // 0xA0-0xA3
	Teleporter, RogueCamp, Map0xA6, WhirlPool, // 0xA4-0xA7
	WindMill, Artifact, Map0xAA, Boat, // 0xA8-0xAB
	RndUltimateArtifact, RndArtifact, RndResource, RndMonster, // 0xAC-0xAF
	RndTown, RndCastle, Map0xB2, RndMonster1, // 0xB0-0xB3
	RndMonster2, RndMonster3, RndMonster4, Hero, // 0xB4-0xB7
	Map0xB8, Map0xB9, WatchTower, SpriteHouse, // 0xB8-0xBB
	SpriteHouseCity, Ruins, Fort, TradingPost, // 0xBC-0xBF
	AbandoneMine, ThatchedHut, StandingStones, Idol, // 0xC0-xC3
	TreeKnowledge, DoctorHut, Temple, HillFort, // 0xC4-0xC7
	HalflingHole, MercenaryCamp, Shrine2, Shrine3, // 0xC8-0xCB
	Pyramid, CityDead, Excavation, Sphinx, // 0xCC-0xCF
	Wagon, Map0xD1, ArtesianSpring, TrollBridge, // 0xD0-0xD3
	WateringHole, WitchHut, Xanadu, CentautCave, // 0xD4-0xD7
	LeanTo, WaterBoatStation, Flotsam, DeRelictShip, // 0xD8-0xDB
	ShipWreckSurvivor, Bottle, MagicWell, MagicGarden, // 0xDC-0xDF
	ObservationTower, FreemanFoundry, Map0xE2, Map0xE3, // 0xE0-0xE3
	Map0xE4, Map0xE5, Map0xE6, Map0xE7, // 0xE4-0xE7
	Map0xE8, Reefs, Map0xEA, Map0xEB, // 0xE8-0xEB
	Mermaid, Sirens, MagiHut, MagiEye, // 0xEC-0xEF
	AlchemyTower, Stables, Arena, BarrowMounds, // 0xF0-0xF3
	RndArtifact1, RndArtifact2, RndArtifact3, Barrier, // 0xF4-0xF7
	TravellerTent, Map0xF9, Map0xFA, Jail, // 0xF8-0xFB
	FireAltar, AirAltar, EarthAltar, WaterAltar, // 0xFC-0xFF
	FirstObject = WaterChest, LastObject = WaterAltar,
	LastAction = LastObject,
	GeniusLamp,
	Mushrooms, Volcano, DiggingHole, Flowers,
	Lake, Hill, Mountains, Trees,
	Rock, Cliff, Hole, Crack, Road, Stream,
	RiverDeltaDown, RiverDeltaUp, Stumps, Leprechaunt,
	// Custom tokens
	FirstCreatureCount, LastCreatureCount = FirstCreatureCount + 6,
	FirstCastle, LastCastle = FirstCastle + 128,
	FirstEvent, LastEvent = FirstEvent + 128,
	FirstSign, LastSign = FirstSign + 128,
	FirstArtifactIndex, LastArtifactIndex = FirstArtifactIndex + 13,
	FirstTroopsIndex, LastTroopsIndex = FirstTroopsIndex + 5 * 2 - 1,
	FirstRecruit, LastRecruit = FirstRecruit + 6,
	FirstCombatant, LastCombatant = FirstCombatant + 5 + 5 + 10,
	FirstEffect, LastEffect = FirstEffect + 32,
	FirstMapObject, LastMapObject = FirstMapObject + 128 * 256
};
enum blocks
{
	PassableSquadWideLeft = 245, PassableSquadWideRight,
	BlockSquad, BlockSquadWideLeft, BlockSquadWideRight, BlockTerrain,
};
enum map_types
{
	TypeShadow, TypeHighObject, TypeBlock, TypeAction, TypeAttack,
};
enum image_flags
{
	AFMirror = 1,
	AFNoOffset = 2,
	AFCenter = 4,
	AFCenterV = 8,
};
enum draw_flags
{
	DWCombat, DWObjects,
	DWMask = 0x0F,
};
namespace res
{
	enum tokens
	{
		FntBIGFONT, FntSMALFONT, SPELLS, PalKB, TEXTBAR,
		FONT, SMALFONT, CATAPULT, TENT, LICHCLOD,
		BOULDER, VIEWGEN, VIEWARMY, BOOK, ADVBORD,
		ADVBORDE, HEROES, TisGROUND32, KNGT32, BARB32,
		SORC32, WRLK32, WZRD32, NECR32, SHADOW32,
		B_FLAG32, G_FLAG32, R_FLAG32, Y_FLAG32, O_FLAG32,
		P_FLAG32, BOAT32, B_BFLG32, G_BFLG32, R_BFLG32,
		Y_BFLG32, O_BFLG32, P_BFLG32, BLUEFIRE, REDFIRE,
		ELECTRIC, PHYSICAL, REDDEATH, MAGIC01, MAGIC02,
		MAGIC03, MAGIC04, MAGIC06, MAGIC07, MAGIC08,
		FIREBALL, FIREBAL2, STORM, METEOR, ADVBTNS,
		RADAR, BinEDITPALT, STRIP, TREASURY, RESOURCE,
		TOWNFIX, TOWNNAME, BinBANKBOX, BinADV_WIND, BinMAGEWIND,
		BinCASLWIND, BinTHIEFWIN, BinWELLWIND, BUYBUILD, BUYBUILE,
		BinRECRUIT0, BinRECRUIT1, BUILDING, SYSTEM, TOWNWIND,
		MONS32, ART32, FLAG32, BinARMYWIN, TAVWIN,
		BinTAVWIN, BinSHIPWIND, BinRCRTHERO, BinHEROWIND, HEROBKG,
		BinTOWNSTRP, ARTIFACT, BinSTATBAR, BinSPELLWIN, SCROLL,
		LOCATORS, BinQHERO0, BinQHERO1, BinQTOWN1, BinQWIKINFO,
		QWIKHERO, QWIKTOWN, QWIKINFO, BinSPLITWIN, BinVGENWIN,
		PUZZLE, BinCMBTWIN, SWAPWIN, BinSWAPWIN, SWAPBTN,
		BinREQUEST, BinREQUESTS, REQUEST, REQUESTS, REQBKG,
		REQSBKG, PORT0000, PORT0001, PORT0002, PORT0003,
		PORT0004, PORT0005, PORT0006, PORT0007, PORT0008,
		PORT0009, PORT0010, PORT0011, PORT0012, PORT0013,
		PORT0014, PORT0015, PORT0016, PORT0017, PORT0018,
		PORT0019, PORT0020, PORT0021, PORT0022, PORT0023,
		PORT0024, PORT0025, PORT0026, PORT0027, PORT0028,
		PORT0029, PORT0030, PORT0031, PORT0032, PORT0033,
		PORT0034, PORT0035, PORT0036, PORT0037, PORT0038,
		PORT0039, PORT0040, PORT0041, PORT0042, PORT0043,
		PORT0044, PORT0045, PORT0046, PORT0047, PORT0048,
		PORT0049, PORT0050, PORT0051, PORT0052, PORT0053,
		PORT0054, PORT0055, PORT0056, PORT0057, PORT0058,
		PORT0059, PORT0090, PORT0091, PORT0092, PORT0093,
		PORT0094, PORT0095, BinSURRENDR, SURDRBKG, SURRENDR,
		RECRBKG, RECRUIT, BIGBAR, BinCPANEL, CPANBKG,
		CPANEL, WINLOSE, WINCMBT, WINCMBTB, BinWINCMBT,
		WavWSND00, WavWSND01, WavWSND02, WavWSND03, WavWSND04,
		WavWSND05, WavWSND06, WavWSND10, WavWSND11, WavWSND12,
		WavWSND13, WavWSND14, WavWSND15, WavWSND16, WavWSND20,
		WavWSND21, WavWSND22, WavWSND23, WavWSND24, WavWSND25,
		WavWSND26, BinTOWNWIND, WavLOOP0000, WavLOOP0001, WavLOOP0002,
		WavLOOP0003, WavLOOP0004, WavLOOP0005, WavLOOP0006, WavLOOP0007,
		WavLOOP0008, WavLOOP0009, WavLOOP0010, WavLOOP0011, WavLOOP0012,
		WavLOOP0013, WavLOOP0014, WavLOOP0015, WavLOOP0016, WavLOOP0017,
		WavLOOP0018, WavLOOP0019, WavLOOP0020, WavLOOP0021, WavLOOP0022,
		WavLOOP0023, WavLOOP0024, WavLOOP0025, WavLOOP0026, WavLOOP0027,
		WavCATSND00, WavCATSND02, BinEVNTWIN0, BinEVNTWIN1, BinEVNTWIN2,
		BinEVNTWIN3, BinEVNTWIN4, BinEVNTWIN5, BinEVNTWIN6, TisCLOF32,
		CLOP32, APANEL, APANBKG, BinAPANEL, CLOUDLUK,
		ROUTE, TisSTON, RESSMALL, SUNMOON, HOURGLAS,
		BRCREST, CREST, MORALEG, MORALEB, MINIPORT,
		PORTXTRA, MOBILITY, HEROLOGO, EXPMRL, MINIMON,
		BinBUYBUIL3, BinBUYBUIL4, BinBUYBUIL5, BinBUYBUIL6, HSBKG,
		HISCORE, BinHISCORE, WINLOSEB, NETBOX, BinNETBOX,
		BinRECRUIQ0, BinRECRUIQ1, RECR2BKG, CONGRATS, BinCONGRATS,
		SCROLL2, BinDATAENTR, REDBACK, BTNCMPGN, BTNCOM,
		BTNHOTST, BTNMODEM, BTNMP, BTNNET, BTNNEWGM,
		BTNNET2, BinSTPMAIN, BinSTPCMPGN, BinSTPCOM, BinSTPHOTST,
		BinSTPMODEM, BinSTPMP, BinSTPNET, BinSTPNEWGM, BinSTPNET2,
		WavBADLUCK, WavBADMRLE, WavBUILDTWN, WavDIGSOUND, WavGOODLUCK,
		WavGOODMRLE, WavKILLFADE, WavPICKUP01, WavPICKUP02, WavPICKUP03,
		WavPICKUP04, WavPICKUP05, WavPICKUP06, WavPICKUP07, WavPREBATTL,
		WavRSBRYFZL, WavTELEIN, SCENIBKG, BinSCENINFO, BinCONGSPRE,
		BORDEDIT, CELLWIN, BinCELLWIN, BinEDITWIND, ESCROLL,
		BinHEROEDIT, BinMONEDIT, OVERLAY, TERRAINS, BinTOWNEDIT,
		ARTFX, TEXTBACK, BinEDITNEW, BinCCYCLE00, BinCCYCLE01,
		BinCCYCLE02, BinCCYCLE03, BinCCYCLE04, BinCCYCLE05, BinCCYCLE06,
		BinCCYCLE07, ADVMCO, CMSECO, SPELCO, BmpADVMBW01,
		BmpADVMBW02, BmpADVMBW03, BmpADVMBW04, BmpADVMBW05, BmpADVMBW06,
		BmpADVMBW07, BmpADVMBW08, BmpADVMBW09, BmpADVMBW10, BmpADVMBW11,
		BmpADVMBW12, BmpADVMBW13, BmpADVMBW14, BmpADVMBW15, BmpADVMBW16,
		BmpADVMBW17, BmpADVMBW18, BmpADVMBW19, BmpADVMBW20, BmpADVMBW21,
		BmpADVMBW22, BmpADVMBW23, BmpADVMBW24, BmpADVMBW25, BmpADVMBW26,
		BmpADVMBW27, BmpADVMBW28, BmpADVMBW29, BmpADVMBW30, BmpADVMBW31,
		BmpADVMBW32, BmpADVMBW33, BmpADVMBW34, BmpADVMBW35, BmpADVMBW36,
		BmpADVMBW37, BmpADVMBW38, BmpADVMBW39, BmpADVMBW40, BmpADVMBW41,
		BmpCMSEBW01, BmpCMSEBW02, BmpCMSEBW03, BmpCMSEBW04, BmpCMSEBW05,
		BmpCMSEBW06, BmpCMSEBW07, BmpCMSEBW08, BmpCMSEBW09, BmpCMSEBW10,
		BmpCMSEBW11, BmpCMSEBW12, BmpCMSEBW13, BmpCMSEBW14, BmpCMSEBW15,
		BmpCMSEBW16, BmpCMSEBW17, BmpCMSEBW18, BmpCMSEBW19, BmpCMSEBW20,
		BmpCMSEBW21, BmpCMSEBW22, BmpCMSEBW23, BmpCMSEBW24, BmpCMSEBW25,
		BmpCMSEBW26, BmpCMSEBW27, BmpCMSEBW28, BmpCMSEBW29, BmpSPELBW00,
		BmpSPELBW01, BmpSPELBW02, BmpSPELBW03, BmpSPELBW04, BmpSPELBW05,
		BmpSPELBW06, BmpSPELBW07, BmpSPELBW08, BmpSPELBW09, BmpSPELBW10,
		BmpSPELBW11, BmpSPELBW12, BmpSPELBW13, BmpSPELBW14, BmpSPELBW15,
		BmpSPELBW16, BmpSPELBW17, BmpSPELBW18, BmpSPELBW19, BmpSPELBW20,
		BmpSPELBW21, BmpSPELBW22, BmpSPELBW23, BmpSPELBW24, BmpSPELBW25,
		BmpSPELBW26, BmpSPELBW27, BmpSPELBW28, BmpSPELBW29, BmpSPELBW30,
		BmpSPELBW31, BmpSPELBW32, BmpSPELBW33, BmpSPELBW34, BmpSPELBW35,
		BmpSPELBW36, BmpSPELBW37, BmpSPELBW38, BinSTPMCFG, BTNMCFG,
		BinSTPBAUD, BTNBAUD, BinSTPDC, BTNDC, BinSTPDCCFG,
		BTNDCCFG, MTNSNOW, MTNSWMP, MTNLAVA, MTNDSRT,
		MTNDIRT, MTNMULT, MTNCRCK, MTNGRAS, TREJNGL,
		TREEVIL, OBJPALET, EXTRAOVR, ROAD, TREFIR,
		TRESNOW, TREFALL, STREAM, CMBTMISC, BinRGBLOOKP,
		TREDECI, STONEBK2, STONEBAK, STONBACK, CBKGSWMP,
		CBKGCRCK, CBKGDIMT, CBKGDITR, CBKGGRMT, CBKGSNTR,
		CBKGSNMT, CBKGGRAV, CBKGGRTR, CBKGBEAC, CBKGWATR,
		CBKGDSRT, CBKGLAVA, MINOTAUR, CAVALRYR, GRIFFIN,
		DRAGBLAK, DRAGRED, DRAGGREE, ELF, ELF2,
		ELF__MSL, MEDUSA, CAVALRYB, BinMINOTFRM, BinCVLRYFRM,
		BinCVLR2FRM, BinELF__FRM, BinMEDUSFRM, BinGRIFFFRM, BinDRAGGFRM,
		BinDRAGRFRM, BinDRAGBFRM, BinHALFLFRM, VIEWARSM, MINILKMR,
		HALFLING, HALFLMSL, MUMMYW, BinMUMMYFRM, BOAR,
		BinBOAR_FRM, GARGOYLE, BinGARGLFRM, MANA, TITANBLA,
		TITANBLU, TITANMSL, BinTITANFRM, BinTITA2FRM, WOLF,
		BinWOLF_FRM, ROC, BinROC__FRM, VAMPIRE, BinVAMPIFRM,
		MAGE1, BinMAGE1FRM, COLDRAY, ICECLOUD, DISRRAY,
		GROUND4, GROUND6, GROUND12, MISC4, LETTER4,
		VWFLAG4, MISC6, LETTER6, VWFLAG6, MISC12,
		LETTER12, VWFLAG12, DROPLIST, DROPLISL, MINIHERO,
		PORTMEDI, BinSTPEMAIN, BinSTPESIZE, BinSTPENEW, BTNEMAIN,
		BTNESIZE, BTNENEW, BinSPECEDIT, OBJNCRCK, OBJNDIRT,
		OBJNDSRT, OBJNGRAS, OBJNGRA2, OBJNLAVA, OBJNSNOW,
		OBJNSWMP, OBJNWATR, OBJNWAT2, OBJNMULT, OBJNMUL2,
		OBJNLAV2, OBJNLAV3, TEXTBAK2, BinEVNTEDIT, LISTBOX,
		BinRUMREDIT, BinRIDLEDIT, COLDRING, COBJ0000, COBJ0001,
		COBJ0002, COBJ0003, COBJ0004, COBJ0005, COBJ0006,
		COBJ0007, COBJ0008, COBJ0009, COBJ0010, COBJ0011,
		COBJ0012, COBJ0013, COBJ0014, COBJ0015, COBJ0016,
		COBJ0017, COBJ0018, COBJ0019, COBJ0020, COBJ0021,
		COBJ0022, COBJ0023, COBJ0024, COBJ0025, COBJ0026,
		COBJ0027, COBJ0028, COBJ0029, COBJ0030, COBJ0031,
		ZOMBIE, BinZOMB_FRM, DRAGBONE, BinDRABNFRM, GHOST,
		BinGHOSTFRM, SKELETON, BinSKEL_FRM, PEASANT, BinPEAS_FRM,
		ARCHER, BinARCHRFRM, ARCH_MSL, LICH, BinLICH_FRM,
		LICH_MSL, YINYANG, BinGOLEMFRM, GOLEM, BinPALADFRM,
		PALADIN, BinGOBLNFRM, GOBLIN, BinCENTRFRM, CENTAUR,
		BinHYDRAFRM, HYDRA, OBJNRSRC, DUMMY, MINOTAU2,
		ARCHER2, PALADIN2, MAGE2, GOLEM2, VAMPIRE2,
		MUMMY2, ZOMBIE2, TOWNBKG5, TOWNBKG4, TOWNBKG3,
		TOWNBKG2, TOWNBKG1, TOWNBKG0, TWNBTENT, TWNBCSTL,
		TWNBDW_0, TWNBDW_2, TWNBDW_1, TWNBDW_3, TWNBDW_4,
		TWNBDW_5, TWNBMAGE, TWNBMARK, TWNBTVRN, TWNBTHIE,
		TWNBDOCK, TWNBBOAT, TWNBWELL, TWNBUP_1, TWNBUP_3,
		TWNBUP_4, TWNBSTAT, TWNBLTUR, TWNBRTUR, TWNBWEL2,
		TWNBMOAT, TWNBSPEC, TWNBCAPT, TWNBEXT0, TWNBEXT1,
		TWNBEXT2, TWNBEXT3, TWNKBOAT, TWNKCAPT, TWNKCSTL,
		TWNKDOCK, TWNKDW_0, TWNKDW_1, TWNKDW_2, TWNKDW_3,
		TWNKDW_4, TWNKDW_5, TWNKLTUR, TWNKMAGE, TWNKMARK,
		TWNKMOAT, TWNKRTUR, TWNKSPEC, TWNKSTAT, TWNKTENT,
		TWNKTHIE, TWNKTVRN, TWNKUP_1, TWNKUP_2, TWNKUP_3,
		TWNKUP_4, TWNKUP_5, TWNKWEL2, TWNKWELL, TWNKEXT0,
		TWNKEXT1, TWNKEXT2, TWNSMAGE, TWNSTHIE, TWNSTVRN,
		TWNSDOCK, TWNSTENT, TWNSWELL, TWNSCSTL, TWNSDW_0,
		TWNSDW_1, TWNSDW_2, TWNSDW_3, TWNSDW_4, TWNSDW_5,
		TWNSLTUR, TWNSMARK, TWNSMOAT, TWNSRTUR, TWNSSPEC,
		TWNSSTAT, TWNSUP_1, TWNSUP_2, TWNSUP_3, TWNSWEL2,
		TWNSBOAT, TWNSCAPT, TWNSEXT0, TWNSEXT1, TWNNSPEC,
		TWNNMAGE, TWNNTHIE, TWNNDOCK, TWNNWELL, TWNNTENT,
		TWNNCSTL, TWNNDW_0, TWNNDW_1, TWNNDW_2, TWNNDW_3,
		TWNNDW_4, TWNNDW_5, TWNNUP_1, TWNNUP_2, TWNNUP_3,
		TWNNUP_4, TWNNSTAT, TWNNLTUR, TWNNRTUR, TWNNMARK,
		TWNNWEL2, TWNNMOAT, TWNNBOAT, TWNNCAPT, TWNNEXT0,
		TWNZBOAT, TWNZCSTL, TWNZDOCK, TWNZDW_0, TWNZDW_1,
		TWNZDW_2, TWNZDW_3, TWNZDW_4, TWNZDW_5, TWNZLTUR,
		TWNZMAGE, TWNZMARK, TWNZMOAT, TWNZRTUR, TWNZSPEC,
		TWNZSTAT, TWNZTENT, TWNZTHIE, TWNZTVRN, TWNZUP_2,
		TWNZUP_4, TWNZUP_5, TWNZWEL2, TWNZWELL, TWNZCAPT,
		TWNZEXT0, TWNWBOAT, TWNWCAPT, TWNWCSTL, TWNWDOCK,
		TWNWDW_0, TWNWDW_1, TWNWDW_2, TWNWDW_3, TWNWDW_4,
		TWNWDW_5, TWNWLTUR, TWNWMAGE, TWNWMARK, TWNWMOAT,
		TWNWRTUR, TWNWSPEC, TWNWSTAT, TWNWTENT, TWNWTHIE,
		TWNWTVRN, TWNWUP5B, TWNWUP_3, TWNWUP_5, TWNWWEL2,
		TWNWWELL, TWNWEXT0, SMALLBAR, HSICONS, HSBTNS,
		SECSKILL, OGRE, PIKEMAN, SPRITE, SWORDSMN,
		BinOGRE_FRM, BinPIKMNFRM, BinSPRITFRM, BinSWRDSFRM, CSTLKNGT,
		CSTLBARB, CSTLSORC, CSTLWRLK, CSTLWZRD, CSTLNECR,
		BTNSHNGL, WELLBKG, WELLXTRA, OBJNTOWN, OBJNTWSH,
		OBJNTWBA, OBJNTWRD, DWARF, BinDWARFFRM, OGRE2,
		DWARF2, PIKEMAN2, SWORDSM2, LICH2, NGHSBKG,
		NGMPBKG, NGSPBKG, NGEXTRA, BinNGHS, BinNGMP,
		BinNGSP, NOMAD, BinNOMADFRM, FROTH, BOATSHAD,
		CASLWIND, BOATWIND, ORC, BinORC__FRM, ORC__MSL,
		TROLL, TROLLMSL, BinTROLLFRM, TROLL2, ORC2,
		BLDGXTRA, DRUID, DRUID2, BinDRUIDFRM, DRUIDMSL,
		WavBOARATTK, WavBOARKILL, WavBOARMOVE, WavBOARWNCE, WavCAVLATTK,
		WavCAVLKILL, WavCAVLMOVE, WavCAVLWNCE, WavDRGNATTK, WavDRGNKILL,
		WavDRGNMOVE, WavDRGNWNCE, WavELF_ATTK, WavELF_KILL, WavELF_MOVE,
		WavELF_SHOT, WavELF_WNCE, WavGARGATTK, WavGARGKILL, WavGARGMOVE,
		WavGARGWNCE, WavGOLMATTK, WavGOLMKILL, WavGOLMMOVE, WavGOLMWNCE,
		WavGRIFATTK, WavGRIFKILL, WavGRIFMOVE, WavGRIFWNCE, WavHALFATTK,
		WavHALFKILL, WavHALFMOVE, WavHALFSHOT, WavHALFWNCE, WavMAGEATTK,
		WavMAGEKILL, WavMAGEMOVE, WavMAGESHOT, WavMAGEWNCE, WavMEDSATTK,
		WavMEDSKILL, WavMEDSMOVE, WavMEDSWNCE, WavMINOATTK, WavMINOKILL,
		WavMINOMOVE, WavMINOWNCE, WavMUMYATTK, WavMUMYKILL, WavMUMYMOVE,
		WavMUMYWNCE, WavROC_ATTK, WavROC_KILL, WavROC_MOVE, WavROC_WNCE,
		WavSKELATTK, WavSKELKILL, WavSKELMOVE, WavSKELWNCE, WavTITNATTK,
		WavTITNKILL, WavTITNMOVE, WavTITNSHOT, WavTITNWNCE, WavWOLFATTK,
		WavWOLFKILL, WavWOLFMOVE, WavWOLFWNCE, OBJNARTI, EDITOR,
		SPELLINF, WavPIKEMOVE, WavPIKEWNCE, WavPIKEKILL, WavPIKEATTK,
		WavOGREWNCE, WavOGREKILL, WavOGREATTK, WavOGREMOVE, WavORC_SHOT,
		WavORC_ATTK, WavORC_KILL, WavORC_MOVE, WavORC_WNCE, WavPLDNMOVE,
		WavPLDNWNCE, WavPLDNKILL, WavPLDNATTK, WavPSNTMOVE, WavPSNTWNCE,
		WavPSNTKILL, WavPSNTATTK, WavSPRTMOVE, WavSPRTWNCE, WavSPRTKILL,
		WavSPRTATTK, WavSWDMMOVE, WavSWDMWNCE, WavSWDMKILL, WavSWDMATTK,
		WavZOMBWNCE, WavZOMBKILL, WavZOMBATTK, WavZOMBMOVE, WavTRLLSHOT,
		WavTRLLMOVE, WavTRLLATTK, WavTRLLWNCE, WavTRLLKILL, WavARCHSHOT,
		WavARCHATTK, WavARCHWNCE, WavARCHMOVE, WavARCHKILL, WavCNTRSHOT,
		WavCNTRATTK, WavCNTRMOVE, WavCNTRWNCE, WavCNTRKILL, WavDWRFMOVE,
		WavDWRFWNCE, WavDWRFKILL, WavDWRFATTK, WavGBLNMOVE, WavGBLNWNCE,
		WavGBLNKILL, WavGBLNATTK, WavGHSTMOVE, WavGHSTWNCE, WavGHSTKILL,
		WavGHSTATTK, WavHYDRMOVE, WavHYDRWNCE, WavHYDRKILL, WavHYDRATTK,
		WavNMADMOVE, WavNMADWNCE, WavNMADKILL, WavNMADATTK, WavBONEKILL,
		WavBONEMOVE, WavBONEATTK, WavBONEWNCE, WavLICHWNCE, WavLICHEXPL,
		WavLICHSHOT, WavLICHATTK, WavLICHKILL, WavLICHMOVE, WavVAMPKILL,
		WavVAMPEXT1, WavVAMPWNCE, WavVAMPMOVE, WavVAMPEXT2, WavVAMPATTK,
		CYCLOPS, BinCYCLOFRM, CASLXTRA, BinULTAEDIT, MAGEGLDK,
		MAGEGLDB, MAGEGLDS, MAGEGLDW, MAGEGLDZ, MAGEGLDN,
		CMBTHROK, CMBTHROB, CMBTHROS, CMBTHROW, CMBTHROZ,
		CMBTHRON, CASTBKGK, CASTLEK, CASTBKGB, CASTLEB,
		CASTBKGS, CASTLES, CASTBKGW, CASTLEW, CASTBKGZ,
		CASTLEZ, CASTBKGN, CASTLEN, SMALCLOD, TRADPOST,
		BinTRADPOST, UNICORN, BinUNICOFRM, ROGUE, BinROGUEFRM,
		HEROFL00, HEROFL01, HEROFL02, HEROFL03, HEROFL04,
		HEROFL05, HEROFL06, HEROEXTG, HEROEXTE, OBJNXTRA,
		BinTOWNPORT, LISTBOXS, KEEP, ADVEBTNS, HEROLOGE,
		SUNMOONE, STONBAKE, SCROLLE, LOCATORE, PRIMSKIL,
		CSTLCAPK, CSTLCAPB, CSTLCAPS, CSTLCAPW, CSTLCAPZ,
		CSTLCAPN, CAPTCOVR, CMBTCAPK, CMBTCAPB, CMBTCAPS,
		CMBTCAPW, CMBTCAPZ, CMBTCAPN, WavTELPTIN, WavTELPTOUT,
		WavRESURECT, WavSTORM, WavSTONESKI, WavSLOW, WavSHIELD,
		WavPROTECT, WavPARALIZE, WavMNRDEATH, WavMIRRORIM, WavMETEOR1,
		WavMASSSLOW, WavMASSSHIE, WavMASSHAST, WavMASSCURS, WavMASSCURE,
		WavMASSBLES, WavLIGHTBLT, WavCHAINLTE, WavHYPNOTIZ, WavHASTE,
		WavFIREBALL, WavDRGNSLAY, WavDISRUPTR, WavDIPMAGK, WavCURSE,
		WavCURE, WavBLOODLUS, WavBLIND, WavBLESS, WavBERZERK,
		WavANTIMAGK, WavRESURTRU, SYSTEME, BinFELEMFRM, FELEM,
		AELEM, WELEM, EELEM, GENIE, BinGENIEFRM,
		SPANBKG, SPANEL, SPANBTN, BinSPANEL, CSPANBKG,
		CSPANEL, CSPANBTN, BinCSPANEL, ESPANBKG, ESPANEL,
		ESPANBTN, BinESPANEL, BinVIEWPUZL, BinVIEWLGND, BinDIMDOOR,
		VIEWWRLD, VIEWRSRC, VIEWRTFX, VIEWHROS, VIEWTWNS,
		VIEW_ALL, VIEWMINE, VIEWPUZL, VIEWDDOR, LGNDXTRA,
		FRNG0001, FRNG0002, FRNG0003, FRNG0004, FRNG0005,
		FRNG0006, FRNG0007, FRNG0008, FRNG0009, FRNG0010,
		FRNG0011, FRNG0012, FRNG0013, PHOENIX, BinPHOENFRM,
		EDITPANL, EDITBTNS, COVR0001, COVR0002, COVR0003,
		COVR0004, COVR0005, COVR0006, COVR0007, COVR0008,
		COVR0009, COVR0010, COVR0011, COVR0012, COVR0013,
		COVR0014, COVR0015, COVR0016, COVR0017, COVR0018,
		COVR0019, COVR0020, COVR0021, COVR0022, COVR0023,
		COVR0024, ECPANEL, BinECPANEL, OBJNHAUN, BERZERK,
		BLESS, SHIELD, HASTE, HYPNOTIZ, PARALYZE,
		SPARKS, DRAGSLAY, BLIND, OVERVIEW, OVERBACK,
		BinOVERWIND, SCROLLCN, MINITOWN, MINICAPT, MINISS,
		MOATPART, MOATWHOL, CMBTSURR, CMBTLOS1, CMBTLOS2,
		CMBTLOS3, CMBTFLE1, CMBTFLE2, CMBTFLE3, WavCYCLATTK,
		WavCYCLKILL, WavCYCLMOVE, WavCYCLWNCE, WavDRUISHOT, WavDRUIATTK,
		WavDRUIKILL, WavDRUIMOVE, WavDRUIWNCE, WavGENIATTK, WavGENIKILL,
		WavGENIMOVE, WavGENIWNCE, WavROGUATTK, WavROGUKILL, WavROGUMOVE,
		WavROGUWNCE, WavUNICATTK, WavUNICKILL, WavUNICMOVE, WavUNICWNCE,
		XMIMIDI0002, XMIMIDI0003, XMIMIDI0004, XMIMIDI0005, XMIMIDI0006,
		XMIMIDI0007, XMIMIDI0008, XMIMIDI0009, XMIMIDI0010, XMIMIDI0011,
		XMIMIDI0013, XMIMIDI0014, XMIMIDI0015, XMIMIDI0017, XMIMIDI0018,
		XMIMIDI0042, XMIMIDI0043, SHNGANIM, WavWELMKILL, WavWELMMOVE,
		WavWELMATTK, WavWELMWNCE, WavEELMWNCE, WavEELMKILL, WavEELMMOVE,
		WavEELMATTK, WavAELMATTK, WavAELMMOVE, WavAELMWNCE, WavAELMKILL,
		WavPHOEKILL, WavPHOEMOVE, WavPHOEATTK, WavPHOEWNCE, WavFELMATTK,
		WavFELMKILL, WavFELMMOVE, WavFELMWNCE, WavKEEPSHOT, STONSKIN,
		STELSKIN, CURSE, CPANBKGE, CPANELE, APANBKGE,
		APANELE, EVIWWRLD, EVIWRSRC, EVIWRTFX, EVIWTWNS,
		EVIWHROS, EVIW_ALL, EVIWMINE, EVIWDDOR, EVIWPUZL,
		LGNDXTRE, SPANBKGE, SPANBTNE, CSPANBKE, CSPANBTE,
		CFLGSMAL, PORTCFLG, TRADPOSE, VIEWARME, CASLBAR,
		VGENBKG, VGENBKGE, WINLOSEE, WINCMBBE, SURRENDE,
		SURDRBKE, SPELLINL, WavSUMNELM, WavERTHQUAK, WavCOLDRING,
		WavCOLDRAY, WavMAGCAROW, WavARMGEDN, MONH0000, MONH0001,
		MONH0002, MONH0003, MONH0004, MONH0005, MONH0006,
		MONH0007, MONH0008, MONH0009, MONH0010, MONH0011,
		MONH0012, MONH0013, MONH0014, MONH0015, MONH0016,
		MONH0017, MONH0018, MONH0019, MONH0020, MONH0021,
		MONH0022, MONH0023, MONH0024, MONH0025, MONH0026,
		MONH0027, MONH0028, MONH0029, MONH0030, MONH0031,
		MONH0032, MONH0033, MONH0034, MONH0035, MONH0036,
		MONH0037, MONH0038, MONH0039, MONH0040, MONH0041,
		MONH0042, MONH0043, MONH0044, MONH0045, MONH0046,
		MONH0047, MONH0048, MONH0049, MONH0050, MONH0051,
		MONH0052, MONH0053, MONH0054, MONH0055, MONH0056,
		MONH0057, MONH0058, MONH0059, MONH0060, MONH0061,
		MONH0062, MONH0063, MONH0064, MONH0065, WavSTELSKIN,
		WavSTONSKIN, BinCAMPAIGN, CAMPBKGG, CAMPXTRG, CAMPBKGE,
		CAMPXTRE, CTRACK00, CTRACK01, CTRACK02, CTRACK03,
		CTRACK04, CTRACK05, CTRACK06, BROTHERS, WavEXPERNCE,
		WavNWHEROLV, WavTREASURE, WavDRAWBRG, WavH2MINE,
		Empthy
	};
	rect					box(int x, int y, tokens res, int frame, unsigned flags = 0);
	tokens					buildings(int race);
	bool					ishight(res::tokens icn, int index);
	int						getcount(tokens res);
	void*					get(tokens res);
	int						height(tokens res, int frame);
	tokens					map(int object);
	point					offset(tokens res, int n);
	int						ox(tokens res, int n);
	int						oy(tokens res, int n);
	tokens					getshooting(int rec);
	int						width(tokens res, int frame);
}
namespace draw
{
	struct state
	{
		state();
		~state();
	private:
		res::tokens			font;
		rect				clipping;
	};
	struct screenshoot
	{
		screenshoot();
		~screenshoot();
		void				blend(screenshoot& source);
		void				restore();
		void				redraw(drawable** objects, unsigned timeout);
		void				redraw(drawable** objects, unsigned timeout, drawable* e1, int stop = 0);
		void				redraw(drawable** objects, unsigned timeout, drawable* e1, drawable* e2);
	private:
		unsigned char*		bits;
	};
	enum justify
	{
		Left, Center, Right,
	};
	const int				width = 640;
	const int				height = 480;
	const int				scanline = 640;
	//
	bool					area(int x1, int y1, int x2, int y2);
	void					button(int x, int y, res::tokens icn, int id, int normal, int hilite, int pressed, int key = 0, unsigned flags = 0, const char* tips = 0, int param = 0);
	void					castle(int x, int y, int tile, int race, bool town, bool shadow = true);
	int						clipart(int x, int y, int id, int param, int param2 = 0, bool border = false);
	extern rect				clipping;
	extern unsigned			counter; // increse every 100 milliseconds
	bool					create(const char* title, unsigned milliseconds, bool fullscreen);
	void					cursor(res::tokens res, int id, int ox = 0, int oy = 0);
	int						dialog(int height);
	void					debug(int ox = 0, int oy = 0);
	void					definput(int id);
	void					edit(int x, int y, char* value, int maximum = 260);
	void					edit(int x, int y, int& value, int maximum, int minimum = 0);
	void					execute(int id, int param = 0, int param2 = 0);
	extern res::tokens		font;
	void					hexagon(int x, int y, unsigned char color);
	void					hexagonf(int x, int y, unsigned char alpha);
	void					image(int x, int y, res::tokens res, unsigned id, unsigned flags = 0, unsigned char* change = 0);
	void					imager(int x, int y, res::tokens res, int id, int mode = 0);
	int						input(bool wait_message = true);
	res::tokens				isevil(res::tokens evil, res::tokens good);
	int						isqrt(int num);
	void					line(int x1, int y1, int x2, int y2, unsigned char color);
	void					pixel(int x, int y, unsigned char color);
	unsigned char*			ptr(int x, int y);
	void					rectb(int x1, int y1, int x2, int y2, unsigned char color);
	void					rectf(int x1, int y1, int x2, int y2, unsigned char color);
	void					resource(int x, int y, const void* cost_ptr);
	void					route(int x, int y, int* rec, int w, int h, int distance);
	void					shadow(int x1, int y1, int x2, int y2, int intense);
	void					splitter(int x, int y, int id, res::tokens icn, int from, int to, int& current);
	void					status(const char* format, ...);
	void					status(int x1, int y1, int x2, int y2);
	void					text(int x, int y, const char* string, int count = -1);
	void					text(int x, int y, int width, justify jf, const char* string, int count = -1);
	int						textbc(const char* string, int width);
	int						textf(int width, const char* text);
	int						textf(int x, int y, int width, const char* text);
	int						texth();
	int						texth(const char* string, int width);
	int						textm(int x, int y, int width, justify jf, const char* string);
	void					textm(int x, int y, int width, int height, justify jf, const char* string);
	int						textw(char ch);
	int						textw(const char* string, int count = -1);
	void					tiles(int x, int y, res::tokens icn, int* rec, int w, int h);
	void					troops(int x, int y, int rec);
};
struct animation : public drawable
{
	struct state
	{
		state(animation* a);
		~state();
	private:
		animation*			a;
		unsigned			flags;
	};
	int						rec;
	point				    pos;
	res::tokens			    icn;
	short					frame, start, count, wait;
	unsigned				flags;
	//
	animation();
	animation(int rec, int action);
	animation(res::tokens base, int frame, int count);
	//
	void					clear();
	static animation*		find(drawable** objects, int rec);
	static int				fly(point* result, point from, point to, int step);
	int						getid() const override { return rec; }
	rect				    getrect() const override;
	point					getzpos() const override;
	virtual unsigned		getrate() const { return 1000 / 8; }
	point					getoffset(int start = -1) const;
	bool					hasanimation(tokens id, int param = 0) const;
	virtual bool			incframe();
	bool					islast() const { return frame == (start + count - 1); }
	void				    painting(point screen, unsigned flags = 0) const override;
	int						priority() const override { return rec; }
	void				    set(int rec, int action, int param = 0);
	virtual void			setaction(tokens action, int param = 0) { set(rec, action, param); }
	void					update() override;
};
namespace show
{
	namespace battle
	{
		void				attack(int rec, int enemy, int damage);
		int					dialog(int side); // dialog surrender, cast, flee
		void				effect(int rec, int type, int param = 0);
		void				fly(int rec, int target);
		void				leader(int side, tokens type);
		void				move(int rec, int target);
		void				shoot(int rec, int target, int damage);
		void				settings();
		int					target(int side, int spell, int target);
		int					unit(int rec, int casted);
	}
	namespace adventure
	{
		void				disapear(int player, int object);
		void				message(const char* text, ...);
		void				move(int from, int to, int hero, int player);
		void				screen(int player);
	}
	bool					ask(const char* format, const char* argument);
	int						build(int rec);
	void				    castle(int rec);
	void					fadeback(int count);
	int					    game(int player);
	void				    hero(tokens rec);
	void				    highscore();
	void					marketplace(int player);
	bool                    recruit(int rec, int& count, int maximum, void* available_resources);
	int						spellbook(int mid, tokens mode = CombatSpells);
	void					tavern();
	void                    tips(const char* text);
	int						thieves(int player);
	void                    unit(int rec, int parent, int count, int index);
}
namespace indexes
{
	int					    animate(res::tokens res, int start, int ticket, bool quantity);
	int					    buildings(int building, int level);
}
namespace hot
{
	extern int				command;
	extern int				level;
	extern int				param2;
};
struct order
{
	int						id;
	int						pos;
	int						param;
	int						target;
};
namespace animate
{
	void					hero(int x, int y, int rec, int ticket);
	void					heroshad(int x, int y, int rec, int ticket);
	void					heroflag(int x, int y, int rec, int ticket);
}
struct gamefile
{
	char					file[32];
	char					name[16];
	char					description[143];
	unsigned char			size;
	unsigned char			wins;
	unsigned char			lose;
	tokens					types[6];
	tokens					races[6];
	bool					blocked[6];
	tokens					level;
	tokens					difficult;
	bool					start_hero;
	//
	operator bool() const { return file[0] != 0; }
	void					clear();
	bool					load(const char* filename);
	void					players(int count);
	int						sides() const;
	void					validate();
};
namespace map
{
	enum directions
	{
		Center = 0,
		LeftUp = 0x01,
		Up = 0x02,
		RightUp = 0x04,
		Right = 0x08,
		RightDown = 0x10,
		Down = 0x20,
		LeftDown = 0x40,
		Left = 0x80,
	};
	extern unsigned char	width;
	extern unsigned char	height;
	extern point			camera;
	inline int				i2x(int i) { return i % 144; }
	inline int				i2y(int i) { return i / 144; }
	bool					isaction(int object);
	bool					isroad(unsigned char object, unsigned char index, unsigned char direct);
	bool					ispassable(int index);
	tokens					gettile(int index);
	void					jumpto(int index);
	bool					load(gamefile& e);
	unsigned				movecost(int index);
	unsigned				movecost(int index, unsigned char direct, unsigned pathfinding = 0);
	int						moveto(int index, directions direction);
	inline int				m2i(int x, int y) { return 144 * y + x; }
	directions				orient(int from, int to);
	int						revers(int direction);
	void					slide(directions type);
	namespace show
	{
		extern unsigned short   tiles[144 * 144];
		extern unsigned char    flags[144 * 144];
		extern unsigned char    type[144 * 144];
	}
	namespace route
	{
		void				around(int index, unsigned m);
		int					get(int index);
		short unsigned*		getpath();
		int					getpathcount();
		void        		load(int* rec, int index, int w, int h);
		bool				pathable(int index);
		void				walk(int start, int dest);
		void				wave(int start, int skill, int ship_master);
	}
};
namespace current
{
	extern int				damage;
	extern int				killed;
};
const int					cell_wd = 45;
const int					cell_hd = 52;
const int					cell_wr = 22;
const int					cell_hr = 13;
namespace combat
{
	const int				awd = 11;
	const int				ahd = 9;
	namespace setting
	{
		extern bool			movement;
		extern bool			cursor;
		extern bool			distance;
		extern bool			grid;
		extern bool			index;
		extern int			speed;
		extern int			info;
		extern bool			spells;
	}
	namespace enviroment
	{
		extern int			morale;
		extern int			luck;
	}
	void					add(int id, int count, int side);
	void					applyeffect(int rec, int effect, int value);
	int						attack(int att, int def);
	tokens					backward(tokens direction);
	void					board(int attacker, int defender);
	bool					candefend(int rec);
	bool					canmove(int rec);
	bool					canshoot(int rec, int target);
	bool					canattack(int rec, int target, int target_index, tokens from_direction);
	bool					cast(int side, int sid, int cid, bool run, bool free, bool interative);
	void					damage(int rec, int value);
	int						getarmycost(int side);
	int						getcombatant(int index);
	tokens					getdirection(int from, int to);
	int						geteffect(int rec, int id);
	unsigned char			getpassable(int index);
	int						getindex(int index);
	int						getwideindex(int index);
	bool					isattacker(int rec);
	bool					isenemy(int rec, int object);
	bool					ispassable(int index);
	point					i2h(int index);
	void					melee(int att, int def, bool interactive);
	void					move(int rec, int index, bool interactive);
	int						move(int* result, int start, int target, int speed);
	int						moveto(int index, int direction);
	int						opposition(int side);
	extern int				rounds;
	void					setaction(int rec, tokens action);
	void					seteffect(int rec, int id, int rounds);
	void					setindex(int rec, int index);
	void					shoot(int att, int def, bool interactive);
	void					start(int attacker, int defender);
	void					wave(int start, bool fly, tokens wdir, int speed);
}
struct list
{
	int						origin;
	int						current;
	int						maximum;
	//
	list();
	void					area(int x, int y, int count);
	void					box(int x, int y, int count);
	void					boxs(int x, int y, int count);
	void					draw(int x, int y, int sy, int sx, int count);
	virtual void			prerender() {}
	virtual void			row(int x, int y, int index) const = 0;
private:
	void					box(int x, int y, int count, res::tokens icn, int dy, int dx, int sbu, int sbd, int sbs, int sdbd, int sb, int bf, res::tokens iss);
	void					flatbutton(int x, int y, res::tokens icn, int index, int command, int count);
};
namespace game
{
	namespace random
	{
		int					artifact(int level);
		int					castle(int type, int player, int index, bool has_castle);
		int					hero(int type);
		void				initialize();
		int					monster(int level);
	}
	bool					additem(int rec, int type);
	void					addresources(void* result, const void* v1, const void* v2, bool negative = false);
	bool					addunit(int rec, int type, int count);
	void					build(int rec, int id);
	bool					canupgrade(int monster, int side);
	void					cleararmy(int rec);
	int						divresource(const void* source_void, const void* divider_void);
	int						get(int rec, int id);
	int						getartifact(int rec, int id);
	int						getbesthero(int rec);
	int						getbestmonster(int rec);
	char*					getbuilding(char* result, tokens race, int building, int level);
	const void*				getbuildingcost(int race, int building, int level);
	int						getbuildingmaxlevel(int race, int building);
	const char*				getbuildingname(int race, int building, int level);
	const char*				getbuildinginfo(int race, int building, int level);
	int						getbuildings(int player, int building);
	int						getcastle(int index);
	int						getcastles(int player, int level);
	char*					getcosttext(char* result, const void* cost);
	int						getday();
	int						getdowngrade(int rec);
	int						getdwelving(int rec);
	int						getframe(int rec);
	int						getgrowth(int rec, int dwelling);
	const int*				gethirecost(int rec);
	int						getincome(int rec);
	int						getmonth();
	int						getmorale(int value);
	int						getmoralechance(int value);
	int						getspeed(int value);
	int						getstrenght(int rec);
	int						getsummary(int rec, int id, int side);
	int						gettarget(int rec);
	int						gettrade(tokens resf, tokens rest, int markets = 0);
	int						getunit(int race, int building, int level);
	int						getupgrade(int rec);
	int						getweek();
	bool					hasspellbook(int rec);
	bool					hire(int hero, int player, int index);
	void					initialize();
	void					interact(int index, int object, int hero, int player);
	bool					isarcher(int rec);
	bool					isboosted(int rec);
	bool					iscombat(int rec);
	bool					isfly(int rec);
	bool					isfriendly(int rec);
	bool					ishostile(int rec);
	bool					ismatch(const void* c1, const void* c2);
	bool					ispenalized(int rec);
	bool					isrequipment(int race, int building, int level, int req, int req_level);
	bool					isstealth(int rec);
	bool					istwiceattack(int rec);
	bool					iswide(int rec);
	bool					ismeleearcher(int rec);
	void					moveto(int hero, int player);
	void					mulresource(int* result, const void* source, int value);
	bool					passrequipment(int rec, int building, int level);
	int						play(gamefile& game);
	void					prepare();
	bool					trade(int player, tokens from, tokens to, int count, int discount = 0);
	int						turn();
	bool					upgrade(int side, int index, bool interactive);
}