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
	int result_hero = 0;
	int result_sternght = 0;
	if(rec >= FirstPlayer && rec <= LastPlayer)
	{
		for(int i = FirstHero; i <= LastHero; i++)
		{
			if(bsget(i, Player) != rec)
				continue;
			int strenght = getstrenght(i);
			if(strenght > result_sternght)
			{
				result_sternght = strenght;
				result_hero = i;
			}
		}
	}
	return result_hero;
}

int game::getbestmonster(int rec)
{
	int result_rec = 0;
	int result_val = 0;
	if((rec >= FirstHero && rec <= LastHero)
		|| (rec >= FirstCastle && rec <= LastCastle))
	{
		for(int i = FirstTroopsIndex; i <= LastTroopsIndex; i += 2)
		{
			int m = bsget(rec, i);
			if(!m)
				continue;
			int c = bsget(rec, i + 1);
			if(!c)
				continue;
			int val = getstrenght(m);
			if(val > result_val)
			{
				result_val = val;
				result_rec = m;
			}
		}
	}
	else if(rec >= FirstPlayer && rec <= LastPlayer)
	{
		for(int i = FirstHero; i <= LastHero; i++)
		{
			if(bsget(i, Player) != rec)
				continue;
			int m = getbestmonster(i);
			int val = getstrenght(m);
			if(val > result_val)
			{
				result_val = val;
				result_rec = m;
			}
		}
	}
	return result_rec;
}

int game::getstrenght(int rec)
{
	if(rec >= FirstMonster && rec <= LastMonster)
		return bsget(rec, Rating);
	else if((rec >= FirstHero && rec <= LastHero)
		|| (rec >= FirstCastle && rec <= LastCastle))
	{
		int result = 0;
		for(int i = FirstTroopsIndex; i <= LastTroopsIndex; i += 2)
		{
			int m = bsget(rec, i);
			if(!m)
				continue;
			int c = bsget(rec, i + 1);
			result += getstrenght(m) * c;
		}
		return result;
	}
	return 0;
}

int game::getbuildings(int player, int building)
{
	int result = 0;
	for(int i = FirstCastle; i <= LastCastle; i++)
	{
		if(bsget(i, Player) != player)
			continue;
		result += bsget(i, building);
	}
	return result;
}