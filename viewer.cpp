//#include "heroview/bsfield.h"
#include "main.h"

// 1 - water / вода (30)
// 2 - grass / трава (62)
// 3 - snow / снег (62)
// 4 - swamp / болото (62)
// 5 - lava / лава (62)
// 6 - desert / пустыня (62)
// 7 - dirt / грязь (38)
// 8 - canion / каньон (62)
// 9 - beach / пляж (17)

// 1) Каждая местность имеет тайлы границ с пляжем и грязью.
// Причем в одном тайле может быть одновременно и то и другое.
// 2) Вода граничит только с пляжем.
// 3) Границу считать пляжем для: вода, пустыня, каньон.
//
// 4 - Земля сверху. Зеркально по верикали - снизу.
// 4 - Земля сверху и справа. Зеркально по вертикале - снизу и справа. Зеркально по горизонтале - сверху и слева. Зеркально по горизонтале и вертикале - снизу и слева.
// 4 - Земля справа. Зеркально по горизонтале - слева.
// 4 - Земля наискосок сверху-справа. Зеркально по аналогично.
// Блок земли (16). Блок песка (16). Блок земли и песка (6). Блок середины (8+8+8=24). Итого 62 на каждую местность.
// 8 - Это тайлы середины
// 8 - Это тайлы середины высоких объектов (холмы)
// 8 - Это тайлы середины низких объектов (цветы, трещинки)

struct terrain
{
	int			first;
	int			last;
	tokens		id;
};
static terrain tarrains[LastTerrain - FirstTerrain + 1] = {
	{0, 29, Water},
	{30, 91, Grass},
	{92, 145, Snow},
	{146, 207, Swamp},
	{208, 261, Lava},
	{262, 320, Desert},
	{321, 360, Dirt},
	{361, 414, Wastelands},
	{415, 431, Beach},
};

const char*	rsname(int res);
void		tilesetsview();

const char* strstr(const char* s1, const char* s2)
{
	auto n = zlen(s2) - 1;
	if(n <= 0)
		return 0;
	while(true)
	{
		auto p = zchr(s1, s2[0]);
		if(!p)
			return 0;
		if(memcmp(p + 1, s2 + 1, n) == 0)
			return p;
		s1 = p + 1;
	}
}

static int res_read(res::tokens* resdata)
{
	int count = 0;
	for(res::tokens i = res::FntBIGFONT; i < res::Empthy; i = (res::tokens)(i + 1))
	{
		const char* fn = rsname(i);
		if(!fn)
			continue;
		const char* ex = szext(fn);
		if(ex[0] != 'I'
			|| ex[1] != 'C'
			|| ex[2] != 'N')
			continue;
		if(resdata)
			resdata[count] = (res::tokens)i;
		count++;
	}
	return count;
}

static int find_index(res::tokens* resdata, int count, res::tokens icn)
{
	for(int i = 0; i < res::Empthy; i++)
	{
		if(resdata[i] == icn)
			return i;
	}
	return 0;
}

static void mapview()
{
	const int mx = 10;
	int offset = 0;
	while(true)
	{
		draw::rectf(0, 0, draw::width - 1, draw::height - 1, 0x12);
		for(int y = 0; y < 16; y++)
		{
			for(int x = 0; x < mx; x++)
			{
				int x1 = x * 32;
				int y1 = y * 32;
				draw::imager(x1, y1, res::TisGROUND32, offset + y*mx + x, 0);
			}
		}
		draw::cursor(res::ADVMCO, 0);
		char temp[32];
		szprint(temp, "%1i", offset + (hot::mouse.y / 32)*mx + (hot::mouse.x / 32));
		draw::text(600, 0, temp);
		int id = draw::input();
		switch(id)
		{
		case KeyEscape:
		case Cancel:
			return;
		case KeyUp:
			if(offset)
				offset -= mx;
			break;
		case KeyDown:
			offset += mx;
			break;
		}
	}
}

