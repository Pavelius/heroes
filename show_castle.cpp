#include "main.h"

const rect& getrect(int race, int building)
{
	// Barbarian, Knight, Necromancer, Sorcerer, Warlock, Wizard,
	static rect data[Tent - Castle][6] =
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
			{113, 155, 113 + 106, 155 + 30},
			{53, 150, 53 + 93, 150 + 30},
			{336, 169, 336 + 98, 169 + 11},
			{143, 169, 143 + 98, 169 + 11},
			{327, 166, 327 + 66, 166 + 17},
			{0, 91, 0 + 198, 91 + 11},
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
			{210, 104, 210 + 40, 104 + 35},
			{293, 109, 293 + 48, 109 + 27},
			{423, 126, 423 + 41, 126 + 46},
			{238, 136, 238 + 32, 136 + 34},
			{420, 102, 420 + 52, 102 + 60},
			{210, 55, 210 + 28, 55 + 32},
		},
		{
			// Well2
			{252, 120, 252 + 44, 120 + 16},
			{288, 97, 288 + 63, 97 + 18},
			{275, 206, 275 + 68, 206 + 39},
			{135, 200, 135 + 63, 200 + 31},
			{69, 46, 69 + 24, 46 + 124},
			{234, 225, 234 + 107, 225 + 22},
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
			{582, 81, 582 + 58, 81 + 40},
			{250, 177, 250 + 70, 177 + 50},
			{108, 69, 108 + 117, 69 + 91},
			{90, 180, 90 + 56, 180 + 21},
			{504, 53, 504 + 38, 53 + 30},
			{156, 139, 156 + 74, 139 + 51},
		},
		{
			// Dwelve 4
			{509, 148, 509 + 123, 148 + 57},
			{328, 195, 328 + 100, 195 + 50},
			{0, 154, 0 + 140, 182 + 74},
			{208, 192, 208 + 127, 192 + 55},
			{154, 168, 154 + 171, 168 + 76},
			{593, 187, 593 + 47, 187 + 28},
		},
		{
			// Dwelve 5
			{331, 186, 331 + 121, 186 + 44},
			{0, 200, 0 + 150, 200 + 55},
			{235, 136, 235 + 53, 136 + 70},
			{345, 231, 345 + 165, 231 + 21},
			{149, 98, 149 + 95, 98 + 58},
			{417, 25, 417 + 35, 25 + 140},
		},
		{
			// Dwelve 6
			{407, 13, 407 + 109, 13 + 80},
			{465, 85, 465 + 175, 85 + 110},
			{468, 112, 468 + 92, 112 + 78},
			{202, 38, 202 + 42, 38 + 65},
			{98, 25, 98 + 55, 25 + 229},
			{196, 7, 196 + 129, 7 + 38},
		},
	};
	return data[building - Castle][race - Barbarian];
}

