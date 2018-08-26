#include "main.h"

struct resdata {
	unsigned		offset;
	void*			data;
	int				size;
};
static resdata		data[res::Empthy];

res::tokens res::map(int object) {
	switch(object) {
		// reserverd
	case 0:
		return Empthy;
		// manual
	case 0x11:
		return MANA;
	case 0x12:
		//	return TELEPORT2;
	case 0x13:
		//	return TELEPORT3;
	case 0x14:
		//	return FOUNTAIN;
	case 0x15: return TREASURY;
	case 0x2C: case 0x2D: case 0x2E: case 0x2F: return OBJNARTI; // artifact
	case 0x30: case 0x31: case 0x32: case 0x33: return MONS32; // monster
	case 0x38: case 0x39: case 0x3A: case 0x3B: return FLAG32; // castle flags
	case 0x54: case 0x55: case 0x56: case 0x57: return MINIHERO; // heroes
	case 0x58: case 0x59: case 0x5A: case 0x5B: return MTNSNOW; // relief: snow
	case 0x5C: case 0x5D: case 0x5E: case 0x5F: return MTNSWMP; // relief: swamp
	case 0x60: case 0x61: case 0x62: case 0x63: return MTNLAVA; // relief: lava
	case 0x64: case 0x65: case 0x66: case 0x67: return MTNDSRT; // relief: desert
	case 0x68: case 0x69: case 0x6A: case 0x6B: return MTNDIRT; // relief: dirt
	case 0x6C: case 0x6D: case 0x6E: case 0x6F: return MTNMULT; // relief: others
	case 0x74: return EXTRAOVR; // mines
	case 0x78: case 0x79: case 0x7A: case 0x7B: return ROAD; // road
	case 0x7C: case 0x7D: case 0x7E: case 0x7F: return MTNCRCK; // relief: crck
	case 0x80: case 0x81: case 0x82: case 0x83: return MTNGRAS; // relief: gras
	case 0x84: case 0x85: case 0x86: case 0x87: return TREJNGL; // trees jungle
	case 0x88: case 0x89: case 0x8A: case 0x8B: return TREEVIL; // trees evil
	case 0x8C: case 0x8D: case 0x8E: case 0x8F: return OBJNTOWN; // castle and tower
	case 0x90: case 0x91: case 0x92: case 0x93: return OBJNTWBA; // castle lands
	case 0x94: case 0x95: case 0x96: case 0x97: return OBJNTWSH; // castle shadow
	case 0x98: case 0x99: case 0x9A: case 0x9B: return OBJNTWRD; // random castle
	case 0xA0: case 0xA1: case 0xA2: case 0xA3: return OBJNWAT2; // water object
	case 0xA4: case 0xA5: case 0xA6: case 0xA7: return OBJNMUL2; // object other
	case 0xA8: case 0xA9: case 0xAA: case 0xAB: return TRESNOW; // trees snow
	case 0xAC: case 0xAD: case 0xAE: case 0xAF: return TREFIR; // trees trefir
	case 0xB0: case 0xB1: case 0xB2: case 0xB3: return TREFALL; // trees
	case 0xB4: case 0xB5: case 0xB6: case 0xB7: return STREAM; // river
	case 0xB8: case 0xB9: case 0xBA: case 0xBB: return OBJNRSRC; // resource
	case 0xC0: case 0xC1: case 0xC2: case 0xC3: return OBJNGRA2; // gras object
	case 0xC4: case 0xC5: case 0xC6: case 0xC7: return TREDECI; // trees tredeci
	case 0xC8: case 0xC9: case 0xCA: case 0xCB: return OBJNWATR; // sea object
		// vegetation gras
	case 0xCC:
	case 0xCD:
	case 0xCE:
	case 0xCF:
		return OBJNGRAS;
		// object on snow
	case 0xD0:
	case 0xD1:
	case 0xD2:
	case 0xD3:
		return OBJNSNOW;
		// object on swamp
	case 0xD4:
	case 0xD5:
	case 0xD6:
	case 0xD7:
		return OBJNSWMP;
		// object on lava
	case 0xD8:
	case 0xD9:
	case 0xDA:
	case 0xDB:
		return OBJNLAVA;
		// object on desert
	case 0xDC:
	case 0xDD:
	case 0xDE:
	case 0xDF:
		return OBJNDSRT;
		// object on dirt
	case 0xE0:
	case 0xE1:
	case 0xE2:
	case 0xE3:
		return OBJNDIRT;
		// object on crck
	case 0xE4:
	case 0xE5:
	case 0xE6:
	case 0xE7:
		return OBJNCRCK;
		// object on lava
	case 0xE8:
	case 0xE9:
	case 0xEA:
	case 0xEB:
		return OBJNLAV3;
		// object on earth
	case 0xEC:
	case 0xED:
	case 0xEE:
	case 0xEF:
		return OBJNMULT;
		//  object on lava
	case 0xF0:
	case 0xF1:
	case 0xF2:
	case 0xF3:
		return OBJNLAV2;
	default:
		return Empthy;
	}
}

