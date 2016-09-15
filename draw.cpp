#include "main.h"

namespace res
{
#pragma pack(push)
#pragma pack(1)
	struct icn
	{
		struct record
		{
			short int	x;
			short int	y;
			short int	width;
			short int	height;
			unsigned char type;
			unsigned	offset;
		};
		unsigned short	count;
		unsigned		size;
		record			records[1];
	};
	struct til
	{
		short unsigned count;
		short unsigned width;
		short unsigned height;
	};
#pragma pack(pop)
}

static unsigned char	bits[draw::width*draw::height];
rect					draw::clipping = {0, 0, draw::width, draw::height};
unsigned				draw::counter;
static rect             status_rect;
static char             status_text[260];
static char             tooltips_text[260];
extern unsigned char    pallette[256][4];
extern unsigned char    pallette2[256];
extern unsigned char    pallette3[256];
extern unsigned char    pallette4[256];
int                     hot::param;
int                     hot::level;
int                     hot::param2;
point                   hot::mouse;
int                     hot::key;
int                     hot::command;
bool                    hot::pressed;
static bool				evil_interface;
// System driver
bool					sys_create(const char* title, int milliseconds, bool fullscreen, unsigned char* bits, int width, int height); // Create system window
int						sys_input(bool wait); // Wait for system input
void*					sys_get_pallette(); // Get system pallette

int draw::isqrt(int num)
{
	int res = 0;
	int bit = 1 << 30;
	// "bit" starts at the highest power of four <= the argument.
	while(bit > num)
		bit >>= 2;
	while(bit != 0)
	{
		if(num >= res + bit)
		{
			num -= res + bit;
			res = (res >> 1) + bit;
		}
		else
			res >>= 1;
		bit >>= 2;
	}
	return res;
}

int	res::width(tokens res, int n)
{
	icn* p = (icn*)get(res);
	if(!p || !p->count)
		return 0;
	return p->records[n % p->count].width;
}

int	res::height(tokens res, int n)
{
	icn* p = (icn*)get(res);
	if(!p || !p->count)
		return 0;
	return p->records[n % p->count].height;
}

int res::getcount(tokens res)
{
	icn* p = (icn*)get(res);
	if(!p)
		return 0;
	return p->count;
}

int	res::ox(tokens res, int n)
{
	icn* p = (icn*)get(res);
	if(!p || !p->count)
		return 0;
	return p->records[n%p->count].x;
}

point res::offset(tokens res, int n)
{
	icn* p = (icn*)get(res);
	if(!p || !p->count)
		return{0, 0};
	n = n % p->count;
	return{p->records[n].x, p->records[n].y};
}

int	res::oy(tokens res, int n)
{
	icn* p = (icn*)res::get(res);
	if(!p || !p->count)
		return 0;
	return p->records[n%p->count].y;
}

rect res::box(int x, int y, res::tokens res, int n, unsigned flags)
{
	icn* p = (icn*)get(res);
	if(!p || !p->count)
		return{0, 0, 0, 0};
	n = n % p->count;
	if((flags&AFNoOffset) == 0)
	{
		x += p->records[n].x;
		y += p->records[n].y;
	}
	return{x, y, x + p->records[n].width, y + p->records[n].height};
}

static unsigned char* skip_v1(unsigned char* src, int dy)
{
	while(true)
	{
		unsigned char c = *src;
		if(c == 0)
		{
			++src;
			if(--dy <= 0)
				return src;
		}
		else if(c < 0x80)	// 0x7F - count data
			src += c + 1;
		else if(c == 0x80) // 0x80 - end data
			return src;
		else if(c < 0xC0) // 0xBF - skip data
			src++;
		else if(c == 0xC0)// 0xC0 - shadow
		{
			++src;
			unsigned char c = *src % 4 ? *src % 4 : *(++src);
			if(c)
			{
			}
			++src;
		}
		else if(c == 0xC1) // fill
			src += 3;
		else
			src += 2;
	}
}

