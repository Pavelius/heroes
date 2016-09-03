#include "main.h"

static unsigned char artifacts[LastArtifact - FirstArtifact + 1];
static unsigned char monsters[LastMonster - FirstMonster + 1];

void game::random::initialize()
{
	memset(artifacts, 0, sizeof(artifacts));
	memset(monsters, 0, sizeof(monsters));
}

static int select_unique(unsigned char* previous, const int* source, int first, int iteration)
{
	for(auto p = source; *p; p++)
	{
		if(previous[*p - first] == iteration)
			return *p;
	}
	return 0;
}

static int select_unique(unsigned char* previous, const int* source, int first)
{
	for(auto i = 0; i<10; i++)
	{
		int result = select_unique(previous, source, first, i);
		if(result)
		{
			previous[result - first]++;
			return result;
		}
	}
	return 0;
}

int game::random::artifact(int level)
{
	int source[LastArtifact - FirstArtifact + 2];
	if(!level)
		level = xrand(1, 3);
	bsselect(source, FirstArtifact, LastArtifact, Level, level);
	zshuffle(source, zlen(source));
	return select_unique(artifacts, source, FirstArtifact);
}

int game::random::monster(int level)
{
	int source[LastMonster - FirstMonster + 2];
	if(!level)
		level = xrand(1, 3);
	bsselect(source, FirstMonster, LastMonster, Level, level);
	zshuffle(source, zlen(source));
	return select_unique(monsters, source, FirstMonster);
}

int game::random::hero(int type)
{
	int source[LastHero - FirstHero + 2];
	bsselect(source, FirstHero, LastHero);
	int count = zlen(source);
	if(!count)
		return 0;
	return source[rand()%count];
}