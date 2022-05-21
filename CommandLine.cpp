#include "CommandLine.h"
#include "Conversion.h"
#include "version.h"

CommandLine::CommandLine()
{
	mArguments.clear();
}

CommandLine::~CommandLine()
{
}

void CommandLine::Help()
{
	wcout << VER_PRODUCTNAME_STR << _T(" ") << VER_FILE_VERSION_STR << endl;
	wcout << _T("\t") << VER_FILE_DESCRIPTION_STR << endl << endl;
	wcout << _T("Usage:") << endl;
	wcout << _T("\t") << VER_ORIGINAL_FILENAME_STR << _T(" [OPTIONS]") << endl << endl;
	wcout << _T("Options:") << endl;

	for (const auto& it : mArguments) {
		for (const auto& text : it.text) {
			if (it.type != _TRUE)
				wcout << (_T("	") + text + _T(" xxx")) << endl;
			else
				wcout << (_T("	") + text) << endl;
		}
		wcout << (_T("		") + it.help) << endl;
	}
}

void CommandLine::Add(ARGUMENT_TYPE _type, int _num, ...)
{
	ARGUMENT arg;
	arg.text.clear();
	arg.type = _type;

	int argCount = _num;
	_num += 2; //help & data pointer
	if (_type == _ENUM) _num++; //convert table

	va_list arglist;
	va_start(arglist, _num);
	for (int x = 0; x < argCount; x++) {
		LPCWSTR tmp = va_arg(arglist, LPCWSTR);
		arg.text.push_back(Conversion::ToLower(Conversion::TrimWhiteChar(tmp)));

#ifdef DEBUG
		for (const auto& itArg : mArguments) {
			for (const auto& itParam : itArg.text) {
				if (itParam == Conversion::ToLower(Conversion::TrimWhiteChar(tmp)))
					wcout << _T("Error - the same switch, you will not be able to use it") << endl;
			}
		}
#endif // DEBUG

	}
	arg.help = va_arg(arglist, LPCWSTR);
	arg.pVar = static_cast <void*> (va_arg(arglist, void*));
	if (_type == _ENUM) arg.pTable = static_cast <map<wstring, UINT> *> (va_arg(arglist, void*));
	else arg.pTable = nullptr;

	va_end(arglist);

	mArguments.push_back(arg);
}

int CommandLine::ParseCommandLine(int _argc, _TCHAR* _pArgv[], int& _correctParameters)
{
	_correctParameters = 0;

	for (int i = 1; i < _argc; i++) {
		bool unknown = true;
		for (unsigned int a = 0; a < mArguments.size(); a++) {
			if (find(mArguments[a].text.begin(), mArguments[a].text.end(), Conversion::TrimWhiteChar(_pArgv[i])) != mArguments[a].text.end()) {
				if (mArguments[a].type == _STRING) {
					i++;
					if (i >= _argc) {
						wcout << _T("Error - missing argument: ") + (wstring)_pArgv[i-1] << endl;
						return 1;
					}
					wstring tmp = Conversion::TrimWhiteChar(_pArgv[i]);
					Conversion::StringReplaceAll(tmp, _T("\\n"), _T("\n"));
					*((wstring*)mArguments[a].pVar) = tmp;
					_correctParameters++;
					unknown = false;
					break;
				}
				else if (mArguments[a].type == _TRUE) {
					*((bool*)mArguments[a].pVar) = true;
					_correctParameters++;
					unknown = false;
					break;
				}
				else if (mArguments[a].type == _INT) {
					i++;
					if (i >= _argc) {
						wcout << _T("Error - missing argument: ") + (wstring)_pArgv[i - 1] << endl;
						return 1;
					}
					*((int*)mArguments[a].pVar) = Conversion::ToInt(Conversion::TrimWhiteChar(_pArgv[i]));
					_correctParameters++;
					unknown = false;
					break;
				}
				else if (mArguments[a].type == _COLOR) {
					i++;
					if (i >= _argc) {
						wcout << _T("Error - missing argument: ") + (wstring)_pArgv[i - 1] << endl;
						return 1;
					}
					((pair<bool, wstring>*)mArguments[a].pVar)->first = true;
					((pair<bool, wstring>*)mArguments[a].pVar)->second = Conversion::TrimWhiteChar(_pArgv[i]);
					_correctParameters++;
					unknown = false;
					break;
				}
				else if (mArguments[a].type == _ENUM) {
					i++;
					if (i >= _argc) {
						wcout << _T("Error - missing argument: ") + (wstring)_pArgv[i - 1] << endl;
						return 1;
					}
					wstring key = Conversion::ToLower(Conversion::TrimWhiteChar(_pArgv[i]));
					auto search = mArguments[a].pTable->find(key);
					if (search != mArguments[a].pTable->end()) {
						*((UINT*)mArguments[a].pVar) = search->second;
						unknown = false;
					}
					else {
						wcout << _T("Error - bad argument: ") + mArguments[a].text[0] + _T(" ") + key << endl;
						return 1;
					}
					_correctParameters++;
					break;
				}
				else {
					wcout << _T("Error - unknown type") << endl;
					return 1;
				}
			}
		}
		if (unknown) {
			wcout << _T("Error - unknown argument: ") + (wstring)_pArgv[i] << endl;
			return 1;
		}
	}
	return 0;
}