// When NN is control byte then:
// 0x00 end line
// 0x01..0x7F copy NN bytes from 'src' to 'dst'
// 0x80 end of data
// 0x81..0xBF skip (NN-0x80) bytes on output
// 0xC0 shadow
// 0xC1 fill byte XX by value XX
// 0xC1..0xFF fill byte (NN-0xC0) by value XX
static void sprite_v1(unsigned char* dst, int scanline, const unsigned char* s, int height, const unsigned char* clip_x1, const unsigned char* clip_x2, unsigned char* change)
{
	unsigned char* d = dst;
	unsigned char* de = dst + scanline*height;
	while(true)
	{
		register unsigned char c = *s;
		if(c == 0x00)
		{
			s++;
			dst += scanline;
			clip_x1 += scanline;
			clip_x2 += scanline;
			d = dst;
			if(d >= de)
				return;
		}
		else if(c < 0x80) // 0..0x7F - bytes copy from src
		{
			s++;
			// left clip part
			if(d + c <= clip_x1 || d > clip_x2)
			{
				d += c;
				s += c;
				continue;
			}
			else if(d < clip_x1)
			{
				unsigned char sk = clip_x1 - d;
				d += sk;
				s += sk;
				c -= sk;
			}
			// show visible part
			if(change)
			{
				do
				{
					if(d >= clip_x2)
						break;
					*d++ = change[*s++];
				} while(--c);
			}
			else
			{
				do
				{
					if(d >= clip_x2)
						break;
					*d++ = *s++;
				} while(--c);
			}
			// right clip part
			if(c)
			{
				s += c;
				d += c;
			}
		}
		else if(c == 0x80) // 0x80 - end data
			break;
		else if(c < 0xC0) // 0xBF - skip data
		{
			s++;
			d += c - 0x80;
		}
		else if(c == 0xC0)// 0xC0 - shadow
		{
			s++;
			c = *s % 4 ? *s % 4 : *(++s);
			s++;
			// left clip part
			if(d + c <= clip_x1 || d > clip_x2)
			{
				d += c;
				continue;
			}
			else if(d < clip_x1)
			{
				unsigned char sk = clip_x1 - d;
				d += sk;
				c -= sk;
			}
			// show visible part
			do
			{
				if(d >= clip_x2)
					break;
				//d++;
				*d++ = pallette4[*d];
			} while(--c);
		}
		else // fill
		{
			s++;
			if(c == 0xC1)
				c = *s++;
			else
				c -= 0xC0;
			unsigned char fill = *s++;
			if(change)
				fill = change[fill];
			// left clip part
			if(d + c <= clip_x1 || d > clip_x2)
			{
				d += c;
				continue;
			}
			else if(d < clip_x1)
			{
				unsigned char sk = clip_x1 - d;
				d += sk;
				c -= sk;
			}
			// show visible part
			do
			{
				if(d >= clip_x2)
					break;
				*d++ = fill;
			} while(--c);
		}
	}
}

static void sprite_v1m(unsigned char* dst, int scanline, const unsigned char* s, int height, const unsigned char* clip_x1, const unsigned char* clip_x2, unsigned char* change)
{
	unsigned char* d = dst;
	unsigned char* de = dst + scanline*height;
	while(true)
	{
		register unsigned char c = *s;
		if(c == 0x00)
		{
			s++;
			dst += scanline;
			clip_x1 += scanline;
			clip_x2 += scanline;
			d = dst;
			if(d >= de)
				return;
		}
		else if(c < 0x80) // 0..0x7F - bytes copy from src
		{
			s++;
			// left clip part
			if(d - c > clip_x2 || d < clip_x1)
			{
				d -= c;
				s += c;
				continue;
			}
			else if(d > clip_x2)
			{
				unsigned char sk = d - clip_x2;
				d -= sk;
				s += sk;
				c -= sk;
			}
			// show visible part
			if(change)
			{
				do
				{
					if(d < clip_x1)
						break;
					*d-- = change[*s++];
				} while(--c);
			}
			else
			{
				do
				{
					if(d < clip_x1)
						break;
					*d-- = *s++;
				} while(--c);
			}
			// right clip part
			if(c)
			{
				s += c;
				d -= c;
			}
		}
		else if(c == 0x80) // 0x80 - end data
			break;
		else if(c < 0xC0) // 0xBF - skip data
		{
			s++;
			d -= c - 0x80;
		}
		else if(c == 0xC0)// 0xC0 - shadow
		{
			s++;
			c = *s % 4 ? *s % 4 : *(++s);
			s++;
			// left clip part
			if(d - c > clip_x2 || d < clip_x1)
			{
				d -= c;
				continue;
			}
			else if(d > clip_x2)
			{
				unsigned char sk = d - clip_x2;
				d -= sk;
				c -= sk;
			}
			// show visible part
			do
			{
				if(d < clip_x1)
					break;
				*d-- = pallette4[*d];
			} while(--c);
		}
		else // fill
		{
			s++;
			if(c == 0xC1)
				c = *s++;
			else
				c -= 0xC0;
			unsigned char fill = *s++;
			if(change)
				fill = change[fill];
			// left clip part
			if(d - c > clip_x2 || d < clip_x1)
			{
				d -= c;
				continue;
			}
			else if(d > clip_x2)
			{
				unsigned char sk = d - clip_x2;
				d -= sk;
				c -= sk;
			}
			// show visible part
			do
			{
				if(d < clip_x1)
					break;
				*d-- = fill;
			} while(--c);
		}
	}
}

