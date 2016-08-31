#include "main.h"

const int FBuildThisTurn = 0x00000001;
const int CastleIncome = 1000;
const int CastleIncomeDungeon = 500;
const int CastleIncomeStatue = 250;
const int CastleIncomeWell = 2;
const int CastleIncomeWell2 = 10;
static struct castle_t
{
    tokens			player;
    tokens			type;
    int				index;
    char			name[14];
    unsigned		flags;
    unsigned		building;
    unsigned char	mageguild;
    short int		dwellings[6];
	short unsigned	army[LastTroopsIndex - FirstTroopsIndex + 1];
} objects[LastCastle-FirstCastle+1];
static const char*	str_dwelling[][2] =
{
    {"Hut", "Хижина"}, {"Thatched Hut", "Соломенная Хижина"}, {"Excavation", "Захоронения"}, {"Treehouse", "Дома на деревьях"}, {"Cave", "Пещера"}, {"Habitat", "Норы"},
    {"Stick Hut", "Хижина"}, {"Archery Range", "Полигон"}, {"Graveyard", "Кладбище"}, {"Cottage", "Коттедж"}, {"Crypt", "Крипта"}, {"Pen", "Загон"},
    {"Den", "Логово"}, {"Blacksmith", "Кузница"}, {"Pyramid", "Пирамида"}, {"Archery Range", "Полигон"}, {"Nest", "Гнездо"}, {"Foundry", "Фабрика"},
    {"Adobe", "Логово"}, {"Armory", "Оружейная"}, {"Mansion", "Особняк"}, {"Stonehenge", "Камни"}, {"Maze", "Лабиринт"}, {"Cliff Nest", "Гнездо на утесе"},
    {"Bridge", "Мост"}, {"Jousting Arena", "Турнирная Арена"}, {"Mausoleum", "Мавзолей"}, {"Fenced Meadow", "Загон"}, {"Swamp", "Болото"}, {"Ivory Tower", "Башня магов"},
    {"Pyramid", "Пирамида"}, {"Cathedral", "Собор"}, {"Laboratory", "Лаборатория"}, {"Red Tower", "Красная Башня"}, {"Green Tower", "Зеленая Башня"}, {"Cloud Castle", "Небесный замок"}
};

void castle::clear()
{
    memset(objects, 0, sizeof(objects));
}

int castle::growth(int rec, int dwelling, bool apply)
{
    static int increment[] = {8, 4, 3, 2, 1, 0};
    bool well = bsget(rec, Well)!=0;
	bool well2 = bsget(rec, Well2)!=0;
    int result = increment[dwelling-Dwelving1];
    if(well)
        result += CastleIncomeWell;
    if(dwelling==Dwelving1 && well2)
        result += CastleIncomeWell2;
    // RULE: barbarian growth fast
	if(bsget(rec, Type)==Barbarian)
        result += CastleIncomeWell;
    if(apply)
        objects[rec-FirstCastle].dwellings[dwelling-Dwelving1] += result;
    return result;
}

static int object_get(int rec, int id)
{
    int m, p;
    castle_t& e = objects[rec-FirstCastle];
    switch(id)
    {
    case Valid:
        return e.index ? 1 : 0;
    case BuildThisTurn:
        return (e.flags&FBuildThisTurn)!=0 ? 1 : 0;
    case Tent:
        return bsget(rec, CastleInTown) ? 0 : 1;
    case MageGuild:
        return e.mageguild;
    case Income:
		p = bsget(rec, Type);
		m = bsget(rec, CastleInTown) ? CastleIncome : CastleIncome/2;
		if(bsget(rec, Statue))
            m += CastleIncomeStatue;
		if(p==Warlock && bsget(rec, SpecialBuilding))
            m += CastleIncomeDungeon;
        return m;
    default:
        if(id>=(int)FirstBuilding && id<=(int)LastBuilding)
            return (e.building&(1<<(id-FirstBuilding))) ? 1 : 0;
        else if(id>=(int)FirstRecruit && id<=(int)LastRecruit)
            return e.dwellings[id-FirstRecruit];
        else if(id>=(int)CanBuild && id<=(int)(CanBuild+(LastBuilding-FirstBuilding+2)))
        {
            switch(id-CanBuild+CastleInTown)
            {
            case CastleInTown:
                break;
            }
        }
        return 0;
    }
}

