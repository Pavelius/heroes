#include "main.h"

static bool ask(int player, int hero, const char* format, ...)
{
	if(bsget(player, PlayerType) != Human)
		return (d100() < 50);
	return show::ask(format, xva_start(format));
}

static void message(int player, int hero, const char* format, ...)
{
	if(bsget(player, PlayerType) != Human)
		return;
	return show::message(format, xva_start(format));
}

void game::interact(int index, int object, int hero, int player)
{
	bool move_to_object = true;
	bool disapear = false;
	bool isinteractive = (bsget(player, PlayerType) == Human);
	bool islog = false;
	auto side = bsget(hero, Player);
	auto type = bsget(object, Type);
	auto count = bsget(object, Count);
	if(type >= FirstResource && type <= LastResource)
	{
		bsadd(player, type, count);
		disapear = true;
		move_to_object = false;
		if(isinteractive)
			show::adventure::message("%1 %2\n$(%3i/%4i)",
				szt("You found", "�� �����"),
				bsgets(type, Name),
				type, count);
	}
	else if(type == TreasureChest)
	{
		disapear = true;
		move_to_object = false;
		// If there is no rooms for artifatcs
		if(isfullartifacts(hero) && count >= 4)
			count = count % 4;
		if(count < 4)
		{
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
		}
		else
		{
			message(player, hero,
				"%1 %3.\n$(%2i)",
				szt("Searching area you found a old treasure chest. Inside you found ancient artifact:", "�������� ���������� �� ���������� �� ������� �����. ������ �� ���������� ��������:"),
				count,
				bsgets(count, Name)
				);
			game::additem(hero, count);
		}
	}
	else if(type == CampFire)
	{
		disapear = true;
		move_to_object = false;
		auto gp = (1 + (count % 4)) * 250;
		auto rs = 2 + ((count>>2) % 4);
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
	if(disapear)
	{
		if(isinteractive)
			show::adventure::disapear(player, object);
		else
			bsdelete(object);
	}
	if(move_to_object)
		bsset(hero, Index, index);
}