#include "main.h"

int game::getcastles(int player, int level)
{
	int result = 0;
	int i2 = bsget(FirstCastle, Last);
	for(int i = FirstCastle; i <= i2; i++)
	{
		if(bsget(i, Player) != player)
			continue;
		if(level && bsget(i, Castle) != level)
			continue;
		result++;
	}
	return result;
}

int game::getbesthero(int rec)
{
	if(rec >= FirstPlayer && rec <= LastPlayer)
	{
		for(int i = FirstHero; i <= LastHero; i++)
		{
			if(bsget(i, Player) != rec)
				continue;
			return i;
		}
	}
	return 0;
}

int game::getstrenght(int rec)
{
	if(rec >= FirstMonster && rec <= LastMonster)
	{
	}
	return 0;
}