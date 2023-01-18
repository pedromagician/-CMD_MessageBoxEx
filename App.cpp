Debug theDebug;

#include "stdafx.h"
#include "CommandLine.h"
#include "Conversion.h"
#include "MessageBoxEx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	_setmode(_fileno(stdout), _O_U8TEXT);

	int correctParameters = 0;
	bool help = false;
	wstring monitor = _T("primary");

	map<wstring, UINT> string2positionType{
		{_T("center"),	MessageBoxEx::_CENTER},
		{_T("top"),		MessageBoxEx::_TOP_CENTER},
		{_T("bottom"),	MessageBoxEx::_BOTTOM_CENTER},
		{_T("left"),	MessageBoxEx::_LEFT_CENTER},
		{_T("right"),	MessageBoxEx::_RIGHT_CENTER},
		{_T("pointer"),	MessageBoxEx::_POINTER},
		{_T("xy"),		MessageBoxEx::_XY},
	};

	map<wstring, UINT> string2buttons{
		{_T("0"),	0},
		{_T("1"),	1},
		{_T("2"),	2},
		{_T("3"),	3}
	};

	map<wstring, UINT> defString2buttons{
		{_T("1"),	1},
		{_T("2"),	2},
		{_T("3"),	3}
	};

	MessageBoxEx::TimeUntilEndOfApplication()		= -1;
	MessageBoxEx::MinimumDisplayTime()				= -1;
	MessageBoxEx::FileRequiredForCompletion()		= _T("");
	MessageBoxEx::DeleteFileRequiredForCompletion() = false;

	MessageBoxEx::Position().monitor	= MessageBoxEx::_PRIMARY;
	MessageBoxEx::Position().id			= 0;
	MessageBoxEx::Position().type		= MessageBoxEx::_CENTER;
	MessageBoxEx::Position().delta		= { 0, 0 };

	CommandLine cmd;
	cmd.Add(CommandLine::_STRING,	2,	_T("-title"),		_T("-t"),				_T("The 'xxx' argument specifies the name of the dialog."),											&MessageBoxEx::Title());
	cmd.Add(CommandLine::_TRUE,		2,	_T("-noTitle"),		_T("-nt"),				_T("This argument turns off the border of the dialog."),											&MessageBoxEx::NoTitle());
	cmd.Add(CommandLine::_STRING,	2,	_T("-message"),		_T("-m"),				_T("The 'xxx' argument specifies the text of the dialog. A new line can be inserted using \\n. Inserting Unicode characters \\u{1F44D}."),	&MessageBoxEx::Prompt());
	cmd.Add(CommandLine::_TRUE,		3,	_T("-help"),		_T("-h"),	_T("-?"),	_T("To view help."),																				&help);
	cmd.Add(CommandLine::_STRING,	2,	_T("-font"),		_T("-f"),				_T("The 'xxx' argument specifies the font of the dialog box."),										&MessageBoxEx::FontName());
	cmd.Add(CommandLine::_INT,		2,	_T("-fontsize"),	_T("-fs"),				_T("The 'xxx' argument specifies the font size of the dialog box."),								&MessageBoxEx::FontSize());
	cmd.Add(CommandLine::_INT,		2,	_T("-lines"),		_T("-l"),				_T("The 'xxx' argument specifies the number of lines of dialogue."),								&MessageBoxEx::LinesOfText());
	cmd.Add(CommandLine::_INT,		2,	_T("-width"),		_T("-w"),				_T("The 'xxx' argument specifies the width(px) of the dialog box."),								&MessageBoxEx::Width());
	cmd.Add(CommandLine::_COLOR,	2,	_T("-pen"),			_T("-p"),				_T("The 'xxx' argument specifies the colour of the pen."),											&MessageBoxEx::Pen());
	cmd.Add(CommandLine::_COLOR,	2,	_T("-background"),	_T("-b"),				_T("The 'xxx' argument specifies the background color."),											&MessageBoxEx::Background());
	cmd.Add(CommandLine::_COLOR,	2,	_T("-brush"),		_T("-br"),				_T("The 'xxx' argument determines the brush."),														&MessageBoxEx::Brush());
	cmd.Add(CommandLine::_STRING,	2,	_T("-icon"),		_T("-i"),				_T("The 'xxx' argument specifies the image file. (BMP)"),											&MessageBoxEx::IconFile());
	cmd.Add(CommandLine::_INT,		1,	_T("-iconsize"),							_T("The 'xxx' argument specifies the size of the image."),											&MessageBoxEx::IconSize());
	cmd.Add(CommandLine::_TRUE,		1,	_T("-iconborder"),							_T("This argument allows you to draw a border around the image."),									&MessageBoxEx::IconBorder());
	cmd.Add(CommandLine::_STRING,	1,	_T("-iconapp"),								_T("The 'xxx' argument specifies the icon file for the application. (ICO/BMP)"),					&MessageBoxEx::IconApp());
	cmd.Add(CommandLine::_ENUM,		1,	_T("-button"),								_T("The argument 'xxx' specifies the number of buttons. Allowed options: 0|1|2|3."),				&MessageBoxEx::Buttons(),			&string2buttons);
	cmd.Add(CommandLine::_INT,		1,	_T("-widthbuttons"),						_T("The 'xxx' argument specifies the bonus width(px) of the buttons."),								&MessageBoxEx::ButtonsWidth());
	cmd.Add(CommandLine::_TRUE,		1,	_T("-center"),								_T("The argument enables button centering."),														&MessageBoxEx::Center());
	cmd.Add(CommandLine::_STRING,	1,	_T("-b1"),									_T("The 'xxx' argument specifies the text of the button 1."),										&MessageBoxEx::Button1());
	cmd.Add(CommandLine::_STRING,	1,	_T("-b2"),									_T("The 'xxx' argument specifies the text of the button 2."),										&MessageBoxEx::Button2());
	cmd.Add(CommandLine::_STRING,	1,	_T("-b3"),									_T("The 'xxx' argument specifies the text of the button 3."),										&MessageBoxEx::Button3());
	cmd.Add(CommandLine::_ENUM,		2,	_T("-default"),		_T("-d"),				_T("The 'xxx' argument specifies the default button. Allowed options: 1|2|3."),						&MessageBoxEx::DefaultButton(),		&defString2buttons);
	cmd.Add(CommandLine::_STRING,	2,	_T("-monitor"),		_T("-mon"),				_T("The 'xxx' argument specifies the default monitor. Allowed options: Primary|Mouse|MousePointer|0|1|2|n."),		&monitor);
	cmd.Add(CommandLine::_ENUM,		2,	_T("-position"),	_T("-pos"),				_T("The 'xxx' argument specifies the default position. Allowed options: Center|Top|Bottom|Left|Right|Pointer|xy."), &MessageBoxEx::Position().type,	&string2positionType);
	cmd.Add(CommandLine::_INT,		1,	_T("-x"),									_T("The 'xxx' argument specifies the position offset along the X coordinate."),						&MessageBoxEx::Position().delta.x);
	cmd.Add(CommandLine::_INT,		1,	_T("-y"),									_T("The 'xxx' argument specifies the position offset along the Y coordinate."),						&MessageBoxEx::Position().delta.y);
	cmd.Add(CommandLine::_TRUE,		1,	_T("-topmost"),								_T("This argument places the window above all windows that are not in the highest position.The window retains its highest position even if it is deactivated."),	&MessageBoxEx::TopMost());
	cmd.Add(CommandLine::_TRUE,		1,	_T("-block"),								_T("This argument blocks the parent process that started the InputBox."),							&MessageBoxEx::BlockParent());
	cmd.Add(CommandLine::_TRUE,		2,	_T("-windowsReturnCode"),	_T("-wrc"),		_T("The argument enables the Windows return code."),												&MessageBoxEx::WindowsReturnCode());
	cmd.Add(CommandLine::_TRUE,		1,	_T("-quiet"),								_T("The argument disables output to the command line."),											&MessageBoxEx::Quiet());
	cmd.Add(CommandLine::_INT,		2,	_T("-minimumDisplayTime"),	_T("-mdt"),		_T("The 'xxx' argument specifies the minimum amount of time that the dialog will be displayed."),	&MessageBoxEx::MinimumDisplayTime());
	cmd.Add(CommandLine::_INT,		2,	_T("-timeToCompletion"),	_T("-ttc"),		_T("The 'xxx' argument specifies the time until the dialog is closed."),							&MessageBoxEx::TimeUntilEndOfApplication());
	cmd.Add(CommandLine::_STRING,	3,	_T("-fileRequiredForCompletion"),		_T("-frfc"),	_T("-file"),		_T("The 'xxx' argument specifies the file that, if it exists, will cause the dialog to close."),	&MessageBoxEx::FileRequiredForCompletion());
	cmd.Add(CommandLine::_TRUE,		3,	_T("-deleteFileRequiredForCompletion"), _T("-dfrfc"),	_T("-deleteFile"),	_T("This argument allows you to delete the test file after closing the dialog box."),				&MessageBoxEx::DeleteFileRequiredForCompletion());

	if (cmd.ParseCommandLine(argc, argv, correctParameters) != 0) {
		cmd.Help();
		return 0;
	}

	if (help || correctParameters == 0) {
		cmd.Help();
		return 0;
	}

	if (MessageBoxEx::Prompt().empty()) {
		wcout << _T("Error - message is emnpty") << endl;
		return 0;
	}

	monitor = Conversion::ToLower(Conversion::TrimWhiteChar(monitor));
	if (monitor == _T("primary"))			MessageBoxEx::Position().monitor = MessageBoxEx::_PRIMARY;
	else if (monitor == _T("mouse"))		MessageBoxEx::Position().monitor = MessageBoxEx::_MOUSE;
	else if (monitor == _T("mousepointer"))	MessageBoxEx::Position().monitor = MessageBoxEx::_MOUSE_POINTER;
	else {
		MessageBoxEx::Position().monitor = MessageBoxEx::_ID;
		MessageBoxEx::Position().id = (UINT)Conversion::ToInt(monitor);
	}

	int result = 0;
	if (MessageBoxEx::MessageBox(result)) {
		if (MessageBoxEx::WindowsReturnCode()) {
			if (result == 1) result = IDYES;
			else if (result == 2) result = IDNO;
			else if (result == 3) result = IDCANCEL;
		}

		if (MessageBoxEx::Quiet() == false)
			wcout << to_wstring(result);
	}

	return result;
}
