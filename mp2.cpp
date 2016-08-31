#include "main.h"

#pragma pack(push)
#pragma pack(1)

#define mp2obj(i) (i-FirstObject+0x80)

namespace mp2
{
struct header
{
    unsigned		id;				// file identification
    short int		level;			// difficult level
    unsigned char	width;
    unsigned char	height;
    unsigned char	player[6];
    unsigned char	humans[6];
    unsigned char	computers[6];
    unsigned char	unknown1[3];
    unsigned char	conditions_wins;
    unsigned char	comp_also_wins;
    unsigned char	allow_normal_victory;
    unsigned short	wins;
    unsigned char	conditions_loss;
    unsigned short	loss;
    unsigned char   hero;
    unsigned char   races[6];
    unsigned short	wins_data;
    unsigned short	loss_data;
    unsigned char   unknown2[10];
    char			name[16];
    unsigned char	unknown4[44];
    char			text[143];
};
struct addon
{
    unsigned short	indexAddon;		// zero or next addons_t
    unsigned char	objectNameN1;	// level 1.N
    unsigned char	indexNameN1;	// level 1.N or 0xFF
    unsigned char	quantityN;
    unsigned char	objectNameN2;	// id of object (use map::object)
    unsigned char	indexNameN2;	// index if file from 'Name'
    unsigned		uniqNumberN1;	// level 1.N
    unsigned		uniqNumberN2;	// level 2.N
};
struct tile
{
    unsigned short	tileIndex;		// tile (ocean, grass, snow, swamp, lava, desert, dirt, wasteland, beach)
    unsigned char	objectName1;	// level 1.0 (resource file name id)
    unsigned char	indexName1;		// index level 1.0 or 0xFF (index in resource file)
    unsigned char	quantity1;		// count
    unsigned char	quantity2;		// count
    unsigned char	objectName2;	// level 2.0
    unsigned char	indexName2;		// index level 2.0 or 0xFF
    unsigned char	shape;			// shape reflect % 4, 0 none, 1 vertical, 2 horizontal, 3 any
    unsigned char	generalObject;	// zero or object from mp2obj(generalObject)
    unsigned short	indexAddon;		// zero or index mp2addon
    unsigned		uniqNumber1;	// level 1.0
    unsigned		uniqNumber2;	// level 2.0
};
struct army
{
    unsigned char	monster[5];
    unsigned short	count[5];
};
struct castle
{
    unsigned char	player; 			// 00 blue, 01 green, 02 red, 03 yellow, 04 orange, 05 purpl, ff unknown
    unsigned char	has_buildings;
    unsigned short	buildings;
    unsigned short	dwellings;
    unsigned char	magic_tower;
    unsigned char	has_army;
    struct army		army;
    unsigned char	has_captain;
    unsigned char	has_name;
    char			name[13];		// name + '\0'
    unsigned char	race;			// 00 knight, 01 barb, 02 sorc, 03 warl, 04 wiz, 05 necr, 06 rnd
    unsigned char	castle_in_town;
    unsigned char	disable_castle_upgrade;	// 00 TRUE, 01 FALSE
    unsigned char	unknown[29];
};
struct hero // 0x004c - size
{
    unsigned char	unknown1;
    unsigned char	has_army;
    struct army		army;
    unsigned char	has_type;
    unsigned char	type;
    unsigned char	artifact[3]; // 0xff none
    unsigned char	unknown2;	// 0
    unsigned		exerience;
    unsigned char	has_skills;
    unsigned char	skill[8]; // 0xff none
    unsigned char	skill_level[8];
    unsigned char	unknown3;	// 0
    unsigned char	has_name;
    char			name[13];	// name + '\0'
    unsigned char	has_patrol;
    unsigned char	patrol;	// for patrol in squards
    unsigned char	unknown4[15];	// 0
};
struct info // origin mp2 sign or buttle
{
    unsigned char	id;		// 0x01
    unsigned char	zero[8];// 8 byte 0x00
    char			text;	// message  + '/0'
};
struct eventcoord // origin mp2 event for coord
{
    unsigned char	id;		// 0x01
    unsigned		wood;
    unsigned		mercury;
    unsigned		ore;
    unsigned		sulfur;
    unsigned		crystal;
    unsigned		gems;
    unsigned		golds;
    unsigned short	artifact;	// 0xffff - none
    unsigned char	computer;	// allow events for computer
    unsigned char	cancel;		// cancel event after first visit
    unsigned char	zero[10];	// 10 byte 0x00
    unsigned char	blue;
    unsigned char	green;
    unsigned char	red;
    unsigned char	yellow;
    unsigned char	orange;
    unsigned char	purple;
    char			text[1];		// message + '/0'
};
struct eventday // origin mp2 event for day
{
    unsigned char	id;		// 0x00
    unsigned		wood;
    unsigned		mercury;
    unsigned		ore;
    unsigned		sulfur;
    unsigned		crystal;
    unsigned		gems;
    unsigned		golds;
    unsigned short	artifact;	// always 0xffff - none
    unsigned short	computer;	// allow events for computer
    unsigned short	first;		// day of first occurent
    unsigned short	subsequent;	// subsequent occurrences
    unsigned char	zero[6];	// 6 byte 0x00 and end 0x01
    unsigned char	blue;
    unsigned char	green;
    unsigned char	red;
    unsigned char	yellow;
    unsigned char	orange;
    unsigned char	purple;
    char			text;		// message + '/0'
};
struct rumor // origin mp2 rumor
{
    unsigned char	id;		// 0x00
    unsigned char	zero[7];	// 7 byte 0x00
    char			text[1];   	// message + '/0'
};
struct riddle // origin mp2 riddle sphinx
{
    unsigned char	id;		// 0x00
    unsigned		wood;
    unsigned		mercury;
    unsigned		ore;
    unsigned		sulfur;
    unsigned		crystal;
    unsigned		gems;
    unsigned		golds;
    unsigned short	artifact;	// 0xffff - none
    unsigned char	count;		// count answers (1, 8)
    char			answer1[13];
    char			answer2[13];
    char			answer3[13];
    char			answer4[13];
    char			answer5[13];
    char			answer6[13];
    char			answer7[13];
    char			answer8[13];
    char			text[1];		// message + '/0'
};
}
#pragma pack(pop)

