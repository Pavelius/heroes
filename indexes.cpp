#include "main.h"

int indexes::buildings(int building, int level)
{
	switch(building)
	{
	case Dwelving1:	return 19;
	case Dwelving2:
		if(level==2)
			return 25;
		return 20;
	case Dwelving3:
		if(level == 2)
			return 26;
		return 21;
	case Dwelving4:
		if(level == 2)
			return 27;
		return 22;
	case Dwelving5:
		if(level == 2)
			return 28;
		return 23;
	case Dwelving6:
		if(level == 3)
			return 30;
		if(level == 2)
			return 29;
		return 24;
	case MageGuild: return 0;
	case ThievesGuild:return 1;
	case Tavern: return 2;
	case Shipyard: return 3;
	case Well: return 4;
	case CastleInTown: return 6;
	case Statue: return 7;
	case LeftTurret: return 8;
	case RightTurret: return 9;
	case MarketPlace: return 10;
	case Well2:	return 11;
	case Moat: return 12;
	case SpecialBuilding: return 13;
	case Captain: return 15;
	default: return 0;
	}
}

int indexes::animate(res::tokens res, int start, int ticket, bool quantity)
{
	switch(res)
	{
	case res::TREASURY:	return start + ticket % 2;

	case res::TWNBBOAT:
	case res::TWNKBOAT:
	case res::TWNNBOAT:
	case res::TWNSBOAT:
	case res::TWNWBOAT:
	case res::TWNZBOAT:	return 1 + ticket % 9;

	case res::CMBTCAPB:
	case res::CMBTCAPK:
	case res::CMBTCAPN:
	case res::CMBTCAPS:
	case res::CMBTCAPW:
	case res::CMBTCAPZ:	return 1 + ticket % 10;

	case res::CMBTHROB:	return 1 + ticket % 18;
	case res::CMBTHROK:	return 1 + ticket % 19;
	case res::CMBTHRON:	return 1 + ticket % 19;
	case res::CMBTHROS:	return 1 + ticket % 16;
	case res::CMBTHROW:	return 1 + ticket % 16;
	case res::CMBTHROZ:	return 1 + ticket % 18;

	case res::HEROFL00:
	case res::HEROFL01:
	case res::HEROFL02:
	case res::HEROFL03:
	case res::HEROFL04:
	case res::HEROFL05:	return ticket % 5;

	case res::TWNBDOCK:
	case res::TWNKDOCK:
	case res::TWNNDOCK:
	case res::TWNSDOCK:
	case res::TWNWDOCK:
	case res::TWNZDOCK:

	case res::TWNBEXT0:
	case res::TWNKEXT0:
	case res::TWNNEXT0:
	case res::TWNSEXT0:
	case res::TWNWEXT0:
	case res::TWNZEXT0:

	case res::TWNBCAPT:
	case res::TWNBDW_3:
	case res::TWNBDW_4:
	case res::TWNBDW_5:
	case res::TWNBEXT1:
	case res::TWNBMOAT:
	case res::TWNBUP_3:
	case res::TWNBUP_4:
	case res::TWNKCSTL:
	case res::TWNKDW_0:
	case res::TWNKLTUR:
	case res::TWNKRTUR:
	case res::TWNKTHIE:
	case res::TWNKTVRN:
	case res::TWNNCSTL:
	case res::TWNNDW_2:
	case res::TWNNUP_2:
	case res::TWNSCAPT:
	case res::TWNSCSTL:
	case res::TWNSDW_0:
	case res::TWNSDW_1:
	case res::TWNSEXT1:
	case res::TWNSTHIE:
	case res::TWNSTVRN:
	case res::TWNSUP_1:
	case res::TWNSWEL2:
	case res::TWNWCAPT:
	case res::TWNWCSTL:
	case res::TWNWMOAT:
	case res::TWNZCSTL:
	case res::TWNZDW_0:
	case res::TWNZDW_2:
	case res::TWNZTHIE:
	case res::TWNZUP_2:	return 1 + ticket % 5;

	case res::TWNBCSTL:
	case res::TWNKDW_2:
	case res::TWNKUP_2:
	case res::TWNNDW_5:
	case res::TWNNWEL2:
	case res::TWNWDW_0:
	case res::TWNWWEL2:
	case res::TWNZTVRN:	return 1 + ticket % 6;

	case res::TWNKDW_4:
	case res::TWNKUP_4:	return 1 + ticket % 7;

	case res::TAVWIN:	return 2 + ticket % 20;

	case res::CMBTLOS1:	return 1 + ticket % 30;
	case res::CMBTLOS2:	return 1 + ticket % 29;
	case res::CMBTLOS3:	return 1 + ticket % 22;
	case res::CMBTFLE1:	return 1 + ticket % 43;
	case res::CMBTFLE2:	return 1 + ticket % 26;
	case res::CMBTFLE3:	return 1 + ticket % 25;
	case res::CMBTSURR:	return 1 + ticket % 20;

	case res::WINCMBT:	return 1 + ticket % 20;

	case res::MINIMON:	return start + 1 + ticket % 6;

	case res::TWNNMAGE:	return start + 1 + ticket % 5;

	case res::TWNBMAGE:	return 4 == start ? start + 1 + ticket % 8 : 0;

	case res::SHNGANIM:	return 1 + ticket % 39;

	case res::BTNSHNGL:	return start + ticket % 4;

	case res::OBJNHAUN:	return ticket % 15;

	case res::OBJNWATR:

		switch(start)
		{
			// buttle
		case 0x00:
			return start + (ticket % 11) + 1;

			// shadow
		case 0x0C:
			// chest
		case 0x13:
			// shadow
		case 0x26:
			// flotsam
		case 0x2D:
			// unkn
		case 0x37:
			// boat
		case 0x3E:
			// waves
		case 0x45:
			// seaweed
		case 0x4C:
		case 0x53:
		case 0x5A:
		case 0x61:
		case 0x68:
			// sailor-man
		case 0x6F:
			// shadow
		case 0xBC:
			// buoy
		case 0xC3:
			// broken ship (right)
		case 0xE2:
		case 0xE9:
		case 0xF1:
		case 0xF8:
			return start + (ticket % 6) + 1;

			// seagull on stones
		case 0x76:
		case 0x86:
		case 0x96:
		case 0xA6:
			return start + (ticket % 15) + 1;

			// whirlpool
		case 0xCA:
		case 0xCE:
		case 0xD2:
		case 0xD6:
		case 0xDA:
		case 0xDE:
			return start + (ticket % 3) + 1;

		default:
			return 0;
		}
		break;

	case res::OBJNWAT2:

		switch(start)
		{
			// sail broken ship (left)
		case 0x03:
		case 0x0C:
			return start + (ticket % 6) + 1;

		default:
			return 0;
		}
		break;

	case res::OBJNCRCK:

		switch(start)
		{
			// pool of oil
		case 0x50:
		case 0x5B:
		case 0x66:
		case 0x71:
		case 0x7C:
		case 0x89:
		case 0x94:
		case 0x9F:
		case 0xAA:
			// smoke from chimney
		case 0xBE:
			// shadow smoke
		case 0xCA:
			return start + (ticket % 10) + 1;

		default:
			return 0;
		}
		break;

	case res::OBJNDIRT:

		switch(start)
		{
			// mill
		case 0x99:
		case 0x9D:
		case 0xA1:
		case 0xA5:
		case 0xA9:
		case 0xAD:
		case 0xB1:
		case 0xB5:
		case 0xB9:
		case 0xBD:
			return start + (ticket % 3) + 1;

		default:
			return 0;
		}
		break;

	case res::OBJNDSRT:

		switch(start)
		{
			// campfire
		case 0x36:
		case 0x3D:
			return start + (ticket % 6) + 1;

		default:
			return 0;
		}
		break;

	case res::OBJNGRA2:

		switch(start)
		{
			// mill
		case 0x17:
		case 0x1B:
		case 0x1F:
		case 0x23:
		case 0x27:
		case 0x2B:
		case 0x2F:
		case 0x33:
		case 0x37:
		case 0x3B:
			return start + (ticket % 3) + 1;

			// smoke from chimney
		case 0x3F:
		case 0x46:
		case 0x4D:
			// archerhouse
		case 0x54:
			// smoke from chimney
		case 0x5D:
		case 0x64:
			// shadow smoke
		case 0x6B:
			// peasanthunt
		case 0x72:
			return start + (ticket % 6) + 1;

		default:
			return 0;
		}
		break;

	case res::OBJNLAV2:

		switch(start)
		{
			// middle volcano
		case 0x00:
			// shadow
		case 0x07:
		case 0x0E:
			// lava
		case 0x15:
			return start + (ticket % 6) + 1;

			// small volcano
			// shadow
		case 0x21:
		case 0x2C:
			// lava
		case 0x37:
		case 0x43:
			return start + (ticket % 10) + 1;

		default:
			return 0;
		}
		break;

	case res::OBJNLAV3:

		// big volcano
		switch(start)
		{
			// smoke
		case 0x00:
		case 0x0F:
		case 0x1E:
		case 0x2D:
		case 0x3C:
		case 0x4B:
		case 0x5A:
		case 0x69:
		case 0x87:
		case 0x96:
		case 0xA5:
			// shadow
		case 0x78:
		case 0xB4:
		case 0xC3:
		case 0xD2:
		case 0xE1:
			return start + (ticket % 14) + 1;

		default:
			return 0;
		}
		break;

	case res::OBJNLAVA:

		switch(start)
		{
			// shadow of lava
		case 0x4E:
		case 0x58:
		case 0x62:
			return start + (ticket % 9) + 1;

		default:
			return 0;
		}
		break;

	case res::OBJNMUL2:

		switch(start)
		{
			// lighthouse
		case 0x3D:
			return start + (ticket % 9) + 1;

			// alchemytower
		case 0x1B:
			// watermill
		case 0x53:
		case 0x5A:
		case 0x62:
		case 0x69:
			// fire in wagoncamp
		case 0x81:
			// smoke smithy (2 chimney)
		case 0xA6:
			// smoke smithy (1 chimney)
		case 0xAD:
			// shadow smoke
		case 0xB4:
			return start + (ticket % 6) + 1;

			// magic garden
		case 0xBE:
			return quantity ? start + (ticket % 6) + 1 : start + 7;

		default:
			return 0;
		}
		break;

	case res::OBJNMULT:
		switch(start)
		{
			// smoke
		case 0x05:
			// shadow
		case 0x0F:
		case 0x19:
			return start + (ticket % 9) + 1;
			// smoke
		case 0x24:
			// shadow
		case 0x2D:
			return start + (ticket % 8) + 1;
			// smoke
		case 0x5A:
			// shadow
		case 0x61:
		case 0x68:
		case 0x7C:
			// campfire
		case 0x83:
			return start + (ticket % 6) + 1;
		default:
			return 0;
		}
	case res::OBJNSNOW:
		switch(start)
		{
			// firecamp
		case 0x04:
			// alchemytower
		case 0x97:
			// watermill
		case 0xA2:
		case 0xA9:
		case 0xB1:
		case 0xB8:
			return start + (ticket % 6) + 1;

			// mill
		case 0x60:
		case 0x64:
		case 0x68:
		case 0x6C:
		case 0x70:
		case 0x74:
		case 0x78:
		case 0x7C:
		case 0x80:
		case 0x84:
			return start + (ticket % 3) + 1;

		default:
			return 0;
		}
	case res::OBJNSWMP:
		switch(start)
		{
			// shadow
		case 0x00:
		case 0x0E:
		case 0x2B:
			// smoke
		case 0x07:
		case 0x22:
		case 0x33:
			// light in window
		case 0x16:
		case 0x3A:
		case 0x43:
		case 0x4A:
			return start + (ticket % 6) + 1;
		default:
			return 0;
		}
	default:
		return 0;
	}
}