static const char* object_gets(int rec, int id)
{
    switch(id)
    {
    case Name:
        return objects[rec-FirstCastle].name;
    case CastleInTown:
        return "Castle";
    case Tavern:
        return "Tavern";
    case ThievesGuild:
        return "Thieves Guild";
    case Shipyard:
        return "Shipyard";
    case Well:
		return "Well";
    case Statue:
        return "Statue";
    case MarketPlace:
        return "Marketplace";
    case LeftTurret:
        return "Left Turret";
    case RightTurret:
        return "Right Turret";
    case Moat:
        return "Moat";
    case Tent:
        return "Tent";
    case Well2:
        switch(objects[rec-FirstCastle].type)
        {
        case Knight: return "Farm";
        case Barbarian: return "Garbage Heap";
        case Sorcerer: return "Crystal Garden";
        case Warlock: return "Waterfall";
        case Wizard: return "Orchard";
        default: return "Skull Pile";
        };
    case SpecialBuilding:
        switch(objects[rec-FirstCastle].type)
        {
        case Knight: return "Fortification";
        case Barbarian: return "Collesium";
        case Sorcerer: return "Rainbow";
        case Warlock: return "Dungeon";
        case Wizard: return "Library";
        default: return "Storm";
        };
    case MageGuild:
        switch(objects[rec-FirstCastle].mageguild)
        {
        case 1: return "Mage Guild 1";
        case 2: return "Mage Guild 2";
        case 3: return "Mage Guild 3";
        case 4: return "Mage Guild 4";
        case 5: return "Mage Guild 5";
        default: return "Mage Guild";
        };
    case Dwelving1:
    case Dwelving2:
    case Dwelving3:
    case Dwelving4:
    case Dwelving5:
    case Dwelving6:
        return str_dwelling[(id-Dwelving1)*6 + (objects[rec-FirstCastle].type -Barbarian)][0];
    default:
        if(id>=(int)Information)
        {
            static char temp[256];
            switch(id-Information+CastleInTown)
            {
            case CastleInTown:
                return szprint(temp, szt("The Castle improves town defense and increases income to %1i gold per day.",""), 1000);
            case Statue:
                return szprint(temp, szt("The Statue increases your town's income by %1i per day.",""), CastleIncomeStatue);
            case MageGuild:
                return szt("The Mage Guild allows heroes to learn spells and replenish their spell points.","");
            case ThievesGuild:
                return szt("The Thieves' Guild provides information on enemy players. Thieves' Guilds can also provide scouting information on enemy towns.","");
            case Tavern:
                return szt("The Tavern increases morale for troops defending the castle.","");
            case Shipyard:
                return szt("The Shipyard allows ships to be built.","");
            case Well:
                return szprint(temp, szt("The Well increases the growth rate of all dwellings by %1i creatures per week.",""), CastleIncomeWell);
            case LeftTurret:
                return szt("The Left Turret provides extra firepower during castle combat.","");
            case RightTurret:
                return szt("The Right Turret provides extra firepower during castle combat.","");
            case MarketPlace:
                return szt("The Marketplace can be used to convert one type of resource into another. The more marketplaces you control, the better the exchange rate.","");
            case Moat:
                return szt("The Moat slows attacking units. Any unit entering the moat must end its turn there and becomes more vulnerable to attack.","");
            case Tent:
                return szt("The Tent provides workers to build a castle, provided the materials and the gold are available.","");
            case Captain:
                return szt("The Captain's Quarters provides a captain to assist in the castle's defense when no hero is present.","");
            case Well2:
                switch(objects[rec-FirstCastle].type)
                {
                case Knight:
                    return szprint(temp, szt("The Farm increases production of Peasants by %1i per week.",""), CastleIncomeWell2);
                case Barbarian:
                    return szprint(temp, szt("The Garbage Heap increases production of Goblins by %1i per week.", ""), CastleIncomeWell2);
                case Sorcerer:
                    return szprint(temp, szt("The Crystal Garden increases production of Sprites by %1i per week.", ""), CastleIncomeWell2);
                case Warlock:
                    return szprint(temp, szt("The Waterfall increases production of Centaurs by %1i per week.", ""), CastleIncomeWell2);
                case Wizard:
                    return szprint(temp, szt("The Orchard increases production of Halflings by %1i per week.", ""), CastleIncomeWell2);
                default:
                    return szprint(temp, szt("The Skull Pile increases production of Skeletons by %1i per week.", ""), CastleIncomeWell2);
                };
            case SpecialBuilding:
                switch(objects[rec-FirstCastle].type)
                {
                case Knight:
                case Barbarian:
                case Sorcerer:
                case Warlock:
                    break;
                case Wizard:
                    return szt("Open one additional spell's scroll for each level of magic guild.", "");
                default:
                    return szt("Increase Spell power by 2 when hero or captain defend this castle.", "");
                }
            case Dwelving1:
            case Dwelving2:
            case Dwelving3:
            case Dwelving4:
            case Dwelving5:
            case Dwelving6:
                return szprint(temp, szt("%1 allow recruit %2.", "%1 позволяет нанимать %2."),
                               bsgets(rec, id-Information+CastleInTown),
                               bsgets(buildings::unit(objects[rec-FirstCastle].type, id-Information+CastleInTown), NameMulti));
            }
        }
        return "";
    }
}