enum conditions
{
    Wins = 0x1000, CaptureTown = 0x1001, DefeatHero = 0x1002, FindArtifact = 0x1003, SideWins = 0x1004, AccumulateGold = 0x1005,
    CompAlsoWins = 0x0100, AllowNormalVictory = 0x0200,
    Loss = 0x2000, LoseTown = 0x2001, LoseHero = 0x2002, OutTime = 0x2003
};

static tokens index2race(int index)
{
    switch(index)
    {
    case 0:
        return Knight;
    case 1:
        return Barbarian;
    case 2:
        return Sorcerer;
    case 3:
        return Warlock;
    case 4:
        return Wizard;
    case 5:
        return Necromancer;
    default:
        return Random;
    }
}

static tokens index2player(int index)
{
    switch(index)
    {
    case 0:
        return PlayerBlue;
    case 1:
        return PlayerGreen;
    case 2:
        return PlayerRed;
    case 3:
        return PlayerYellow;
    case 4:
        return PlayerOrange;
    case 5:
        return PlayerPurple;
    default:
        return PlayerNeutral;
    }
}

static tokens oppose_type(tokens type)
{
    switch(type)
    {
    case Knight:
    case Barbarian:
        return tokens(xrand(Necromancer, Wizard));
    default:
        return tokens(xrand(Barbarian, Wizard));
    }
}

static int mini2player(int index)
{
    if(index < 7) return PlayerBlue;
    else if(index < 14) return PlayerGreen;
    else if(index < 21) return PlayerRed;
    else if(index < 28) return PlayerYellow;
    else if(index < 35) return PlayerOrange;
    else return PlayerPurple;
}

