#include "main.h"

static int player_index;

static int xpos(int current, int count, int width)
{
    return current * width * 6 / count + (width * (6 - count) / (2 * count));
}

static int mapinfo(gamefile* info, const char* map_ext="mp2")
{
    int count = 0;
	for(io::file::find e("maps"); e; e.next())
    {
        if(e.name()[0]=='.')
            continue;
		const char* ex = szext(e.name());
		if(!ex)
			continue;
		if(strcmp(ex, map_ext)!=0)
			continue;
		if(info)
		{
			char temp[261];
			info[count].load(e.fullname(temp));
		}
		count++;
    }
    return count;
}

static int size_index(int id)
{
    switch(id)
    {
    case SmallSize:
        return 26;
    case MediumSize:
        return 27;
    case LargeSize:
        return 28;
    case XLargeSize:
        return 29;
    default:
        return 27;
    }
}

static bool choose_scenario(gamefile& result)
{
    struct list_scenarios : public list
    {

        int			filter;
        int			cashed_filter;
        int			file_maximum;
        gamefile*	file_data;
        gamefile*	data[512];

        void row(int x, int y, int index) const
        {
            int width = 264;
            draw::image(x, y+1, res::REQUESTS, 19 + data[index]->sides()); // flags
            draw::image(x+18, y+1, res::REQUESTS, size_index(data[index]->size));
            draw::image(x+width-18, y+1, res::REQUESTS, 30+data[index]->wins);
            draw::image(x+width-18*2, y+1, res::REQUESTS, 36+data[index]->lose);
            draw::textm(x+40, y+3, width-36-36, draw::Center, data[index]->name);
        }

        void prerender()
        {
            if(filter!=cashed_filter)
            {
                maximum = 0;
                for(int i = 0; i<file_maximum; i++)
                {
                    if(filter!=All && file_data[i].size!=filter)
                        continue;
                    data[maximum++] = &file_data[i];
                }
            }
        }

        list_scenarios()
        {
            cashed_filter = 0;
            filter = All;
            file_maximum = mapinfo(0);
            file_data = new gamefile[file_maximum];
            mapinfo(file_data);
        }

        ~list_scenarios()
        {
            if(file_data)
                delete file_data;
        }

    } scenarios;
    const int x = draw::width-res::width(res::REQSBKG, 0)-24;
    const int y = (draw::height-res::height(res::REQSBKG, 0))/2;
    if(!scenarios.file_maximum)
    {
        dlgmsg(0, szt("You don't have any game map. Try load some from internet.", "У вас нету ни одной карты. Попробуйте загрузить несколько из интернета."));
        return 0;
    }
    while(true)
    {
        draw::image(0, 0, res::HEROES, 0);
        draw::image(x, y, res::REQSBKG, 0);
        scenarios.area(x+57, y+55, 9);
        gamefile& e = *scenarios.data[scenarios.current];
        draw::image(x+85, y+265, res::REQUESTS, 19 + e.sides()); // flags
        draw::image(x+103, y+265, res::REQUESTS, size_index(e.size));
        draw::textm(x + 120 + 1, y + 265 + 2, 160, draw::Center, e.name);
        draw::image(x+288, y+265, res::REQUESTS, 30+e.wins);
        draw::image(x+288+18, y+265, res::REQUESTS, 36+e.lose);
        draw::text(x + 66, y + 294, szt("Map Difficulty:", "Сложность карты:"), -1);
        draw::textm(x + 236, y + 294, 114, draw::Center, bsgets(e.level,Name));
        draw::textm(x + 58, y + 324, 294, draw::Center, e.description);
        draw::button(x + 140 + 16, y + 410, res::REQUESTS, Accept, 1, 1, 2, KeyEnter);
        // maps
        draw::button(x + 37 + 16, y + 22, res::REQUESTS, SmallSize, (scenarios.filter==SmallSize) ? 10 : 9, (scenarios.filter==SmallSize) ? 10 : 9, 10); // small
        draw::button(x + 99 + 16, y + 22, res::REQUESTS, MediumSize, (scenarios.filter==MediumSize) ? 12 : 11, (scenarios.filter==MediumSize) ? 12 : 11, 12); // medium
        draw::button(x + 161 + 16, y + 22, res::REQUESTS, LargeSize, (scenarios.filter==LargeSize) ? 14 : 13, (scenarios.filter==LargeSize) ? 14 : 13, 14);
        draw::button(x + 223 + 16, y + 22, res::REQUESTS, XLargeSize, (scenarios.filter==XLargeSize) ? 16 : 15, (scenarios.filter==XLargeSize) ? 16 : 15, 16);
        draw::button(x + 285 + 16, y + 22, res::REQUESTS, All, (scenarios.filter==All) ? 18 : 17, (scenarios.filter==All) ? 18 : 17, 18);
        draw::cursor(res::ADVMCO, 0);
        int id = draw::input();
        switch(id)
        {
        case 0:
        case Cancel:
            return 0;
        case SmallSize:
        case MediumSize:
        case LargeSize:
        case XLargeSize:
        case All:
            scenarios.filter = id;
            break;
        case Accept:
            result = *scenarios.data[scenarios.current];
            return true;
        }
    }
}