static void sprite_v2(unsigned char* dst, int scanline, const unsigned char* s, int height, unsigned char* clip_x1, unsigned char* clip_x2)
{
	unsigned char* d = dst;
	unsigned char* de = dst + scanline*height;
	while(true)
	{
		unsigned char c = *s;
		if(c == 0)
		{
			s++;
			dst += scanline;
			clip_x1 += scanline;
			clip_x2 += scanline;
			d = dst;
			if(d >= de)
				return;
		}
		else if(c < 0x80) // 0..0x7F - bytes from src
		{
			unsigned char c = *s++;
			// left clip part
			if(d + c <= clip_x1 || d > clip_x2)
			{
				d += c;
				continue;
			}
			else if(d < clip_x1)
			{
				unsigned char sk = clip_x1 - d;
				d += sk;
				c -= sk;
			}
			// show visible part
			do
			{
				if(d >= clip_x2)
					break;
				*d++ = pallette4[*d];
			} while(--c);
		}
		else if(c == 0x80) // 0x80 - end data
			break;
		else
			d += (*s++) - 0x80;
	}
}

static void sprite_v2m(unsigned char* dst, int scanline, const unsigned char* s, int height, unsigned char* clip_x1, unsigned char* clip_x2)
{
	unsigned char* d = dst;
	unsigned char* de = dst + scanline*height;
	while(true)
	{
		unsigned char c = *s;
		if(c == 0)
		{
			s++;
			dst += scanline;
			clip_x1 += scanline;
			clip_x2 += scanline;
			d = dst;
			if(d >= de)
				return;
		}
		else if(c < 0x80) // 0..0x7F - bytes from src
		{
			unsigned char c = *s++;
			// left clip part
			if(d - c > clip_x2 || d < clip_x1)
			{
				d -= c;
				continue;
			}
			else if(d > clip_x2)
			{
				unsigned char sk = d - clip_x2;
				d -= sk;
				c -= sk;
			}
			// show visible part
			do
			{
				if(d < clip_x1)
					break;
				*d-- = pallette4[*d];
			} while(--c);
		}
		else if(c == 0x80) // 0x80 - end data
			break;
		else
			d -= (*s++) - 0x80;
	}
}

void draw::image(int x, int y, res::tokens res, unsigned frame, unsigned flags, unsigned char* change)
{
	res::icn* p = (res::icn*)res::get(res);
	if(!p || !p->count)
		return;
	res::icn::record& r = p->records[frame % p->count];
	unsigned char* d = (unsigned char*)p->records + r.offset;
	if((flags&AFNoOffset) == 0)
	{
		if(flags&AFMirror)
			x -= r.x;
		else
			x += r.x;
		y += r.y;
	}
	if(flags&AFCenter)
		x -= r.width / 2;
	if(flags&AFCenterV)
		y -= r.height / 2;
	if(y + height < clipping.y1 || y >= clipping.y2)
		return;
	if(r.type == 0x20)
	{
		if(flags&AFMirror)
		{
			sprite_v2m(ptr(x, y), width, d, clipping.y2 - y,
				ptr(clipping.x1, 0),
				ptr(clipping.x2, height));
		}
		else
		{
			sprite_v2(ptr(x, y), width, d, clipping.y2 - y,
				ptr(clipping.x1, y),
				ptr(clipping.x2, y));
		}
	}
	else
	{
		if(y < clipping.y1)
		{
			d = skip_v1(d, clipping.y1 - y);
			y = clipping.y1;
		}
		if(flags&AFMirror)
		{
			sprite_v1m(ptr(x, y), width, d, clipping.y2 - y,
				ptr(clipping.x1, y),
				ptr(clipping.x2, y),
				change);
		}
		else
		{
			if(x + width < clipping.x1 || x >= clipping.x2)
				return;
			sprite_v1(ptr(x, y), width, d, clipping.y2 - y,
				ptr(clipping.x1, y),
				ptr(clipping.x2, y),
				change);
		}
	}
}