static void palview()
{
	while(true)
	{
		int index = -1;
		draw::rectf(0, 0, draw::width - 1, draw::height - 1, 0x12);
		for(int i = 0; i < 256; i++)
		{
			rect rc = {(i % 16) * 16 + 10, (i / 16) * 16 + 10};
			rc.x2 = rc.x1 + 14;
			rc.y2 = rc.y1 + 14;
			draw::rectf(rc.x1, rc.y1, rc.x2, rc.y2, i);
			if(hot::mouse.in(rc))
				index = i;
		}
		if(index != -1)
		{
			draw::state push;
			draw::font = res::SMALFONT;
			char temp[32];
			sznum(temp, index, 2, 0, 16);
			draw::text(320, 10, temp);
		}
		draw::cursor(res::ADVMCO, 0);
		int id = draw::input();
		switch(id)
		{
		case KeyEscape:
		case Cancel:
			return;
		}
	}
}

static void shadowview()
{
	while(true)
	{
		draw::rectf(0, 0, draw::width - 1, draw::height - 1, 0x12);
		auto x = 200;
		auto y = 200;
		auto index = 96;
		auto icn = res::OBJNTWSH;
		draw::line(x - 32, y, x + 32, y, 0x0C);
		draw::line(x, y - 32, x, y + 32, 0x0C);
		x -= 2 * 32;
		y -= 3 * 32;
		for(int iy = 0; iy < 4; iy++)
		{
			for(int ix = -2; ix <= 1; ix++)
			{
				int x1 = x + ix * 32;
				int y1 = y + iy * 32;
				if(iy == 3)
					x1 += 32;
				draw::image(x1, y1, res::OBJNTWSH, index + iy * 4 + (ix + 2));
			}
		}
		icn = res::OBJNTOWN;
		draw::image(x + 2 * 32, y, res::OBJNTOWN, index);
		for(int ii = 0; ii < 5; ++ii)
			draw::image(x + ii * 32, y + 1 * 32, icn, index + 1 + ii);
		for(int ii = 0; ii < 5; ++ii)
			draw::image(x + ii * 32, y + 2 * 32, icn, index + 6 + ii);
		for(int ii = 0; ii < 5; ++ii)
			draw::image(x + ii * 32, y + 3 * 32, icn, index + 11 + ii);
		draw::cursor(res::ADVMCO, 0);
		int id = draw::input();
		switch(id)
		{
		case KeyEscape:
		case Cancel:
			return;
		}
	}
}

static int search_image()
{
	static struct resources_list : public list
	{

		bool	need_update;
		int		data[res::Empthy];
		char    name[32];

		static int compare(const void* p1, const void* p2)
		{
			return strcmp(rsname(*((int*)p1)), rsname(*((int*)p2)));
		}

		void prerender()
		{
			if(!need_update)
				return;
			maximum = 0;
			origin = 0;
			memset(data, 0, sizeof(data));
			for(res::tokens i = res::FntBIGFONT; (int)i < res::Empthy; i = res::tokens((int)i + 1))
			{
				const char* p = rsname(i);
				if(!strstr(p, ".ICN"))
					continue;
				if(strstr(p, name))
					data[maximum++] = i;
			}
			qsort(data, maximum, sizeof(data[0]), compare);
			need_update = false;
		}

		void row(int x, int y, int index) const
		{
			draw::state push;
			draw::font = res::SMALFONT;
			char temp[48];
			int id = data[index];
			zcpy(temp, rsname(id));
			*((char*)strstr(temp, ".ICN")) = 0;
			y += 6;
			x += 4;
			draw::text(x, y, temp);
			sznum(temp, res::getcount(res::tokens(id)));
			draw::text(x + 150, y, temp);
		}

	} resources;
	draw::screenshoot surface;
	char temp[260];
	int w1 = res::width(res::LISTBOXS, 0);
	int x = draw::width - w1;
	int y = 0;
	resources.need_update = true;
	while(true)
	{
		surface.restore();
		zcpy(temp, resources.name);
		draw::image(x, y, res::LISTBOXS, 0);
		draw::edit(x + w1 / 2, y + 5, resources.name, sizeof(resources.name));
		if(strcmp(temp, resources.name) != 0)
			resources.need_update = true;
		resources.boxs(x, y + 20, 9);
		draw::cursor(res::ADVMCO, 0);
		int id = draw::input();
		switch(id)
		{
		case Cancel:
		case KeyEscape:
			return -1;
		case KeyEnter:
			return resources.data[resources.current];
		case InputSymbol:
			break;
		}
	}
}

