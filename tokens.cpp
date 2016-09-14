#include "main.h"

static struct token_string
{
	const char*	name[2];
} objects[] = {
	{{"Empthy"}},
	// Basic ablity
	{{"Attack", "Атака"}},
	{{"Defence", "Защита"}},
	{{"Spell Power", "Сила магии"}},
	{{"Knowledge", "Знания"}},
	// Skills
	{{"Pathfinding", "Поиск пути"}},
	{{"Archery", "Лучник"}},
	{{"Logistics", "Логистика"}},
	{{"Scounting", "Разведка"}},
	{{"Diplomacy", "Дипломатия"}},
	{{"Navigation", "Навигация"}},
	{{"Leadership", "Лидерство"}},
	{{"Wisdom", "Мудрость"}},
	{{"Mysticism", "Мистицизм"}},
	{{"Luck", "Удача"}},
	{{"Ballistics", "Баллистика"}},
	{{"Eagle Eye", "Орлинный глаз"}},
	{{"Necromancy", "Некромантия"}},
	{{"Estates", "Доход"}},
	//
	{{"Luck", "Удача"}},
	{{"Cursed", "Проклят"}},
	{{"Awful", "Ужас"}},
	{{"Bad", "Плохая"}},
	{{"Normal", "Норма"}},
	{{"Good", "Хорошо"}},
	{{"Great", "Отлично"}},
	{{"Irish", "Чудо"}},
	//
	{{"Morale", "Мораль"}},
	{{"Treason", "Измена"}},
	{{"Awful", "Ужас"}},
	{{"Poor", "Плохо"}},
	{{"Normal", "Норма"}},
	{{"Good", "Хорошо"}},
	{{"Great", "Отлично"}},
	{{"Blood", "Кровь"}},
	//
	{{"Speed", "Скорость"}},
	{{"Crawling", "Ползет"}},
	{{"Very Slow", "Оч.медленно"}},
	{{"Slow", "Медленно"}},
	{{"Average", "Средне"}},
	{{"Fast", "Быстро"}},
	{{"Very Fast", "Оч.быстро"}},
	{{"Ultra Fast", "Ультр.быстро"}},
	//
	{{"Easy", "Лекго"}}, {{"Normal", "Обычно"}}, {{"Hard", "Тяжело"}}, {{"Very Hard", "Очень тяжело"}}, {{"Impossible", "Невозможно"}},
	{{"Beginner", "Начинающий"}}, {{"Normal", "Норма"}}, {{"Hard", "Тяжело"}}, {{"Expert", "Эксперт"}},
	{{"Barbarian", "Варвар"}}, {{"Knight", "Рыцарь"}}, {{"Necromancer", "Некромансер"}}, {{"Sorcerer", "Волшебница"}}, {{"Warlock", "Колдун"}}, {{"Wizard", "Волшебник"}}, {{"Elementals", "Элементали"}},
	{{"Magic", "Магия"}}, {{"Magic (max)", "Магия (макс)"}},
	{{"Hits", "Хиты"}}, {{"Hits", "Хиты"}}, {{"Shoots", "Выстрелы"}},
	{{"Gold", "Золото"}}, {{"Wood", "Дерево"}}, {{"Mercury", "Ртуть"}}, {{"Ore", "Камни"}}, {{"Sulfur", "Сера"}}, {{"Crystal", "Кристалы"}}, {{"Gems", "Драгоценности"}},
	//
	{{"Basic", "Базово"}}, {{"Advanced", "Продвинуто"}}, {{"Expert", "Эксперт"}},
};
static bsmeta::field fields[] = {
	BSREQ(token_string, name, Name, Text),
};
BSMETA(token_string, "Tokens", "Токены", 0);

static int object_get(int rec, int id)
{
	switch(id)
	{
	case Rating:
		switch(rec)
		{
		case EasyDifficulty: return 70;
		case NormalDifficulty: return 100;
		case HardDifficulty: return 120;
		case VeryHardDifficulty: return 160;
		case ImpossibleDifficulty: return 180;
		default: return 0;
		}
	default:
		return 0;
	}
}