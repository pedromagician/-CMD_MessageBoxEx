#pragma once

#include "stdafx.h"

class Monitors
{
public:
	vector<size_t>		iMonitors;
	vector<HMONITOR>	hMonitors;
	vector<HDC>			hdcMonitors;
	vector<RECT>		rcMonitors;

	Monitors();
	static BOOL CALLBACK MonitorEnum(HMONITOR _hMon, HDC _hdc, LPRECT _lprcMonitor, LPARAM _pData);

	static bool GetMonitorInfoPrimary(RECT& _monitor);
	static bool GetMonitorInfoMouse(RECT& _monitor);
	static bool GetMonitorInfoId(UINT _id, RECT& _monitor);
};
