#pragma once

#include "stdafx.h"

class CommandLine
{
public:
	enum ARGUMENT_TYPE { _STRING, _TRUE, _INT, _COLOR, _ENUM };
	struct ARGUMENT {
		vector<wstring>		text;
		ARGUMENT_TYPE		type;
		void* pVar;
		map<wstring, UINT>* pTable;
		wstring				help;
	};

	enum Type {
		Ok = MB_OK,
		OkCancel = MB_OKCANCEL,
		YesNo = MB_YESNO,
		YesNoCancel = MB_YESNOCANCEL,
	};

	enum Icon {
		NoIcon = 0x00000000L,
		Information = MB_ICONINFORMATION,
		Question = MB_ICONQUESTION,
		Warning = MB_ICONWARNING,
		Error = MB_ICONERROR,
	};

	enum DefaultButton {
		Def1 = MB_DEFBUTTON1,
		Def2 = MB_DEFBUTTON2,
		Def3 = MB_DEFBUTTON3,
	};

private:
	vector<ARGUMENT> mArguments;

public:
	CommandLine();
	~CommandLine();
	void Add(ARGUMENT_TYPE _type, int _num, ...);

	int ParseCommandLine(int _argc, _TCHAR* _pArgv[], int& _correctParameters);

	void Help();
};
