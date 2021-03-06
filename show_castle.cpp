#include "main.h"

const rect& getrect(int race, int building) {
	// Barbarian, Knight, Necromancer, Sorcerer, Warlock, Wizard,
	static rect data[Tent - Castle][6] = {{{0, 88, 0 + 202, 88 + 62},// Castle
	{0, 55, 0 + 290, 55 + 85},
	{322, 63, 322 + 73, 63 + 97},
	{0, 67, 0 + 198, 67 + 100},
	{268, 35, 268 + 131, 35 + 129},
	{0, 48, 0 + 187, 48 + 39}},
	{{478, 100, 478 + 76, 100 + 42},// ThievesGuild
	{0, 130, 0 + 50, 130 + 60},
	{291, 134, 291 + 43, 134 + 59},
	{423, 165, 423 + 65, 165 + 49},
	{525, 109, 525 + 60, 109 + 48},
	{507, 55, 507 + 47, 55 + 42}},
	{{0, 205, 0 + 125, 205 + 60},// Tavern
	{350, 110, 350 + 46, 110 + 56},
	{0, 0, 0 + 0, 0 + 0},
	{494, 140, 494 + 131, 140 + 87},
	{479, 100, 479 + 39, 100 + 52},
	{0, 160, 0 + 118, 160 + 50}},
	{{535, 210, 535 + 105, 210 + 45},// Shipyard
	{537, 221, 537 + 103, 221 + 33},
	{516, 221, 516 + 124, 221 + 28},
	{0, 220, 0 + 134, 220 + 35},
	{520, 206, 520 + 120, 206 + 47},
	{0, 218, 0 + 185, 218 + 35}},
	{{272, 215, 272 + 44, 215 + 32}, // Well
	{194, 225, 194 + 29, 225 + 27},
	{217, 225, 217 + 23, 225 + 26},
	{346, 209, 346 + 43, 209 + 25},
	{348, 221, 348 + 63, 221 + 30},
	{254, 143, 254 + 19, 143 + 28}},
	{{470, 180, 470 + 30, 180 + 58},// Statue
	{480, 205, 480 + 45, 205 + 50},
	{374, 174, 374 + 26, 174 + 70},
	{158, 173, 158 + 17, 173 + 58},
	{473, 172, 473 + 45, 172 + 51},
	{464, 58, 464 + 26, 58 + 62}},
	{{0, 0, 0 + 0, 0 + 0},// Left turret
	{0, 0, 0 + 0, 0 + 0},
	{0, 0, 0 + 0, 0 + 0},
	{0, 0, 0 + 0, 0 + 0},
	{0, 0, 0 + 0, 0 + 0},
	{0, 0, 0 + 0, 0 + 0}},
	{{0, 0, 0 + 0, 0 + 0},// Right turret
	{0, 0, 0 + 0, 0 + 0},
	{0, 0, 0 + 0, 0 + 0},
	{0, 0, 0 + 0, 0 + 0},
	{0, 0, 0 + 0, 0 + 0},
	{0, 0, 0 + 0, 0 + 0}},
	{{113, 155, 113 + 106, 155 + 30}, // Moat
	{53, 150, 53 + 93, 150 + 30},
	{336, 169, 336 + 98, 169 + 11},
	{143, 169, 143 + 98, 169 + 11},
	{327, 166, 327 + 66, 166 + 17},
	{0, 91, 0 + 198, 91 + 11}},
	{{224, 168, 224 + 52, 168 + 36}, // Marketplace
	{220, 144, 220 + 115, 144 + 20},
	{415, 216, 415 + 85, 216 + 35},
	{412, 122, 412 + 56, 122 + 40},
	{391, 185, 391 + 70, 185 + 26},
	{254, 176, 254 + 105, 176 + 39}},
	{{210, 104, 210 + 40, 104 + 35}, // Captain
	{293, 109, 293 + 48, 109 + 27},
	{423, 126, 423 + 41, 126 + 46},
	{238, 136, 238 + 32, 136 + 34},
	{420, 102, 420 + 52, 102 + 60},
	{210, 55, 210 + 28, 55 + 32}},
	{{252, 120, 252 + 44, 120 + 16},// Well2
	{288, 97, 288 + 63, 97 + 18},
	{275, 206, 275 + 68, 206 + 39},
	{135, 200, 135 + 63, 200 + 31},
	{69, 46, 69 + 24, 46 + 124},
	{234, 225, 234 + 107, 225 + 22}},
	{{223, 79, 223 + 124, 79 + 27},// Specail Building
	{0, 80, 0 + 250, 80 + 20},
	{0, 0, 0 + 640, 0 + 54},
	{147, 0, 147 + 252, 147 + 30},
	{0, 162, 0 + 70, 162 + 77},
	{304, 111, 304 + 95, 111 + 50}},
	{{258, 142, 258 + 71, 142 + 41},// Dwelve 1
	{195, 175, 195 + 50, 175 + 40},
	{404, 181, 404 + 56, 181 + 25},
	{478, 70, 478 + 92, 70 + 62},
	{0, 63, 0 + 68, 63 + 53},
	{459, 181, 459 + 45, 181 + 32}},
	{{152, 190, 152 + 68, 190 + 50},// Dwelve 2
	{145, 155, 145 + 58, 155 + 20},
	{147, 184, 147 + 92, 184 + 32},
	{345, 149, 345 + 70, 149 + 56},
	{248, 192, 248 + 60, 192 + 55},
	{253, 69, 253 + 90, 69 + 29}},
	{{582, 81, 582 + 58, 81 + 40},// Dwelve 3
	{250, 177, 250 + 70, 177 + 50},
	{108, 69, 108 + 117, 69 + 91},
	{90, 180, 90 + 56, 180 + 21},
	{504, 53, 504 + 38, 53 + 30},
	{156, 139, 156 + 74, 139 + 51}},
	{{509, 148, 509 + 123, 148 + 57},// Dwelve 4
	{328, 195, 328 + 100, 195 + 50},
	{0, 154, 0 + 140, 182 + 74},
	{208, 192, 208 + 127, 192 + 55},
	{154, 168, 154 + 171, 168 + 76},
	{593, 187, 593 + 47, 187 + 28}},
	{{331, 186, 331 + 121, 186 + 44},// Dwelve 5
	{0, 200, 0 + 150, 200 + 55},
	{235, 136, 235 + 53, 136 + 70},
	{345, 231, 345 + 165, 231 + 21},
	{149, 98, 149 + 95, 98 + 58},
	{417, 25, 417 + 35, 25 + 140}},
	{{407, 13, 407 + 109, 13 + 80},// Dwelve 6
	{465, 85, 465 + 175, 85 + 110},
	{468, 112, 468 + 92, 112 + 78},
	{202, 38, 202 + 42, 38 + 65},
	{98, 25, 98 + 55, 25 + 229},
	{196, 7, 196 + 129, 7 + 38}},
	{{348, 20, 348 + 50, 20 + 124},// Mage guild
	{398, 55, 398 + 58, 55 + 125},
	{570, 45, 570 + 61, 45 + 162},
	{285, 32, 285 + 55, 32 + 129},
	{590, 14, 590 + 50, 14 + 155},
	{585, 0, 585 + 57, 0 + 122}},
	};
	return data[building - Castle][race - Barbarian];
}

