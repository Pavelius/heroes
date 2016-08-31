#include "cface/sprite.h"
#include "main.h"

namespace res
{
#pragma pack(push)
#pragma pack(1)
	struct icn
	{
		struct record
		{
			short int		x;
			short int		y;
			short int		width;
			short int		height;
			unsigned char	type;
			unsigned		offset;
		};
		unsigned short		count;
		unsigned			size;
		record				records[1];
	};
	struct til
	{
		short unsigned		count;
		short unsigned		width;
		short unsigned		height;
	};
#pragma pack(pop)
}

static unsigned char	picture_data[256*256*4*2];
static sprite			picture;
const char*				rsname(int i);

static const char* gpath(const char* fn, const char* ext = 0)
{
	static char temp[261];
	zcpy(temp, "d:/applications/heroes");
	zcat(temp, "/");
	zcat(temp, fn);
	if(ext)
	{
		zcat(temp, ".");
		zcat(temp, ext);
	}
	return temp;
}

// (00) end of line
// (01 - 7F) draw p[0] of pixels leaded after
// (80, XX) draw count of XX bytes of alpha AA pixels
// (81 - 9F) draw count of (B-0x80) bytes of alpha AA pixels
// (A0, XX) skip count of XX pixels
// (A1 - FF) skip count of (B-0xA0) pixels
static unsigned char* encode_xx(unsigned char* dst, const unsigned char* src, int height)
{
	unsigned char* x = dst;
	while(true)
	{
		unsigned char c = *src++;
		if(c == 0)
		{
			*dst++ = c;
			if(--height <= 0)
				break;
		}
		else if(c < 0x80) // (01..7F) - bytes copy from src
		{
			*dst++ = c;
			while(c--)
				*dst++ = *src++;
		}
		else if(c == 0x80) // 0x80 - end data
		{
			*dst++ = 0;
			break;
		}
		else if(c < 0xC0) // 0xBF - skip data
			*dst++ = (c - 0x80) + 0xA1;
		else if(c == 0xC0)// 0xC0 - shadow
		{
			unsigned char c = *src % 4 ? *src % 4 : *(++src);
			++src;
			if(c < (0x9F - 0x81))
				*dst++ = 0x81 + (c - 1);
			else
			{
				*dst++ = 0xA0;
				*dst++ = c;
			}
		}
		else // fill
		{
			++src;
			if(c==0xC1)
				c = *src++;
			else
				c -= 0xC0;
			unsigned char fill = *src++;
			// fill value
			while(c--)
				*x++ = fill;
		}
	}
}

static int encode_frame(unsigned char* d, unsigned char* s)
{
}

static void add_frame(res::icn& s, res::icn::record& r)
{
	sprite::frame& d = picture.frames[picture.count];
	d.ox = r.x;
	d.oy = r.y;
	d.sx = r.width;
	d.sy = r.height;
	d.encode = sprite::RLE8;
	d.offset = picture.size;
	unsigned char* d1 = picture_data + (d.offset - picture.frames[0].offset);
	unsigned char* s1 = ((unsigned char*)&s) + r.offset;
	picture.size += encode_frame(d1, s1);
}

void show_type_0x20()
{
	int count = 0;
	for(int i = 0; i < res::Empthy; i++)
	{
		res::icn* p = (res::icn*)res::get(res::tokens(i));
		if(!p)
			continue;
		const char* name = rsname(i);
		if(!name)
			continue;
		const char* ext = szext(name);
		if(!ext)
			continue;
		if(strcmp(ext, "ICN") != 0)
			continue;
		for(int j = 0; j < p->count; j++)
		{
			res::icn::record& e = p->records[j];
			if(e.type == 0x20)
			{
				char temp[512];
				szprint(temp, "%1 %2i\n", name, j);
				printc(temp);
			}
		}
	}
}

static void conver(res::tokens id, const char* output)
{
	res::icn* p = (res::icn*)res::get(id);
	picture.setup(p->count);
	for(int i = 0; i<p->count; i++)
		add_frame(*p, p->records[i]);
	picture.write(gpath(output, "pma"));
}