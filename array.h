//Copyright 2013 by Pavel Chistyakov
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#pragma once

// Use to clean up sequenced resources if destructor. Use it like 'clear(this)'.
template<class T> inline void seqclear(T* p)
{
	T* z = p->next;
	while(z)
	{
		T* n = z->next;
		z->next = 0;
		delete z;
		z = n;
	}
	p->next = 0;
}
// Return last element in sequence.
template<class T> inline T* seqlast(T* p)
{
	while(p->next)
		p = p->next;
	return p;
}
// Link sequenced class
template<class T> inline void seqlink(T* p)
{
	p->next = 0;
	if(!T::first)
		T::first = p;
	else
		seqlast(T::first)->next = p;		
}
// Find by ID in collections
template<class T> inline T* colfind(int id, T* p)
{
	while(p->id)
	{
		if(p->id == id)
			return p;
		p++;
	}
	return 0;
}
// Get total count of records.
inline int optimal(int need_count, int mc = 256*256)
{
	int m = 64;
	while(m<mc)
	{
		if(need_count<m)
			return m;
		m = m<<1;
	}
	return m;
}
// Standart non-template container.
// May be table controls datasource.
struct array
{
	void*					data;
	int						count;
	int						count_max;
	int						size;
	//
	array(int size = 0, int count_max = 0);
	~array();
	//
	void*					add(const void* object = 0);
	void					clear();
	void					copy(void* dest, const void* source);
	bool					equal(void* e1, void* e2) const;
	void*					find(const void* e1);
	void*					find(int o1, int v1, int o2 = -1, int v2 = 0);
	int						gcount() const { return count; }
	const void*				get(int index) const { return (char*)data + index*size; }
	int						indexof(const void* e) const;
	void*					insert(int index, const void* object);
	void					remove(int index, int count = 1);
	bool					reserve(int count);
	void					setup(int size);
	void					sort(int i1, int i2, int(*compare)(const void* p1, const void* p2, void* param), void* param);
	void					swap(void* p1, void* p2);
};
// Simplest array - pointer and count. Used only for POD types.
// Complex types NEED sublass clear(), reserve(), reserve(n).
template<class T> struct aref
{
	T*						data;
	int						count;
	//
	inline T&				operator[](int index) { return data[index]; }
	//
	T&						add() { return data[count++]; }
	T&						addr() { reserve(); return data[count++]; }
	void					add(const T& e) { data[count++] = e; }
	template<class Z> T*	addu(Z value) { auto e = find(value); if(e) return e; reserve(); return data + (count++); }
	T*						begin() { return data; }
	const T*				begin() const { return data; }
	void					initialize() { data = 0; count = 0; }
	void					clear() { delete data; initialize(); }
	T*						end() { return data+count; }
	const T*				end() const { return data+count; }
	template<class Z> T*	find(Z value) { for(auto& e : *this) { if(e.id==value) return &e; } return 0; }
	int						indexof(const T* t) const { if(t<data || t>data+count) return -1; return t-data; }
	int						indexof(const T t) const { for(int i = 0; i<count; i++) if(data[i]==t) return i; return -1; }
	void					reserve(int count) { data = (T*)rmreserve(data, sizeof(T)*optimal(count, 256*256*256)); }
	void					reserve() { reserve(count+1); }
	void					remove(int index) { if(index<0 || index>=count) return; count--; if(index>=count) return; memmove(data+index, data+index+1, sizeof(data[0])*(count-index)); }
};
// Use when we don't want use allocator and must have static data
// Beware - it's not have constructors and destructor
template<class T, int count_max = 128> struct adat
{
	T						data[count_max];
	int						count;
	//
	inline T&				operator[](int index) { return data[index]; }
	//
	T*						add() { if(count<count_max) return data + (count++); return 0; }
	void					add(const T& e) { if(count<count_max) data[count++] = e; }
	T*						begin() { return data; }
	const T*				begin() const { return data; }
	void					clear() { count = 0; }
	T*						end() { return data+count; }
	const T*				end() const { return data+count; }
	template<class Z> T*	find(Z id) { auto e1 = data + count; for(T* e = data; e<e1; e++) { if(e->id == id) return e; } return 0; }
	int						indexof(const T* e) const { if(e>=data && e<=data+count) return e-data; return -1; }
	int						indexof(const T t) const { for(int i = 0; i<count; i++) if(data[i] == t) return i; return -1; }
	void					remove(int index) { if(index<0) return; if(count) { memmove(data+index, data+index+1, sizeof(data[0])); count--; }}
};
// Auto grow simple class. Primary use for custom allocators.
// No destructor!! When program exit atexit() may destroy allocator object
// before any static instance of this class.
template<typename T, int max_count = 4096> struct autogrow
{
	T						data[max_count];
	int						count;
	autogrow*				next;
	autogrow() : next(0), count(0) {}
	T*						add() { auto t = this; while(t->count >= max_count) { if(!t->next) t->next = new autogrow(); t = t->next; } return t->data + t->count++; }
	T*						begin() { return data; }
	T*						end() { return data + count; }
	void					clear() { seqclear(this); }
};
// Simple value list
struct vallist
{
	struct element
	{
		int					value;
		unsigned			flags;
		char				name[64 - sizeof(int) * 2];
	};
	aref<element>			rows;
	vallist();
	element&				add(int value, const char* name, unsigned flags = 0);
};