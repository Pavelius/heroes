//   Copyright 2013 by Pavel Chistyakov
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

#define mdw(l, h) ((int)((((unsigned)h)<<16)|l))
#define glw(i) ((int)(((unsigned)i)&0xFFFF))
#define ghw(i) ((int)(((unsigned)i)>>16))
#define xva_start(v) ((const char*)&v + sizeof(v))

#ifdef _DEBUG
#define assert(e) if(!(e)) { dlgerr("Assertion failed", "File: %1\nLine: %2i", __FILE__, __LINE__); exit(255); }
#else
#define assert(e)
#endif

extern "C" void						abort();
extern "C" int						atexit(void(*func)(void));
extern "C" void*					bsearch(const void* key, const void *base, unsigned num, unsigned size, int(*compar)(const void *, const void *));
extern "C" unsigned					clock();
extern "C" void						exit(int exit_code);
extern "C" int						memcmp(const void* p1, const void* p2, unsigned size);
extern "C" void*					memmove(void* destination, const void* source, unsigned size);
extern "C" void*					memcpy(void* destination, const void* source, unsigned size);
extern "C" void*					memset(void* destination, int value, unsigned size);
extern "C" void						qsort(void* base, unsigned num, unsigned size, int(*compar)(const void*, const void*));
extern "C" int						rand(void); // Get next random value
extern "C" void						sleep(unsigned seconds); // Suspend execution for an interval of time
extern "C" void						srand(unsigned seed); // Set random seed
extern "C" int						strcmp(const char* s1, const char* s2); // Compare two strings
extern "C" int						system(const char* command); // Execute system command

