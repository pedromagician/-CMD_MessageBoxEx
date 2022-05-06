#include "stdafx.h"
#include "MessageBoxEx.h"
#include "Conversion.h"
#include "Monitors.h"

HFONT	MessageBoxEx::mhFont		= nullptr;
HWND	MessageBoxEx::mhWndParent	= nullptr;
HWND	MessageBoxEx::mhWndPrompt	= nullptr;
HWND	MessageBoxEx::mhWndMessageBoxEx	= nullptr;
HWND	MessageBoxEx::mhWnd1		= nullptr;
HWND	MessageBoxEx::mhWnd2		= nullptr;
HWND	MessageBoxEx::mhWnd3		= nullptr;
HWND	MessageBoxEx::mhWndIcon		= nullptr;
HBRUSH	MessageBoxEx::mhbrBkgnd		= nullptr;
int		MessageBoxEx::mResultFromButtons = 0;
HBITMAP	MessageBoxEx::mIcon			= nullptr;

int		MessageBoxEx::width					= 600;
int		MessageBoxEx::fontSize				= 22;
int		MessageBoxEx::linesOfText			= 1;
wstring	MessageBoxEx::fontName				= _T("Consolas");

wstring MessageBoxEx::title					= _T("MessageBoxEx");
wstring MessageBoxEx::prompt				= _T("Message.");
int		MessageBoxEx::buttons				= 1;
int		MessageBoxEx::defaultButton			= 1;
wstring MessageBoxEx::button1				= _T("Yes");
wstring MessageBoxEx::button2				= _T("No");
wstring MessageBoxEx::button3				= _T("Cancel");
bool	MessageBoxEx::center				= false;

wstring MessageBoxEx::iconFile				= _T("");
int		MessageBoxEx::iconSize				= 0;
bool	MessageBoxEx::iconBorder			= false;

pair<bool, wstring>	MessageBoxEx::brush			= pair<bool, wstring>(false, _T("#000000"));
pair<bool, wstring> MessageBoxEx::background	= pair<bool, wstring>(false, _T("#000000"));
pair<bool, wstring> MessageBoxEx::pen			= pair<bool, wstring>(false, _T("#ffffff"));

struct MessageBoxEx::InformationAboutPositionOfMessageBoxEx MessageBoxEx::position;

void MessageBoxEx::SetTextAlignment(HWND _hwnd, int _textAlignment)
{
	LONG_PTR tmp;
	if (GetWindowLongPtr(_hwnd, GWL_STYLE) != _textAlignment) {
		//clean up
		if (_textAlignment == 0) {
			tmp = GetWindowLongPtr(_hwnd, GWL_STYLE);
			tmp = tmp & ~(SS_LEFT);
			SetWindowLongPtr(_hwnd, GWL_STYLE, (LONG_PTR)tmp);
		} else if (_textAlignment == 1) {
			tmp = GetWindowLongPtr(_hwnd, GWL_STYLE);
			tmp = tmp & ~(SS_CENTER);
			SetWindowLongPtr(_hwnd, GWL_STYLE, (LONG_PTR)tmp);
		} else if (_textAlignment == 2) {
			tmp = GetWindowLongPtr(_hwnd, GWL_STYLE);
			tmp = tmp & ~(SS_RIGHT);
			SetWindowLongPtr(_hwnd, GWL_STYLE, (LONG_PTR)tmp);
		}

		//set new text alignment
		if (_textAlignment == 0) {
			tmp = GetWindowLongPtr(_hwnd, GWL_STYLE);
			tmp = tmp | (SS_LEFT);
			SetWindowLongPtr(_hwnd, GWL_STYLE, (LONG_PTR)tmp);
		} else if (_textAlignment == 1) {
			tmp = GetWindowLongPtr(_hwnd, GWL_STYLE);
			tmp = tmp | (SS_CENTER);
			SetWindowLongPtr(_hwnd, GWL_STYLE, (LONG_PTR)tmp);
		} else if (_textAlignment == 2) {
			tmp = GetWindowLongPtr(_hwnd, GWL_STYLE);
			tmp = tmp | (SS_RIGHT);
			SetWindowLongPtr(_hwnd, GWL_STYLE, (LONG_PTR)tmp);
		}
		SetWindowPos(_hwnd, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_DRAWFRAME);
	}
}

