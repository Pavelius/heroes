#include "main.h"

static int day, week_of, month_of;
static int pla, her;
static tokens week_monsters[] =
{
    Goblin, Orc, Wolf, Ogre, Troll, Cyclop,
    Peasant, Archer, Cavalry,
    Centaur, Gargoyle, Griffin,
    Sprite, Dwarf, Elf,
    Halfling, Boar, Roc,
};

void world::clear()
{
    day = 0;
    week_of = 0;
    month_of = 0;
    her = pla = 0;
    player::clear();
    castle::clear();
}

int world::get(int id)
{
    switch(id)
    {
    case Player:
        return pla;
    case Hero:
        return her;
    case Day:
        return (day%7) + 1;
    case Week:
        return ((day/7)%4) + 1;
    case Month:
        return (day/(7*4) + 1);
    default:
        return 0;
    }
}

static int oppose(int value)
{
    switch(value)
    {
    case EasyDifficulty:
        return ImpossibleDifficulty;
    case NormalDifficulty:
        return VeryHardDifficulty;
    case VeryHardDifficulty:
        return NormalDifficulty;
    case ImpossibleDifficulty:
        return EasyDifficulty;
    default:
        return HardDifficulty;
    }
}

static void difficult(int mid, int value)
{
    switch(value)
    {
    case EasyDifficulty:
        player::set(mid, Ore, 20);
        player::set(mid, Wood, 20);
        player::set(mid, Crystal, 10);
        player::set(mid, Sulfur, 10);
        player::set(mid, Gems, 10);
        player::set(mid, Mercury, 10);
        player::set(mid, Gold, 20000);
        break;
    case NormalDifficulty:
        player::set(mid, Ore, 12);
        player::set(mid, Wood, 12);
        player::set(mid, Crystal, 6);
        player::set(mid, Sulfur, 6);
        player::set(mid, Gems, 6);
        player::set(mid, Mercury, 6);
        player::set(mid, Gold, 10000);
        break;
    case HardDifficulty:
        player::set(mid, Ore, 5);
        player::set(mid, Wood, 5);
        player::set(mid, Crystal, 0);
        player::set(mid, Sulfur, 0);
        player::set(mid, Gems, 0);
        player::set(mid, Mercury, 0);
        player::set(mid, Gold, 2500);
        break;
    }
}

bool world::set(int id, int value)
{
    switch(id)
    {
    case Difficulty:
        if(value)
        {
            for(int mid = FirstPlayer; mid<=(int)LastPlayer; mid++)
            {
                if(player::get(mid, PlayerType)==Human)
                    difficult(mid, value);
                else
                    difficult(mid, oppose(value));
            }
        }
        break;
    case Player:
        pla = value;
        break;
    default:
        return false;
    }
    return true;
}

int world::turn()
{
    for(int rec = (int)FirstPlayer; rec<=(int)LastPlayer; rec++)
    {
        set(Player, rec);
        switch(player::get(rec, PlayerType))
        {
        case Human:
            switch(show::game())
            {
            case 0:
                return 0;
            default:
                break;
            }
            break;
        case Computer:
            break;
        }
    }
    return EndTurn;
}

static void all_heroes_refresh()
{
    for(int rec = FirstHero; rec<=(int)LastHero; rec++)
    {
        if(bsget(rec, Position)==-1)
            continue;
        // Spell points
        int sp = bsget(rec, SpellPoints);
        int spm = bsget(rec, SpellPointsMax);
        sp = imin(sp + 1 + bsget(rec, SkillMysticism), spm);
        bsset(rec, SpellPoints, sp);
        // Move points
        bsset(rec, MovePoints, bsget(rec, MovePointsMax));
    }
}

static void all_castles_refresh()
{
    for(int rec = FirstCastle; rec<=(int)LastCastle; rec++)
    {
        // Update build flags
        bsset(rec, BuildThisTurn, 0);
        // Get income for all player exept neutral
        int p = bsget(rec, Player);
        if(p!=PlayerNeutral)
        {
            int m = bsget(rec, Income);
            bsadd(p, Gold, m);
        }
    }
}

static void all_dwellings_growth()
{
    int m_base = bsget(week_of, Base);
    int m_type = bsget(m_base, Type);
    int m_dwll = bsget(m_base, Dwelve);
    for(int rec = FirstCastle; rec<=(int)LastCastle; rec++)
    {
        for(int i = (int)Dwelving1; i<=(int)Dwelving6; i++)
        {
            if(!bsget(rec, i))
                continue;
            castle::growth(rec, i, true);
        }
        // Week monster growth fast
        if(m_type==bsget(rec, Type) && bsget(rec, m_dwll))
            bsadd(rec, FirstRecruit+m_dwll-Dwelving1, 5);
    }
}

int world::game(gamefile& game)
{
    game.validate();
    clear();
    map::load(game);
    all_dwellings_growth();
    while(true)
    {
        int result = turn();
        if(!result)
            return 0;
        all_heroes_refresh();
        all_castles_refresh();
        day++;
        if((day%7)==0)
        {
            week_of = week_monsters[rand()%(sizeof(week_monsters)/sizeof(week_monsters[0]))];
            all_dwellings_growth();
        }
    }
}