enum codepages						{ CPNONE, CP1251, CPUTF8, CPU16BE, CPU16LE };
enum crterrors
{
	NoCrtErrors,
	ErrorFileNotFound, ErrorImagePluginNotFound, ErrorCompressPluginNotFound,
	ErrorInvalidInputParameters, ErrorIllegalByteSequence,
};
// Custom print tokens set
struct printfn
{
	struct element
	{
		const char*					name;
		void						(*proc)(char* result, int param);
		int							param;
		//
		static int					compare(const void* p1, const void* p2);
	};
	element*						elements;
	int								count;
	printfn*						next;
	static printfn*					first;
	printfn(element* elements, int size);
};
struct converter
{
	const char*						name;
	converter*						next;
	static converter*				first;
	converter(const char* name);
	virtual unsigned				decode(void* output, int output_size, const void* input, int input_size) const = 0;
	virtual unsigned				encode(void* output, int output_size, const void* input, int input_size) const = 0;
	static converter*				find(const char* name);
};
struct command
{
	struct plugin
	{
		const char*					name;
		const command*				commands;
		plugin*						next;
		static command::plugin*		first;
		plugin(const char* name, const command* commands);
	};
	const char*						name;
	void							(*proc)();
	static void						execute(const char* group, const char* name);
};
namespace metrics
{
	const codepages					code = CP1251;
}
//
int									getdigitscount(unsigned number); // Get digits count of number. For example if number=100, result be 3.
inline int							gethour(unsigned d) { return (d/60)%24; } // Get hour
inline int							getminute(unsigned d) { return (d % 60); } // Get minute
bool								ischa(unsigned u); // is alphabetical character?
bool								issignature(const char name[4], const char* s); // Is name equal to string s?
inline bool							isnum(unsigned u) { return u>='0' && u<='9'; } // is numeric character?
void*								loadb(const char* url, int* size = 0, int additional_bytes_alloated = 0); // Load binary file.
char*								loadt(const char* url, int* size = 0); // Load text file and decode it to system codepage.
extern int							locale; // Current system locale. Use in szt() and alike functions.
void								printc(const char* format, ...); // Analog prinf() but use format of this library
void								printcnf(const char* text); // Write to console this text
void								dlgerr(const char* title, const char* format, ...); // Show error.
void								dlgmsg(const char* title, const char* text); // System message dialog
bool								rmblock(void* ptr); // Is memory ptr was previously allocated?
int									rmblockcount(); // Get allocated memory block count. Can be used for allocation error detection.
void*								rmreserve(void* ptr, int size); // Resize memory block. Return new value.
int									rmsize(void* ptr); // Get allocated blok size.
void								setsignature(char name[4], const char* s); // Set signature to name
int									sz2num(const char* p1, const char** pp1 = 0);
void								szencode(char* output, int output_count, codepages output_code, const char* input, int input_count, codepages input_code);
unsigned							szget(const char** input, codepages page = metrics::code);
int									szcmpi(const char* p1, const char* p2);
int									szcmpi(const char* p1, const char* p2, int count);
const char*							szdup(const char *text);
const char*							szext(const char* path);
const char*							szfname(const char* text); // Get file name from string (no fail, always return valid value)
const char*							szfnamewe(char* result, const char* name); // get file name without extension (no fail)
const char*							szline(const char* start_text, int line_number);
const char*							szline(const char* start_text, int line_number, int column_number);
int									szline(const char* start_text, const char* position);
const char*							szlineb(const char* start_text, const char* position);
const char*							szlinee(const char* position);
unsigned							szlower(unsigned u); // to lower reg
void								szlower(char* p, int count=1); // to lower reg
bool								szmatch(const char* text, const char* name); //
char*								sznum(char* p1, int num, int precision = 0, const char* empthy = 0, int radix = 10);
char*					            szprint(char* dst, const char* src, ...);
char*								szprintv(char* result, const char* src, const char* vl);
void								szput(char** output, unsigned u, codepages page = metrics::code);
char*								szput(char* output, unsigned u, codepages page = metrics::code); // Fast symbol put function. Return 'output'.
char*								szsep(char* result, const char* sep = ", ");
const char*							szskipcr(const char* p);
const char*							szskipcrr(const char* p0, const char* p);
inline const char*					szt(const char* en, const char* ru) { return locale ? ru : en; }
unsigned							szupper(unsigned u);
char*								szupper(char* p, int count=1); // to upper reg
char*								szurl(char* p, const char* path, const char* name, const char* ext = 0, const char* suffix = 0);
char*								szurlc(char* p1);
unsigned							xdecode(const char* name, void* output, unsigned output_size, const void* input, unsigned input_size);
inline int							xrand(int n1, int n2) { return n1+rand()%(n2-n1+1); }
inline int							xroll(int c, int d, int b = 0) { while(c--) b += 1+(rand()%d); return b; }
// Common used templates
inline int							ifloor(double n) { return (int)n; }
template<class T> inline T			imax(T a, T b) { return a > b ? a : b; }
template<class T> inline T			imin(T a, T b) { return a < b ? a : b; }
template<class T> inline T			iabs(T a) { return a > 0 ? a : -a; }
template<class T> inline void		iswap(T& a, T& b) { T i = a; a = b; b = i; }
// Inline strings functions
template<class T> inline T*			zchr(T* p, T e) { while(*p) { if(*p == e) return p; p++; } return 0; }
template<class T> inline const T*	zchr(const T* p, T e) { while(*p) { if(*p==e) return p; p++; } return 0; }
template<class T> inline void		zcpy(T* p1, const T* p2) { while(*p2) *p1++ = *p2++; *p1 = 0; }
template<class T> inline void		zcpy(T* p1, const T* p2, int max_count) { while(*p2 && max_count-->0) *p1++ = *p2++; *p1 = 0;}
template<class T> inline T*			zend(T* p) { while(*p) p++; return p; }
template<class T> inline void		zcat(T* p1, const T e) { p1 = zend(p1); p1[0] = e; p1[1] = 0; }
template<class T> inline void		zcat(T* p1, const T* p2) { zcpy(zend(p1), p2); }
template<class T> inline int		zlen(T* p) { return zend(p) - p; }
template<class T> inline int		zindex(const T* p, T e) { if(!p) return -1; const T* p1=p; while(*p) { if(*p++==e) return p-p1-1; } return -1; }
template<class T> inline const T*	zfindi(const T* p, int id) { while(p->id) { if(p->id == id) return p; p++; } return 0; }
template<class T> inline int		zfind(const T* p, T e) { if(!p) return -1; const T* p1=p; while(*p) { if(*p++==e) return p-p1-1; } return -1; }
template<class T> inline const T*	zright(const T* p, int count) { auto e = zend(p) - count; return (e < p) ? p : e; }
template<class T> inline void		zshuffle(T* p, int count) { for(int i = 0; i < count; i++) iswap(p[i], p[rand() % count]); }
template<class T> inline T*			zskipsp(T* p) { if(p) while(*p==32 || *p==9) p++; return p; }
template<class T> inline T*			zskipspcr(T* p) { if(p) while(*p==' ' || *p=='\t' || *p=='\n' || *p=='\r') p++; return p; }
// Inline other function
inline int							d100() { return rand()%100; }
inline int							d10() { return 1+(rand() % 10); }
inline int							d20() { return 1 + (rand() % 20); }