static void object_sets(int rec, int id, const char* value)
{
	switch(id)
	{
	case Name:
		zcpy(objects[rec].name, value, sizeof(objects[rec].name)-1);
		break;
	}
}

static const rect& rectangle(int race, int building)
{
    // Barbarian, Knight, Necromancer, Sorcerer, Warlock, Wizard,
    static rect data[Tent-CastleInTown][6] =
    {
        {
            // Castle
            {0, 88, 0 + 202, 88 + 62},
            {0, 55, 0 + 290, 55 + 85},
            {322, 63, 322 + 73, 63 + 97},
            {0, 67, 0 + 198, 67 + 100},
            {268, 35, 268 + 131, 35 + 129},
            {0, 48, 0 + 187, 48 + 39},
        },
        {
            // ThievesGuild
            {478, 100, 478 + 76, 100 + 42},
            {0, 130, 0 + 50, 130 + 60},
            {291, 134, 291 + 43, 134 + 59},
            {423, 165, 423 + 65, 165 + 49},
            {525, 109, 525 + 60, 109 + 48},
            {507, 55, 507 + 47, 55 + 42},
        },
        {
            // Tavern
            {0, 205, 0 + 125, 205 + 60},
            {350, 110, 350 + 46, 110 + 56},
            {0, 0, 0 + 0, 0 + 0},
            {494, 140, 494 + 131, 140 + 87},
            {479, 100, 479 + 39, 100 + 52},
            {0, 160, 0 + 118, 160 + 50},
        },
        {
            // Shipyard
            {535, 210, 535 + 105, 210 + 45},
            {537, 221, 537 + 103, 221 + 33},
            {516, 221, 516 + 124, 221 + 28},
            {0, 220, 0 + 134, 220 + 35},
            {520, 206, 520 + 120, 206 + 47},
            {0, 218, 0 + 185, 218 + 35},
        },
        {
            // Well
            {272, 215, 272 + 44, 215 + 32},
            {194, 225, 194 + 29, 225 + 27},
            {217, 225, 217 + 23, 225 + 26},
            {346, 209, 346 + 43, 209 + 25},
            {348, 221, 348 + 63, 221 + 30},
            {254, 143, 254 + 19, 143 + 28},
        },
        {
            // Statue
            {470, 180, 470 + 30, 180 + 58},
            {480, 205, 480 + 45, 205 + 50},
            {374, 174, 374 + 26, 174 + 70},
            {158, 173, 158 + 17, 173 + 58},
            {473, 172, 473 + 45, 172 + 51},
            {464, 58, 464 + 26, 58 + 62},
        },
        {
            // Left turret
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
        },
        {
            // Right turret
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
        },
        {
            // Moat
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
        },
        {
            // Marketplace
            {224, 168, 224 + 52, 168 + 36},
            {220, 144, 220 + 115, 144 + 20},
            {415, 216, 415 + 85, 216 + 35},
            {412, 122, 412 + 56, 122 + 40},
            {391, 185, 391 + 70, 185 + 26},
            {254, 176, 254 + 105, 176 + 39},
        },
        {
            // Captain
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
        },
        {
            // Well2
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
        },
        {
            // Specail Building
            {223, 79, 223 + 124, 79 + 27},
            {0, 80, 0 + 250, 80 + 20},
            {0, 0, 0 + 640, 0 + 54},
            {147, 0, 147 + 252, 147 + 30},
            {0, 162, 0 + 70, 162 + 77},
            {304, 111, 304 + 95, 111 + 50},
        },
        {
            // Dwelve 1
            {258, 142, 258 + 71, 142 + 41},
            {195, 175, 195 + 50, 175 + 40},
            {404, 181, 404 + 56, 181 + 25},
            {478, 70, 478 + 92, 70 + 62},
            {0, 63, 0 + 68, 63 + 53},
            {459, 181, 459 + 45, 181 + 32},
        },
        {
            // Dwelve 2
            {152, 190, 152 + 68, 190 + 50},
            {145, 155, 145 + 58, 155 + 20},
            {147, 184, 147 + 92, 184 + 32},
            {345, 149, 345 + 70, 149 + 56},
            {248, 192, 248 + 60, 192 + 55},
            {253, 69, 253 + 90, 69 + 29},
        },
        {
            // Dwelve 3
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
        },
        {
            // Dwelve 4
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
        },
        {
            // Dwelve 5
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
        },
        {
            // Dwelve 6
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
            {0, 0, 0 + 0, 0 + 0},
        },
    };
    return data[building-CastleInTown][race-Barbarian];
}