static tokens mini2type(int index)
{
    tokens r = tokens((index%7) + PlayerBlue);
    if(r==PlayerPurple+1)
        r = Random;
    return r;
}

unsigned short getLE16(unsigned short u)
{
    return (u<<8)|(u>>8);
}

unsigned getLE32(unsigned u)
{
    return u;
}

static void update_castle(int index, int race, mp2::tile* tiles, mp2::addon* addons)
{
    static const int points[][2] =
    {
        {0, -3}, // for castle only
        //
        {-2, -2},
        {-1, -2},
        {0, -2},
        {+1, -2},
        {+2, -2},
        // T2
        {-2, -1},
        {-1, -1},
        {0, -1},
        {+1, -1},
        {+2, -1},
        // B1
        {-2, 0},
        {-1, 0},
        {0, 0},
        {+1, 0},
        {+2, 0},
        // B2
        {-2, +1},
        {-1, +1},
        {0, +1},
        {+1, +1},
        {+2, +1},
    };
    int x = index%map::width;
    int y = index/map::width;
    int a = 0;
    switch(race)
    {
    case Knight:
        break;
    case Barbarian:
        a = 32;
        break;
    case Sorcerer:
        a = 64;
        break;
    case Warlock:
        a = 96;
        break;
    case Wizard:
        a = 128;
        break;
    case Necromancer:
        a = 160;
        break;
    default:
        break;
    }
    for(auto& p : points)
    {
        int x1 = x + p[0];
        int y1 = y + p[1];
        if(y1<0)
            continue;
        if(y1>=map::height)
            continue;
        if(x1<0)
            continue;
        if(x1>=map::width)
            continue;
        mp2::tile& tile = tiles[y1*map::width + x1];
        if(res::map(tile.objectName1)==res::OBJNTWRD && tile.indexName1<32)
        {
            tile.objectName1 -= 12;
            tile.indexName1 += a;
        }
        if(res::map(tile.objectName2)==res::OBJNTWRD && tile.indexName2<32)
        {
            tile.objectName2 -= 12;
            tile.indexName2 += a;
        }
        int ai = tile.indexAddon;
        while(ai)
        {
            mp2::addon& addon = addons[ai];
            if(res::map(addon.objectNameN1)==res::OBJNTWRD && addon.indexNameN1<32)
            {
                addon.objectNameN1 -= 12;
                addon.indexNameN1 += a;
            }
            if(res::map(addon.objectNameN2)==res::OBJNTWRD && addon.indexNameN1<32)
            {
                addon.objectNameN2 -= 12;
                addon.indexNameN2 += a;
            }
            ai = addon.indexAddon;
        }
    }
}

static tokens get_free_hero(tokens type)
{
    int count;
    int data[LastHero-FirstHero+1];
    int* p = data;
    for(int rec = FirstHero; rec<=(int)Celia; rec++)
    {
        if(tokens(bsget(rec, Type))!=type)
            continue;
        if(bsget(rec, Recruit))
            continue;
        if(bsget(rec, Position)!=-1)
            continue;
        *p++ = rec;
    }
    *p++ = 0;
    count = zlen(data);
    if(count)
        return tokens(data[rand()%count]);
    for(int rec = FirstHero; rec<=(int)Celia; rec++)
    {
        if(bsget(rec, Recruit)!=Empthy)
            continue;
        if(bsget(rec, Position)!=-1)
            continue;
        *p++ = rec;
    }
    *p++ = 0;
    count = zlen(data);
    if(count)
        return tokens(data[rand()%count]);
    return Empthy;
}

static void load_object(mp2::army* p, int rec)
{
    for(int i = 0; i<5; i++)
    {
        if(!p->count[i])
            continue;
		bsset(rec, FirstTroopsIndex + i * 2 + 0, FirstMonster + p->monster[i]);
		bsset(rec, FirstTroopsIndex + i * 2 + 1, FirstMonster + getLE16(p->count[i]));
    }
}

