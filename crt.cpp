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
#include "crt.h"

int							locale; // Current localization
extern "C" void*			malloc(unsigned size);
extern "C" void*			realloc(void *ptr, unsigned size);
extern "C" void				free(void* pointer);

///////////////////////////////////////////////////////////
// SIGNATURE

void setsignature(char d[4], const char* s)
{
	d[0] = s[0];
	d[1] = s[1];
	d[2] = s[2];
	d[3] = 0;
}

bool issignature(const char d[4], const char* s)
{
	return (d[0] == s[0])
		&& (d[1] == s[1])
		&& (d[2] == s[2])
		&& d[3]==0;
}

///////////////////////////////////////////////////////////
// MEMORY ALLOCATION

struct crt_memory_block
{
	char				signature[4];
	int					size;
	int					lenght; // count = size/lenght;
	crt_memory_block*	next;
};
static crt_memory_block	first_block = {"CRT"};

static crt_memory_block* getprev(crt_memory_block* p)
{
	for(auto p1 = &first_block; p1->next; p1 = p1->next)
	{
		if(p1->next==p)
			return p1;
	}
	return 0;
}

bool rmblock(void* ptr)
{
	crt_memory_block* p = (crt_memory_block*)ptr-1;
	return getprev(p)!=0 && issignature(p->signature, "CRT");
}

// Used for rellocate simple classes like new char[1000]
void* operator new(unsigned size)
{
	crt_memory_block* p = (crt_memory_block*)malloc(size+sizeof(crt_memory_block));
	assert(p);
	setsignature(p->signature, "CRT");
	p->size = size;
	p->lenght = 1;
	p->next = 0;
	seqlast(&first_block)->next = p;
	return p + 1;
}

// Used for rellocate simple classes like new char[1000]
void* operator new[](unsigned size)
{
	return operator new(size);
}

// Used for delete simple classes like delete char*
void operator delete(void* ptr)
{
	if(!ptr)
		return;
	crt_memory_block* p = (crt_memory_block*)ptr-1;
	crt_memory_block* pr = getprev(p);
	if(!pr)
		return;
	pr->next = p->next;
	free(p);
}

void operator delete(void* p, unsigned size)
{
	operator delete(p);
}

int rmblockcount()
{
	int result = 0;
	for(auto p = first_block.next; p; p = p->next)
		result++;
	return result;
}

int rmsize(void* ptr)
{
	crt_memory_block* p = (crt_memory_block*)ptr-1;
	return p->size;
}

int rmlenght(void* ptr)
{
	crt_memory_block* p = (crt_memory_block*)ptr-1;
	return p->lenght;
}

int rmcount(void* ptr)
{
	crt_memory_block* p = (crt_memory_block*)ptr-1;
	return p->size/p->lenght;
}

void* rmreserve(void* ptr, int size)
{
	if(!ptr)
		return new char[size];
	crt_memory_block* p = (crt_memory_block*)ptr-1;
	crt_memory_block* pr = getprev(p);
	if(!pr)
		return ptr;
	if(p->size>=size)
		return ptr;
	p = (crt_memory_block*)realloc(p, size + sizeof(crt_memory_block));
	p->size = size;
	pr->next = p;
	return p + 1;
}

///////////////////////////////////////////////////////////
// STRING OPERATIONS

char* szsep(char* result, const char* separator)
{
	if(result[0]==0)
		return result;
	zcat(result, separator);
	return zend(result);
}

void szadd(char* output, const char* input, const char* separator)
{
	if(output[0])
		zcat(output, separator);
	zcat(output, input);
}

const char* szskipcr(const char* p)
{
	if(*p == '\n')
	{
		p++;
		if(*p == '\r')
			p++;
	}
	else if(*p == '\r')
	{
		p++;
		if(*p == '\n')
			p++;
	}
	return p;
}

const char* szskipcrr(const char* p0, const char* p)
{
	if(!p)
		return 0;
	if(p0 >= p)
		return p;
	if(p[-1] == '\n')
	{
		p--;
		if(p0 >= p)
			return p;
		if(p[-1] == '\r')
			p--;
	}
	else if(p[-1] == '\r')
	{
		p--;
		if(p0 >= p)
			return p;
		if(p[-1] == '\n')
			p--;
	}
	return p;
}

int szcmp(const char* p1, const char* p2, int max_count)
{
	for(; *p2 && *p1 && max_count>0; p1++, p2++, max_count--)
	{
		if(*p1 == *p2)
			continue;
		return *p1 - *p2;
	}
	if(!max_count)
		return 0;
	return *p1 - *p2;
}