static res::tokens getrs(int race, int building, int level)
{
	if(race == Barbarian)
	{
		switch(building)
		{
		case Castle:
			if(level==2)
				return res::TWNBCSTL;
			else if(level==1)
				return res::TWNBTENT;
			return res::Empthy;
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
			if(level == 2)
				return res::TWNBUP_1;
			return res::TWNBDW_1;
		case Dwelving3:
			return res::TWNBDW_2;
		case Dwelving4:
			if(level == 2)
				return res::TWNBUP_3;
			return res::TWNBDW_3;
		case Dwelving5:
			if(level == 2)
				return res::TWNBUP_4;
			return res::TWNBDW_4;
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
		case Castle:
			if(level == 2)
				return res::TWNKCSTL;
			else if(level == 1)
				return res::TWNKTENT;
			return res::Empthy;
		case Tent: return res::TWNKTENT;
		case SpecialBuilding: return res::TWNKSPEC;
		case Captain: return res::TWNKCAPT;
		case Well2: return res::TWNKWEL2;
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
			if(level==2)
				return res::TWNKUP_1;
			return res::TWNKDW_1;
		case Dwelving3:
			if(level == 2)
				return res::TWNKUP_2;
			return res::TWNKDW_2;
		case Dwelving4:
			if(level == 2)
				return res::TWNKUP_3;
			return res::TWNKDW_3;
		case Dwelving5:
			if(level == 2)
				return res::TWNKUP_4;
			return res::TWNKDW_4;
		case Dwelving6:
			if(level == 2)
				return res::TWNKUP_5;
			return res::TWNKDW_5;
		default:
			break;
		}
	}
	else if(race == Necromancer)
	{
		switch(building)
		{
		case Castle:
			if(level == 2)
				return res::TWNNCSTL;
			else if(level == 1)
				return res::TWNNTENT;
			return res::Empthy;
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
			if(level==2)
				return res::TWNNUP_1;
			return res::TWNNDW_1;
		case Dwelving3:
			if(level == 2)
				return res::TWNNUP_2;
			return res::TWNNDW_2;
		case Dwelving4:
			if(level == 2)
				return res::TWNNUP_3;
			return res::TWNNDW_3;
		case Dwelving5:
			if(level == 2)
				return res::TWNNUP_4;
			return res::TWNNDW_4;
		case Dwelving6:
			return res::TWNNDW_5;
		default:
			break;
		}
	}
	else if(race == Sorcerer)
	{
		switch(building)
		{
		case Castle:
			if(level == 2)
				return res::TWNSCSTL;
			else if(level == 1)
				return res::TWNSTENT;
			return res::Empthy;
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
			if(level==2)
				return res::TWNSUP_1;
			return res::TWNSDW_1;
		case Dwelving3:
			if(level == 2)
				return res::TWNSUP_2;
			return res::TWNSDW_2;
		case Dwelving4:
			if(level == 2)
				return res::TWNSUP_3;
			return res::TWNSDW_3;
		case Dwelving5:
			return res::TWNSDW_4;
		case Dwelving6:
			return res::TWNSDW_5;
		default:
			break;
		}
	}
	else if(race == Warlock)
	{
		switch(building)
		{
		case Castle:
			if(level == 2)
				return res::TWNWCSTL;
			else if(level == 1)
				return res::TWNWTENT;
			return res::Empthy;
		case SpecialBuilding: return res::TWNWSPEC;
		case Captain: return res::TWNWCAPT;
		case Well2: return res::TWNWWEL2;
		case LeftTurret: return res::TWNWLTUR;
		case RightTurret: return res::TWNWRTUR;
		case Moat: return res::TWNWMOAT;
		case MarketPlace: return res::TWNWMARK;
		case ThievesGuild: return res::TWNWTHIE;
		case Tavern: return res::TWNWTVRN;
		case Well: return res::TWNWWELL;
		case Statue: return res::TWNWSTAT;
		case Shipyard: return res::TWNWDOCK;
		case MageGuild: return res::TWNWMAGE;
		case Dwelving1: return res::TWNWDW_0;
		case Dwelving2: return res::TWNWDW_1;
		case Dwelving3: return res::TWNWDW_2;
		case Dwelving4:
			if(level == 2)
				return res::TWNWUP_3;
			return res::TWNWDW_3;
		case Dwelving5: return res::TWNWDW_4;
		case Dwelving6:
			if(level == 3)
				return res::TWNWUP5B;
			if(level == 2)
				return res::TWNWUP_5;
			return res::TWNWDW_5;
		}
	}
	else if(race == Wizard)
	{
		switch(building)
		{
		case Castle:
			if(level == 2)
				return res::TWNZCSTL;
			else if(level == 1)
				return res::TWNZTENT;
			return res::Empthy;
		case SpecialBuilding: return res::TWNZSPEC;
		case Captain: return res::TWNZCAPT;
		case Well2: return res::TWNZWEL2;
		case LeftTurret: return res::TWNZLTUR;
		case RightTurret: return res::TWNZRTUR;
		case Moat: return res::TWNZMOAT;
		case MarketPlace: return res::TWNZMARK;
		case ThievesGuild: return res::TWNZTHIE;
		case Tavern: return res::TWNZTVRN;
		case Well: return res::TWNZWELL;
		case Statue: return res::TWNZSTAT;
		case Shipyard: return res::TWNZDOCK;
		case MageGuild: return res::TWNZMAGE;
		case Dwelving1: return res::TWNZDW_0;
		case Dwelving2: return res::TWNZDW_1;
		case Dwelving3:
			if(level == 2)
				return res::TWNZUP_2;
			return res::TWNZDW_2;
		case Dwelving4: return res::TWNZDW_3;
		case Dwelving5:
			if(level == 2)
				return res::TWNZUP_4;
			return res::TWNZDW_4;
		case Dwelving6:
			if(level == 2)
				return res::TWNZUP_5;
			return res::TWNZDW_5;
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

static void panorama(int x, int y, int mid)
{
	int race = bsget(mid, Type);
	bool coastal = true;
	switch(race)
	{
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
	if(coastal)
	{
		switch(race)
		{
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
	for(int* pi = priority(race); *pi; pi++)
	{
		int building = *pi;
		if(!building)
			break;
		int level = bsget(mid, building);
		if(!level)
			continue;
		res::tokens icn = getrs(race, building, level);
		if(icn == res::Empthy)
			continue;
		int index = 0;
		if(building == MageGuild)
		{
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
		if(hot::mouse.in(getrect(race, building)))
		{
			if(building >= Dwelving1 && building <= Dwelving6)
				draw::status(szt("Recruit %1", "Нанять %1"),
					bsgets(game::getunit(race, building, bsget(mid, building)), NameMulti));
			else
				draw::status(game::getbuildingname(race, building, level));
			if(hot::key == MouseLeft && hot::pressed)
				draw::execute(building);
			else if(hot::key == MouseRight && hot::pressed)
			{
				hot::level = bsget(mid, building);
				draw::execute(Information, building, race);
			}
		}
		index = indexes::animate(icn, index, draw::counter, false);
		if(index)
			draw::image(x, y, icn, index);
	}
}

static void paint_panel(int x, int y, int mid, int hero)
{
	draw::image(x, y, res::STRIP, 0);
	draw::image(x + 5, y + 6, res::CREST, 3);
	draw::troops(x + 112, y + 6, mid);
	if(hero)
	{
		draw::clipart(x + 5 + 50, y + 105, hero, LargeSize);
		draw::troops(x + 112, y + 105, hero);
	}
	else
	{
		if(bsget(mid, Captain))
			draw::clipart(x + 5 + 50, y + 105, bsget(mid, Type) - Barbarian + BarbarianCaptain, bsget(mid, Player));
		else
			draw::image(x + 5, y + 105, res::STRIP, 3);
		draw::image(x + 112, y + 105, res::STRIP, 11);
	}
}

static void name()
{
	draw::state push;
	draw::font = res::SMALFONT;
	draw::image(0, 0, res::TOWNNAME, 0);
	draw::text(231 + 179 / 2, 248 + 2, "Вертиго", 0);
}

void show::castle(int rec)
{
	int unit;
	int army_index = -1;
	int player = bsget(rec, Player);
	while(true)
	{
		auto index = bsget(rec, Index);
		auto hero = bsfind(FirstHero, Index, index);
		draw::status(21, draw::height - 16, 21 + res::width(res::SMALLBAR, 0), draw::height - 1);
		draw::button(0, draw::height - 19, res::SMALLBAR, KeyLeft, 1, 1, 2, 0, 0, szt("Previous town", ""));
		draw::image(21, draw::height - 19, res::SMALLBAR, 0);
		draw::button(draw::width - 21, draw::height - 19, res::SMALLBAR, KeyRight, 3, 3, 4, 0, 0, szt("Next town", ""));
		panorama(0, 0, rec);
		paint_panel(0, 256, rec, hero);
		draw::resource(552, 262, bsptr(player, FirstResource));
		draw::button(553, 428, res::SWAPBTN, Cancel, 0, 0, 1, KeyEscape, 0, szt("Leave town", ""));
		name();
		draw::cursor(res::ADVMCO, 0);
		int id = draw::input();
		switch(id)
		{
		case Dwelving1:
		case Dwelving2:
		case Dwelving3:
		case Dwelving4:
		case Dwelving5:
		case Dwelving6:
			unit = game::getunit(bsget(rec, Type), id, bsget(rec, id));
			if(unit)
			{
				auto count = 0;
				auto max_count = bsget(rec, FirstCreatureCount + (id - Dwelving1));
				auto available_resources = bsptr(player, FirstResource);
				auto monster_resources = bsptr(unit, FirstResource);
				if(show::recruit(unit, count, max_count, available_resources))
				{
					if(!game::addunit(rec, unit, count))
						dlgerr(0, szt("Your garrison is out of space.", "В вашем гарнизоне не хватает места."));
					else
					{
						int total[LastResource - FirstResource + 1];
						game::mulresource(total, monster_resources, count);
						game::addresources(available_resources, available_resources, total, true);
					}
				}
			}
			break;
		case Castle:
			show::build(rec);
			break;
		case MarketPlace:
			show::marketplace(player);
			break;
		case Cancel:
			return;
		default:
			draw::definput(id);
			break;
		}
	}
}