static void load_object(mp2::castle* p, int rec)
{
    tokens pla = index2player(p->player);
    bsset(rec, Player, pla);
    bsset(rec, Type, index2race(p->race));
    // custom building
    if(p->has_buildings)
    {
        // building
        int build = getLE16(p->buildings);
        if(0x0002 & build) bsset(rec, ThievesGuild, 1);
        if(0x0004 & build) bsset(rec, Tavern, 1);
        if(0x0008 & build) bsset(rec, Shipyard, 1);
        if(0x0010 & build) bsset(rec, Well, 1);
        if(0x0080 & build) bsset(rec, Statue, 1);
        if(0x0100 & build) bsset(rec, LeftTurret, 1);
        if(0x0200 & build) bsset(rec, RightTurret, 1);
        if(0x0400 & build) bsset(rec, MarketPlace, 1);
        if(0x1000 & build) bsset(rec, Moat, 1);
        if(0x0800 & build) bsset(rec, Well2, 1);
        if(0x2000 & build) bsset(rec, SpecialBuilding, 1);
        // dwelling
        int dwell = getLE16(p->dwellings);
        if(0x0008 & dwell) bsset(rec, Dwelving1, 1);
        if(0x0010 & dwell) bsset(rec, Dwelving2, 1);
        if(0x0020 & dwell) bsset(rec, Dwelving3, 1);
        if(0x0040 & dwell) bsset(rec, Dwelving4, 1);
        if(0x0080 & dwell) bsset(rec, Dwelving5, 1);
        if(0x0100 & dwell) bsset(rec, Dwelving6, 1);
        if(0x0200 & dwell) bsset(rec, Upgrade2, 1);
        if(0x0400 & dwell) bsset(rec, Upgrade3, 1);
        if(0x0800 & dwell) bsset(rec, Upgrade4, 1);
        if(0x1000 & dwell) bsset(rec, Upgrade5, 1);
        if(0x2000 & dwell) bsset(rec, Upgrade6, 1);
        // magic tower
        bsset(rec, MageGuild, p->magic_tower);
    }
    else
        bsset(rec, Dwelving1, 1);
    // custom troops
    if(p->has_army)
        load_object(&p->army, rec);
    // captain
    if(p->has_captain)
        bsset(rec, Captain, 1);
    // race
    if(index2race(p->race)==Random)
    {
        if(bsget(rec, Player)==PlayerNeutral)
            bsset(rec, Type, xrand(Barbarian, Wizard));
        else
            bsset(rec, Type, player::get(bsget(rec, Player), Type));
    }
    // name
    if(p->has_name)
        bsset(rec, Name, p->name);
    bsset(rec, CastleInTown, p->castle_in_town);
    // allow upgrade to castle (0 - true, 1 - false)
    bsset(rec, DisableCastleUpgrade, p->disable_castle_upgrade);
}

static void load_object(mp2::hero* p, int rec)
{
    // custom troops
    if(p->has_army)
        load_object(&p->army, rec);
    // custom portrate
    if(p->has_type)
        bsset(rec, Portrait, p->type);
    // 3 artifacts
    for(int i = 0; i<3; i++)
    {
        if(p->artifact[i]==0xFF)
            continue;
        bsset(rec, p->artifact[i] + FirstArtifact, 1);
    }
    // experience
    bsset(rec, Experience, getLE32(p->exerience));
    // custom skill
    if(p->has_skills)
    {
        for(int i = 0; i<8; i++)
        {
            if(p->skill_level[i])
                bsset(rec, FirstSkill + p->skill[i], p->skill_level[i]);
        }
    }
    // custom name
    bsset(rec, Name, p->name);
    // patrol
    if(p->has_patrol)
    {
    }
}

