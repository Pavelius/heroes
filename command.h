#pragma once

struct command
{
	const char*			name;
	command*			next;
	void				(*proc)();
	static command*		first;
	command(const char* name, void(*proc)());
	static void			execute(const char* name);
};
