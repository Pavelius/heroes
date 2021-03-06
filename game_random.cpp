#include "main.h"

static unsigned char artifacts[LastArtifact - FirstArtifact + 1];
static unsigned char monsters[LastMonster - FirstMonster + 1];

COMMAND(game_initialize) {
	memset(artifacts, 0, sizeof(artifacts));
	memset(monsters, 0, sizeof(monsters));
}

static int select_unique(unsigned char* previous, const int* source, int first, int iteration) {
	for(auto p = source; *p; p++) {
		if(previous[*p - first] == iteration)
			return *p;
	}
	return 0;
}

static int select_unique(unsigned char* previous, const int* source, int first) {
	for(auto i = 0; i < 10; i++) {
		int result = select_unique(previous, source, first, i);
		if(result) {
			previous[result - first]++;
			return result;
		}
	}
	return 0;
}

static bool is_recruit(int rec) {
	for(int i = FirstPlayer; i <= LastPlayer; i++) {
		if(!bsget(i, Type))
			continue;
		if(bsget(i, Recruit) == rec)
			return true;
		if(bsget(i, RecruitLast) == rec)
			return true;
	}
	return false;
}

int game::random::artifact(int level) {
	int source[LastArtifact - FirstArtifact + 2];
	if(!level)
		level = xrand(1, 3);
	bsselect(source, FirstArtifact, LastArtifact, Level, level);
	zshuffle(source, zlen(source));
	return select_unique(artifacts, source, FirstArtifact);
}

int game::random::monster(int level) {
	int source[LastMonster - FirstMonster + 2];
	if(!level)
		level = xrand(1, 3);
	bsselect(source, FirstMonster, LastMonster, Level, level);
	zshuffle(source, zlen(source));
	return select_unique(monsters, source, FirstMonster);
}

int game::random::hero(int type) {
	int source[LastHero - FirstHero + 2];
	int* p = source;
	for(int i = FirstHero; i <= Celia; i++) {
		if(type && game::get(i, Type) != type)
			continue;
		if(bsget(i, Player))
			continue;
		if(is_recruit(i))
			continue;
		*p++ = i;
	}
	*p = 0;
	int count = zlen(source);
	if(!count)
		return 0;
	return source[rand() % count];
}

void game::random::spell(int castle, int level) {
	int source[LastSpell - FirstSpell + 2];
	bsselect(source, FirstSpell, -1, Level, level);
	zshuffle(source, zlen(source));
	for(int i = 0; source[i]; i++) {
		if(bsget(castle, source[i]))
			continue;
		bsset(castle, source[i], 1);
		return;
	}
}

void game::random::spells(int rec) {
	spell(rec, 1);
	spell(rec, 1);
	spell(rec, 1);
	spell(rec, 2);
	spell(rec, 2);
	spell(rec, 2);
	spell(rec, 3);
	spell(rec, 3);
	spell(rec, 4);
	spell(rec, 4);
	spell(rec, 5);
}

int game::random::castle(int type, int player, int index, bool has_castle) {
	int rec = bscreate(FirstCastle);
	bsset(rec, Type, type);
	bsset(rec, Player, player);
	bsset(rec, Index, index);
	bsset(rec, Dwelving1, 1);
	if(has_castle)
		bsset(rec, Castle, 2);
	else
		bsset(rec, Castle, 1);
	spells(rec);
	return rec;
}