#include "command.h"

command* command::first;

command::command(const char* name, void(*proc)()) : name(name), proc(proc), next(0)
{
	if(!first)
		first = this;
	else
	{
		auto p = first;
		while(p->next)
			p = p->next;
		p->next = this;
	}
}

static bool equal(const char* n1, const char* n2)
{
	int i = 0;
	while(n2[i])
	{
		if(n1[i] != n2[i])
			return false;
		i++;
	}
	return n1[i]==n2[i];
}

void command::execute(const char* name)
{
	for(auto p = command::first; p; p = p->next)
	{
		if(!equal(p->name, name))
			continue;
		p->proc();
	}
}