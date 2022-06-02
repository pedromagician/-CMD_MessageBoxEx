#pragma once

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
		{_T("1"),	1},
		{_T("2"),	2},
		{_T("3"),	3}
	};

	MessageBoxEx::position.monitor	= MessageBoxEx::_PRIMARY;
	MessageBoxEx::position.id		= 0;
	MessageBoxEx::position.type		= MessageBoxEx::_CENTER;
	MessageBoxEx::position.delta	= { 0, 0 };

	CommandLine cmd;
	cmd.Add(CommandLine::_STRING,	2,	_T("-title"),		_T("-t"),				_T("The 'xxx' argument specifies the name of the dialog."),											&MessageBoxEx::title);
	cmd.Add(CommandLine::_STRING,	2,	_T("-message"),		_T("-m"),				_T("The 'xxx' argument specifies the text of the dialog. A new line can be inserted using \\n."),	&MessageBoxEx::prompt);
	cmd.Add(CommandLine::_TRUE,		3,	_T("-help"),		_T("-h"),	_T("-?"),	_T("To view help."),																				&help);
	cmd.Add(CommandLine::_STRING,	2,	_T("-font"),		_T("-f"),				_T("The 'xxx' argument specifies the font of the dialog box."),										&MessageBoxEx::fontName);
	cmd.Add(CommandLine::_INT,		2,	_T("-fontsize"),	_T("-fs"),				_T("The 'xxx' argument specifies the font size of the dialog box."),								&MessageBoxEx::fontSize);
	cmd.Add(CommandLine::_INT,		2,	_T("-lines"),		_T("-l"),				_T("The 'xxx' argument specifies the number of lines of dialogue."),								&MessageBoxEx::linesOfText);
	cmd.Add(CommandLine::_INT,		2,	_T("-width"),		_T("-w"),				_T("The 'xxx' argument specifies the width(px) of the dialog box."),								&MessageBoxEx::width);
	cmd.Add(CommandLine::_COLOR,	2,	_T("-pen"),			_T("-p"),				_T("The 'xxx' argument specifies the colour of the pen."),											&MessageBoxEx::pen);
	cmd.Add(CommandLine::_COLOR,	2,	_T("-background"),	_T("-b"),				_T("The 'xxx' argument specifies the background color."),											&MessageBoxEx::background);
	cmd.Add(CommandLine::_COLOR,	2,	_T("-brush"),		_T("-br"),				_T("The 'xxx' argument determines the brush."),														&MessageBoxEx::brush);
	cmd.Add(CommandLine::_STRING,	2,	_T("-icon"),		_T("-i"),				_T("The 'xxx' argument specifies the icon file. (BMP)"),											&MessageBoxEx::iconFile);
	cmd.Add(CommandLine::_INT,		1,	_T("-iconsize"),							_T("The 'xxx' argument specifies the size of the icon."),											&MessageBoxEx::iconSize);
	cmd.Add(CommandLine::_TRUE,		1,	_T("-iconborder"),							_T("This argument allows you to draw a border around the icon."),									&MessageBoxEx::iconBorder);
	cmd.Add(CommandLine::_ENUM,		1,	_T("-button"),								_T("The argument 'xxx' specifies the number of buttons. Allowed options: 1|2|3."),					&MessageBoxEx::buttons,			&string2buttons);
	cmd.Add(CommandLine::_TRUE,		1,	_T("-center"),								_T("The argument enables button centering."),														&MessageBoxEx::center);
	cmd.Add(CommandLine::_STRING,	1,	_T("-b1"),									_T("The 'xxx' argument specifies the text of the button 1."),										&MessageBoxEx::button1);
	cmd.Add(CommandLine::_STRING,	1,	_T("-b2"),									_T("The 'xxx' argument specifies the text of the button 2."),										&MessageBoxEx::button2);
	cmd.Add(CommandLine::_STRING,	1,	_T("-b3"),									_T("The 'xxx' argument specifies the text of the button 3."),										&MessageBoxEx::button3);
	cmd.Add(CommandLine::_ENUM,		2,	_T("-default"),		_T("-d"),				_T("The 'xxx' argument specifies the default button. Allowed options: 1|2|3."),						&MessageBoxEx::defaultButton,	&string2buttons);
	cmd.Add(CommandLine::_STRING,	2,	_T("-monitor"),		_T("-mon"),				_T("The 'xxx' argument specifies the default monitor. Allowed options: Primary|Mouse|MousePointer|0|1|2|n."),		&monitor);
	cmd.Add(CommandLine::_ENUM,		2,	_T("-position"),	_T("-pos"),				_T("The 'xxx' argument specifies the default position. Allowed options: Center|Top|Bottom|Left|Right|Pointer|xy."), &MessageBoxEx::position.type,	&string2positionType);
	cmd.Add(CommandLine::_INT,		1,	_T("-x"),									_T("The 'xxx' argument specifies the position offset along the X coordinate."),						&MessageBoxEx::position.delta.x);
	cmd.Add(CommandLine::_INT,		1,	_T("-y"),									_T("The 'xxx' argument specifies the position offset along the Y coordinate."),						&MessageBoxEx::position.delta.y);
	cmd.Add(CommandLine::_TRUE,		1,	_T("-topmost"),								_T("This argument places the window above all windows that are not in the highest position.The window retains its highest position even if it is deactivated."),	&MessageBoxEx::topMost);

	if (cmd.ParseCommandLine(argc, argv, correctParameters) != 0) {
		cmd.Help();
		return 0;
	}

	if (help || correctParameters == 0) {
		cmd.Help();
		return 0;
	}

	if (MessageBoxEx::prompt.empty()) {
		wcout << _T("Error - message is emnpty") << endl;
		return 1;
	}

	monitor = Conversion::ToLower(Conversion::TrimWhiteChar(monitor));
	if (monitor == _T("primary"))			MessageBoxEx::position.monitor = MessageBoxEx::_PRIMARY;
	else if (monitor == _T("mouse"))		MessageBoxEx::position.monitor = MessageBoxEx::_MOUSE;
	else if (monitor == _T("mousepointer"))	MessageBoxEx::position.monitor = MessageBoxEx::_MOUSE_POINTER;
	else {
		MessageBoxEx::position.monitor = MessageBoxEx::_ID;
		MessageBoxEx::position.id = (UINT)Conversion::ToInt(monitor);
	}

	int result = 0;
	if (MessageBoxEx::MessageBox(result) != 0)
		wcout << to_wstring(result);

	return 0;
}