int szcmpi(const char* p1, const char* p2)
{
	while(*p2 && *p1)
	{
		unsigned s1 = szupper(szget(&p1));
		unsigned s2 = szupper(szget(&p2));
		if(s1 == s2)
			continue;
		return s1 - s2;
	}
	unsigned s1 = szupper(szget(&p1));
	unsigned s2 = szupper(szget(&p2));
	return s1 - s2;
}

int szcmpi(const char* p1, const char* p2, int max_count)
{
	while(*p2 && *p1 && max_count-- > 0)
	{
		unsigned s1 = szupper(szget(&p1));
		unsigned s2 = szupper(szget(&p2));
		if(s1 == s2)
			continue;
		return s1 - s2;
	}
	if(!max_count)
		return 0;
	unsigned s1 = szupper(szget(&p1));
	unsigned s2 = szupper(szget(&p2));
	return s1 - s2;
}

char* sznum(char* result, int num, int precision, const char* empthy, int radix)
{
	char* p1 = result;
	if(num == 0)
	{
		if(empthy)
			zcpy(p1, empthy);
		else
		{
			zcpy(p1, "0");
			while(--precision > 0)
				zcat(p1, "0");
		}
		p1 = zend(p1);
	}
	else
	{
		char temp[32];
		int p = 0;
		if(num < 0)
		{
			*p1++ = '-';
			num = -num;
		}
		switch(radix)
		{
		case 16:
			while(num)
			{
				int a = (num%radix);
				if(a > 9)
					temp[p++] = 'A' - 10 + a;
				else
					temp[p++] = '0' + a;
				num /= radix;
			}
			break;
		default:
			while(num)
			{
				temp[p++] = '0' + (num%radix);
				num /= radix;
			}
			break;
		}
		while(precision-- > p)
			*p1++ = '0';
		while(p)
			*p1++ = temp[--p];
		p1[0] = 0;
	}
	return result;
}

int sz2num(const char* p1, const char** pp1)
{
	int result = 0;
	bool sign = false;
	const int radix = 10;
	while(*p1 && *p1 != '-' && (*p1 < '0' || *p1 > '9'))
		p1++;
	if(*p1 == '-')
	{
		sign = true;
		p1++;
	}
	while(*p1)
	{
		char a = *p1;
		if(a < '0' || a > '9')
			break;
		result = result*radix;
		result += a - '0';
		p1++;
	}
	if(sign)
		result = -result;
	if(pp1)
		*pp1 = p1;
	return result;
}

char* szurlc(char* result)
{
	char* p = result;
	while(*p)
	{
		if(*p == '\\')
			*p = '/';
		p++;
	}
	return zskipspcr(result);
}

char* szurl(char* p, const char* path, const char* name, const char* ext, const char* suffix)
{
	if(!p)
		return 0;
	*p = 0;
	if(path)
	{
		zcpy(p, path);
		zcat(p, "/");
	}
	if(name)
		zcat(p, name);
	if(suffix)
		zcat(p, suffix);
	if(ext && szext(p) == 0)
	{
		zcat(p, ".");
		zcat(p, ext);
	}
	return szurlc(p);
}

const char* szext(const char* path)
{
	for(const char* r = zend((char*)path); r > path; r--)
	{
		if(*r == '.')
			return r + 1;
		else if(*r == '\\' || *r == '/')
			return 0;
	}
	return 0;
}

const char* szfname(const char* path)
{
	for(const char* r = zend((char*)path); r > path; r--)
	{
		if(*r == '\\' || *r == '/')
			return r + 1;
	}
	return path;
}

const char* szfnamewe(char* result, const char* name)
{
	zcpy(result, szfname(name));
	char* p = (char*)szext(result);
	if(p && p!=result)
		p[-1] = 0;
	return result;
}

printfn* printfn::first;

int	printfn::element::compare(const void* p1, const void* p2)
{
	return strcmp(((element*)p1)->name, ((element*)p2)->name);
}

printfn::printfn(printfn::element* elements, int size) : elements(elements), count(size / sizeof(element))
{
	seqlink(this);
	qsort(elements, count, sizeof(element), element::compare);
}