bool gamefile::load(const char* filename)
{
	char temp[260];
    clear();
	io::file st(szurl(temp, "maps", filename));
    if(!st)
        return false;
    mp2::header header; // (36,36), (72,72), (108,108), (144,144)
    st.read(&header, sizeof(header));
    if(header.id != 0x5C)
        return false;
    // difficult
    level = tokens(LevelEasy+header.level);
    // size
    switch(header.width)
    {
    case 32:
        size = SmallSize;
        break;
    case 72:
        size = MediumSize;
        break;
    case 108:
        size = LargeSize;
        break;
    case 144:
        size = XLargeSize;
        break;
    }
    // start player
    if(header.hero)
        start_hero = true;
    // text info
    zcpy(file, filename);
    zcpy(description, header.text, sizeof(description)-1);
    zcpy(name, header.name, sizeof(name)-1);
    for(int i=0; i<6; i++)
    {
        if(!header.player[i])
            continue;
        if(header.humans[i])
            types[i] = Human;
        else if(header.computers[i])
            types[i] = ComputerOnly;
        races[i] = index2race(header.races[i]);
        blocked[i] = races[i]!=Random;
    }
    return true;
}

int random::artifact(int level)
{
    int a[LastArtifact-FirstArtifact+1];
    int* p = a;
    for(int mid = FirstArtifact; mid<=(int)LastArtifact; mid++)
    {
        if(level && bsget(mid, Level)!=level)
            continue;
        if(bsfind(FirstMoveable, Type, mid))
            continue;
        *p++ = mid;
    }
    int count = p - a;
    if(!count)
        return 0;
    return a[rand()%count];
}

static void add_object(int pos, unsigned char object, unsigned char index, int quantity)
{
    switch(res::map(object))
    {
    case res::OBJNARTI: // turn off atrifacts
    case res::MONS32: // turn off monsters
    case res::MINIHERO: // turn off heroes
    case res::SHADOW32: // turn off all shadows
    case res::OBJNRSRC: // turn off all resources
        return;
    case res::OBJNMUL2:
        if(index==163) // event
            return;
        break;
    case res::FLAG32:
        return;
    default:
        break;
    }
    // autodetect level
    unsigned level = 0;
    while(map::show::objects[pos][level][0])
    {
        level++;
        if(level>=sizeof(map::show::objects[pos])/sizeof(map::show::objects[pos][0]))
            return;
    }
    map::show::objects[pos][level][0] = object;
    map::show::objects[pos][level][1] = index;
    map::show::objects[pos][level][2] = quantity%4;
}

static void add_moveable(int index, int id, int quality)
{
	int rec = bscreate(FirstMoveable);
	bsset(rec, Type, id);
	bsset(rec, Position, index);
	// Count depends on type
	int count = 0;
	switch(bsget(id, First))
	{
	case FirstResource:
		switch(id)
		{
		case Ore:
		case Wood:
			count = xrand(5, 10);
			break;
		case Gold:
			count = 100*xrand(5, 10);
			break;
		default:
			count = xrand(3, 6);
			break;
		}
		break;
	case FirstMonster:
		count = xrand(12, 40);
		break;
	default:
		break;
	}
	bsset(rec, Count, count);
}

