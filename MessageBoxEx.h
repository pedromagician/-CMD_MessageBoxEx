#pragma once

class MessageBoxEx
{
public:
	enum MONITOR {
		_PRIMARY,
		_MOUSE,
		_MOUSE_POINTER,
		_ID,
	};

	enum POSITION {
		_CENTER,
		_TOP_CENTER,
		_BOTTOM_CENTER,
		_LEFT_CENTER,
		_RIGHT_CENTER,
		_XY,
		_POINTER,
	};

	static struct InformationAboutPositionOfMessageBoxEx {
		MONITOR		monitor;
		UINT		id;
		POSITION	type;
		POINT		delta;
	} position;

	static int width;
	static int fontSize;
	static int linesOfText;
	static wstring fontName;

	static pair<bool, wstring> brush;
	static pair<bool, wstring> background;
	static pair<bool, wstring> pen;

	static wstring title;
	static wstring prompt;
	static wstring iconFile;
	static int iconSize;
	static bool iconBorder;
	static bool topMost;

	static int buttons;
	static int defaultButton;
	static wstring button1;
	static wstring button2;
	static wstring button3;
	static bool center;

	static bool MessageBox(int& _result);

private:
	static HFONT	mhFont;
	static HWND		mhWndParent;
	static HWND		mhWndPrompt;
	static HWND		mhWndMessageBoxEx;
	static HWND		mhWnd1;
	static HWND		mhWnd2;
	static HWND		mhWnd3;
	static HBRUSH	mhbrBkgnd;
	static int		mResultFromButtons;
	static HWND		mhWndIcon;
	static HBITMAP	mIcon;

	static LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);

	static void SetTextAlignment(HWND _hwnd, int _textAlignment);

	static long GetDiameterX(RECT _rect);
	static long GetDiameterY(RECT _rect);
	static long GetWidth(RECT _rect);
	static long GetHeight(RECT _rect);
};
