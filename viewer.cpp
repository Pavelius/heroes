#include "main.h"

const char*	rsname(int res);

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
		if(memcmp(p+1, s2+1, n) == 0)
			return p;
		s1 = p + 1;
	}
}

static int res_read(res::tokens* resdata)
{
	int count = 0;
	for(res::tokens i = res::FntBIGFONT; i < res::Empthy; i = (res::tokens)(i+1))
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
		if(resdata[i]==icn)
			return i;
	}
	return 0;
}

static void palview()
{
	while(true)
	{
		int index = -1;
		draw::rectf(0, 0, draw::width - 1, draw::height - 1, 0x12);
		for(int i = 0; i<256; i++)
		{
			rect rc = {(i%16)*16 + 10, (i/16)*16 + 10};
			rc.x2 = rc.x1 + 14;
			rc.y2 = rc.y1 + 14;
			draw::rectf(rc.x1, rc.y1, rc.x2, rc.y2, i);
			if(hot::mouse.in(rc))
				index = i;
		}
		if(index!=-1)
		{
			draw::fontsm push;
			char temp[32];
			sznum(temp, index, 2, 0, 16);
			draw::text(320, 10, temp);
		}
		draw::cursor(res::ADVMCO, 0);
		int id = draw::input();
		switch(id)
		{
		case 0:
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
            return strcmp(rsname(*((int*)p1)),rsname(*((int*)p2)));
        }

        void prerender()
        {
            if(!need_update)
                return;
            maximum = 0;
            origin = 0;
            memset(data, 0, sizeof(data));
            for(res::tokens i=res::FntBIGFONT; (int)i<res::Empthy; i = res::tokens((int)i+1))
            {
                const char* p = rsname(i);
                if(!strstr(p,".ICN"))
                    continue;
                if(strstr(p, name))
                    data[maximum++] = i;
            }
            qsort(data, maximum, sizeof(data[0]), compare);
            need_update = false;
        }

        void row(int x, int y, int index) const
        {
            draw::fontsm push;
            char temp[48];
            int id = data[index];
            zcpy(temp, rsname(id));
            *((char*)strstr(temp,".ICN")) = 0;
            y += 6;
            x += 4;
            draw::text(x,y,temp);
            sznum(temp, res::frames(res::tokens(id)));
            draw::text(x+150,y,temp);
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
	    draw::edit(x+w1/2, y+5, resources.name, sizeof(resources.name));
	    if(strcmp(temp, resources.name)!=0)
            resources.need_update = true;
	    resources.boxs(x, y+20, 9);
		draw::cursor(res::ADVMCO, 0);
		int id = draw::input();
		switch(id)
		{
		case 0:
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
	bool center_image = false;
	bool border_image = false;
	while(true)
	{
		char temp[256];
		char temp2[64];
		res::tokens icn = resdata[current_resource];
		int max_frame = res::frames(icn);
		if(current_frame>max_frame)
			current_frame = max_frame - 1;
		draw::rectf(0, 0, draw::width - 1, draw::height - 1, 0x12);
		int x = (draw::width - res::width(icn, current_frame))/ 2;
		int y = (draw::height - res::height(icn, current_frame)) / 2;
		if(!center_image)
		{
			x = 640 / 2;
			y = 480 / 2 - res::height(icn, current_frame);
		}
		draw::image(x, y, icn, current_frame, AFNoOffset);
		if(border_image)
		{
			draw::line(x-32, y, x+32, y, 0xBD);
			draw::line(x, y - 32, x, y + 32, 0xBD);
		}
        zcpy(temp2, rsname(icn));
        if(zfind(temp2, '.')!=-1)
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
        case Ctrl+Alpha+'F':
            draw::execute(Accept);
            break;
        case Alpha+'P':
            draw::execute(Paladin);
            break;
		case Alpha + 'C':
			center_image = !center_image;
			break;
		case Alpha + 'B':
			border_image = !border_image;
			break;
		case Paladin:
			palview();
			break;
        case Accept:
            id = search_image();
            if(id!=-1)
                current_resource = find_index(resdata, max_resource, res::tokens(id));
            break;
		case 0:
			return 0;
		}
		if(current_frame>=max_frame)
            current_frame = max_frame-1;
		if(current_frame<0)
            current_frame = 0;
		if(current_resource>=max_resource)
            current_resource = max_resource-1;
        if(current_resource<0)
            current_resource = 0;
	}
}

int draw::start()
{
	draw::font = res::FONT;
	draw::create("Heroes II - Resource Viewer", 0, false);
	view();
	return 0;
}