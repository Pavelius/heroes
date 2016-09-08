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
	{Requisites, Text, 0, {"Name", "������������"}},
	{Requisites, Text, 0, {"Description", "��������"}},
	{Requisites, Text, 0, {"Identifier", "�������������"}},
	{Requisites, Number, 0, {"Number", "�����"}},
	{Requisites, Reference, 0, {"Parent", "��������"}},
	{Requisites, Number, 0, {"Value", "��������"}},
	{Requisites, Reference, 0, {"Type", "���"}},
	{Requisites, Text, 0, {"Path", "����"}},
	{Requisites, Number, 0, {"Flags", "�����"}},
	{Requisites, Number, 0, {"Width", "������"}},
	{Requisites, Number, 0, {"Height", "������"}},
	{Requisites, Boolean, 0, {"Modified", "�������"}},
	{Requisites, Number, 0, {"Line", "������"}},
	{Requisites, Number, 0, {"Column", "�������"}},
	// Types
	{Type, 0, 0, {"Text", "�����"}},
	{Type, 0, 0, {"Number", "�����"}},
	{Type, 0, 0, {"Date", "����"}},
	{Type, 0, 0, {"Boolean", "������"}},
	{Type, 0, 0, {"Structure", "���������"}},
	{Type, 0, 0, {"Array", "������"}},
	{Type, 0, 0, {"Table", "�������"}},
	{Type, 0, 0, {"Image", "�����������"}},
	{Type, 0, 0, {"Flag", "����"}},
	{Type, 0, 0, {"Function", "�������"}},
	{Type, 0, 0, {"Record", "������"}},
	{Type, 0, 0, {"Reference", "������"}},
	{Type, 0, 0, {"Point", "�����"}},
	{Type, 0, 0, {"Color", "����"}},
	//
	{Type, 0, 0, {"Conditions", "�������"}},
	{Conditions, Record, 0, {"Equal", "�����"}},
	{Conditions, Record, 0, {"NotEqual", "�� �����"}},
	{Conditions, Record, 0, {"Lesser", "������"}},
	{Conditions, Record, 0, {"Lesser or equal", "������ ��� �����"}},
	{Conditions, Record, 0, {"Greater", "������"}},
	{Conditions, Record, 0, {"Greater or equal", "������ ��� �����"}},
	{Conditions, Record, 0, {"Between", "�����"}},
	{Conditions, Record, 0, {"In list", "� ������"}},
	{Conditions, Record, 0, {"Not in list", "�� � ������"}},
	{Conditions, Record, 0, {"In hierarhy", "� ��������"}},
	{Conditions, Record, 0, {"Like", "��������"}},
	//
	{0, 0, 0, {"Commands", "�������"}},
	{0, 0, 0, {"Columns", "�������"}},
	{0, 0, 0, {"Selectors", "������"}},
	{0, 0, 0, {"Orders", "����������"}},
	{0, 0, 0, {"Requisites", "���������"}},
	//
	{0, 0, 0, {"Token", "�����"}},
	{0, 0, 0, {"Property", "��������"}},
	{0, 0, 0, {"Root", "������"}},
	//
	{Commands, Commands, 9, {"Add", "��������"}},
	{Commands, Commands, 8, {"Copy", "����������"}},
	{Commands, Commands, 10, {"Change", "��������"}},
	{Commands, Commands, 19, {"Delete", "�������"}},
	{Commands, Commands, 21, {"Move Up", "����������� �����"}},
	{Commands, Commands, 22, {"Move Down", "����������� ����"}},
	{Commands, Commands, 27, {"Move Left", "����������� �����"}},
	{Commands, Commands, 28, {"Move Right", "����������� ������"}},
	{Commands, Commands, 16, {"Setting", "���������"}},
	{Commands, Commands, 11, {"Sort Ascending", "����������� �� �����������"}},
	{Commands, Commands, 12, {"Sort Descending", "����������� �� ��������"}},
	{Commands, Commands, 0, {"Export", "�������"}},
	{Commands, Commands, 0, {"Import", "������"}},
	{Commands, Commands, 0, {"Update", "��������"}},
	{Commands, Commands, 0, {"Clear", "��������"}},
	{Commands, Commands, 0, {"Visibility", "���������"}},
	{Commands, Commands, 0, {"Usability", "�������������"}},
	{Commands, Commands, 0, {"Docking", "�������������"}},
	//
	{Docking, Record, 0, {"Left edge", "����� ����"}},
	{Docking, Record, 0, {"Left and bottom edge", "����� ���� �����"}},
	{Docking, Record, 0, {"Right edge", "������ ����"}},
	{Docking, Record, 0, {"Right and bottom edge", "������ ���� �����"}},
	{Docking, Record, 0, {"Bottom edge", "���"}},
	{Docking, Record, 0, {"Workspace", "������� ����"}},
	//
	{Commands, Commands, 0, {"Create", "�������"}},
	{Commands, Commands, 1, {"Open", "�������"}},
	{Commands, Commands, 2, {"Save", "���������"}},
	{Commands, Commands, 3, {"Cut", "��������"}},
	{Commands, Commands, 4, {"Copy", "����������"}},
	{Commands, Commands, 5, {"Paste", "��������"}},
	{Commands, Commands, 0, {"Caption", "���������"}},
	{Commands, Commands, 0, {"Toolbar", "������ ������������"}},
	{Commands, Commands, 0, {"Group", "������"}},
	{Commands, Commands, 0, {"About", "� ���������"}},
	{Commands, Commands, 0, {"Cancel", "��������"}},
	{Commands, Commands, 0, {"Continue", "����������"}},
	{Commands, Commands, 0, {"OK", "OK"}},
	//
	{0, 0, 0, {"First", "������"}},
	{0, 0, 0, {"Last", "���������"}},
	{0, 0, 0, {"Last Possible", "��������� ���������"}},
	{0, 0, 0, {"Valid", "����������"}},
};
static bsmeta::field fields[] = {
	BSREQ(bstoken, name, Name, Text),
	BSREQ(bstoken, tag, Identifier, Text),
	BSREQ(bstoken, type, Type, Reference),
	BSREQ(bstoken, parent, Parent, Reference),
	BSREQ(bstoken, image, Image, Number),
	{0}
};
BSMETA(bstoken, "Standart tokens", "����������� ��������", FirstBaseTag)

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