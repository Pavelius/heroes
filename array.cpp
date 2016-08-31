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

array::array(int size, int count_max) : data(0), count(0), size(size), count_max(count_max)
{
}

array::~array()
{
	clear();
}

void array::clear()
{
	count = 0;
	if(rmblock(data))
	{
		delete data;
		data = 0;
		count_max = 0;
	}
}

void array::setup(int size)
{
	clear();
	this->size = size;
}

bool array::reserve(int count)
{
	if(!size)
		return false;
	if(data)
	{
		if(count_max >= count)
			return true;
		if(!rmblock(data))
			return false;
	}
	count_max = optimal(count);
	data = rmreserve(data, count_max*size);
	return true;
}

void* array::add(const void* object)
{
	if(!reserve(count + 1))
		return 0;
	void* p = (char*)data + size*count;
	if(object)
		memcpy(p, object, size);
	else
		memset(p, 0, size);
	count++;
	return p;
}

void* array::insert(int index, const void* object)
{
	if(!reserve(count + 1))
		return 0;
	memmove((char*)data + (index + 1)*size, (char*)data + index*size, (count - index)*size);
	void* p = (char*)data + index*size;
	if(object)
		memcpy(p, object, size);
	else
		memset(p, 0, size);
	count++;
	return p;
}

void array::remove(int index, int elements_count)
{
	if(((unsigned)index)>=((unsigned)count))
		return;
	if(index < count - 1)
		memcpy((char*)data + index*size,
			(char*)data + (index + elements_count)*size,
			(count - (index + elements_count))*size);
	count -= elements_count;
}

void array::copy(void* dest, const void* source)
{
	memcpy(dest, source, size);
}

bool array::equal(void* e1, void* e2) const
{
	return memcmp(e1, e2, size) == 0;
}

int array::indexof(const void* e) const
{
	if((unsigned)e < (unsigned)data || (unsigned)e >= (unsigned)((char*)data + count_max*size))
		return -1;
	return ((unsigned)e - (unsigned)data) / size;
}

void array::swap(void* p1, void* p2)
{
	for(int i = 0; i < size; i++)
		iswap(((char*)p1)[i], ((char*)p2)[i]);
}

void array::sort(int i1, int i2, int(*compare)(const void* p1, const void* p2, void* param), void* param)
{
	for(int i = i2; i > i1; i--)
	{
		for(int j = i1; j < i; j++)
		{
			void* t1 = (void*)get(j);
			void* t2 = (void*)get(j + 1);
			if(compare(t1, t2, param) > 0)
				swap(t1, t2);
		}
	}
}

void* array::find(const void* e1)
{
	auto p1 = (char*)data;
	auto p2 = (char*)data + size*count;
	for(; p1 < p2; p1 += size)
	{
		if(memcmp(p1, e1, size) == 0)
			return p1;
	}
	return 0;
}

void* array::find(int o1, int v1, int o2, int v2)
{
	char* p1 = (char*)data;
	char* p2 = (char*)data + size*count;
	for(; p1 < p2; p1 += size)
	{
		if(*((int*)(p1 + o1)) != v1)
			continue;
		if(o2 != -1 && *((int*)(p1 + o2)) != v2)
			continue;
		return p1;
	}
	return 0;
}