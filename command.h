#pragma once

// Use this macro and don't have any problem
#define COMMAND(name) static void name();\
static command command_##name(#name, name);\
static void name()

struct command
{
	const char*			name;
	command*			next;
	void				(*proc)();
	static command*		first;
	command(const char* name, void(*proc)());
	static void			execute(const char* name);
};