static int* priority(int race) {
	static int data[6][32] =
	{
		{
			// Barbarian
			SpecialBuilding, Well2, Dwelving6, MageGuild, Captain, Castle, LeftTurret, RightTurret, Moat,
			Dwelving3, ThievesGuild, Tavern, Dwelving1, MarketPlace,
			Dwelving2, Dwelving4,
			Dwelving5,
			Well, Statue, Shipyard, 0
		},
		{
			// Knight
			Castle, SpecialBuilding, Well2, Captain, LeftTurret, RightTurret, Moat,
			MarketPlace, Dwelving2, ThievesGuild, Tavern, MageGuild,
			Dwelving5, Dwelving6,
			Dwelving1, Dwelving3, Dwelving4,
			Well, Statue, Shipyard, 0
		},
		{
			// Necromancer
			SpecialBuilding, Tavern, Castle, Captain, LeftTurret, RightTurret, Dwelving6, Moat,
			Dwelving1, ThievesGuild, Dwelving3, Dwelving5, Dwelving2,
			Dwelving4, MageGuild, Shipyard, Well2, MarketPlace,
			Statue, Well, 0
		},
		{
			// Sorcerer
			SpecialBuilding, Dwelving6, MageGuild, Captain, Castle, LeftTurret, RightTurret, Moat,
			Dwelving3, Shipyard, MarketPlace, Dwelving2,
			ThievesGuild, Dwelving1, Tavern, Statue, Well2, Dwelving4, Well, Dwelving5, 0
		},
		{
			// Warlock
			Dwelving5, Dwelving3, Castle, LeftTurret, RightTurret, Captain, Moat,
			Shipyard, MageGuild, Tavern, ThievesGuild, MarketPlace, Statue,
			Dwelving1, Well2, SpecialBuilding,
			Dwelving4, Dwelving2, Dwelving6, Well, 0
		},
		{
			// Wizard
			Dwelving6, Tent, Castle, LeftTurret, RightTurret, Moat, Captain,
			Dwelving2, ThievesGuild, Tavern, Shipyard, Well, SpecialBuilding,
			Dwelving3, Dwelving5, MageGuild, Statue,
			Dwelving1, Dwelving4, MarketPlace, Well2, 0
		},
	};
	return data[race - Barbarian];
}