static res::tokens getrs(int race, int building)
{
    if(race==Barbarian)
    {
        switch(building)
        {
        case CastleInTown:
            return res::TWNBCSTL;
        case Tent:
            return res::TWNBTENT;
        case SpecialBuilding:
            return res::TWNBSPEC;
        case Captain:
            return res::TWNBCAPT;
        case Well2:
            return res::TWNBWEL2;
        case LeftTurret:
            return res::TWNBLTUR;
        case RightTurret:
            return res::TWNBRTUR;
        case Moat:
            return res::TWNBMOAT;
        case MarketPlace:
            return res::TWNBMARK;
        case ThievesGuild:
            return res::TWNBTHIE;
        case Tavern:
            return res::TWNBTVRN;
        case Well:
            return res::TWNBWELL;
        case Statue:
            return res::TWNBSTAT;
        case Shipyard:
            return res::TWNBDOCK;
        case MageGuild:
            return res::TWNBMAGE;
        case Dwelving1:
            return res::TWNBDW_0;
        case Dwelving2:
            return res::TWNBDW_1;
        case Upgrade2:
            return res::TWNBUP_1;
        case Dwelving3:
            return res::TWNBDW_2;
        case Dwelving4:
            return res::TWNBDW_3;
        case Upgrade4:
            return res::TWNBUP_3;
        case Dwelving5:
            return res::TWNBDW_4;
        case Upgrade5:
            return res::TWNBUP_4;
        case Dwelving6:
            return res::TWNBDW_5;
        default:
            break;
        }
    }
    else if(race == Knight)
    {
        switch(building)
        {
        case CastleInTown:
            return res::TWNKCSTL;
        case Tent:
            return res::TWNKTENT;
        case SpecialBuilding:
            return res::TWNKSPEC;
        case Captain:
            return res::TWNKCAPT;
        case Well2:
            return res::TWNKWEL2;
        case LeftTurret:
            return res::TWNKLTUR;
        case RightTurret:
            return res::TWNKRTUR;
        case Moat:
            return res::TWNKMOAT;
        case MarketPlace:
            return res::TWNKMARK;
        case ThievesGuild:
            return res::TWNKTHIE;
        case Tavern:
            return res::TWNKTVRN;
        case Well:
            return res::TWNKWELL;
        case Statue:
            return res::TWNKSTAT;
        case Shipyard:
            return res::TWNKDOCK;
        case MageGuild:
            return res::TWNKMAGE;
        case Dwelving1:
            return res::TWNKDW_0;
        case Dwelving2:
            return res::TWNKDW_1;
        case Upgrade2:
            return res::TWNKUP_1;
        case Dwelving3:
            return res::TWNKDW_2;
        case Upgrade3:
            return res::TWNKUP_2;
        case Dwelving4:
            return res::TWNKDW_3;
        case Upgrade4:
            return res::TWNKUP_3;
        case Dwelving5:
            return res::TWNKDW_4;
        case Upgrade5:
            return res::TWNKUP_4;
        case Dwelving6:
            return res::TWNKDW_5;
        case Upgrade6:
            return res::TWNKUP_5;
        default:
            break;
        }
    }
    else if(race==Necromancer)
    {
        switch(building)
        {
        case CastleInTown:
            return res::TWNNCSTL;
        case Tent:
            return res::TWNNTENT;
        case SpecialBuilding:
            return res::TWNNSPEC;
        case Captain:
            return res::TWNNCAPT;
        case Well2:
            return res::TWNNWEL2;
        case LeftTurret:
            return res::TWNNLTUR;
        case RightTurret:
            return res::TWNNRTUR;
        case Moat:
            return res::TWNNMOAT;
        case MarketPlace:
            return res::TWNNMARK;
        case ThievesGuild:
            return res::TWNNTHIE;
        //case Tavern:	return res::TWNNTVRN;
        case Well:
            return res::TWNNWELL;
        case Statue:
            return res::TWNNSTAT;
        case Shipyard:
            return res::TWNNDOCK;
        case MageGuild:
            return res::TWNNMAGE;
        case Dwelving1:
            return res::TWNNDW_0;
        case Dwelving2:
            return res::TWNNDW_1;
        case Upgrade2:
            return res::TWNNUP_1;
        case Dwelving3:
            return res::TWNNDW_2;
        case Upgrade3:
            return res::TWNNUP_2;
        case Dwelving4:
            return res::TWNNDW_3;
        case Upgrade4:
            return res::TWNNUP_3;
        case Dwelving5:
            return res::TWNNDW_4;
        case Upgrade5:
            return res::TWNNUP_4;
        case Dwelving6:
            return res::TWNNDW_5;
        default:
            break;
        }
    }
    else if(race==Sorcerer)
    {
        switch(building)
        {
        case CastleInTown:
            return res::TWNSCSTL;
        case Tent:
            return res::TWNSTENT;
        case SpecialBuilding:
            return res::TWNSSPEC;
        case Captain:
            return res::TWNSCAPT;
        case Well2:
            return res::TWNSWEL2;
        case LeftTurret:
            return res::TWNSLTUR;
        case RightTurret:
            return res::TWNSRTUR;
        case Moat:
            return res::TWNSMOAT;
        case MarketPlace:
            return res::TWNSMARK;
        case ThievesGuild:
            return res::TWNSTHIE;
        case Tavern:
            return res::TWNSTVRN;
        case Well:
            return res::TWNSWELL;
        case Statue:
            return res::TWNSSTAT;
        case Shipyard:
            return res::TWNSDOCK;
        case MageGuild:
            return res::TWNSMAGE;
        case Dwelving1:
            return res::TWNSDW_0;
        case Dwelving2:
            return res::TWNSDW_1;
        case Upgrade2:
            return res::TWNSUP_1;
        case Dwelving3:
            return res::TWNSDW_2;
        case Upgrade3:
            return res::TWNSUP_2;
        case Dwelving4:
            return res::TWNSDW_3;
        case Upgrade4:
            return res::TWNSUP_3;
        case Dwelving5:
            return res::TWNSDW_4;
        case Dwelving6:
            return res::TWNSDW_5;
        default:
            break;
        }
    }
    else if(race==Warlock)
    {
        switch(building)
        {
        case CastleInTown:
            return res::TWNWCSTL;
        case Tent:
            return res::TWNWTENT;
        case SpecialBuilding:
            return res::TWNWSPEC;
        case Captain:
            return res::TWNWCAPT;
        case Well2:
            return res::TWNWWEL2;
        case LeftTurret:
            return res::TWNWLTUR;
        case RightTurret:
            return res::TWNWRTUR;
        case Moat:
            return res::TWNWMOAT;
        case MarketPlace:
            return res::TWNWMARK;
        case ThievesGuild:
            return res::TWNWTHIE;
        case Tavern:
            return res::TWNWTVRN;
        case Well:
            return res::TWNWWELL;
        case Statue:
            return res::TWNWSTAT;
        case Shipyard:
            return res::TWNWDOCK;
        case MageGuild:
            return res::TWNWMAGE;
        case Dwelving1:
            return res::TWNWDW_0;
        case Dwelving2:
            return res::TWNWDW_1;
        case Dwelving3:
            return res::TWNWDW_2;
        case Dwelving4:
            return res::TWNWDW_3;
        case Upgrade4:
            return res::TWNWUP_3;
        case Dwelving5:
            return res::TWNWDW_4;
        case Dwelving6:
            return res::TWNWDW_5;
        case Upgrade6:
            return res::TWNWUP_5;
        case Upgrade62:
            return res::TWNWUP5B;
        default:
            break;
        }
    }
    else if(race==Wizard)
    {
        switch(building)
        {
        case CastleInTown:
            return res::TWNZCSTL;
        case Tent:
            return res::TWNZTENT;
        case SpecialBuilding:
            return res::TWNZSPEC;
        case Captain:
            return res::TWNZCAPT;
        case Well2:
            return res::TWNZWEL2;
        case LeftTurret:
            return res::TWNZLTUR;
        case RightTurret:
            return res::TWNZRTUR;
        case Moat:
            return res::TWNZMOAT;
        case MarketPlace:
            return res::TWNZMARK;
        case ThievesGuild:
            return res::TWNZTHIE;
        case Tavern:
            return res::TWNZTVRN;
        case Well:
            return res::TWNZWELL;
        case Statue:
            return res::TWNZSTAT;
        case Shipyard:
            return res::TWNZDOCK;
        case MageGuild:
            return res::TWNZMAGE;
        case Dwelving1:
            return res::TWNZDW_0;
        case Dwelving2:
            return res::TWNZDW_1;
        case Dwelving3:
            return res::TWNZDW_2;
        case Upgrade3:
            return res::TWNZUP_2;
        case Dwelving4:
            return res::TWNZDW_3;
        case Dwelving5:
            return res::TWNZDW_4;
        case Upgrade5:
            return res::TWNZUP_4;
        case Dwelving6:
            return res::TWNZDW_5;
        case Upgrade6:
            return res::TWNZUP_5;
        default:
            break;
        }
    }
    return res::Empthy;
}

