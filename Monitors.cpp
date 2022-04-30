#include "Monitors.h"

Monitors::Monitors()
{
	EnumDisplayMonitors(0, 0, MonitorEnum, (LPARAM)this);
}

BOOL CALLBACK Monitors::MonitorEnum(HMONITOR _hMon, HDC _hdc, LPRECT _lprcMonitor, LPARAM _pData)
{
	Monitors* pThis = reinterpret_cast<Monitors*>(_pData);

	pThis->hMonitors.push_back(_hMon);
	pThis->hdcMonitors.push_back(_hdc);
	pThis->rcMonitors.push_back(*_lprcMonitor);
	pThis->iMonitors.push_back(pThis->hdcMonitors.size());
	return TRUE;
}

bool Monitors::GetMonitorInfoId(UINT _id, RECT& _monitor)
{
	Monitors monitors;

	if (_id > monitors.iMonitors.size() - 1) {
		_tprintf(wstring(_T("Error - problem with monitor ") + to_wstring(_id)).c_str());
		_id = 0;
	}

	_monitor = monitors.rcMonitors[_id];
	return true;
}

bool Monitors::GetMonitorInfoPrimary(RECT& _monitor)
{
	MONITORINFO monitorInfo = { 0 };
	monitorInfo.cbSize = sizeof(monitorInfo);
	HMONITOR hMonitor = MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTONULL);

	if (!hMonitor)
		return false;

	if (!GetMonitorInfo(hMonitor, &monitorInfo))
		return false;

	_monitor = monitorInfo.rcMonitor;
	return true;
}

bool Monitors::GetMonitorInfoMouse(RECT& _monitor)
{
	POINT mouse;
	::GetCursorPos(&mouse);

	MONITORINFO monitorInfo = { 0 };
	monitorInfo.cbSize = sizeof(monitorInfo);
	HMONITOR hMonitor = MonitorFromPoint(mouse, MONITOR_DEFAULTTONULL);

	if (!hMonitor)
		return false;

	if (!GetMonitorInfo(hMonitor, &monitorInfo))
		return false;

	_monitor = monitorInfo.rcMonitor;
	return true;
}
