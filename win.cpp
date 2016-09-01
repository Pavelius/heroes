#include "win.h"
#include "main.h"

struct video_8t
{
	BITMAPINFO		bmp;
	unsigned char	bmp_pallette[256*4];
};

static const char*	window_class_name = "UIWindow32";
static HWND			main_window;
static bool			window_fullscreen;
video_8t			video;
extern unsigned char pallette[256][4];

static int tokey(int vk)
{
	switch(vk)
	{
	case VK_CONTROL:
		return Ctrl;
	case VK_MENU:
		return Alt;
	case VK_SHIFT:
		return Shift;
	case VK_LEFT:
		return KeyLeft;
	case VK_RIGHT:
		return KeyRight;
	case VK_UP:
		return KeyUp;
	case VK_DOWN:
		return KeyDown;
	case VK_PRIOR:
		return KeyPageUp;
	case VK_NEXT:
		return KeyPageDown;
	case VK_HOME:
		return KeyHome;
	case VK_END:
		return KeyEnd;
	case VK_BACK:
		return KeyBackspace;
	case VK_DELETE:
		return KeyDelete;
	case VK_RETURN:
		return KeyEnter;
	case VK_ESCAPE:
		return KeyEscape;
	case VK_SPACE:
		return KeySpace;
	case VK_TAB:
		return KeyTab;
	case VK_F1:
		return F1;
	case VK_F2:
		return F2;
	case VK_F3:
		return F3;
	case VK_F4:
		return F4;
	case VK_F5:
		return F5;
	case VK_F6:
		return F6;
	case VK_F7:
		return F7;
	case VK_F8:
		return F8;
	case VK_F9:
		return F9;
	case VK_F10:
		return F10;
	case VK_F11:
		return F11;
	case VK_F12:
		return F12;
	case VK_MULTIPLY:
		return Alpha+(unsigned)'*';
	case VK_DIVIDE:
		return Alpha+(unsigned)'/';
	case VK_ADD:
		return Alpha+(unsigned)'+';
	case VK_SUBTRACT:
		return Alpha+(unsigned)'-';
	case VK_OEM_COMMA:
		return Alpha+(unsigned)',';
	case VK_OEM_PERIOD:
		return Alpha+(unsigned)'.';
	default:
		return Alpha+vk;
	}
}

static void colorize(unsigned char* pallette, int index, int count)
{
	unsigned char b1[4];
	unsigned char* p1 = &pallette[index*4];
	b1[0] = p1[0];
	b1[1] = p1[1];
	b1[2] = p1[2];
	b1[3] = p1[3];
	//
	for(int i = 1; i<count; i++)
	{
		p1[(i-1)*4+0] = p1[i*4+0];
		p1[(i-1)*4+1] = p1[i*4+1];
		p1[(i-1)*4+2] = p1[i*4+2];
		p1[(i-1)*4+3] = p1[i*4+3];
	}
	p1[(count-1)*4+0] = b1[0];
	p1[(count-1)*4+1] = b1[1];
	p1[(count-1)*4+2] = b1[2];
	p1[(count-1)*4+3] = b1[3];
}

static void colorize()
{
	static unsigned tick = clock();
	if(tick<clock())
	{
		colorize(&video.bmp.bmiColors[0].rgbBlue, 0xD6, 4);
		colorize(&video.bmp.bmiColors[0].rgbBlue, 0xDA, 4);
		colorize(&video.bmp.bmiColors[0].rgbBlue, 0xEE, 4);
		colorize(&video.bmp.bmiColors[0].rgbBlue, 0xE7, 4);
		tick += 250;
	}
}

static int handle(HWND hwnd, MSG& msg)
{
    colorize();
	switch(msg.message)
	{
	case WM_MOUSEMOVE:
		if(msg.hwnd!=hwnd)
			break;
		hot::mouse.x = LOWORD(msg.lParam);
		hot::mouse.y = HIWORD(msg.lParam);
		return MouseMove;
	case WM_LBUTTONDOWN:
		if(msg.hwnd!=hwnd)
			break;
		hot::pressed = true;
		return MouseLeft;
	case WM_LBUTTONDBLCLK:
		if(msg.hwnd!=hwnd)
			break;
		hot::pressed = true;
		return MouseLeftDBL;
	case WM_LBUTTONUP:
		if(msg.hwnd!=hwnd)
			break;
		if(!hot::pressed)
			break;
		hot::pressed = false;
		return MouseLeft;
	case WM_RBUTTONDOWN:
		hot::pressed = true;
		return MouseRight;
	case WM_RBUTTONUP:
		hot::pressed = false;
		return MouseRight;
	case WM_MOUSEWHEEL:
		if(msg.wParam&0x80000000)
			return MouseWheelDown;
		else
			return MouseWheelUp;
		break;
	case WM_TIMER:
		if(msg.hwnd!=hwnd)
			break;
		if(msg.wParam==InputTimer)
			return InputTimer;
		break;
	case WM_KEYDOWN:
		return tokey(msg.wParam);
	case WM_CHAR:
		hot::symbol = msg.wParam;
		return InputSymbol;
	case WM_COMMAND:
		if(HIWORD(msg.wParam)==0)
			return msg.lParam;
		break;
	}
	return 0;
}

