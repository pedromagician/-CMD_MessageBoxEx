#include "conversion.h"
#include <codecvt>

#define WHITE_SPACE_CHARACTERS _T(" \t\n\v\f\r ")

int Conversion::ToInt(const wstring& _buff, int _base)
{
	return _tcstol(_buff.c_str(), nullptr, _base);
}

wstring Conversion::LeftTrimString(wstring _str, const wstring& _val)
{
	wstring::size_type pos = _str.find_first_not_of(_val);
	if (pos != wstring::npos)
		_str.erase(0, pos);
	else
		_str.erase(_str.begin(), _str.end());

	return _str;
}

wstring Conversion::RightTrimString(wstring _str, const wstring& _val)
{
	wstring::size_type pos = _str.find_last_not_of(_val);
	if (pos != wstring::npos)
		_str.erase(pos + 1);
	else
		_str.erase(_str.begin(), _str.end());

	return _str;
}

wstring Conversion::TrimString(wstring _str, const wstring& _val)
{
	wstring::size_type pos = _str.find_last_not_of(_val);
	if (pos != wstring::npos)
	{
		_str.erase(pos + 1);
		pos = _str.find_first_not_of(_val);
		if (pos != wstring::npos)
			_str.erase(0, pos);
	}
	else
		_str.erase(_str.begin(), _str.end());

	return _str;
}

wstring Conversion::TrimWhiteChar(const wstring& _val)
{
	return TrimString(_val, WHITE_SPACE_CHARACTERS);
}

wstring Conversion::ToLower(wstring _val)
{
	transform(_val.begin(), _val.end(), _val.begin(), [](wchar_t c) { return towlower(c); });
	return _val;
}

void Conversion::StringReplaceAll(wstring& _mess, const wstring& _oldStr, const wstring& _newStr)
{
	const size_t oldLen = _oldStr.length();
	const size_t newLen = _newStr.length();
	size_t position = 0;
	while ((position = _mess.find(_oldStr, position)) != string::npos)
	{
		_mess.replace(position, oldLen, _newStr);
		position += newLen;
	}
}

void Conversion::HexToRGB(wstring _hex, int& _r, int& _g, int& _b)
{
	_hex = TrimWhiteChar(_hex);
	_hex = LeftTrimString(_hex, _T("#"));

	wstring sr = _T("ff"), sg = _T("ff"), sb = _T("ff");
	if (_hex.length() == 6) {
		sr = _hex.substr(0, 2);
		sg = _hex.substr(2, 2);
		sb = _hex.substr(4, 2);
	}
	else if (_hex.length() == 3) {
		sr = _hex.substr(0, 1);
		sg = _hex.substr(1, 1);
		sb = _hex.substr(2, 1);
	}
	else {
		wcout << _T("Error - bad color") << endl;
	}
	_r = ToInt(sr, 16);
	_g = ToInt(sg, 16);
	_b = ToInt(sb, 16);
}

void Conversion::UnicodeCodeConverter(wstring& _mess)
{
	size_t startPos = 0, endPos = 0;

	bool unicodeProcess = true;
	while (unicodeProcess)
	{
		startPos = _mess.find(_T("\\u{"), startPos);
		if (startPos == string::npos) break;
		if (startPos != 0) {
			if (_mess[startPos - 1] == '\\') {
				wstring left = _mess.substr(0, startPos);
				wstring right = _mess.substr(startPos + 1, _mess.length() - startPos - 1);
				_mess = left + right;
				continue;
			}
		}

		endPos = _mess.find(_T("}"), startPos);
		if (endPos == string::npos) break;

		size_t tmpPos = _mess.find(_T("\\u{"), startPos + 1);
		if (tmpPos < endPos) {
			startPos = endPos;
		}
		else {
			if (startPos < endPos) {
				wstring before = _mess.substr(0, startPos);
				wstring unicode = Conversion::TrimWhiteChar(_mess.substr(startPos + 3, endPos - startPos - 3));
				wstring after = _mess.substr(endPos + 1, _mess.length() - endPos - 1);

				if (unicode.empty() == false) {
					if (unicode.length() % 2 != 0)
						unicode = _T("0") + unicode;

					char32_t ch32 = (int)_tcstol(unicode.c_str(), nullptr, 16);
					wstring_convert<codecvt_utf8<char32_t>, char32_t> converter_32_8;
					string utf8String = converter_32_8.to_bytes(static_cast<char32_t>(ch32));

					wstring_convert<codecvt_utf8_utf16<wchar_t>> convert_8_16;
					wstring utf16String = convert_8_16.from_bytes(utf8String);
					_mess = before + utf16String + after;
				}
				else {
					startPos = endPos;
				}
			}
			else {
				startPos = endPos;
			}
		}
	}
}