static int* priority(int race)
{
    static int data[6][32] =
    {
        {
            // Barbarian
            SpecialBuilding, Well2, Dwelving6, MageGuild, Captain, CastleInTown, LeftTurret, RightTurret, Moat,
            Dwelving3, ThievesGuild, Tavern, Dwelving1, MarketPlace,
            Dwelving2, Upgrade2, Dwelving4, Upgrade4,
            Dwelving5, Upgrade5,
            Well, Statue, Shipyard, 0
        },
        {
            // Knight
            CastleInTown, SpecialBuilding, Well2, Captain, LeftTurret, RightTurret, Moat,
            MarketPlace, Dwelving2, Upgrade2, ThievesGuild, Tavern, MageGuild,
            Dwelving5, Upgrade5, Dwelving6, Upgrade6,
            Dwelving1, Dwelving3, Upgrade3, Dwelving4, Upgrade4,
            Well, Statue, Shipyard, 0
        },
        {
            // Necromancer
            SpecialBuilding, Tavern, CastleInTown, Captain, LeftTurret, RightTurret, Dwelving6, Moat,
            Dwelving1, ThievesGuild, Dwelving3, Upgrade3, Dwelving5, Upgrade5, Dwelving2, Upgrade2,
            Dwelving4, Upgrade4, MageGuild, Shipyard, Well2, MarketPlace,
            Statue, Well, 0
        },
        {
            // Sorcerer
            SpecialBuilding, Dwelving6, MageGuild, Captain, CastleInTown, LeftTurret, RightTurret, Moat,
            Dwelving3, Upgrade3, Shipyard, MarketPlace, Dwelving2, Upgrade2,
            ThievesGuild, Dwelving1, Tavern, Statue, Well2, Upgrade4, Dwelving4, Well, Dwelving5, 0
        },
        {
            // Warlock
            Dwelving5, Upgrade5, Dwelving3, CastleInTown, LeftTurret, RightTurret, Captain, Moat,
            Shipyard, MageGuild, Tavern, ThievesGuild, MarketPlace, Statue,
            Dwelving1, Well2, SpecialBuilding,
            Dwelving4, Upgrade4, Dwelving2, Dwelving6, Upgrade6, Upgrade62, Well, 0
        },
        {
            // Wizard
            Dwelving6, Upgrade6, Tent, CastleInTown, LeftTurret, RightTurret, Moat, Captain,
            Dwelving2, ThievesGuild, Tavern, Shipyard, Well, SpecialBuilding,
            Dwelving3, Upgrade3, Dwelving5, Upgrade5, MageGuild, Statue,
            Dwelving1, Dwelving4, MarketPlace, Well2, 0
        },
    };
    return data[race-Barbarian];
}

