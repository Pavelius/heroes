#include "main.h"

static struct combat_effect
{
	int		type;
	int		parent;
	int		expire;
} objects[LastEffect - FirstEffect + 1];
static bsmeta::field fields[] = {
	BSREQ(combat_effect, type, Type, Number),
	BSREQ(combat_effect, parent, Parent, Number),
	BSREQ(combat_effect, expire, Value, Number),
	{0}
};
BSMETA(combat_effect, "Effects", "Ёфекты", FirstEffect);