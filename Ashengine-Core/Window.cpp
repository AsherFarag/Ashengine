#include "Window.hpp"

Window* Window::s_MainWindow = nullptr;

Window::Window(const char* a_Title, int a_Width, int a_Height, int a_PixelWidth, int a_PixelHeight)
    : m_ScreenBuffer(a_Width, a_Height)
    , m_TitleBuffer()
    , m_WindowRegion()
    , m_WindowHandle()
    , m_RawWidth(0)
    , m_RawHeight(0)
    , m_PixelWidth(0)
    , m_PixelHeight(0)
{
    PixelColourMap::Init();

    // Retrieve handles for console window.
    m_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    if (m_ConsoleHandle == INVALID_HANDLE_VALUE)
    {
        if (AllocConsole())
        {
            m_IsValid = false;
            return;
        }

        m_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    }

    m_WindowHandle = GetConsoleWindow();

    // Set console font.
    a_PixelWidth = min(a_PixelWidth, static_cast<short>(8));
    a_PixelHeight = min(a_PixelHeight, static_cast<short>(8));
    CONSOLE_FONT_INFOEX FontInfo;
    FontInfo.cbSize = sizeof(FontInfo);
    FontInfo.nFont = 0;
    FontInfo.dwFontSize = { (short)a_PixelWidth, (short)a_PixelHeight };
    FontInfo.FontFamily = FF_DONTCARE;
    FontInfo.FontWeight = FW_NORMAL;
    wcscpy_s(FontInfo.FaceName, L"Terminal");
    SetCurrentConsoleFontEx(m_ConsoleHandle, false, &FontInfo);

    // Get screen buffer info object.
    CONSOLE_SCREEN_BUFFER_INFOEX ScreenBufferInfo;
    ScreenBufferInfo.cbSize = sizeof(ScreenBufferInfo);
    GetConsoleScreenBufferInfoEx(m_ConsoleHandle, &ScreenBufferInfo);

    for (int i = 0; i < 16; ++i)
    {
        COLORREF& ColourRef = ScreenBufferInfo.ColorTable[i];
        Colour SeedColour = PixelColourMap::SeedColours[i];
        ColourRef =
            SeedColour.B << 16 |
            SeedColour.G << 8 |
            SeedColour.R;
    }

    SetConsoleScreenBufferInfoEx(m_ConsoleHandle, &ScreenBufferInfo);

    // Get largest possible window size that can fit on screen.
    COORD LargestWindow = GetLargestConsoleWindowSize(m_ConsoleHandle);

	/*DWORD prev_mode;
	GetConsoleMode(m_ConsoleHandle, &prev_mode);
	SetConsoleMode(m_ConsoleHandle, ENABLE_EXTENDED_FLAGS |
		(prev_mode & ~ENABLE_QUICK_EDIT_MODE));*/

    // If smaller than requested size, exit.
    if (LargestWindow.X < a_Width ||
        LargestWindow.Y < a_Height)
    {
        m_IsValid = false;
        return;
    }

    // Set screen buffer.
    COORD WindowSize = { (short)GetWidth(), (short)GetHeight() };

    // Set window region rect.
    m_WindowRegion.Left = 0;
    m_WindowRegion.Top = 0;
    m_WindowRegion.Right = WindowSize.X - 1;
    m_WindowRegion.Bottom = WindowSize.Y - 1;

    // Set console attributes.
    SetConsoleScreenBufferSize(m_ConsoleHandle, { (short)a_Width, (short)a_Height });
    SetConsoleWindowInfo(m_ConsoleHandle, true, &m_WindowRegion);
    GetConsoleScreenBufferInfoEx(m_ConsoleHandle, &ScreenBufferInfo);
    SetConsoleScreenBufferSize(m_ConsoleHandle, { (short)a_Width, (short)a_Height });

    // Set cursor attributes.
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(m_ConsoleHandle, &CursorInfo);
    CursorInfo.bVisible = false;
    SetConsoleCursorInfo(m_ConsoleHandle, &CursorInfo);

    // Set window attributes.
    SetWindowLong(m_WindowHandle, GWL_STYLE, WS_CAPTION | DS_MODALFRAME | WS_MINIMIZEBOX | WS_SYSMENU);
    SetWindowPos(m_WindowHandle, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW);
    SetTitle(a_Title);

    RECT WindowRect;
    GetWindowRect(m_WindowHandle, &WindowRect);

    m_RawWidth = WindowRect.right - WindowRect.left;
    m_RawHeight = WindowRect.bottom - WindowRect.top;
    m_PixelWidth = m_RawWidth / m_ScreenBuffer.GetWidth();
    m_PixelHeight = m_RawHeight / m_ScreenBuffer.GetHeight();
    m_IsValid = true;


}