char* szprintv(char* result, const char* src, const char* vl)
{
	char* dst = result;
	bool prefix_plus;
	while(true)
	{
		switch(*src)
		{
		case 0:
			*dst = 0;
			return result;
		case '%':
			src++;
			if(*src == '%')
			{
				*dst++ = *src++;
				continue;
			}
			*dst = 0;
			prefix_plus = false;
			if(*src == '+')
			{
				prefix_plus = true;
				src++;
			}
			if(*src >= '0' && *src <= '9')
			{
				// Если число, просто подставим нужный параметр
				int pn = 0;
				while(isnum(*src))
					pn = pn * 10 + (*src++) - '0';
				if(*src == 'i')
				{
					src++;
					int value = ((int*)vl)[pn - 1];
					if(prefix_plus && value >= 0)
						*dst++ = '+';
					sznum(dst, value, 0, 0, 10);
				}
				else
				{
					if(((char**)vl)[pn - 1])
						zcpy(dst, ((char**)vl)[pn - 1]);
				}
			}
			else if(printfn::first)
			{
				char temp[64];
				const char* p1 = src;
				while(*src)
				{
					const char* s1 = src;
					unsigned ch = szget(&src);
					if(!ischa(ch) && !isnum(ch) && ch != '_')
					{
						src = s1;
						break;
					}
				}
				int s = src - p1;
				if(s != 0 && s<int(sizeof(temp) - 1))
				{
					memcpy(temp, p1, s);
					temp[s] = 0;
				}
				printfn::element e;
				e.name = temp;
				e.param = 0;
				for(auto n = printfn::first; n; n = n->next)
				{
					auto p = (printfn::element*)bsearch(&e, n->elements, n->count,
						sizeof(printfn::element), printfn::element::compare);
					if(p)
					{
						p->proc(dst, p->param);
						break;
					}
				}
			}
			dst = zend(dst);
			break;
		default:
			*dst++ = *src++;
			break;
		}
	}
	return result;
}

void printc(const char* format, ...)
{
	char temp[4096 * 4];
	szprintv(temp, format, xva_start(format));
	printcnf(temp);
}

char* szprint(char* result, const char* src, ...)
{
	result = szprintv(result, src, xva_start(src));
	return result;
}

const char* szline(const char* p, int number)
{
	if(number < 1)
		return p;
	while(true)
	{
		switch(*p)
		{
		case 0:
			return p;
		case '\n':
		case '\r':
			if((p[0] == '\n' && p[1] == '\r')
				|| (p[0] == '\r' && p[1] == '\n'))
				p += 2;
			else
				p++;
			if(--number == 0)
				return p;
			break;
		default:
			p++;
			break;
		}
	}
}

const char* szline(const char* p, int line_number, int column_number)
{
	const char* p1 = szline(p, line_number);
	if(column_number < 0)
		return p1;
	while(column_number--)
	{
		unsigned sym = szget(&p1);
		if(sym == '\n' || sym == '\r')
			return p1;
	}
	return p1;
}

const char* szlineb(const char* start_text, const char* position)
{
	if(!start_text || !position)
		return position;
	while(position > start_text)
	{
		// Для формата unicode это также будет работать
		if(position[-1] == '\n' || position[-1] == '\r')
			return position;
		position--;
	}
	return start_text;
}

const char* szlinee(const char* string)
{
	register const char* p = string;
	while(*p && *p != '\n' && *p != '\r')
		p++;
	return p;
}

int szline(const char* p, const char* pos)
{
	int r = 0;
	while(true)
	{
		switch(*p)
		{
		case 0:
			return r;
		case '\n':
		case '\r':
			if((p[0] == '\n' && p[1] == '\r')
				|| (p[0] == '\r' && p[1] == '\n'))
				p += 2;
			else
				p++;
			if(p > pos)
				return r;
			r++;
			break;
		default:
			p++;
			break;
		}
	}
}

bool szmatch(const char* text, const char* name)
{
	while(*name)
	{
		if(*name++ != *text++)
			return false;
	}
	if(ischa(*text))
		return false;
	return true;
}

bool ischa(unsigned u)
{
	return (u >= 'A' && u <= 'Z')
		|| (u >= 'a' && u <= 'z')
		|| (u >= 0x410 && u <= 0x44F);
}

unsigned szupper(unsigned u)
{
	if(u >= 0x61 && u <= 0x7A)
		return u - 0x61 + 0x41;
	else if(u >= 0x430 && u <= 0x44F)
		return u - 0x430 + 0x410;
	return u;
}

char* szupper(char* p, int count)
{
	char* s1 = p;
	const char* p1 = p;
	while(count-- > 0)
		szput(&s1, szupper(szget(&p1)));
	return p;
}

unsigned szlower(unsigned u)
{
	if(u >= 0x41 && u <= 0x5A)
		return u - 0x41 + 0x61;
	else if(u >= 0x410 && u <= 0x42F)
		return u - 0x410 + 0x430;
	return u;
}

int getdigitscount(unsigned number)
{
	if(number < 10)
		return 1;
	if(number < 100)
		return 2;
	if(number < 1000)
		return 3;
	if(number < 10000)
		return 4;
	if(number < 100000)
		return 5;
	if(number < 1000000)
		return 6;
	if(number < 10000000)
		return 7;
	if(number < 100000000)
		return 8;
	return 9;
}