static void panorama(int x, int y, int mid) {
	int race = bsget(mid, Type);
	bool coastal = true;
	switch(race) {
	case Knight:
		draw::image(x, y, res::TOWNBKG0, 0, 0);
		break;
	case Barbarian:
		draw::image(x, y, res::TOWNBKG1, 0, 0);
		draw::image(x, y, res::TWNBEXT1, 1 + draw::counter % 5, 0);
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
	if(coastal) {
		switch(race) {
		case Knight:
			draw::image(x, y, res::TWNKEXT0, 0);
			draw::image(x, y, res::TWNKEXT0, 1 + draw::counter % 5);
			break;
		case Barbarian:
			draw::image(x, y, res::TWNBEXT0, 0);
			draw::image(x, y, res::TWNBEXT0, 1 + draw::counter % 5);
			break;
		case Sorcerer:
			draw::image(x, y, res::TWNSEXT0, 0);
			draw::image(x, y, res::TWNSEXT0, 1 + draw::counter % 5);
			break;
		case Necromancer:
			draw::image(x, y, res::TWNNEXT0, 0);
			draw::image(x, y, res::TWNNEXT0, 1 + draw::counter % 5);
			break;
		case Warlock:
			draw::image(x, y, res::TWNWEXT0, 0);
			draw::image(x, y, res::TWNWEXT0, 1 + draw::counter % 5);
			break;
		default:
			draw::image(x, y, res::TWNZEXT0, 0);
			draw::image(x, y, res::TWNZEXT0, 1 + draw::counter % 5);
			break;
		}
	}
	for(int* pi = priority(race); *pi; pi++) {
		int building = *pi;
		if(!building)
			break;
		int level = bsget(mid, building);
		if(!level)
			continue;
		res::tokens icn = res::building(race, building, level);
		if(icn == res::Empthy)
			continue;
		int index = 0;
		if(building == MageGuild) {
			switch(level) {
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
		if(hot::mouse.in(getrect(race, building))) {
			if(building >= Dwelving1 && building <= Dwelving6)
				draw::status(szt("Recruit %1", "������ %1"),
					bsgets(game::getunit(race, building, bsget(mid, building)), NameMulti));
			else
				draw::status(game::getbuildingname(race, building, level));
			if(hot::key == MouseLeft && hot::pressed)
				draw::execute(building);
			else if(hot::key == MouseRight && hot::pressed) {
				hot::level = bsget(mid, building);
				draw::execute(Information, building, race);
			}
		}
		index = indexes::animate(icn, index, draw::counter, false);
		if(index)
			draw::image(x, y, icn, index);
	}
}

static void paint_panel(int x, int y, int mid, int hero) {
	draw::image(x, y, res::STRIP, 0);
	draw::image(x + 5, y + 6, res::CREST, 3);
	draw::troops(x + 112, y + 6, mid);
	if(hero) {
		draw::clipart(x + 5 + 50, y + 105, hero, LargeSize, 0, false, true);
		draw::troops(x + 112, y + 105, hero);
	} else {
		if(bsget(mid, Captain))
			draw::clipart(x + 5 + 50, y + 105, bsget(mid, Type) - Barbarian + BarbarianCaptain, bsget(mid, Player));
		else
			draw::image(x + 5, y + 105, res::STRIP, 3);
		draw::image(x + 112, y + 105, res::STRIP, 11);
	}
}

static void name() {
	draw::state push;
	draw::font = res::SMALFONT;
	draw::image(0, 0, res::TOWNNAME, 0);
	draw::text(231 + 179 / 2, 248 + 2, "�������", 0);
}

static void paint_castle(int rec, int hero, int player) {
	draw::status(21, draw::height - 16, 21 + res::width(res::SMALLBAR, 0), draw::height - 1);
	draw::button(0, draw::height - 19, res::SMALLBAR, KeyLeft, 1, 1, 2, 0, 0, szt("Previous town", ""));
	draw::image(21, draw::height - 19, res::SMALLBAR, 0);
	draw::button(draw::width - 21, draw::height - 19, res::SMALLBAR, KeyRight, 3, 3, 4, 0, 0, szt("Next town", ""));
	panorama(0, 0, rec);
	paint_panel(0, 256, rec, hero);
	draw::resource(552, 262, bsptr(player, FirstResource));
	draw::button(553, 428, res::SWAPBTN, Cancel, 0, 0, 1, KeyEscape, 0, szt("Leave town", ""));
	name();
}

void show::castle(int rec) {
	int unit;
	int army_index = -1;
	int player = bsget(rec, Player);
	while(true) {
		auto index = bsget(rec, Index);
		auto hero = bsfind(FirstHero, Index, index);
		paint_castle(rec, hero, player);
		draw::cursor(res::ADVMCO, 0);
		int id = draw::input();
		switch(id) {
		case Dwelving1:
		case Dwelving2:
		case Dwelving3:
		case Dwelving4:
		case Dwelving5:
		case Dwelving6:
			unit = game::getunit(bsget(rec, Type), id, bsget(rec, id));
			if(unit) {
				auto count = 0;
				auto max_count = bsget(rec, FirstCreatureCount + (id - Dwelving1));
				auto available_resources = bsptr(player, FirstResource);
				auto monster_resources = bsptr(unit, FirstResource);
				if(show::recruit(unit, count, max_count, available_resources)) {
					if(!game::addunit(rec, unit, count))
						dlgerr(0, szt("Your garrison is out of space.", "� ����� ��������� �� ������� �����."));
					else {
						int total[LastResource - FirstResource + 1];
						game::mulresource(total, monster_resources, count);
						game::addresources(available_resources, available_resources, total, true);
						bsadd(rec, FirstCreatureCount + (id - Dwelving1), -count);
					}
				}
			}
			break;
		case Castle:
			id = show::build(rec);
			if(id >= FirstBuilding && id <= LastBuilding) {
				paint_castle(rec, hero, player);
				draw::screenshoot source;
				game::build(rec, id);
				paint_castle(rec, hero, player);
				draw::screenshoot dest;
				source.blend(dest);
			} else if(id >= FirstHero && id <= LastHero) {
				paint_castle(rec, hero, player);
				draw::screenshoot source;
				auto index = bsget(rec, Index);
				if(game::hire(id, player, index)) {
					paint_castle(rec, bsfind(FirstHero, Index, index), player);
					draw::screenshoot dest;
					source.blend(dest);
				}
			}
			break;
		case MarketPlace:
			show::marketplace(player);
			break;
		case ThievesGuild:
			show::thieves(player);
			break;
		case MageGuild:
			show::mageguild(rec);
			break;
		case Well:
			show::well(rec);
			break;
		case Cancel:
			return;
		default:
			draw::definput(id);
			break;
		}
	}
}