static void button_difficult(int x, int y, int id, int id_select)
{
    rect rc = {x,y,x+res::width(res::NGEXTRA,62),y+res::height(res::NGEXTRA,62)};
    if(id_select==id)
        draw::image(x, y, res::NGEXTRA, 62);
    if(hot::mouse.in(rc))
    {
        if(hot::key==MouseLeft && hot::pressed)
            draw::execute(id);
    }
	draw::state push;
	draw::font = res::SMALFONT;
	const char* p = bsgets(id,Name);
    draw::text(x+(rc.width()-draw::textw(p))/2, y+rc.height()+2, p);
}

static int race2index(int race, bool class_color)
{
    switch(race)
    {
    case Knight:
        return class_color ? 51 : 70;
        break;
    case Barbarian:
        return class_color ? 52 : 71;
        break;
    case Sorcerer:
        return class_color ? 53 : 72;
        break;
    case Warlock:
        return class_color ? 54 : 73;
        break;
    case Wizard:
        return class_color ? 55 : 74;
        break;
    case Necromancer:
        return class_color ? 56 : 75;
        break;
    case MultiPlayers:
        return 76;
        break;
    default:
        return 58;
    }
}

static void button_type(int x, int y, tokens type, int i, bool show_name = true)
{
    int index = 0;
    switch(type)
    {
    case Computer:
        index = 3+i;
        break;
    case ComputerOnly:
        index = 3+12+i;
        break;
    case Human:
        index = 9+i;
        break;
    default:
        break;
    }
    if(show_name)
        index += 24;
    draw::image(x, y, res::NGEXTRA, index, AFNoOffset);
    rect rc = {x,y,x+res::width(res::NGEXTRA,3),y+res::height(res::NGEXTRA,3)};
    if(show_name)
    {
		draw::state push;
		draw::font = res::SMALFONT;
		const char* p = bsgets(FirstPlayer+i+1,Name);
        draw::text(x+(rc.width()-draw::textw(p))/2, y+rc.height(), p);
    }
    if(hot::mouse.in(rc))
    {
        player_index = i;
        if(hot::key==MouseLeft && hot::pressed)
        {
            if(type==ComputerOnly)
                return;
            draw::execute(Human);
        }
    }
}

static void button_race(int x, int y, int race, int i, bool show_name, bool disabled)
{
    int index = race2index(race, true);
    if(disabled)
        index += 19;
    draw::image(x, y, res::NGEXTRA, index, AFNoOffset);
    int w = res::width(res::NGEXTRA, index);
    int h = res::height(res::NGEXTRA, index);
    if(show_name)
    {
		draw::state push;
		draw::font = res::SMALFONT;
		const char* p = bsgets(race,Name);
        if(race==Necromancer)
            p = szt("Necrom.", "Некром.");
        else if(race==Warlock)
            p = szt("Warlock","Чародей");
        else if(race==Sorcerer)
            p = szt("Sorcerer", "Волшеб.");
        else if(race==Random)
            p = szt("Random", "Случ.");
        else if(race==MultiPlayers)
            p = szt("Multi", "Много");
        draw::text(x+(w-draw::textw(p))/2, y+h+2, p);
    }
    rect rc = {x,y,x+w,y+h};
    if(hot::mouse.in(rc) && !disabled)
    {
        player_index = i;
        if(hot::key==MouseLeft && hot::pressed)
            draw::execute(Type);
    }
}

