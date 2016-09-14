#include "main.h"

static struct player
{
	const char*	name[2];
	int			type;
	int			control_type;
	int			recruiters[RecruitLast - Recruit + 1];
	int			cost[LastResource - FirstResource + 1];
} objects[LastPlayer - FirstPlayer + 1] = {
	{{"Blue", "�����"}},
	{{"Green", "�������"}},
	{{"Red", "�������"}},
	{{"Yellow", "������"}},
	{{"Orange", "���������"}},
	{{"Purple", "����������"}},
};
static bsmeta::field fields[] = {
	BSREQ(player, type, Type, Number),
	BSREQ(player, control_type, PlayerType, Number),
	BSINT(player, cost, FirstResource, Number),
	BSINT(player, recruiters, Recruit, Number),
	BSREQ(player, name, Name, Text),
	{0}
};
BSMETA(player, "Players", "������", FirstPlayer);