void szlower(char* p, int count)
{
	char* s1 = p;
	const char* p1 = p;
	if(count == -1)
	{
		while(true)
		{
			unsigned sym = szget(&p1);
			if(!sym)
				break;
			szput(&s1, szlower(sym));
		}
		szput(&s1, 0);
	}
	else
	{
		while(count-- > 0)
			szput(&s1, szlower(szget(&p1)));
	}
}

///////////////////////////////////////////////////////////
// UNICODE SUPPORT

unsigned szget(const char** input, codepages code)
{
	const unsigned char* p;
	unsigned result;
	switch(code)
	{
	case CPUTF8:
		p = (unsigned char*)*input;
		result = *p++;
		if(result >= 192 && result <= 223)
			result = (result - 192) * 64 + (*p++ - 128);
		else if(result >= 224 && result <= 239)
		{
			result = (result - 224) * 4096 + (p[0] - 128) * 64 + (p[1] - 128);
			p += 2;
		}
		*input = (const char*)p;
		return result;
	case CPU16LE:
		p = (unsigned char*)*input;
		result = p[0]|(p[1]<<8);
		*input = (const char*)(p + 2);
		return result;
	case CP1251:
		result = (unsigned char)*(*input)++;
		if(((unsigned char)result >= 0xC0))
			return result - 0xC0 + 0x410;
		else switch(result)
		{
		case 0xB2: return 0x406;
		case 0xAF: return 0x407;
		case 0xB3: return 0x456;
		case 0xBF: return 0x457;
		}
		return result;
	default:
		return *(*input)++;
	}
}

void szput(char** output, unsigned value, codepages code)
{
	char* p;
	switch(code)
	{
	case CPUTF8:
		p = *output;
		if(((unsigned short)value) < 128)
			*p++ = (unsigned char)value;
		else if(((unsigned short)value) < 2047)
		{
			*p++ = (unsigned char)(192 + (((unsigned short)value) / 64));
			*p++ = (unsigned char)(128 + (((unsigned short)value) % 64));
		}
		else
		{
			*p++ = (unsigned char)(224 + (((unsigned short)value) / 4096));
			*p++ = (unsigned char)(128 + ((((unsigned short)value) / 64) % 64));
			*p++ = (unsigned char)(224 + (((unsigned short)value) % 64));
		}
		*output = p;
		break;
	case CP1251:
		if(value >= 0x410 && value <= 0x44F)
			value = value - 0x410 + 0xC0;
		else switch(value)
		{
		case 0x406: value = 0xB2; break; // I
		case 0x407: value = 0xAF; break; // Ї
		case 0x456: value = 0xB3; break;
		case 0x457: value = 0xBF; break;
		}
		*(*output)++ = (unsigned char)value;
		break;
	case CPU16LE:
		*(*output)++ = (unsigned char)(value & 0xFF);
		*(*output)++ = (unsigned char)(((unsigned)value >> 8));
		break;
	case CPU16BE:
		*(*output)++ = (unsigned char)(((unsigned)value >> 8));
		*(*output)++ = (unsigned char)(value & 0xFF);
		break;
	default:
		*(*output)++ = (unsigned char)value;
		break;
	}
}

char* szput(char* result, unsigned sym, codepages page)
{
	char* p = result;
	szput(&p, sym, page);
	*p = 0;
	return result;
}

void szencode(char* output, int output_count, codepages output_code, const char* input, int input_count, codepages input_code)
{
	char* s1 = output;
	char* s2 = s1 + output_count;
	const char* p1 = input;
	const char* p2 = p1 + input_count;
	while(p1 < p2 && s1 < s2)
		szput(&s1, szget(&p1, input_code), output_code);
	if(s1 < s2)
	{
		s1[0] = 0;
		if((output_code == CPU16BE || output_code == CPU16LE) && (s1+1)<s2)
			s1[1] = 0;
	}
}

///////////////////////////////////////////////////////////
// CONVERTER

converter* converter::first;

converter::converter(const char* name) :name(name)
{
	seqlink(this);
}

converter* converter::find(const char* name)
{
	for(auto p = first; p; p = p->next)
	{
		if(strcmp(p->name, name) == 0)
			return p;
	}
	return 0;
}

unsigned xdecode(const char* name, void* output, unsigned output_size, const void* input, unsigned input_size)
{
	for(auto n = converter::first; n; n = n->next)
	{
		if(strcmp(name, n->name) == 0)
			return n->decode(output, output_size, input, input_size);
	}
	return 0;
}