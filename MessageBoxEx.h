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

	struct InformationAboutPositionOfMessageBoxEx {
		MONITOR		monitor;
		UINT		id;
		POSITION	type;
		POINT		delta;
	};

	static InformationAboutPositionOfMessageBoxEx& Position() { static InformationAboutPositionOfMessageBoxEx mPosition; return mPosition; };

	static int& Width() { static int mWidth = 600; return mWidth; };
	static int& FontSize() { static int mFontSize = 22; return mFontSize; };
	static int& LinesOfText() { static int mLinesOfText = 1; return mLinesOfText; };
	static wstring& FontName() { static wstring mFontName = _T("Consolas"); return mFontName; };

	static pair<bool, wstring>& Brush() { static pair<bool, wstring> mBrush = pair<bool, wstring>(false, _T("#000000")); return mBrush; };
	static pair<bool, wstring>& Background() { static pair<bool, wstring> mBackground = pair<bool, wstring>(false, _T("#000000")); return mBackground; };
	static pair<bool, wstring>& Pen() { static pair<bool, wstring> mPen = pair<bool, wstring>(false, _T("#ffffff")); return mPen; };

	static wstring& Title() { static wstring mTitle = _T("MessageBoxEx"); return mTitle; };

	static bool& NoTitle() { static bool mNoTitle = false; return mNoTitle; };
	static wstring& Prompt() { static wstring mPrompt = _T("Message."); return mPrompt; };
	static wstring& IconFile() { static wstring mIconFile = _T(""); return mIconFile; };
	static wstring& IconApp() { static wstring mIconApp = _T(""); return mIconApp; };
	static int& IconSize() { static int mIconSize = 0; return mIconSize; };
	static bool& IconBorder() { static bool mIconBorder = false; return mIconBorder; };

	static bool& TopMost() { static bool mTopMost = false; return mTopMost; };
	static bool& BlockParent() { static bool mBlockParent = false; return mBlockParent; };

	static bool& WindowsReturnCode() { static bool mWindowsReturnCode = false; return mWindowsReturnCode; };

	static bool& Quiet() { static bool mQuiet = false; return mQuiet; };

	static int& ButtonsWidth() { static int mButtonsWidth = 0; return mButtonsWidth; };
	static int& Buttons() { static int mButtons = 1; return mButtons; };
	static int& DefaultButton() { static int mDefaultButton = 1; return mDefaultButton; };
	static wstring& Button1() { static wstring mButton1 = _T("Yes"); return mButton1; };
	static wstring& Button2() { static wstring mButton2 = _T("No"); return mButton2; };
	static wstring& Button3() { static wstring mButton3 = _T("Cancel"); return mButton3; };
	static bool& Center() { static bool mCenter = false; return mCenter; };

	static int& TimeUntilEndOfApplication() { static int mTimeUntilEndOfApplication = -1; return mTimeUntilEndOfApplication; };
	static int& MinimumDisplayTime() { static int mMinimumDisplayTime = -1; return mMinimumDisplayTime; };
	static wstring& FileRequiredForCompletion() { static wstring mFileRequiredForCompletion = _T(""); return mFileRequiredForCompletion; };
	static bool& DeleteFileRequiredForCompletion() { static bool mDeleteFileRequiredForCompletion = false; return mDeleteFileRequiredForCompletion; };

	static bool MessageBox(int& _result);

private:
	static LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);

	static void SetTextAlignment(HWND _hwnd, int _textAlignment);

	static long GetDiameterX(RECT _rect);
	static long GetDiameterY(RECT _rect);
	static long GetWidth(RECT _rect);
	static long GetHeight(RECT _rect);
};