LRESULT CALLBACK MessageBoxEx::WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	LOGFONT lfont;
	HINSTANCE hInst = nullptr;

	switch (_message) {
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORSTATIC:
		{
			int r = 0, g = 0, b = 0;
			if (brush.first) {
				if (mhbrBkgnd == nullptr) {
					Conversion::HexToRGB(brush.second, r, g, b);
					mhbrBkgnd = CreateSolidBrush(RGB(r, g, b));
				}
			}

			HDC hdcStatic = (HDC)_wParam;
			if (pen.first) {
				Conversion::HexToRGB(pen.second, r, g, b);
				SetTextColor(hdcStatic, RGB(r, g, b));
			}

			if (background.first) {
				Conversion::HexToRGB(background.second, r, g, b);
				SetBkColor(hdcStatic, RGB(r, g, b));
			}

			return (INT_PTR)mhbrBkgnd;
			break;
		}
		case WM_CREATE: {
			memset(&lfont, 0, sizeof(lfont));
			lstrcpy(lfont.lfFaceName, (LPWSTR)MessageBoxEx::fontName.c_str());
			lfont.lfHeight			= fontSize;
			lfont.lfWeight			= FW_NORMAL;//FW_BOLD;
			lfont.lfItalic			= FALSE;
			lfont.lfCharSet			= DEFAULT_CHARSET;
			lfont.lfOutPrecision	= OUT_DEFAULT_PRECIS;
			lfont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
			lfont.lfQuality			= DEFAULT_QUALITY;
			lfont.lfPitchAndFamily	= DEFAULT_PITCH;
			mhFont = CreateFontIndirect(&lfont);

			hInst = GetModuleHandle(nullptr);

			// buttons
			int buttonX = 0;
			int buttonWidth = fontSize * 120 / 22;
			int buttonHeight = fontSize + 8;
			int buttonY = 10 + fontSize / 2 + fontSize * linesOfText + 10;

			// button 1
			if (MessageBoxEx::center) {
				if (MessageBoxEx::buttons == 1)
					buttonX = MessageBoxEx::width / 2 - buttonWidth / 2;
				else if (MessageBoxEx::buttons == 2)
					buttonX = MessageBoxEx::width / 2 - (buttonWidth * 2 + 20) / 2 - 10;
				else
					buttonX = MessageBoxEx::width / 2 - (buttonWidth * 3 + 40) / 2 - 5;
			}
			else {
				if (MessageBoxEx::buttons == 1)
					buttonX = MessageBoxEx::width - 25 - buttonWidth * 1 - 10;
				else if (MessageBoxEx::buttons == 2)
					buttonX = MessageBoxEx::width - 25 - buttonWidth * 2 - 10 - 20;
				else
					buttonX = MessageBoxEx::width - 25 - buttonWidth * 3 - 10 - 20 * 2;
			}

			mhWnd1 = CreateWindowEx(WS_EX_STATICEDGE, _T("Button"), MessageBoxEx::button1.c_str(), WS_VISIBLE | WS_CHILD | WS_TABSTOP, buttonX, buttonY, buttonWidth, buttonHeight, _hWnd, nullptr, hInst, nullptr);
			if (mhWnd1 == nullptr) return (LRESULT)nullptr;
			SendMessage((mhWnd1), WM_SETFONT, (WPARAM)mhFont, 0);

			// button 2
			if (MessageBoxEx::buttons >= 2) {
				if (MessageBoxEx::center) {
					if (MessageBoxEx::buttons == 2)
						buttonX = MessageBoxEx::width / 2 - (buttonWidth * 2 + 20) / 2 + buttonWidth + 20 - 10;
					else
						buttonX = MessageBoxEx::width / 2 - (buttonWidth * 3 + 40) / 2 + (buttonWidth + 20) * 1 - 5;
				}
				else {
					if (MessageBoxEx::buttons == 2)
						buttonX = MessageBoxEx::width - 25 - buttonWidth * 1 - 10;
					else
						buttonX = MessageBoxEx::width - 25 - buttonWidth * 2 - 10 - 20;
				}

				mhWnd2 = CreateWindowEx(WS_EX_STATICEDGE, _T("Button"), MessageBoxEx::button2.c_str(), WS_VISIBLE | WS_CHILD | WS_TABSTOP, buttonX, buttonY, buttonWidth, buttonHeight, _hWnd, nullptr, hInst, nullptr);
				if (mhWnd2 == nullptr) return (LRESULT)nullptr;
				SendMessage((mhWnd2), WM_SETFONT, (WPARAM)mhFont, 0);
			}

			// button 3
			if (MessageBoxEx::buttons == 3) {
				if (MessageBoxEx::center)
					buttonX = MessageBoxEx::width / 2 - (buttonWidth * 3 + 40) / 2 + (buttonWidth + 20) * 2 - 5;
				else
					buttonX = MessageBoxEx::width - 25 - buttonWidth * 1 - 10;

				mhWnd3 = CreateWindowEx(WS_EX_STATICEDGE, _T("Button"), MessageBoxEx::button3.c_str(), WS_VISIBLE | WS_CHILD | WS_TABSTOP, buttonX, buttonY, buttonWidth, buttonHeight, _hWnd, nullptr, hInst, nullptr);
				if (mhWnd3 == nullptr) return (LRESULT)nullptr;
				SendMessage((mhWnd3), WM_SETFONT, (WPARAM)mhFont, 0);
			}

			// icon
			if (MessageBoxEx::iconFile.empty() == false) {
				if (MessageBoxEx::iconSize == 0) MessageBoxEx::iconSize = 48;
				MessageBoxEx::mIcon = (HBITMAP)LoadImage(hInst, MessageBoxEx::iconFile.c_str(), IMAGE_BITMAP, MessageBoxEx::iconSize, MessageBoxEx::iconSize, LR_LOADFROMFILE);
				if (MessageBoxEx::mIcon) {
					if (MessageBoxEx::iconBorder)	mhWndIcon = CreateWindow(_T("Static"), NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP | WS_BORDER	, 10, 10, MessageBoxEx::iconSize, MessageBoxEx::iconSize, _hWnd, NULL, hInst, NULL);
					else							mhWndIcon = CreateWindow(_T("Static"), NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP 				, 10, 10, MessageBoxEx::iconSize, MessageBoxEx::iconSize, _hWnd, NULL, hInst, NULL);
					SendMessage(mhWndIcon, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)MessageBoxEx::mIcon);
				}
				else {
					//MessageBoxEx::iconSize = 0;
					//MessageBoxEx::iconBorder = false;
					mhWndIcon = CreateWindow(_T("Static"), NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP | WS_BORDER, 10, 10, MessageBoxEx::iconSize, MessageBoxEx::iconSize, _hWnd, NULL, hInst, NULL);
				}
			}
			else {
				MessageBoxEx::iconSize = 0;
				MessageBoxEx::iconBorder = false;
			}

			// message
			int messX = 5;
			int messY = 10;
			int messWidth = MessageBoxEx::width - 30;
			int messHeight = fontSize * linesOfText;

			if (MessageBoxEx::iconSize > 0) {
				messX = messX + MessageBoxEx::iconSize + 15;
				messWidth = messWidth - MessageBoxEx::iconSize - 15;
			}

			mhWndPrompt = CreateWindowEx(NULL, _T("Static"), _T(""), WS_VISIBLE | WS_CHILD, messX, messY, messWidth, messHeight, _hWnd, nullptr, hInst, nullptr);
			if (mhWndPrompt == nullptr) return (LRESULT)nullptr;
			SendMessage((mhWndPrompt), WM_SETFONT, (WPARAM)mhFont, 0);

			// position
			RECT dialogRect;
			GetWindowRect(_hWnd, &dialogRect);

			bool monitor = true;
			RECT monitorSize = { 0 };

			switch (MessageBoxEx::position.monitor) {
			case _PRIMARY:
				monitor = Monitors::GetMonitorInfoPrimary(monitorSize);
				break;
			case _MOUSE:
				monitor = Monitors::GetMonitorInfoMouse(monitorSize);
				break;
			case _MOUSE_POINTER:
				monitor = Monitors::GetMonitorInfoMouse(monitorSize);
				MessageBoxEx::position.type = _POINTER;
				break;
			case _ID:
				monitor = Monitors::GetMonitorInfoId(MessageBoxEx::position.id, monitorSize);
				break;
			}

			if (MessageBoxEx::position.type == _POINTER) {
				RECT mouseMonitorSize = { 0 };
				monitor = Monitors::GetMonitorInfoMouse(mouseMonitorSize);
				if (EqualRect(&mouseMonitorSize, &monitorSize) == false) {
					MessageBoxEx::position.type = _CENTER;
				}
			}

			long x = 0;
			long y = 0;
			if (monitor) {
				switch (MessageBoxEx::position.type) {
				case _CENTER: {
					x = GetDiameterX(monitorSize) - GetWidth(dialogRect) / 2;
					y = GetDiameterY(monitorSize) - GetHeight(dialogRect) / 2;
					break;
				}
				case _XY: {
					x = monitorSize.left;
					y = monitorSize.top;
					break;
				}
				case _TOP_CENTER: {
					x = GetDiameterX(monitorSize) - GetWidth(dialogRect) / 2;
					y = monitorSize.top;
					break;
				}
				case _BOTTOM_CENTER: {
					x = GetDiameterX(monitorSize) - GetWidth(dialogRect) / 2;
					y = monitorSize.bottom - GetHeight(dialogRect);
					break;
				}
				case _LEFT_CENTER: {
					x = monitorSize.left;
					y = GetDiameterY(monitorSize) - GetHeight(dialogRect) / 2;
					break;
				}
				case _RIGHT_CENTER: {
					x = monitorSize.right - GetWidth(dialogRect);
					y = GetDiameterY(monitorSize) - GetHeight(dialogRect) / 2;
					break;
				}
				case _POINTER: {
					POINT mouse;
					::GetCursorPos(&mouse);

					x = mouse.x - GetWidth(dialogRect) / 2;
					y = mouse.y - GetHeight(dialogRect) / 2;

					break;
				}
				default:
					wcout << _T("Error - unknown position: ") + to_wstring(MessageBoxEx::position.type) << endl;
					MessageBoxEx::position.type = _CENTER;
					monitor = Monitors::GetMonitorInfoPrimary(monitorSize);
					x = GetDiameterX(monitorSize) - GetWidth(dialogRect) / 2;
					y = GetDiameterY(monitorSize) - GetHeight(dialogRect) / 2;
					break;
				}
			}
			else {
				wcout << _T("Error - problem loading information from the monitor") << endl;
				MessageBoxEx::position.monitor = _PRIMARY;
				MessageBoxEx::position.type = _XY;
				x = monitorSize.left;
				y = monitorSize.top;
			}

			x += MessageBoxEx::position.delta.x;
			y += MessageBoxEx::position.delta.y;

			SetWindowPos(_hWnd, HWND_TOPMOST, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_SHOWWINDOW);

			break;
		}
		case WM_DESTROY: {
			DeleteObject(mhFont);
			EnableWindow(mhWndParent, TRUE);
			SetForegroundWindow(mhWndParent);
			DestroyWindow(_hWnd);
			PostQuitMessage(0);
			break;
		}
		case WM_COMMAND: {
			switch (HIWORD(_wParam)) {
				case BN_CLICKED:
					if ((HWND)_lParam == mhWnd1) {
						MessageBoxEx::mResultFromButtons = 1;
						PostMessage(mhWndMessageBoxEx, WM_KEYDOWN, VK_RETURN, 0);
					}
					if ((HWND)_lParam == mhWnd2) {
						MessageBoxEx::mResultFromButtons = 2;
						PostMessage(mhWndMessageBoxEx, WM_KEYDOWN, VK_RETURN, 0);
					}
					if ((HWND)_lParam == mhWnd3) {
						MessageBoxEx::mResultFromButtons = 3;
						PostMessage(mhWndMessageBoxEx, WM_KEYDOWN, VK_RETURN, 0);
					}
					break;
			}
			break;
		}
		default: {
			return DefWindowProc(_hWnd, _message, _wParam, _lParam);
		}
	}
	return 0;
}

