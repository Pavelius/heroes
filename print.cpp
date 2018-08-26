#include "main.h"

strparams	game::params;

strparams::strparams() : strparams(game::params) {}

strparams::~strparams() {
	game::params = *this;
}

static void get_player_text(char* result, int id) {
	int value = game::get(game::params.hero, id);
	if(!value)
		return;
	const char* p = bsgets(value, Name);
	if(p && p[0]) {
		zcpy(result, p);
		szlower(result);
	}
}

static void get_spell_num(char* result, int id) {
	int value = game::get(game::params.hero, id);
	if(!value)
		return;
	sznum(result, value);
}

static void gender(char* result, int player, const char* m, const char* f) {
	if(game::isfemale(game::params.hero)) {
		if(f)
			zcpy(result, f);
	} else {
		if(m)
			zcpy(result, m);
	}
}

static void get_weight(char* result, int param) {}

static void get_hero(char* result, int param) {
	auto p = bsgets(game::params.hero, Name);
	if(p)
		zcpy(result, p);
}

static void get_hero_a(char* result, int param) {
	get_hero(result, param);
	//grammar::noun::of(temp, result);
	//zcpy(result, temp);
}

static void get_a(char* result, int param) {
	gender(result, game::params.hero, 0, "а");
}

static void get_A(char* result, int param) {
	gender(result, game::params.hero, 0, "а");
}

static void get_la(char* result, int param) {
	gender(result, game::params.hero, 0, "ла");
}

static printfn::element objects[] = {{"class", get_player_text, Type},
{"hero", get_hero},
{"spell_value", get_spell_num, Value},
{"а", get_a},
{"А", get_A},
{"ла", get_la},
};
static printfn objects_plugin(objects, sizeof(objects));