static void colorize(unsigned char* pallette, int index, int count)
{
	unsigned char b1[4];
	unsigned char* p1 = &pallette[index * 4];
	b1[0] = p1[0];
	b1[1] = p1[1];
	b1[2] = p1[2];
	b1[3] = p1[3];
	//
	for(int i = 1; i < count; i++)
	{
		p1[(i - 1) * 4 + 0] = p1[i * 4 + 0];
		p1[(i - 1) * 4 + 1] = p1[i * 4 + 1];
		p1[(i - 1) * 4 + 2] = p1[i * 4 + 2];
		p1[(i - 1) * 4 + 3] = p1[i * 4 + 3];
	}
	p1[(count - 1) * 4 + 0] = b1[0];
	p1[(count - 1) * 4 + 1] = b1[1];
	p1[(count - 1) * 4 + 2] = b1[2];
	p1[(count - 1) * 4 + 3] = b1[3];
}

static void colorize()
{
	unsigned char* pal = (unsigned char*)sys_get_pallette();
	if(((draw::counter / 2) % 2)==0)
	{
		colorize(pal, 0xD6, 4);
		colorize(pal, 0xDA, 4);
		colorize(pal, 0xEE, 4);
		colorize(pal, 0xE7, 4);
	}
}

unsigned char* draw::ptr(int x, int y)
{
	return &bits[y*draw::width + x];
}

void draw::hexagon(int x, int y, unsigned char m)
{
	// border parts
	line(x - cell_wr, y - cell_hr, x - cell_wr, y + cell_hr - 1, m);
	line(x + cell_wr, y - cell_hr, x + cell_wr, y + cell_hr - 1, m);
	// left lines
	line(x - cell_wr, y - cell_hr, x, y - cell_hd / 2, m);
	line(x - cell_wr, y + cell_hr - 1, x, y + cell_hd / 2 - 1, m);
	// right lines
	line(x, y - cell_hd / 2, x + cell_wr, y - cell_hr, m);
	line(x, y + cell_hd / 2 - 1, x + cell_wr, y + cell_hr - 1, m);
}

void draw::hexagonf(int x, int y, unsigned char intense)
{
	static int points[] = {1, 2, 2, 1, 2, 2, 2, 1, 2, 2, 1, 2, 2};
	shadow(x - cell_wr, y - cell_hr, x + cell_wr - 1, y + cell_hr - 1, intense);
	int j = 0;
	for(unsigned i = 0; i < sizeof(points) / sizeof(points[0]); i++)
	{
		j += points[i];
		shadow(x - cell_wr + j,
			y - cell_hr - 1 - i,
			x + cell_wr - j,
			y - cell_hr - 1 - i,
			intense);
	}
	j = 0;
	for(unsigned i = 1; i < sizeof(points) / sizeof(points[0]); i++)
	{
		j += points[i];
		shadow(x - cell_wr + j,
			y + cell_hr - 1 + i,
			x + cell_wr - j,
			y + cell_hr - 1 + i,
			intense);
	}
}

void draw::rectf(int x1, int y1, int x2, int y2, unsigned char m)
{
	int w1 = iabs(x1 - x2);
	unsigned char* p = ptr(imin(x1, x2), imin(y1, y2));
	int scan_line = width - w1 - 1;
	for(int h = iabs(y1 - y2); h >= 0; h--)
	{
		for(int w = w1; w >= 0; w--)
			*p++ = m;
		p += scan_line;
	}
}