static LRESULT CALLBACK WndProc(HWND hwnd, unsigned uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_ERASEBKGND:
		SetDIBitsToDevice((void*)wParam, 0, 0, draw::width, draw::height, 0, 0,
			0, draw::height,
			draw::ptr(0, 0), &video.bmp, DIB_RGB_COLORS);
		return 1;
	case WM_CLOSE:
		PostQuitMessage(-1);
		break;
	default:
		return DefWindowProcA(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

bool draw::create(const char* title, int frames_per_second, bool fullscreen)
{

	WNDCLASS    wc;								// Windows Class Structure
	DWORD       dwStyle = WS_CAPTION|WS_SYSMENU|WS_BORDER;	// Windows Style;
	RECT		WindowRect = {0, 0, width, height}; // Grabs Rectangle Upper Left / Lower Right Values
	void*		hInstance = GetModuleHandleA(0);	// Grab An Instance For Our Window

	wc.style = CS_OWNDC|CS_DBLCLKS;		                // Own DC For Window.
	wc.lpfnWndProc = WndProc;							// WndProc Handles Messages
	wc.cbClsExtra = 0;									// No Extra Window Data
	wc.cbWndExtra = 0;									// No Extra Window Data
	wc.hInstance = hInstance;							// Set The Instance
	wc.hIcon = LoadIconA(GetModuleHandleA(0), (const char*)1);			// Load The Default Icon
	wc.hCursor = LoadCursorA(0, IDC_ARROW);				// Load The Arrow Pointer
	wc.hbrBackground = 0;								// No Background Required For GL
	wc.lpszMenuName = 0;								// We Don't Want A Menu
	wc.lpszClassName = window_class_name;				// Set The Class Name

	if(!RegisterClassA(&wc))								// Attempt To Register The Window Class
		return false;									// Return FALSE

	if(fullscreen)										// Attempt Fullscreen Mode?
	{
	//	DEVMODE	dmScreenSettings;						// Device Mode
	//	memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
	//	dmScreenSettings.dmSize = sizeof(dmScreenSettings);		// Size Of The Devmode Structure
	//	dmScreenSettings.dmPelsWidth = width;			// Selected Screen Width
	//	dmScreenSettings.dmPelsHeight = height;			// Selected Screen Height
	//	dmScreenSettings.dmBitsPerPel = 32;						// Selected Bits Per Pixel
	//	dmScreenSettings.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
	//	// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
	//	if(ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
	//		return false;
		dwStyle = WS_POPUP;								// Windows Style
	}

	window_fullscreen = fullscreen;
	AdjustWindowRectEx(&WindowRect, dwStyle, 0, 0);	// Adjust Window To True Requested Size

	// Create The Window
	main_window = CreateWindowExA(0, window_class_name, title, dwStyle,	// Required Window Style
		0x80000000, 0x80000000, WindowRect.right-WindowRect.left, WindowRect.bottom-WindowRect.top,
		0, 0, hInstance, 0);

	if(!main_window)
		return false;								// Return FALSE

	ShowCursor(0);
	if(frames_per_second)
		SetTimer(main_window, InputTimer, 1000/frames_per_second, 0);
	ShowWindow(main_window, SW_SHOW);				// Show The Window
	SetForegroundWindow(main_window);				// Slightly Higher Priority
	SetFocus(main_window);						    // Sets Keyboard Focus To The Window

	memset(&video.bmp, 0, sizeof(video.bmp));
	memcpy(video.bmp.bmiColors, pallette, sizeof(pallette));
	video.bmp.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	video.bmp.bmiHeader.biWidth = draw::width;
	video.bmp.bmiHeader.biHeight = -draw::height;
	video.bmp.bmiHeader.biPlanes = 1;
	video.bmp.bmiHeader.biBitCount = 8;
	video.bmp.bmiHeader.biCompression = 0;
	video.bmp.bmiHeader.biSizeImage = draw::width*draw::height;

	return true;
}

static void update(HWND hwnd)
{
	if(!IsWindowVisible((HWND)hwnd))
		ShowWindow((HWND)hwnd, SW_SHOW);
	// After create window prepare
	if(GetFocus()!=(HWND)hwnd)
		SetFocus((HWND)hwnd);
	InvalidateRect((HWND)hwnd, 0, 1);
	UpdateWindow((HWND)hwnd);
}

static void add_modificators()
{
	if(hot::key >= (int)KeyLeft)
	{
		if(GetKeyState(VK_SHIFT)<0)
			hot::key |= Shift;
		if(GetKeyState(VK_MENU)<0)
			hot::key |= Alt;
		if(GetKeyState(VK_CONTROL)<0)
			hot::key |= Ctrl;
	}
}

int draw::sysinput(bool wait_message)
{
	MSG	msg;
	if(!main_window)
		return 0;
	update(main_window);
	if(wait_message)
	{
		while(GetMessageA(&msg, 0, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
			hot::key = handle(main_window, msg);
			if(hot::key)
			{
				add_modificators();
				return hot::key;
			}
		}
		return 0;
	}
	else
	{
		while(PeekMessageA(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
			hot::key = handle(main_window, msg);
			if(hot::key)
				add_modificators();
		}
		return InputResize;
	}
}

int main()
{
	int result = draw::start();
	//if(window_fullscreen)
	//	ChangeDisplaySettings(0, 0);
	ShowCursor(1);
	return result;
}

int __stdcall WinMain(void* ci, void* pi, char* cmd, int sw)
{
	return main();
}