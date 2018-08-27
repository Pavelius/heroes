#include "main.h"

static bool ask(int player, int hero, const char* format, ...) {
	if(bsget(player, PlayerType) != Human)
		return (d100() < 50);
	return show::ask(format, xva_start(format));
}

static void message(int player, int hero, const char* format, ...) {
	if(bsget(player, PlayerType) != Human)
		return;
	return show::message(format, xva_start(format));
}

static const char* get_mine_of(tokens id) {
	switch(id) {
	case AlchemyLab: return "������������ �����������";
	case SawMill: return "���������";
	case MineOre: return "������ �����";
	case MineSulfur: return "������ �����";
	case MineCrystal: return "����������� �����";
	case MineGems: return "����� ���������� ������";
	case MineGold: return "������� �����";
	default: return "";
	}
}

static const char* get_resource_of_pluar(tokens id) {
	switch(id) {
	case Wood: return "������";
	case Ore: return "����";
	case Mercury: return "�����";
	case Sulfur: return "����";
	case Crystal: return "���������";
	case Gems: return "����������� �����";
	case Gold: return "�������";
	default: return "";
	}
}

static const char* get_resource_of_single(tokens id) {
	switch(id) {
	case Wood: return "������";
	case Ore: return "����";
	case Mercury: return "�����";
	case Sulfur: return "����";
	case Crystal: return "��������";
	case Gems: return "����������� ������";
	case Gold: return "�������";
	default: return "";
	}
}

static const char* get_resource_of(tokens id, int count) {
	if(count == 1)
		return get_resource_of_single(id);
	else
		return get_resource_of_pluar(id);
}

void game::interact(int index, int object, int hero, int player) {
	bool move_to_object = true;
	bool disapear = false;
	bool isinteractive = (bsget(player, PlayerType) == Human);
	bool islog = false;
	auto side = (tokens)bsget(hero, Player);
	auto type = (tokens)bsget(object, Type);
	auto count = bsget(object, Count);
	switch(type) {
	case Knight:
	case Barbarian:
	case Necromancer:
	case Warlock:
	case Wizard:
	case Sorcerer:
		// When visited castle learn all spells
		if(game::hasspellbook(hero)) {
			auto n = bsget(object, MageGuild);
			if(n) {
				for(int i = FirstSpell; i <= LastSpell; i++) {
					if(!bsget(object, i))
						continue;
					if(bsget(i, Level) > n)
						continue;
					bsset(hero, i, 1);
				}
			}
		}
		break;
	case Gold:
	case Ore:
	case Wood:
	case Sulfur:
	case Crystal:
	case Gems:
	case Mercury:
		bsadd(player, type, count);
		disapear = true;
		move_to_object = false;
		if(isinteractive)
			show::adventure::message("%1 %2\n$(%3i/%4i)",
				szt("You found", "�� �����"),
				bsgets(type, Name),
				type, count);
		break;
	case TreasureChest:
		disapear = true;
		move_to_object = false;
		// If there is no rooms for artifatcs
		if(isfullartifacts(hero) && count >= 4)
			count = count % 4;
		if(count < 4) {
			auto gp = 1000 + count * 500;
			auto ep = gp - 500;
			if(ep > 1500)
				ep = 1500;
			auto result = ask(player, hero,
				"%1\n$(p%2i/%3i,%4i/%5i)",
				szt("After scouring the area, you fall upon a hidden treasure cache. You may take the gold or distribute the gold to the peasants for experience. Do you wish to keep the gold?", "�������� ���������� �� ���������� �� ������� �����. ������ ����� �������� ���� ��� ������ ���������� � ����� �� ����. �������� ������ ����?"),
				Gold, gp,
				Experience, ep
			);
			if(result)
				bsadd(side, Gold, gp);
			else
				bsadd(hero, Experience, ep);
		} else {
			message(player, hero,
				"%1 %3.\n$(%2i)",
				szt("Searching area you found a old treasure chest. Inside you found ancient artifact:", "�������� ���������� �� ���������� �� ������� �����. ������ �� ���������� ��������:"),
				count,
				bsgets(count, Name)
			);
			game::additem(hero, count);
		}
		break;
	case CampFire:
		disapear = true;
		move_to_object = false;
		if(true) {
			auto gp = (1 + (count % 4)) * 250;
			auto rs = 2 + ((count >> 2) % 4);
			auto rt = Mercury + (count >> 4) % 4;
			message(player, hero,
				"%1\n$(%2i/%3i,%4i/%5i)",
				szt("Searching enemy camp you found hidden treasure.", "������� ��������� ������ �� ���������� ������� ����."),
				Gold, gp,
				rt, rs
			);
			bsadd(side, Gold, gp);
			bsadd(side, rt, rs);
		}
		break;
	case MineCrystal:
	case MineGems:
	case MineGold:
	case MineOre:
	case MineSulfur:
	case SawMill:
	case AlchemyLab:
		if(true) {
			auto rcount = game::getmineincome(type);
			auto rid = game::getresource(type);
			message(player, hero,
				szt("You become a owner of %1. You gain %2i %3 per day.\n$(d%4i/%2i)", "�� ����� �������� %1. �� ������ �������� %2i %3 � ����.\n$(d%4i/%2i)"),
				get_mine_of(type),
				rcount,
				get_resource_of(rid, rcount),
				rid
			);
			bsset(object, Count, side);
		}
		break;
	}
	if(disapear) {
		if(isinteractive)
			show::adventure::disapear(player, object);
		else
			bsdelete(object);
	}
	if(move_to_object)
		bsset(hero, Index, index);
}