static void panorama(int x, int y, int mid)
{
    int race = bsget(mid, Type);
    bool coastal = false;
    switch(race)
    {
    case Knight:
        draw::image(x, y, res::TOWNBKG0, 0, 0);
        break;
    case Barbarian:
        draw::image(x, y, res::TOWNBKG1, 0, 0);
        draw::image(x, y, res::TWNBEXT1, 1 + (draw::frame%5), 0);
        break;
    case Sorcerer:
        draw::image(x, y, res::TOWNBKG2, 0, 0);
        break;
    case Warlock:
        draw::image(x, y, res::TOWNBKG3, 0, 0);
        break;
    case Necromancer:
        draw::image(x, y, res::TOWNBKG5, 0, 0);
        break;
    default:
        draw::image(x, y, res::TOWNBKG4, 0, 0);
        break;
    }
    if(coastal)
    {
        switch(race)
        {
        case Knight:
            draw::image(x, y, res::TWNKEXT0, 0, 0);
            draw::image(x, y, res::TWNKEXT0, 0, 1 + draw::frame%5);
            break;
        case Barbarian:
            draw::image(x, y, res::TWNBEXT0, 0, 0);
            draw::image(x, y, res::TWNBEXT0, 0, 1 + draw::frame%5);
            break;
        case Sorcerer:
            draw::image(x, y, res::TWNSEXT0, 0, 0);
            draw::image(x, y, res::TWNSEXT0, 0, 1 + draw::frame%5);
            break;
        case Necromancer:
            draw::image(x, y, res::TWNNEXT0, 0, 0);
            draw::image(x, y, res::TWNNEXT0, 0, 1 + draw::frame%5);
            break;
        case Warlock:
            draw::image(x, y, res::TWNWEXT0, 0, 0);
            draw::image(x, y, res::TWNWEXT0, 0, 1 + draw::frame%5);
            break;
        default:
            draw::image(x, y, res::TWNZEXT0, 0, 0);
            draw::image(x, y, res::TWNZEXT0, 0, 1 + draw::frame%5);
            break;
        }
    }
    for(int* pi = priority(race); *pi; pi++)
    {
        int building = *pi;
        if(!building)
            break;
        if(!bsget(mid, building))
            continue;
        res::tokens icn = getrs(race, building);
        if(icn==res::Empthy)
            continue;
        int index = 0;
        if(building==MageGuild)
        {
            int level = bsget(mid, MageGuild);
            switch(level)
            {
            case 2:
                index = (race == Necromancer) ? 6 : 1;
                break;
            case 3:
                index = (race == Necromancer) ? 12 : 2;
                break;
            case 4:
                index = (race == Necromancer) ? 18 : 3;
                break;
            case 5:
                index = (race == Necromancer) ? 24 : 4;
                break;
            }
        }
        draw::image(x, y, icn, index, 0);
        if(hot::mouse.in(rectangle(race, (building==Tent) ? CastleInTown : building)))
        {
            if(building>=(int)Dwelving1 && building<=(int)Upgrade62)
                draw::status(szt("Recruit %1","Нанять %1"),
                             (int)bsgets(buildings::unit(race, building), NameMulti));
            else
                draw::status(bsgets(mid, building));
            if(hot::key==MouseLeft && hot::pressed)
                draw::execute(building);
            else if(hot::key==MouseRight && hot::pressed)
                draw::execute(building+Information);
        }
        index = indexes::animate(icn, index, draw::frame, false);
        if(index)
            draw::image(x, y, icn, index);
    }
}

