#include "main.h"

void show::highscore() {
	struct hightscore {
		char	name[32];
		char	map[32];
		int		days;
		int		score;
	};
	static hightscore hightscores[10] = {{"Павелиус", "Мир ветров", 100, 2031},
	{"Лорд Киллбурн", "Жадность", 110, 2000},
	{"Сер Галлахарт", "Земли драконов", 100, 1900},
	{"Мириус", "Земли драконов", 100, 1900},
	{"Грейтхарт", "Земли драконов", 100, 1900},
	{"Мистли", "Мир ветров", 100, 1900},
	{"Максимус", "Мир ветров", 100, 1900},
	{"Елена", "Сломанный", 100, 1900},
	{"Юлианна", "Сломанный", 100, 1900},
	{"Неуч", "Жадность", 100, 1900},
	};
	while(true) {
		draw::image(0, 0, res::HSBKG, 0, 0);
		draw::image(50, 31, res::HISCORE, 6);
		int y = 0;
		for(auto& e : hightscores) {
			if(!e.name[0])
				continue;
			char temp[32];
			draw::text(88, y + 70, e.name);
			draw::text(246, y + 70, e.map);
			sznum(temp, e.days);
			draw::text(420 - draw::textw(temp), y + 70, temp);
			sznum(temp, e.score);
			draw::text(456, y + 70, temp);
			//animate::monster(540, y+86, y%(WaterElement-FirstMonster)+FirstMonster, y);
			y += 40;
		}
		draw::button(9, 315, res::HISCORE, NewGame, 0, 0, 1);
		draw::button(640 - 36, 315, res::HISCORE, Cancel, 4, 4, 5);
		draw::cursor(res::ADVMCO, 0);
		int id = draw::input();
		switch(id) {
		case 0:
		case Cancel:
			return;
		default:
			break;
		}
	}
}