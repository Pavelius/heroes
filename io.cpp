//Copyright 2013 by Pavel Chistyakov
//
//	Licensed under the Apache License, Version 2.0 (the "License");
//	you may not use this file except in compliance with the License.
//	You may obtain a copy of the License at
//
//	http://www.apache.org/licenses/LICENSE-2.0
//
//	Unless required by applicable law or agreed to in writing, software
//	distributed under the License is distributed on an "AS IS" BASIS,
//	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//	See the License for the specific language governing permissions and
//	limitations under the License.

#include "array.h"
#include "crt.h"
#include "io.h"

io::stream& io::stream::operator<<(const int n)
{
	char temp[32];
	sznum(temp, n);
	return *this << temp;
}

io::stream&	io::stream::operator<<(const char* t)
{
	if(!t)
		return *this;
	// ѕриведем формат строки из стандартной кодировки
	while(*t)
	{
		char temp[8];
		char* s1 = temp;
		unsigned u = szget(&t);
		szput(&s1, u, CPUTF8);
		write(temp, s1-temp);
	}
	return *this;
}

void io::stream::writescan(void* p, int width, int height, int scan_line, int element_size)
{
	char* pc = (char*)p;
	while(height>0)
	{
		write(pc, width*element_size);
		pc += scan_line;
		height--;
	}
}

unsigned char io::stream::get()
{
	unsigned char r = 0;
	read(&r, 1);
	return r;
}

unsigned short io::stream::getLE16()
{
	unsigned char u2 = get();
	unsigned char u1 = get();
	return (u2<<8) | u1;
}

unsigned io::stream::getLE32()
{
	unsigned char u4 = get();
	unsigned char u3 = get();
	unsigned char u2 = get();
	unsigned char u1 = get();
	return (u4<<24) | (u3<<16) | (u2<<8) | u1;
}

short unsigned io::stream::get16()
{
	short unsigned r = 0;
	read(&r, sizeof(r));
	return r;
}

unsigned io::stream::get32()
{
	unsigned r = 0;
	read(&r, sizeof(r));
	return r;
}

unsigned io::stream::getsize()
{
	unsigned s = seek(0, SeekCur);
	unsigned r = seek(0, SeekEnd);
	seek(s, SeekSet);
	return r;
}

void io::stream::puttext(const char* text)
{
	int m = zlen(text);
	write(m);
	if(m)
		write(text, m);
}

void io::stream::gettext(char* result, int max_buffer)
{
	int m;
	result[0] = 0;
	read(&m, sizeof(m));
	if(!m)
		return;
	max_buffer = imin(m, max_buffer-1);
	read(result, max_buffer);
	result[max_buffer] = 0;
	m -= max_buffer;
	if(m)
		seek(m, SeekCur);
}

////////////////////////////////////////////////////////////////
// FILE i/o

io::file::file() : handle(0)
{
}

io::file::file(const char* url, unsigned flags) : file()
{
    create(url, flags);
}

io::file::~file()
{
    close();
}

////////////////////////////////////////////////////////////////
// MEMORY i/o

io::memory::memory(void* data, int size) : data((unsigned char*)data), pos(0), size(size)
{
}

int io::memory::read(void* p, int size)
{
	if(pos>=this->size)
		return 0;
	if(pos+size<this->size)
		size = this->size - pos;
	memcpy(p, data+pos, size);
    return size;
}

int io::memory::write(const void* p, int size)
{
	if(pos>=this->size)
		return 0;
	if(pos+size<this->size)
		size = this->size - pos;
	memcpy(data+pos, p, size);
    return size;
}

int io::memory::seek(int pos, int rel)
{
	switch(rel)
	{
	case SeekCur:
		this->pos = imin(size, imax(0, this->pos + pos));
		break;
	case SeekEnd:
		this->pos = imin(size, imax(0, size+pos));
		break;
	case SeekSet:
		this->pos = imin(size, imax(0, pos));
		break;
	}
    return this->pos;
}

