#include "main.h"

static struct sign
{
	int			index;
	const char*	text;
} objects[LastSign-FirstSign+1];
static bsmeta::field fields[] = {
	BSREQ(sign, index, Index, Number),
	BSREQ(sign, text, Name, Text),
	{0}
};
BSMETA(sign, "Signs", "Указатели", FirstSign);