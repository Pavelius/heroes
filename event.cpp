#include "main.h"

static struct event {
	short unsigned	index;
	const char*		text;
	unsigned char	computer;
	unsigned char	onetime;
	unsigned char	disabled;
	unsigned char	players[LastPlayer - FirstPlayer + 1];
	int				cost[LastResource - FirstResource + 1];
} objects[LastEvent - FirstEvent + 1];