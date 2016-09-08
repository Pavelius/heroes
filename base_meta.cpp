/* Copyright 2013 by Pavel Chistyakov
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#include "array.h"
#include "base.h"
#include "crt.h"

bsmeta* bsmeta::first;

static bstoken objects[] = {
	{Requisites, Text, 0, {"Name", "Наименование"}},
	{Requisites, Text, 0, {"Description", "Описание"}},
	{Requisites, Text, 0, {"Identifier", "Идентификатор"}},
	{Requisites, Number, 0, {"Number", "Число"}},
	{Requisites, Reference, 0, {"Parent", "Родитель"}},
	{Requisites, Number, 0, {"Value", "Значение"}},
	{Requisites, Reference, 0, {"Type", "Тип"}},
	{Requisites, Text, 0, {"Path", "Путь"}},
	{Requisites, Number, 0, {"Flags", "Флаги"}},
	{Requisites, Number, 0, {"Width", "Ширина"}},
	{Requisites, Number, 0, {"Height", "Высота"}},
	{Requisites, Boolean, 0, {"Modified", "Изменен"}},
	{Requisites, Number, 0, {"Line", "Строка"}},
	{Requisites, Number, 0, {"Column", "Колонка"}},
	// Types
	{Type, 0, 0, {"Text", "Текст"}},
	{Type, 0, 0, {"Number", "Число"}},
	{Type, 0, 0, {"Date", "Дата"}},
	{Type, 0, 0, {"Boolean", "Булево"}},
	{Type, 0, 0, {"Structure", "Структура"}},
	{Type, 0, 0, {"Array", "Массив"}},
	{Type, 0, 0, {"Table", "Таблица"}},
	{Type, 0, 0, {"Image", "Изображение"}},
	{Type, 0, 0, {"Flag", "Флаг"}},
	{Type, 0, 0, {"Function", "Функция"}},
	{Type, 0, 0, {"Record", "Запись"}},
	{Type, 0, 0, {"Reference", "Ссылка"}},
	{Type, 0, 0, {"Point", "Точка"}},
	{Type, 0, 0, {"Color", "Цвет"}},
	//
	{Type, 0, 0, {"Conditions", "Условия"}},
	{Conditions, Record, 0, {"Equal", "Равно"}},
	{Conditions, Record, 0, {"NotEqual", "Не равно"}},
	{Conditions, Record, 0, {"Lesser", "Меньше"}},
	{Conditions, Record, 0, {"Lesser or equal", "Меньше или равно"}},
	{Conditions, Record, 0, {"Greater", "Больше"}},
	{Conditions, Record, 0, {"Greater or equal", "Больше или равно"}},
	{Conditions, Record, 0, {"Between", "Между"}},
	{Conditions, Record, 0, {"In list", "В списке"}},
	{Conditions, Record, 0, {"Not in list", "Не в списке"}},
	{Conditions, Record, 0, {"In hierarhy", "В иерархии"}},
	{Conditions, Record, 0, {"Like", "Содержит"}},
	//
	{0, 0, 0, {"Commands", "Команды"}},
	{0, 0, 0, {"Columns", "Колонки"}},
	{0, 0, 0, {"Selectors", "Отборы"}},
	{0, 0, 0, {"Orders", "Сортировка"}},
	{0, 0, 0, {"Requisites", "Реквизиты"}},
	//
	{0, 0, 0, {"Token", "Токен"}},
	{0, 0, 0, {"Property", "Свойство"}},
	{0, 0, 0, {"Root", "Корень"}},
	//
	{Commands, Commands, 9, {"Add", "Добавить"}},
	{Commands, Commands, 8, {"Copy", "Копировать"}},
	{Commands, Commands, 10, {"Change", "Изменить"}},
	{Commands, Commands, 19, {"Delete", "Удалить"}},
	{Commands, Commands, 21, {"Move Up", "Передвинуть вверх"}},
	{Commands, Commands, 22, {"Move Down", "Передвинуть вниз"}},
	{Commands, Commands, 27, {"Move Left", "Передвинуть влево"}},
	{Commands, Commands, 28, {"Move Right", "Передвинуть вправо"}},
	{Commands, Commands, 16, {"Setting", "Настройка"}},
	{Commands, Commands, 11, {"Sort Ascending", "Сортировать по возрастанию"}},
	{Commands, Commands, 12, {"Sort Descending", "Сортировать по убыванию"}},
	{Commands, Commands, 0, {"Export", "Экспорт"}},
	{Commands, Commands, 0, {"Import", "Импорт"}},
	{Commands, Commands, 0, {"Update", "Обновить"}},
	{Commands, Commands, 0, {"Clear", "Очистить"}},
	{Commands, Commands, 0, {"Visibility", "Видимость"}},
	{Commands, Commands, 0, {"Usability", "Использование"}},
	{Commands, Commands, 0, {"Docking", "Присоединение"}},
	//
	{Docking, Record, 0, {"Left edge", "Левый край"}},
	{Docking, Record, 0, {"Left and bottom edge", "Левый край внизу"}},
	{Docking, Record, 0, {"Right edge", "Правый край"}},
	{Docking, Record, 0, {"Right and bottom edge", "Правый край внизу"}},
	{Docking, Record, 0, {"Bottom edge", "Низ"}},
	{Docking, Record, 0, {"Workspace", "Рабочий стол"}},
	//
	{Commands, Commands, 0, {"Create", "Создать"}},
	{Commands, Commands, 1, {"Open", "Открыть"}},
	{Commands, Commands, 2, {"Save", "Сохранить"}},
	{Commands, Commands, 3, {"Cut", "Вырезать"}},
	{Commands, Commands, 4, {"Copy", "Копировать"}},
	{Commands, Commands, 5, {"Paste", "Вставить"}},
	{Commands, Commands, 0, {"Caption", "Заголовок"}},
	{Commands, Commands, 0, {"Toolbar", "Панель инструментов"}},
	{Commands, Commands, 0, {"Group", "Группа"}},
	{Commands, Commands, 0, {"About", "О программе"}},
	{Commands, Commands, 0, {"Cancel", "Отменить"}},
	{Commands, Commands, 0, {"Continue", "Продолжить"}},
	{Commands, Commands, 0, {"OK", "OK"}},
	//
	{0, 0, 0, {"First", "Первый"}},
	{0, 0, 0, {"Last", "Последний"}},
	{0, 0, 0, {"Last Possible", "Последний возможный"}},
	{0, 0, 0, {"Valid", "Правильный"}},
};
static bsmeta::field fields[] = {
	BSREQ(bstoken, name, Name, Text),
	BSREQ(bstoken, tag, Identifier, Text),
	BSREQ(bstoken, type, Type, Reference),
	BSREQ(bstoken, parent, Parent, Reference),
	BSREQ(bstoken, image, Image, Number),
	{0}
};
BSMETA(bstoken, "Standart tokens", "Стандартные элементы", FirstBaseTag)

char* bsmeta::field::ptr(void* data, int id)
{
	if(to == 0)
		return (char*)data + offset;
	else
		return (char*)data + offset + (id - this->id)*size;
}

void* bsmeta::ref::ptr(int id)
{
	if(!metadata)
		return 0;
	field* f = metadata->find(id);
	if(!f)
		return 0;
	return f->ptr(data, id);
}

int bsmeta::ref::get(int id)
{
	if(!metadata)
		return 0;
	field* f = metadata->find(id);
	if(!f)
		return 0;
	char* p = f->ptr(data, id);
	if(f->type == Number || f->type == Reference)
	{
		switch(f->size)
		{
		case 1: return *((unsigned char*)p);
		case 2: return *((short*)p);
		case 4: return *((int*)p);
		}
	}
	else if(f->type == Flags)
		return (*((int*)p)&(1 << (id - f->id))) ? 1 : 0;
	return 0;
}

void bsmeta::ref::set(int id, int value)
{
	if(!metadata)
		return;
	field* f = metadata->find(id);
	if(!f)
		return;
	char* p = f->ptr(data, id);
	if(f->type == Number || f->type == Reference)
	{
		switch(f->size)
		{
		case 1: *((unsigned char*)p) = value; break;
		case 2: *((short*)p) = value; break;
		case 4: *((int*)p) = value; break;
		}
	}
	else if(f->type == Flags)
		*((int*)p) |= 1 << (id - f->id);
}

const char* bsmeta::ref::gets(int id)
{
	if(!metadata)
		return "";
	field* f = metadata->find(id);
	if(!f)
		return "";
	char* p = f->ptr(data, id);
	if(f->type == Text)
	{
		if(f->size == 4)
			return *((const char**)p);
		else if(f->size == 8)
			return ((const char**)p)[locale];
		return (const char*)p;
	}
	return "";
}

const char* bsmeta::ref::gets(int id, int locale)
{
	if(!metadata)
		return "";
	field* f = metadata->find(id);
	if(!f)
		return "";
	char* p = f->ptr(data, id);
	if(f->type == Text)
	{
		if(f->size == 4)
			return *((const char**)p);
		else
		{
			auto pp = ((const char**)p)[locale];
			if(pp && pp[0])
				return pp;
			else
				return ((const char**)p)[0];
		}
	}
	return "";
}

void bsmeta::ref::set(int id, const char* value)
{
	if(!metadata)
		return;
	field* f = metadata->find(id);
	if(!f)
		return;
	char* p = f->ptr(data, id);
	if(f->type == Text)
	{
		if(f->size == 4)
			*((const char**)p) = value;
		else if(f->size == sizeof(char*) * 2)
			((const char**)p)[locale] = value;
	}
}

void bsmeta::ref::set(int id, const void* value, int index)
{
	if(!metadata)
		return;
	field* f = metadata->find(id);
	if(!f)
		return;
	memcpy((char*)ptr(f->id) + index*f->size, value, f->size);
}

void bsmeta::ref::create(const char* identifier)
{
	metadata = getbase(identifier);
	clear();
}

void bsmeta::ref::clear()
{
	if(metadata)
		memcpy(data, metadata->clone, metadata->size);
}

bsmeta::object::object()
{
	data = source;
	metadata = 0;
}

bsmeta::bsmeta(const char* identifier, const char* name_en, const char* name_ru,
	int from, int to, bsmeta::field* fields, void* objects, int object_size, int objects_size, void* clone)
	: data(objects), size(object_size), count(objects_size/object_size),
	identifier(identifier), from(from), to(to), fields(fields), clone(clone), next(0)
{
	seqlink(this);
	name[0] = name_en;
	name[1] = name_ru;
	if(clone && memcmp(objects, clone, object_size) == 0)
		count = 0;
}

bsmeta::ref bsmeta::getref(int rec)
{
	for(auto e = first; e; e = e->next)
	{
		if(rec >= e->from && rec <= e->to)
			return{e, (char*)e->data + e->size*(rec - e->from)};
	}
	return{0, 0};
}

bsmeta::field* bsmeta::find(int id)
{
	for(auto p = fields; p->name; p++)
	{
		if(p->id == id)
			return p;
		else if(p->to && id > p->id && id <= p->to)
			return p;
	}
	return 0;
}

bsmeta* bsmeta::getbase(const char* name)
{
	for(auto p = bsmeta::first; p; p = p->next)
	{
		if(strcmp(p->identifier, name) == 0)
			return p;
	}
	return 0;
}

void bsmeta::clear()
{
	count = 0;
}

int bstag(const char* string)
{
	for(auto e = bsmeta::first; e; e = e->next)
	{
		if(!e->find(Identifier))
			continue;
		for(int rec = e->from; rec <= e->to; rec++)
		{
			const char* p1 = bsgets(rec, Identifier);
			if(!p1)
				continue;
			if(p1 == string)
				return rec;
			else if(strcmp(p1, string) == 0)
				return rec;
		}
	}
	return 0;
}

int bsrec(const char* value)
{
	if(!value)
		return 0;
	int len = zlen(value);
	if(!len)
		return 0;
	for(auto e = bsmeta::first; e; e = e->next)
	{
		if(!e->find(Name))
			continue;
		for(int rec = e->from; rec <= e->to; rec++)
		{
			const char* p1 = bsgets(rec, Name);
			if(!p1)
				continue;
			if(szcmpi(p1, value, len) == 0)
				return rec;
		}
	}
	return 0;
}

int bsget(int rec, int id)
{
	auto e = bsmeta::getref(rec);
	if(!e.metadata)
		return 0;
	switch(id)
	{
	case First: return e.metadata->from;
	case Last: return e.metadata->from + e.metadata->count - 1;
	case LastPossible: return e.metadata->to;
	default: return e.get(id);
	}
}

const char* bsgets(int rec, int id)
{
	return bsmeta::getref(rec).gets(id);
}

const char* bsgets(int rec, int id, int locale)
{
	return bsmeta::getref(rec).gets(id, locale);
}

void* bsptr(int rec, int id)
{
	return bsmeta::getref(rec).ptr(id);
}

void bsset(int rec, int id, int value)
{
	bsmeta::getref(rec).set(id, value);
}

void bsset(int rec, int id, const char* value)
{
	bsmeta::getref(rec).set(id, value);
}

bool bsempthy(int rec)
{
	auto e = bsmeta::getref(rec);
	if(!e.metadata)
		return true;
	return memcmp(e.data, e.metadata->clone, e.metadata->size) == 0;
}

void bsdelete(int rec)
{
	bsmeta::ref e = bsmeta::getref(rec);
	e.clear();
}

void bsreset(int rec)
{
	auto m = bsmeta::getref(rec);
	if(!m.metadata)
		return;
	m.metadata->clear();
}

int bscreate(int rec, bool test_zero)
{
	bsmeta::ref e = bsmeta::getref(rec);
	if(!e.metadata)
		return 0;
	if(e.metadata->from + e.metadata->count > e.metadata->to)
		return 0;
	int size = e.metadata->size;
	if(test_zero)
	{
		for(int i = 0; i < e.metadata->count; i++)
		{
			if(memcmp((char*)e.metadata->data + size*i, e.metadata->clone, size) == 0)
				return e.metadata->from + i;
		}
	}
	return e.metadata->from + e.metadata->count++;
}

int bsfind(int rec, int id, int value)
{
	bsmeta::ref e1 = bsmeta::getref(rec);
	bsmeta::ref e2 = bsmeta::getref(bsget(rec, Last));
	if(!e1.metadata || !e2.metadata)
		return 0;
	bsmeta::field* f = e1.metadata->find(id);
	if(!f)
		return 0;
	char* p1 = f->ptr(e1.data, id);
	char* p2 = f->ptr(e2.data, id);
	char* b1 = p1;
	int size = e1.metadata->size;
	switch(f->size)
	{
	case sizeof(int) :
		while(p1 <= p2)
		{
			if((*(int*)p1) == value)
				return e1.metadata->from + ((p1 - b1) / size);
			p1 += size;
		}
					 break;
	case sizeof(char) :
		while(p1 <= p2)
		{
			if((*p1) == value)
				return e1.metadata->from + ((p1 - b1) / size);
			p1 += size;
		}
					  break;
	case sizeof(short) :
		while(p1 <= p2)
		{
			if((*(short*)p1) == value)
				return e1.metadata->from + ((p1 - b1) / size);
			p1 += size;
		}
					   break;
	}
	return 0;
}