void map::load(gamefile& game)
{
    char temp[260];
    io::file st(szurl(temp,"maps",game.file));
    if(!st)
        return;
    // apply players
    for(int i = 0; i<6; i++)
    {
        player::set(PlayerBlue+i, Type, game.races[i]);
        player::set(PlayerBlue+i, PlayerType, game.types[i]);
    }
    // width and heigh
    st.seek(420, SeekSet);
    st.read(&map::width, 4);
    st.read(&map::height, 4);
    // tiles loading
    int tiles_count = map::height * map::width;
    mp2::tile* tiles = new mp2::tile[tiles_count];
    st.read(tiles, tiles_count*sizeof(mp2::tile));
    // addons loading
    int addon_count = st.getLE32();
    mp2::addon* addons = new mp2::addon[addon_count];
    st.read(addons, addon_count*sizeof(mp2::addon));
    // normalize addon
    for(int i = 0; i<addon_count; i++)
        addons[i].objectNameN1 = addons[i].objectNameN1*2;
    // clear map
    memset(map::show::objects, 0, sizeof(map::show::objects));
    // load castle coordinates
    for(int ii = 0; ii < 72; ii++)
    {
        int rec;
        unsigned char cx = st.get();
        unsigned char cy = st.get();
        unsigned char id = st.get();
        int index;
        // empty block
        if(0xFF == cx && 0xFF == cy)
            continue;
        switch(id)
        {
        case 0x00: // tower: knight
        case 0x80: // castle: knight
        case 0x01: // tower: barbarian
        case 0x81: // castle: barbarian
        case 0x02: // tower: sorceress
        case 0x82: // castle: sorceress
        case 0x03: // tower: warlock
        case 0x83: // castle: warlock
        case 0x04: // tower: wizard
        case 0x84: // castle: wizard
        case 0x05: // tower: necromancer
        case 0x85: // castle: necromancer
        case 0x06: // tower: random
        case 0x86: // castle: random
			rec = bscreate(FirstCastle);
            index = map::m2i(cx, cy);
            bsset(rec, Position, index);
            break;
        default:
            break;
        }
    }
    // cood resource kingdoms
    // 144 x 3 byte (cx, cy, id)
    for(int ii = 0; ii < 144; ++ii)
    {
        unsigned char cx = st.get();
        unsigned char cy = st.get();
        st.get();
        // empty block
        if(0xFF == cx && 0xFF == cy)
            continue;
    }
    int obelisc_count = st.get();
    // count final mp2 blocks
    int countblock = 0;
    while(1)
    {
        unsigned l = st.get();
        unsigned h = st.get();
        if(0 == h && 0 == l)
            break;
        else
            countblock = 256 * h + l - 1;
    }
    for(int ii = 0; ii < countblock; ++ii)
    {
        unsigned char pblock[512];
        int findobject = -1;
        // read block
        unsigned sizeblock = st.getLE16();
        if(sizeblock>sizeof(pblock))
        {
            st.seek(sizeblock, SeekCur);
            continue;
        }
        st.read(pblock, sizeblock);
        for(int id = 0; id<tiles_count; id++)
        {
            const mp2::tile& tile = tiles[id];
            // orders(quantity2, quantity1)
            unsigned orders = (tile.quantity2 ? tile.quantity2 : 0);
            orders <<= 8;
            orders |= tile.quantity1;
            if(orders && !(orders%0x08) && (ii + 1 == orders/0x08))
            {
                findobject = id;
                break;
            }
        }
        if(findobject>0)
        {
            const mp2::tile& tile = tiles[findobject];
            //const mp2::addon* addon = 0;
            switch(tile.generalObject)
            {
            case mp2obj(CastleOnMap):
            case mp2obj(RndTown):
            case mp2obj(RndCastle):
                // add castle
                if(sizeblock==sizeof(mp2::castle))
                {
                    int rec = bsfind(FirstCastle, Position, findobject);
                    load_object((mp2::castle*)pblock, rec);
                    if(tile.generalObject==mp2obj(RndTown) || tile.generalObject==mp2obj(RndCastle))
                        update_castle(findobject, bsget(rec, Type), tiles, addons);
                }
                break;
            case mp2obj(Hero):
                // add heroes
                if(sizeblock == sizeof(mp2::hero))
                {
                    int rec = 0;
                    int pla = mini2player(tiles[findobject].indexName1);
                    tokens type = mini2type(tiles[findobject].indexName1);
                    if(type==Random)
                    	type = tokens(player::get(pla, Type));
                    if(pblock[17] && pblock[18]<=(Bax-FirstHero))
                    {
                        rec = pblock[18] + FirstHero;
                        load_object((mp2::hero*)pblock, rec);
                    }
                    if(!rec)
                        rec = get_free_hero(type);
                    bsset(rec, Player, pla);
                    bsset(rec, Position, findobject);
                    bsset(rec, Direction, map::Right);
                }
                break;
            case mp2obj(Sign):
            case mp2obj(Bottle):
                // add sign or buttle
                if(sizeblock>sizeof(mp2::info)-1 && pblock[0]==0x01)
                {
					int rec = bscreate(FirstSign);
                    bsset(rec, Position, findobject);
					bsset(rec, Name, (char*)&pblock[9]);
                }
                break;
            case mp2obj(Event):
                // add event maps
                if(sizeblock>sizeof(mp2::eventday)-1 && pblock[0]==0x01)
                {
                    mp2::eventcoord& e = (mp2::eventcoord&)pblock;
					int rec = bscreate(FirstEvent);
                    bsset(rec, Valid, 1);
                    bsset(rec, Position, findobject);
                    bsset(rec, Name, e.text);
                    bsset(rec, OneTime, e.cancel);
                    bsset(rec, Computer, e.computer);
                    bsset(rec, PlayerBlue, e.blue);
                    bsset(rec, PlayerGreen, e.green);
                    bsset(rec, PlayerRed, e.red);
                    bsset(rec, PlayerYellow, e.yellow);
                    bsset(rec, PlayerOrange, e.orange);
                    bsset(rec, PlayerPurple, e.purple);
                    bsset(rec, Gold, e.golds);
                    bsset(rec, Mercury, e.mercury);
                    bsset(rec, Sulfur, e.sulfur);
                    bsset(rec, Crystal, e.crystal);
                    bsset(rec, Ore, e.ore);
                    bsset(rec, Wood, e.wood);
                    bsset(rec, Gems, e.gems);
                    if(e.artifact!=0xFFFF)
                        bsset(rec, Artifact, e.artifact);
                }
                break;
            case mp2obj(Sphinx):
                // add riddle sphinx
                if(sizeblock>sizeof(mp2::riddle)-1 && pblock[0]==0x00)
                {
                    mp2::riddle& e = (mp2::riddle&)pblock;
					int rec = bscreate(FirstEvent);
                    bsset(rec, Valid, 1);
                    bsset(rec, Position, findobject);
                    bsset(rec, Name, e.text);
                    bsset(rec, Gold, e.golds);
                    bsset(rec, Mercury, e.mercury);
                    bsset(rec, Sulfur, e.sulfur);
                    bsset(rec, Crystal, e.crystal);
                    bsset(rec, Ore, e.ore);
                    bsset(rec, Wood, e.wood);
                    bsset(rec, Gems, e.gems);
                    if(e.artifact!=0xFFFF)
                        bsset(rec, Artifact, e.artifact);
                    //ne->answers[0] = szdup(e.answer1);
                    //ne->answers[1] = szdup(e.answer2);
                    //ne->answers[2] = szdup(e.answer3);
                    //ne->answers[3] = szdup(e.answer4);
                    //ne->answers[4] = szdup(e.answer5);
                    //ne->answers[5] = szdup(e.answer6);
                    //ne->answers[6] = szdup(e.answer7);
                    //ne->answers[7] = szdup(e.answer8);
                }
                break;
            }
        }
        else if(pblock[0]==0)
        {
            if(sizeblock>sizeof(mp2::eventday)-1 && pblock[42]==1)
            {
                // add event day
            }
            else if(sizeblock>sizeof(mp2::rumor)-1 && pblock[8])
            {
                //int mid = signs::add();
                //signs::set(mid, Position, e.id);
                //signs::set(mid, Name, e.text);
            }
        }
        else
        {
            // error
        }
    }
    // Prepare normal monster/artifact/resource
    // Before random, because random analyze exist items
    for(int i = 0; i<tiles_count; i++)
    {
        switch(tiles[i].generalObject)
        {
        case mp2obj(Artifact):
            add_moveable(i, FirstArtifact + (tiles[i].indexName1-1)/2, 0);
            break;
        case mp2obj(RndMonster):
            add_moveable(i, random::monster(-1), 0);
            break;
        case mp2obj(RndMonster1):
        case mp2obj(RndMonster2):
        case mp2obj(RndMonster3):
        case mp2obj(RndMonster4):
            add_moveable(i, random::monster(tiles[i].generalObject-mp2obj(RndMonster1)+1), 0);
            break;
        case mp2obj(Monster):
            add_moveable(i, FirstMonster + tiles[i].indexName1, 0);
            break;
        case mp2obj(Resource):
            add_moveable(i, FirstResource + (tiles[i].indexName1-1)/2, 0);
            break;
        case mp2obj(RndResource):
            add_moveable(i, FirstResource + xrand(0, 6), 0);
            break;
        case mp2obj(TreasureChest):
            if(res::map(tiles[i].objectName1)==res::OBJNRSRC)
                add_moveable(i, TreasureChest, 0);
            break;
        }
    }
    // Transform random artifact values
    for(int i = 0; i<tiles_count; i++)
    {
        int m = tiles[i].generalObject;
        switch(m)
        {
        case mp2obj(RndArtifact1):
        case mp2obj(RndArtifact2):
        case mp2obj(RndArtifact3):
            add_moveable(i, random::artifact(m-mp2obj(RndArtifact1)+1), 0);
            break;
        case mp2obj(RndArtifact):
            add_moveable(i, random::artifact(0), 0);
            break;
        case mp2obj(RndUltimateArtifact):
            add_moveable(i, random::artifact(4), 0);
            break;
        default:
            break;
        }
    }
    // after load tiles
    for(int i = 0; i<tiles_count; i++)
    {
        show::tiles[i] = tiles[i].tileIndex;
        show::flags[i] = tiles[i].shape%4;
        for(int level = 3; level>=0; level--)
        {
            // level 1.0
            if(tiles[i].indexName1!=0xFF && (tiles[i].quantity1%4)==level)
                add_object(i, tiles[i].objectName1, tiles[i].indexName1, tiles[i].quantity1);
            // level 1.N
            if(tiles[i].indexAddon)
            {
                int ai = tiles[i].indexAddon;
                while(ai)
                {
                    mp2::addon& a = addons[ai];
                    if(a.objectNameN1 && a.indexNameN1!=0xFF && (a.quantityN%4)==level)
                        add_object(i, a.objectNameN1, a.indexNameN1, a.quantityN);
                    ai = a.indexAddon;
                }
            }
            // level 2.0
            if(tiles[i].indexName2!=0xFF && (tiles[i].quantity2%4)==level)
                add_object(i, tiles[i].objectName2, tiles[i].indexName2, tiles[i].quantity2);
            // level 2.N
            if(tiles[i].indexAddon)
            {
                int ai = tiles[i].indexAddon;
                while(ai)
                {
                    mp2::addon& a = addons[ai];
                    if(a.objectNameN2 && a.indexNameN2!=0xFF && (a.quantityN%4)==level)
                        add_object(i, a.objectNameN2, a.indexNameN2, a.quantityN);
                    ai = a.indexAddon;
                }
            }
        }
    }
    // Create start heroes if need
    // Heroes must be valid race
    if(game.start_hero)
    {
        for(int i = (int)FirstPlayer; i<=(int)LastPlayer; i++)
        {
            if(!bsget(i, Valid))
                continue;
            tokens hrc;
            // Get first castle and create hero
            tokens type = tokens(player::get(i, Type));
            int rec = bsfind(FirstCastle, Player, i);
            if(rec!=-1)
            {
                hrc = get_free_hero(type);
                bsset(hrc, Player, i);
                bsset(hrc, Position, bsget(rec, Position));
                bsset(hrc, Direction, Right);
            }
            // Two hero recruit in castle
            hrc = get_free_hero(type);
            bsset(hrc, Recruit, i);
            hrc = get_free_hero(oppose_type(type));
            bsset(hrc, Recruit, i);
        }
    }
    world::set(Difficulty, game.difficult);
    delete tiles;
    delete addons;
}