void draw::rectb(int x1, int y1, int x2, int y2, unsigned char m)
{
	line(x1, y1, x2, y1, m);
	line(x1, y2, x2, y2, m);
	line(x1, y1, x1, y2, m);
	line(x2, y1, x2, y2, m);
}

inline void line_v1(unsigned char* p1, unsigned char* p2, unsigned char m, int scan_line)
{
	while(p1 <= p2)
	{
		*p1 = m;
		p1 += scan_line;
	}
}

void draw::pixel(int x, int y, unsigned char a)
{
	if(x >= clipping.x1 && x < clipping.x2 && y >= clipping.y1 && y < clipping.y2)
		*ptr(x, y) = a;
}

void draw::line(int x1, int y1, int x2, int y2, unsigned char m)
{
	if(y1 == y2)
	{
		if(y1 >= clipping.y2 || y1 < clipping.y1)
			return;
		auto n1 = imin(x1, x2);
		auto n2 = imax(x1, x2);
		if(n1 < clipping.x1)
			n1 = clipping.x1;
		if(n2 >= clipping.x2)
			n2 = clipping.x2 - 1;
		if(n1>n2)
			return;
		line_v1(ptr(n1, y1), ptr(n2, y1), m, 1);
	}
	else if(x1 == x2)
	{
		if(x1 >= clipping.x2 || x1 < clipping.x1)
			return;
		auto n1 = imin(y1, y2);
		auto n2 = imax(y1, y2);
		if(n1 < clipping.y1)
			n1 = clipping.y1;
		if(n2 >= clipping.y2)
			n2 = clipping.y2 - 1;
		if(n1>n2)
			return;
		line_v1(ptr(x1, n1), ptr(x1, n2), m, width);
	}
	else
	{
		const int deltaX = iabs(x2 - x1);
		const int deltaY = iabs(y2 - y1);
		const int signX = x1 < x2 ? 1 : -1;
		const int signY = y1 < y2 ? 1 : -1;
		//
		int error = deltaX - deltaY;
		//
		pixel(x2, y2, m);
		while(x1 != x2 || y1 != y2)
		{
			pixel(x1, y1, m);
			const int error2 = error * 2;
			//
			if(error2 > -deltaY)
			{
				error -= deltaY;
				x1 += signX;
			}
			if(error2 < deltaX)
			{
				error += deltaX;
				y1 += signY;
			}
		}
	}
}

void draw::shadow(int x1, int y1, int x2, int y2, int intense)
{
	int w1 = iabs(x1 - x2);
	unsigned char* p = ptr(imin(x1, x2), imin(y1, y2));
	int scan_line = width - w1 - 1;
	for(int h = iabs(y1 - y2); h >= 0; h--)
	{
		unsigned char* p1 = p + w1;
		switch(intense)
		{
		case 1:
			while(p <= p1)
			{
				*p = pallette4[*p];
				p++;
			}
			break;
		case 2:
			while(p <= p1)
			{
				*p = pallette2[pallette4[*p]];
				p++;
			}
			break;
		default:
			while(p <= p1)
			{
				*p = pallette3[*p];
				p++;
			}
			break;
		}
		p += scan_line;
	}
}

inline void sprite_v3(unsigned char* dst, unsigned char* src, int width, int height, int scan)
{
	while(height)
	{
		for(int i = 0; i < width; i++)
			*dst++ = *src++;
		dst += scan;
		height--;
	}
}

inline void sprite_v3m(unsigned char* dst, unsigned char* src, int width, int height, int scan)
{
	while(height)
	{
		for(int i = 0; i < width; i++)
			*dst-- = *src++;
		dst += scan;
		height--;
	}
}

void draw::imager(int x, int y, res::tokens res, int n, int mode)
{
	res::til* p = (res::til*)res::get(res);
	if(!p)
		return;
	if(n >= p->count)
		return;
	switch(mode)
	{
	case 1:
		sprite_v3(ptr(x, y + p->height - 1), (unsigned char*)p + sizeof(res::til) + p->width*p->height*n,
			p->width, p->height, -width - p->width);
		break;
	case 2:
		sprite_v3m(ptr(x + p->width - 1, y), (unsigned char*)p + sizeof(res::til) + p->width*p->height*n,
			p->width, p->height, width + p->width);
		break;
	case 3:
		sprite_v3m(ptr(x + p->width - 1, y + p->height - 1), (unsigned char*)p + sizeof(res::til) + p->width*p->height*n,
			p->width, p->height, -width + p->width);
		break;
	default:
		sprite_v3(ptr(x, y), (unsigned char*)p + sizeof(res::til) + p->width*p->height*n,
			p->width, p->height, width - p->width);
		break;
	}
}