static bool tune_scenario(gamefile& result)
{
    gamefile e = result;
    const int x = draw::width-res::width(res::NGHSBKG, 0) - 8;
    const int y = (draw::height-res::height(res::NGHSBKG, 0))/2;
    const int w = res::width(res::NGHSBKG, 0);
    char temp[64];
    const char* p;
    while(true)
    {
        draw::image(0, 0, res::HEROES, 0);
        draw::image(x, y, res::NGHSBKG, 0, AFNoOffset);
        // map
        p = szt("Scenario:", "Сценарий:");
        draw::text(x+(w-draw::textw(p))/2, y+24, p);
        draw::button(x+309, y + 45, res::NGEXTRA, StandartGame, 64, 64, 65, F4);
        p = e.name;
        draw::text(x+(w-draw::textw(p))/2, y+48, p);
        // difficult
        p = szt("Difficult:", "Сложность:");
        draw::text(x+(w-draw::textw(p))/2, y+76, p);
        button_difficult(x+21, y+91, EasyDifficulty, e.difficult);
        button_difficult(x+98, y+91, NormalDifficulty, e.difficult);
        button_difficult(x+174, y+91, HardDifficulty, e.difficult);
        button_difficult(x+251, y+91, VeryHardDifficulty, e.difficult);
        button_difficult(x+328, y+91, ImpossibleDifficulty, e.difficult);
        // info
        int maximum = e.sides();
        int current = 0;
        int y1 = y + 200;
        int w1 = res::width(res::NGEXTRA,3);
        for(int i=0; i<6; i++)
        {
            if(e.types[i]==Empthy)
                continue;
            int x1 = x + (w-w1*6)/2 + xpos(current,maximum,w1);
            button_type(x1, y1, e.types[i], i, true);
            button_race(x1, y1 + 78, e.races[i], i, true, e.blocked[i]);
            current++;
        }
        // opponents
        p = szt("Opponents:", "Оппоненты:");
        draw::text(x+(w-draw::textw(p))/2, y+182, p);
        // class
        p = szt("Class:", "Класс:");
        draw::text(x+(w-draw::textw(p))/2, y+262, p);
        // rating
        szprint(temp, "%1 %2i%%", szt("Rating", "Рейтинг"), bsget(e.difficult, Rating));
        draw::text(x+(w-draw::textw(temp))/2, y+383, temp);
        // buttons
        draw::button(x + 31, y + 380, res::NGEXTRA, Accept, 66, 66, 67, KeyEnter, 0,
                     szt("Start scenario with current setting.", "Начать сценарий с текущими настройками."));
        draw::button(x + 287, y + 380, res::NGEXTRA, Cancel, 68, 68, 69, KeyEscape, 0,
                     szt("Cancel current selection.","Отменить текущий выбор."));
        draw::debug();
        draw::cursor(res::ADVMCO, 0);
        int id = draw::input();
        switch(id)
        {
        case StandartGame:
            choose_scenario(e);
            break;
        case 0:
        case Cancel:
            return false;
        case Human:
            if(e.types[player_index]==Computer)
            {
                int index = 0;
                for(; index<6; index++)
                {
                    if(e.types[index]==Human)
                        break;
                }
                iswap(e.types[player_index], e.types[index]);
                iswap(e.races[player_index], e.races[index]);
            }
            break;
        case Type:
            if(e.races[player_index]==MultiplayerGame)
                break;
            if(e.races[player_index]==Random)
                e.races[player_index]=Barbarian;
            else if(e.races[player_index]==Wizard)
                e.races[player_index]=Random;
            else
                e.races[player_index] = tokens(e.races[player_index]+1);
            break;
        case Accept:
            result = e;
            return true;
        default:
            if(id>=(int)EasyDifficulty && id<=(int)ImpossibleDifficulty)
                e.difficult = tokens(id);
            break;
        }
    }
}