res::tokens res::buildings(int race) {
	switch(race) {
	case Barbarian: return CSTLBARB;
	case Knight: return CSTLKNGT;
	case Necromancer: return CSTLNECR;
	case Sorcerer: return CSTLSORC;
	case Warlock: return CSTLWRLK;
	case Wizard: return CSTLWZRD;
	default: return Empthy;
	}
}

void* res::get(tokens icn) {
	if((int)icn >= res::Empthy)
		return 0;
	if(!data[icn].data) {
		char temp[260];
		if(data[icn].size)
			return 0;
		io::file file(szurl(temp, "data", "heroes2", "agg"), StreamRead);
		if(!file)
			return 0;
		if(!data[icn].offset) {
			// header
			// 0 - record count [word]
			// records ...
			// 0 - crc
			// 4 - offset
			// 8 - size
			file.seek(2 + (4 * 3 * icn) + 4, SeekSet);
			file.read(&data[icn].offset, sizeof(data[icn].offset));
			file.read(&data[icn].size, sizeof(data[icn].size));
		}
		if(!data[icn].size)
			return 0;
		file.seek(data[icn].offset, SeekSet);
		data[icn].data = new char[data[icn].size];
		file.read(data[icn].data, data[icn].size);
	}
	return data[icn].data;
}

bool res::ishight(res::tokens icn, int index) {
	switch(icn) {
	case OBJNDIRT:
		// wind mill
		if(154 <= index && index <= 160)
			return true;
		break;
	case OBJNGRA2:
		// wind mill
		if(24 <= index && index <= 30)
			return true;
		break;
	case OBJNLAV2:
	case OBJNLAV3:
		// fog lava
		return true;
	case OBJNMUL2:
		// dragon city
		if(35 == index || 37 == index || 38 == index || 40 == index || 41 == index)
			return true;
		// ligth
		if(59 == index)
			return true;
		// water mill
		if(82 == index)
			return true;
		break;
	case OBJNMULT:
		// fort
		if(36 <= index && index <= 44)
			return true;
		// tree
		if(117 == index || 118 == index)
			return true;
		break;
	case OBJNSNOW:
		// wind mill
		if(97 <= index && index <= 103)
			return true;
		// water mill
		if(161 == index)
			return true;
		break;
	case OBJNSWMP:
		if(35 <= index && index <= 42)
			return true;
		break;
	case OBJNTOWN:
		if(1 <= index && index <= 5)
			return true;
		if(32 <= index && index <= 37)
			return true;
		if(64 <= index && index <= 69)
			return true;
		if(96 <= index && index <= 101)
			return true;
		if(128 <= index && index <= 133)
			return true;
		if(160 <= index && index <= 165)
			return true;
		break;
	case FLAG32:
		return true;
	default:
		break;
	}
	return false;
}

res::tokens res::getshooting(int rec) {
	if(rec >= FirstCombatant && rec <= LastCombatant)
		rec = bsget(rec, Type);
	switch(rec) {
	case Orc:
	case OrcChief:
		return res::ORC__MSL;
	case Druid:
	case GreaterDruid:
		return res::DRUIDMSL;
	case Halfling:
		return res::HALFLMSL;
	case Titan:
		return res::TITANMSL;
	case Troll:
	case WarTroll:
		return res::TROLLMSL;
	case Lich:
	case PowerLich:
		return res::LICH_MSL;
	case Mage:
	case ArchMage:
	case Centaur:
	case Archer:
	case Ranger:
		return res::ARCH_MSL;
	case Elf:
	case GrandElf:
		return res::ELF__MSL;
	default:
		return res::Empthy;
	}
}

res::tokens res::building(int race, int building, int level) {
	if(race == Barbarian) {
		switch(building) {
		case Castle:
			if(level == 2)
				return res::TWNBCSTL;
			else if(level == 1)
				return res::TWNBTENT;
			return res::Empthy;
		case SpecialBuilding: return res::TWNBSPEC;
		case Captain: return res::TWNBCAPT;
		case Well2: return res::TWNBWEL2;
		case LeftTurret: return res::TWNBLTUR;
		case RightTurret: return res::TWNBRTUR;
		case Moat: return res::TWNBMOAT;
		case MarketPlace: return res::TWNBMARK;
		case ThievesGuild: return res::TWNBTHIE;
		case Tavern: return res::TWNBTVRN;
		case Well: return res::TWNBWELL;
		case Statue: return res::TWNBSTAT;
		case Shipyard: return res::TWNBDOCK;
		case MageGuild: return res::TWNBMAGE;
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
	} else if(race == Knight) {
		switch(building) {
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
			if(level == 2)
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
	} else if(race == Necromancer) {
		switch(building) {
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
			if(level == 2)
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
	} else if(race == Sorcerer) {
		switch(building) {
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
			if(level == 2)
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
	} else if(race == Warlock) {
		switch(building) {
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
	} else if(race == Wizard) {
		switch(building) {
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