void Window::SetTitle(const char* a_Title)
{
    size_t Length = strlen(a_Title) + 1;
    Length = Length > 64 ? 64 : Length;
    mbstowcs_s(nullptr, m_TitleBuffer, Length, a_Title, Length);
    SetConsoleTitleW(m_TitleBuffer);
}

void Window::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, Colour a_Colour)
{
    SetLine(x1, y1, x2, y2, a_Colour);
    SetLine(x2, y2, x3, y3, a_Colour);
    SetLine(x3, y3, x1, y1, a_Colour);
}

void Window::DrawFillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, Colour a_Colour)
{
	auto drawline = [&](int sx, int ex, int ny)
	{ 
		for (int i = sx; i <= ex; i++)
		{
			SetColour(i, ny, a_Colour);
		}
	};

	int t1x, t2x, y, minx, maxx, t1xp, t2xp;
	bool changed1 = false;
	bool changed2 = false;
	int signx1, signx2, dx1, dy1, dx2, dy2;
	int e1, e2;
	// Sort vertices
	if (y1 > y2) { std::swap(y1, y2); std::swap(x1, x2); }
	if (y1 > y3) { std::swap(y1, y3); std::swap(x1, x3); }
	if (y2 > y3) { std::swap(y2, y3); std::swap(x2, x3); }

	t1x = t2x = x1; y = y1;   // Starting points
	dx1 = (int)(x2 - x1);
	if (dx1 < 0)
	{
		dx1 = -dx1; signx1 = -1;
	}
	else
		signx1 = 1;
	dy1 = (int)(y2 - y1);

	dx2 = (int)(x3 - x1);
	if (dx2 < 0)
	{
		dx2 = -dx2; signx2 = -1;
	}
	else
		signx2 = 1;
	dy2 = (int)(y3 - y1);

	if (dy1 > dx1) {   // std::swap values
		std::swap(dx1, dy1);
		changed1 = true;
	}
	if (dy2 > dx2) {   // std::swap values
		std::swap(dy2, dx2);
		changed2 = true;
	}

	e2 = (int)(dx2 >> 1);
	// Flat top, just process the second half
	if (y1 == y2) goto next;
	e1 = (int)(dx1 >> 1);

	for (int i = 0; i < dx1;) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i < dx1) {
			i++;
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) t1xp = signx1;//t1x += signx1;
				else          goto next1;
			}
			if (changed1) break;
			else t1x += signx1;
		}
		// Move line
	next1:
		// process second line until y value is about to change
		while (1) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;//t2x += signx2;
				else          goto next2;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next2:
		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
		drawline(minx, maxx, y);    // Draw line from min to max points found on the y
		// Now increase y
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y == y2) break;

	}
next:
	// Second half
	dx1 = (int)(x3 - x2); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(y3 - y2);
	t1x = x2;

	if (dy1 > dx1) {   // std::swap values
		std::swap(dy1, dx1);
		changed1 = true;
	}
	else changed1 = false;

	e1 = (int)(dx1 >> 1);

	for (int i = 0; i <= dx1; i++) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i < dx1) {
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) { t1xp = signx1; break; }//t1x += signx1;
				else          goto next3;
			}
			if (changed1) break;
			else   	   	  t1x += signx1;
			if (i < dx1) i++;
		}
	next3:
		// process second line until y value is about to change
		while (t2x != x3)
		{
			e2 += dy2;
			while (e2 >= dx2)
			{
				e2 -= dx2;
				if (changed2)
					t2xp = signx2;
				else    
					goto next4;
			}
			if (changed2) 
				break;
			else
				t2x += signx2;
		}
	next4:

		if (minx > t1x) minx = t1x;
		if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x;
		if (maxx < t2x) maxx = t2x;

		drawline(minx, maxx, y);

		if (!changed1)
			t1x += signx1;
		t1x += t1xp;

		if (!changed2)
			t2x += signx2;
		t2x += t2xp;

		y += 1;
		if (y > y3)
			return;
	}
}