static int newgame()
{
    while(true)
    {
        draw::image(0, 0, res::HEROES, 0);
        draw::image(405, 5, res::REDBACK, 0);
        draw::button(455, 45, res::BTNNEWGM, StandartGame, 0, 0, 1);
        draw::button(455, 110, res::BTNNEWGM, CampaignGame, 2, 2, 3);
        draw::button(455, 175, res::BTNNEWGM, MultiplayerGame, 4, 4, 5);
        draw::button(455, 375, res::BTNNEWGM, Cancel, 6, 6, 7);
        draw::cursor(res::ADVMCO, 0);
        int id = draw::input();
        switch(id)
        {
        case 0:
        case Cancel:
            return 0;
        case MultiplayerGame:
        case StandartGame:
        case CampaignGame:
            return id;
        }
    }
}

void test_combat()
{
	game::initialize();
	game::prepare();
	bsset(LordKillburn, Player, PlayerBlue);
	bsset(LordKillburn, SpellBless, 1);
	bsset(LordKillburn, SpellCurse, 1);
	bsset(LordKillburn, SpellSlow, 1);
	bsset(LordKillburn, SpellCure, 1);
	bsset(LordKillburn, SpellBlind, 1);
	bsset(LordKillburn, SpellDispel, 1);
	game::addunit(LordKillburn, Swordsman, 1);
	game::addunit(LordKillburn, Centaur, 5);
	game::additem(LordKillburn, MagicBook);
	bsset(Tsabu, Player, PlayerRed);
	game::addunit(Tsabu, Troll, 1);
	game::addunit(Tsabu, Gargoyle, 1);
	combat::start(LordKillburn, Tsabu);
}

void test_castle()
{
	game::initialize();
	int type = xrand(Barbarian, Wizard);
	int player = PlayerBlue;
	bsset(player, Type, type);
	bsset(player, PlayerType, Human);
	int castle = game::random::castle(type, player, 400, true);
	game::addunit(castle, Boar, 4);
	game::addunit(castle, Halfling, 10);
	game::prepare();
	show::castle(castle);
}

static int mainmenu()
{
	animation latern(res::SHNGANIM, 1, 39);
    int game_type;
    gamefile game;
    game.clear();
    while(true)
    {
        draw::image(0, 0, res::HEROES, 0);
        draw::button(0, 0, res::BTNSHNGL, NewGame, 1, 2, 3, 0, 0,
                     szt("Begin new single or multiplayer game.", "Начать новую одиночную или мультиплеер игру."));
        draw::button(0, 0, res::BTNSHNGL, LoadGame, 5, 6, 7, Ctrl+Alpha+'L', 0,
                     szt("Load early saved game.", "Загрузить ранее сохраненную игру."));
        draw::button(0, 0, res::BTNSHNGL, HightScores, 9, 10, 11);
        draw::button(0, 0, res::BTNSHNGL, Credits, 13, 14, 15);
        draw::button(0, 0, res::BTNSHNGL, QuitGame, 17, 18, 19);
		latern.painting({0, 0});
		draw::cursor(res::ADVMCO, 0);
        int id = draw::input();
        switch(id)
        {
        case NewGame:
            game_type = newgame();
            switch(game_type)
            {
            case StandartGame:
                if(choose_scenario(game))
                {
                    game.players(1);
                    if(tune_scenario(game))
						game::play(game);
                }
                break;
            }
            break;
        case LoadGame:
            game.load("_brokena.mp2");
            game.load("_beltway.mp2");
            game.players(1);
            if(!tune_scenario(game))
				break;
			game::play(game);
            break;
        case Credits:
        	//test_combat();
			test_castle();
            break;
        case HightScores:
            show::highscore();
            break;
		case QuitGame:
			return Cancel;
		case InputTimer:
			latern.update();
			break;
        case 0:
            return 0;
        }
    }
    return 0;
}

int main()
{
	draw::font = res::FONT;
	draw::create("Heroes II", 200, false);
	return mainmenu();
}

int __stdcall WinMain(void* ci, void* pi, char* cmd, int sw)
{
	return main();
}