bool MessageBoxEx::MessageBox(int& _result)
{
	mhWndParent = GetActiveWindow();
	if (!mhWndParent)
		mhWndParent = GetForegroundWindow();
	if (!mhWndParent)
		mhWndParent = GetDesktopWindow();

	RECT rc;
	GetWindowRect(mhWndParent, &rc);

	HINSTANCE hInst = GetModuleHandle(nullptr);
	WNDCLASSEX wcex;
	if (!GetClassInfoEx(hInst, _T("MessageBoxEx"), &wcex)) {
		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = (WNDPROC)WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInst;
		wcex.hIcon = nullptr;
		wcex.hCursor = nullptr;
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = _T("MessageBoxEx");
		wcex.hIconSm = nullptr;

		if (RegisterClassEx(&wcex) == 0) {
			wcout << _T("Error - register class") << endl;
			return false;
		}
	}

	// window
	int buttonY = 10 + fontSize / 2 + fontSize * linesOfText + 10;
	int buttonHeight = fontSize + 8;
	mhWndMessageBoxEx = CreateWindowEx(
		WS_EX_DLGMODALFRAME, _T("MessageBoxEx"), title.c_str(), 
		WS_POPUPWINDOW | WS_CAPTION | WS_TABSTOP | WS_VISIBLE, 
		(rc.right - MessageBoxEx::width) / 2, (rc.bottom - MessageBoxEx::width / 2) / 2, 
		MessageBoxEx::width, 50 + buttonY + buttonHeight,
		mhWndParent, nullptr, nullptr, nullptr
	);
	if (mhWndMessageBoxEx == nullptr) {
		return false;
	}

	SetTextAlignment(mhWndPrompt, SS_LEFT);
	SetWindowText(mhWndPrompt, prompt.c_str());
	SetForegroundWindow(mhWndMessageBoxEx);

	SendMessage((HWND)mhWnd1, BM_SETSTYLE, (WPARAM)LOWORD(BS_PUSHBUTTON), MAKELPARAM(TRUE, 0));
	SendMessage((HWND)mhWnd2, BM_SETSTYLE, (WPARAM)LOWORD(BS_PUSHBUTTON), MAKELPARAM(TRUE, 0));
	SendMessage((HWND)mhWnd3, BM_SETSTYLE, (WPARAM)LOWORD(BS_PUSHBUTTON), MAKELPARAM(TRUE, 0));
	
	// default button
	{
		if (MessageBoxEx::defaultButton < 0 || MessageBoxEx::defaultButton > MessageBoxEx::buttons) MessageBoxEx::defaultButton = 1;

		if (MessageBoxEx::defaultButton == 1)		SetFocus(mhWnd1);
		else if (MessageBoxEx::defaultButton == 2)	SetFocus(mhWnd2);
		else if (MessageBoxEx::defaultButton == 3)	SetFocus(mhWnd3);

		MessageBoxEx::mResultFromButtons = MessageBoxEx::defaultButton;
	}

	EnableWindow(mhWndParent, FALSE);
	ShowWindow(mhWndMessageBoxEx, SW_SHOW);
	UpdateWindow(mhWndMessageBoxEx);

	BOOL ret = false;
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (msg.message == WM_KEYDOWN) {
			if (msg.wParam == VK_ESCAPE) {
				SendMessage(mhWndMessageBoxEx, WM_DESTROY, 0, 0);
				MessageBoxEx::mResultFromButtons = 0;
				ret = false;
			} else if (msg.wParam == VK_RETURN) {
				SendMessage(mhWndMessageBoxEx, WM_DESTROY, 0, 0);
				ret = true;
			} else if (msg.wParam == VK_TAB) {
				HWND hWndFocused = GetFocus();
				if (hWndFocused == mhWnd1) {
					if (MessageBoxEx::buttons > 1) {
						SetFocus(mhWnd2);
						MessageBoxEx::mResultFromButtons = 2;
					}
				}
				if (hWndFocused == mhWnd2) {
					if (MessageBoxEx::buttons == 2) {
						SetFocus(mhWnd1);
						MessageBoxEx::mResultFromButtons = 1;
					}
					else {
						SetFocus(mhWnd3);
						MessageBoxEx::mResultFromButtons = 3;
					}
				}
				if (hWndFocused == mhWnd3) {
					SetFocus(mhWnd1);
					MessageBoxEx::mResultFromButtons = 1;
				}
			}
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	_result = MessageBoxEx::mResultFromButtons;
	if (MessageBoxEx::mIcon) {
		DeleteObject(MessageBoxEx::mIcon);
		MessageBoxEx::mIcon = nullptr;
	}

	return ret;
}

long MessageBoxEx::GetDiameterX(RECT _rect)
{
	return _rect.left + (_rect.right - _rect.left) / 2;
}

long MessageBoxEx::GetDiameterY(RECT _rect)
{
	return _rect.top + (_rect.bottom - _rect.top) / 2;
}

long MessageBoxEx::GetWidth(RECT _rect)
{
	return (_rect.right - _rect.left);
}

long MessageBoxEx::GetHeight(RECT _rect)
{
	return (_rect.bottom - _rect.top);
}