void* loadb(const char* url, int* size, int additional)
{
	void* p = 0;
	if(size)
		*size = 0;
	if(!url || url[0] == 0)
		return 0;
	io::file file(url, StreamRead);
	if(!file)
		return 0;
	int s = file.seek(0, SeekEnd) + additional;
	file.seek(0, SeekSet);
	p = new char[s];
	memset(p, 0, s);
	file.read(p, s);
	if(size)
		*size = s;
	return p;
}

char* loadt(const char* url, int* size)
{
	int s1;
	if(size)
		*size = 0;
	unsigned char* p = (unsigned char*)loadb(url, &s1, 1);
	if(!p)
		return 0;
	if(p[0] == 0xEF && p[1] == 0xBB && p[2] == 0xBF)
	{
		// UTF8
		// ѕерекодируем блок через декодировщик. ћожет быть только меньше,
		// так как системна€ кодировка ANSI
		szencode((char*)p, s1, metrics::code, (char*)p + 3, s1, CPUTF8);
	}
	if(size)
		*size = s1;
	return (char*)p;
}

io::plugin* io::plugin::first;

io::plugin::plugin() : name(0), fullname(0), filter(0)
{
	seqlink(this);
}

io::plugin* io::plugin::find(const char* name)
{
	for(auto p = first; p; p = p->next)
	{
		if(!p->name)
			continue;
		if(strcmp(p->name, name) == 0)
			return p;
	}
	return 0;
}

io::strategy* io::strategy::first;

io::strategy::strategy(const char* id, const char* type) : id(id), type(type)
{
	seqlink(this);
}

io::strategy* io::strategy::find(const char* name)
{
	for(auto n = first; n; n = n->next)
	{
		if(strcmp(n->id, name) == 0)
			return n;
	}
	return 0;
}

int io::node::getlevel() const
{
	int result = 0;
	for(auto p = parent; p; p = p->parent)
		result++;
	return result;
}

bool io::read(const char* url, io::reader& e)
{
	auto pp = io::plugin::find(szext(url));
	if(!pp)
		return false;
	const char* source = loadt(url);
	if(!source || source[0]==0)
		return false;
	pp->read(source, e);
	delete source;
	return true;
}

bool io::read(const char* url, const char* root_name, void* param)
{
	struct proxy : public io::reader
	{

		const char*		root_name;
		void*			param;
		io::strategy*	st;

		void open(node& e)
		{
			switch(e.getlevel())
			{
			case 0:
				if(e.name[0]!=0 && strcmp(e.name, root_name)!=0)
					e.skip = true;
				break;
			case 1:
				st = strategy::find(e.name);
				if(!st)
					e.skip = true;
				break;
			default:
				if(!st)
					e.skip = true;
				else
					st->open(e);
				break;
			}
		}

		void set(node& e, int value)
		{
			if(!st)
				e.skip = true;
			else
				st->set(e, value);
		}

		void set(node& e, const char* value)
		{
			if(!st)
				e.skip = true;
			else
				st->set(e, value);
		}

		void close(node& e)
		{
			if(!st)
				e.skip = true;
			else
				st->close(e);
		}

	};
	proxy reader_proxy;
	reader_proxy.root_name = root_name;
	reader_proxy.param = param;
	return read(url, reader_proxy);
}

bool io::write(const char* url, const char* root_name, void* param)
{
	io::plugin* pp = plugin::find(szext(url));
	if(!pp)
		return false;
	io::file file(url, StreamWrite|StreamText);
	if(!file)
		return false;
	io::writer* pw = pp->write(file);
	if(!pw)
		return false;
	pw->open(root_name);
	for(auto p = strategy::first; p; p = p->next)
	{
		if(strcmp(p->type, root_name)!=0)
			continue;
		auto ps = p;
		pw->open(ps->id);
		ps->write(*pw, param);
		pw->close(ps->id);
	}
	pw->close(root_name);
	return true;
}

io::node::node(int type) : parent(0), name(""), type(type), index(0), skip(false)
{
}

io::node::node(node& parent, const char* name, int type) : parent(&parent), name(name), type(type), index(0), skip(parent.skip)
{
	memset(params, 0, sizeof(params));
}