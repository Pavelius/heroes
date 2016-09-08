//Copyright 2014 by Pavel Chistyakov
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

#define FO(c,f) (const int)&((c*)0)->f
#define BSREQ(c,field,id,type) {#field, id, type, FO(c,field), sizeof(c::field), sizeof(c::field)}
#define BSARR(c,field,id,type) {#field, id, type, FO(c,field), sizeof(c::field), sizeof(c::field[0])}
#define BSINT(c,field,id,type) {#field, id, type, FO(c,field), sizeof(c::field), sizeof(c::field[0]), id + (sizeof(c::field)/sizeof(c::field[0])) - 1}
#define BSTBL(c,field,id) {#field, id, Table, FO(c,field), sizeof(c::field), sizeof(c::field.data[0])}
#define BSMETA(c,name_en,name_ru,from) \
static c clone;\
bsmeta c##s(#c, name_en, name_ru,from, from+sizeof(objects)/sizeof(objects[0])-1, fields,\
	objects, sizeof(objects[0]), sizeof(objects), &clone);

// Standart tags used to work with bases
// Used interval from 0xDF00 to 0xE000
enum base_tags
{
	// Common properties
	Name = 0xDF00, Description, Identifier, Key, Parent, Value, Type, Path, Flags,
	Width, Height,
	Modified,
	LineNumber, ColumnNumber,
	FirstBaseRequisit = Name, LastBaseRequisit = ColumnNumber,
	// Controls and types
	Text, Number, Date,
	Boolean, // Type that have view in 'yes/no' style. Like checkbox.
	Structure,
	Array,
	Table,
	Image, // View has image of standart toolbar or other image panel.
	Flag, // Bits flags data. Parent is base for shift. Formula in bits is 1<<(This-Parent-1). So if we have 'RecordState, Inherted, Definded' then 'Inherted' was 1 bit, 'Defined' 2 bit.
	Function,
	Record, // Enumeration record (like 'male' or 'female' in gender classification). Parent must cast to enumerator (like 'gender'  in gender classification).
	Reference, // Enumerator (like 'gender' in gender classification).
	Point, Color,
	FirstBaseType = Text, LastBaseType = Color,
	// Conditions
	Conditions,
	Equal, NotEqual, Lesser, LesserEqual, Greater, GreaterEqual, Between, InList, NotInList, InHierarhy, Like,
	// Common parent
	Commands, Columns, Selectors, Orders, Requisites, Token, Property, Root,
	// Table commands
	Add, AddCopy, Change, Delete, MoveUp, MoveDown, MoveLeft, MoveRight, Setting, SortAscending, SortDescending,
	TableExport, TableImport,
	Update, Clear, Visibility, Usability, Docking,
	// Docking
	DockLeft, DockLeftBottom, DockRight, DockRightBottom, DockBottom, DockWorkspace,
	FirstDock = DockLeft, LastDock = DockWorkspace,
	// Application coommands
	Create, Open, Save,
	Cut, Copy, Paste,
	Caption, Toolbar, Group,
	About, Cancel, Continue, OK,
	First, Last, LastPossible,
	FirstBaseTag = Name, LastBaseTag = LastPossible,
};
struct bstoken // Default tokens structure
{
	int					parent;
	int					type;
	int					image;
	const char*			name[2];
	const char*			tag;
};
struct bsmeta
{
	struct field
	{
		const char*		name;
		int				id;
		int				type;
		int				offset;
		int				size_total;
		int				size;
		int				to;
		char*			ptr(void* data, int id);
	};
	struct ref
	{
		bsmeta*			metadata;
		void*			data;
		operator bool() const { return metadata != 0; }
		void			clear();
		void			create(const char* name);
		int				get(int id);
		template<class T> T* getv(int id) { return (T*)ptr(id); }
		const char*		gets(int id);
		const char*		gets(int id, int locale);
		void*			ptr(int id);
		void			set(int id, int value);
		void			set(int id, const char* value);
		void			set(int id, const void* value, int index = 0);
	};
	struct object : ref
	{
		object();
	private:
		char source[256 * 8];
	};
	void*				data;
	int					size;
	int					count;
	const char*			identifier;
	const char*			name[2];
	field*				fields;
	int					from, to;
	void*				clone; // Fill for structures with virtual members
	bsmeta*				next;
	static bsmeta*		first;
	//
	bsmeta(const char* identifier, const char* name_en, const char* name_ru, int from, int to, field* fields, void* objects, int object_size, int objects_size, void* clone);
	//
	field*				find(int id);
	void				clear();
	inline ref			get(int index) { return{this, (char*)data + index*size}; }
	static bsmeta*		getbase(const char* name);
	static ref			getref(int rec);
	inline bool			has(int rec) { return rec >= from && rec <= from + count; }
};
struct bsval
{
	int					id;
	int					value;
};
void					bsadd(int rec, int id, int value);
void					bsadd(int rec, int id, int value, int minimum);
void					bsadd(int rec, int id, int value, int minimum, int maximum);
int						bscreate(int rec);
int						bscompareas(const void* p1, const void* p2);
int						bscompareds(const void* p1, const void* p2);
int						bscomparenm(const void* p1, const void* p2);
void					bsdelete(int rec);
bool					bsempthy(int rec);
int*					bsfilter(int* result, int* source, int i1, int v1);
int						bsfind(int rec, int id, int value);
int						bsfind(int rec, int id, int value, int id2, int value2);
int						bsfirstby(int rec, int par, int id);
int						bsget(int rec, int id);
const char*				bsgets(int rec, int id);
const char*				bsgets(int rec, int id, int locale);
bool					bshierarhy(int rec, int par, int id);
bool					bsmatch(int rec, bsval* e);
void*					bsptr(int rec, int id);
int						bsrec(const char* value);
void					bsreset(int rec); // Clear all records in base
int*					bsselect(int* result, int r1, int r2);
int*					bsselect(int* result, int r1, int r2, int i1, int v1);
void					bsset(int rec, int id, int value);
void					bsset(int rec, int id, const char* value);
int*					bssort(int* elements);
int						bstag(const char* string);
int*					bsuniq(int* result);