void draw::status(int x1, int y1, int x2, int y2)
{
	status_rect.set(x1, y1, x2, y2);
}

void draw::status(const char* format, ...)
{
	szprintv(status_text, format, xva_start(format));
}

void draw::execute(int id, int param, int param2)
{
	hot::key = 0;
	hot::command = id;
	hot::param = param;
	hot::param2 = param2;
	hot::pressed = false;
}

void draw::setevil(bool value)
{
	evil_interface = value;
}

res::tokens draw::isevil(res::tokens evil, res::tokens good)
{
	return evil_interface ? evil : good;
}

draw::state::state() : font(draw::font), clipping(draw::clipping)
{
}

draw::state::~state()
{
	draw::font = font;
	draw::clipping = clipping;
}

void draw::cursor(res::tokens icn, int id, int ox, int oy)
{
	if(status_rect && status_text[0])
	{
		text(status_rect.x1 + (status_rect.width() - textw(status_text)) / 2,
			status_rect.y1 + (status_rect.height() - texth()) / 2,
			status_text);
	}
	status_rect.clear();
	status_text[0] = 0;
	if(!hot::command)
		image(hot::mouse.x + ox, hot::mouse.y + oy, icn, id, 0);
}

void draw::button(int x, int y, res::tokens res, int id, int normal, int hilite, int pressed, int key, unsigned flags, const char* tips, int param)
{
	static int id_pressed;
	if((flags&Disabled) == 0)
	{
		int i = normal;
		rect rc = res::box(x, y, res, i);
		if(area(rc.x1, rc.y1, rc.x2, rc.y2))
		{
			i = hilite;
			if(hot::pressed)
				i = pressed;
			if(tips)
			{
				if(status_rect)
					status(tips);
				else if(hot::key == MouseRight && hot::pressed)
					szprint(tooltips_text, tips);
			}
			if(hot::key == MouseLeft)
			{
				if(!hot::pressed)
				{
					if(id_pressed==id)
						execute(id, param);
				}
				else
					id_pressed = id;
			}
		}
		if(flags&Checked)
			i = pressed;
		image(x, y, res, i, flags);
		if(key && hot::key == key)
			execute(id, param);
	}
	else
	{
		image(x, y, res, pressed, flags);
		shadow(x, y, x + res::width(res, pressed) - 1, y + res::height(res, pressed) - 1, 2);
	}
}

bool draw::area(int x1, int y1, int x2, int y2)
{
	return hot::mouse.x >= x1 && hot::mouse.x <= x2
		&& hot::mouse.y >= y1 && hot::mouse.y <= y2;
}

void hot::clear()
{
	hot::command = 0;
	hot::param = 0;
	hot::key = 0;
}

int draw::input(bool wait_input)
{
	int i = hot::command;
	int p = hot::param;
	hot::clear();
	if(i)
	{
		hot::key = i;
		hot::param = p;
		return hot::key;
	}
	int id = sys_input(wait_input);
	if(id == 0)
		id = Cancel;
	else if(id == InputTimer)
	{
		colorize();
		counter++;
	}
	return id;
}

bool draw::create(const char* title, unsigned milliseconds, bool fullscreen)
{
	// Set pallette
	auto pal3 = (unsigned char*)res::get(res::PalKB);
	auto pal4 = (unsigned char*)sys_get_pallette();
	for(int i = 0; i < 256; i++)
	{
		int i3 = i * 3;
		int i4 = i * 4;
		pal4[i4 + 2] = pal3[i3 + 0]<<2;
		pal4[i4 + 1] = pal3[i3 + 1] << 2;
		pal4[i4 + 0] = pal3[i3 + 2] << 2;
	}
	// Set colors
	colorize();
	command::execute("initialize");
	// Create Window
	return sys_create(title, milliseconds, fullscreen, bits, width, height);
}

void dlgerr(const char* title, const char* format, ...)
{
}