static void paint_panel(int x, int y, int mid, int hid)
{
    int hero2 = hid;
    draw::image(x, y, res::STRIP, 0);
    draw::image(x+5, y+6, res::CREST, 3);
    if(hero2)
        draw::clipart(x+5, y+105, bsget(hero2, Portrait), LargeSize);
    else
    {
        if(bsget(mid, Captain))
            draw::image(x+5, y+105, (res::tokens)(res::PORT0090+bsget(mid, Type)-Knight), 0);
        else
            draw::image(x+5, y+105, res::STRIP, 3);
        draw::image(x+112, y+105, res::STRIP, 11);
    }
}

static void name()
{
    draw::fontsm push;
    draw::image(0, 0, res::TOWNNAME, 0);
    draw::text(231+179/2, 248+2, "Вертиго", 0);
}

void show::castle(int rec)
{
    int count;
    int army_index = -1;
    int player = bsget(rec, Player);
    while(true)
    {
        draw::status(21, draw::height-16, 21+res::width(res::SMALLBAR, 0), draw::height-1);
        draw::button(0, draw::height-19, res::SMALLBAR, KeyLeft, 1, 1, 2, 0, 0, szt("Previous town",""));
        draw::image(21, draw::height-19, res::SMALLBAR, 0);
        draw::button(draw::width-21, draw::height-19, res::SMALLBAR, KeyRight, 3, 3, 4, 0, 0, szt("Next town", ""));
        panorama(0, 0, rec);
        draw::troops(112, 262, rec, army_index);
        paint_panel(0, 256, rec, 0);
        draw::resource(552, 262, player::gcost(player));
        draw::button(553, 428, res::SWAPBTN, Cancel, 0, 0, 1, KeyEscape, 0, szt("Leave town", ""));
        name();
        draw::cursor(res::ADVMCO, 0);
        int id = draw::input();
        switch(id)
        {
        case Cancel:
        case 0:
            return;
        case Dwelving1:
        case Dwelving2:
        case Dwelving3:
        case Dwelving4:
        case Dwelving5:
            id = buildings::unit(bsget(rec, Type), id);
            count = 0;
            if(!id)
                break;
            if(show::recruit(id, count, 10))
            {
            }
            break;
        case CastleInTown:
            show::build(rec);
            break;
        }
    }
}