static int view()
{
	res::tokens resdata[res::Empthy];
	memset(resdata, 0, sizeof(resdata));
	int max_resource = res_read(resdata);
	int current_resource = 0;//find_index(resdata, max_resource, res::P_FLAG32);
	int current_frame = 0;
	int mode = 2;
	bool border_image = false;
	unsigned flags = 0;
	while(true)
	{
		char temp[256];
		char temp2[64];
		res::tokens icn = resdata[current_resource];
		int max_frame = res::getcount(icn);
		if(current_frame > max_frame)
			current_frame = max_frame - 1;
		draw::rectf(0, 0, draw::width - 1, draw::height - 1, 0x12);
		int x = draw::width / 2;
		int y = draw::height / 2;
		if(mode == 0)
		{
			x = 0;
			y = 0;
		}
		draw::image(x, y, icn, current_frame, flags | ((mode <= 1) ? AFNoOffset : 0));
		if(border_image)
		{
			draw::line(x - 32, y, x + 32, y, 0xBD);
			draw::line(x, y - 32, x, y + 32, 0xBD);
		}
		zcpy(temp2, rsname(icn));
		if(zfind(temp2, '.') != -1)
			temp2[zfind(temp2, '.')] = 0;
		szprint(temp, "icon \'%1\' is %2i of %3i, frame (%4i of %5i)", temp2, current_resource, max_resource, current_frame, max_frame);
		draw::text(10, 10, temp, -1);
		szprint(temp, "x=%1i, y=%2i, sx=%3i, sy=%4i", res::ox(icn, current_frame), res::oy(icn, current_frame), res::width(icn, current_frame), res::height(icn, current_frame));
		draw::text(10, 30, temp, -1);
		draw::cursor(res::ADVMCO, 0);
		int id = draw::input();
		switch(id)
		{
		case KeyLeft:
			current_frame--;
			break;
		case KeyRight:
			current_frame++;
			break;
		case KeyUp:
			current_resource++;
			current_frame = 0;
			break;
		case Shift + KeyUp:
			current_resource += 50;
			current_frame = 0;
			break;
		case KeyDown:
			current_resource--;
			current_frame = 0;
			break;
		case Shift + KeyDown:
			current_resource -= 50;
			current_frame = 0;
			break;
		case Ctrl + Alpha + 'F':
			draw::execute(Accept);
			break;
		case Alpha + 'G':
			tilesetsview();
			break;
		case Alpha + 'P':
			palview();
			break;
		case Alpha + 'T':
			mapview();
			break;
		case Alpha + 'C':
			mode = (mode + 1) % 3;
			break;
		case Alpha + 'B':
			border_image = !border_image;
			break;
		case Alpha + 'S':
			shadowview();
			break;
		case Alpha + 'M':
			flags ^= AFMirror;
			break;
		case Accept:
			id = search_image();
			if(id != -1)
				current_resource = find_index(resdata, max_resource, res::tokens(id));
			break;
		case Cancel:
			return 0;
		}
		if(current_frame >= max_frame)
			current_frame = max_frame - 1;
		if(current_frame < 0)
			current_frame = 0;
		if(current_resource >= max_resource)
			current_resource = max_resource - 1;
		if(current_resource < 0)
			current_resource = 0;
	}
}

void util_main();

int main()
{
	draw::font = res::FONT;
	util_main();
	draw::create("Heroes II - Resource Viewer", 200, false);
	view();
	return 0;
}

int __stdcall WinMain(void* ci, void* pi, char* cmd, int sw)
{
	return main();
}