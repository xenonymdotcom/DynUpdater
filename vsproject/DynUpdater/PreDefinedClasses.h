#pragma once
#include "WindowsCtrl.h"

#include <map>
#include <functional>

class PredefinedClasses
{
public:
	static TCHAR const Static[];
	static TCHAR const Edit[];
	static TCHAR const Button[];
};

template <TCHAR const Name[]> class WindowsClass : public WindowsCtrl
{
public:
	typedef std::map<UINT, std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>> MessageMap;
	typedef std::map<unsigned, std::function<LRESULT(HWND, HWND, unsigned, unsigned)>> CmdMap;
	typedef std::shared_ptr<WindowsClass> SharedPtr;

	void addHandler( UINT msg, std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> handler )
	{
		msgMap[msg] = handler;
	}

	void onCommand( unsigned cmd, std::function<LRESULT(HWND, HWND, unsigned, unsigned)> handler )
	{
		cmdMap[cmd] = handler;
	}

	static std::shared_ptr<WindowsClass> create
	(
		  DWORD style
		, TCHAR const * windowName
		, DWORD stlye
		, int x
		, int y
		, int width
		, int height
		, HWND parent
		, HMENU menu
		, HINSTANCE instance
		, LPVOID param
		)
	{
		HWND hwnd = CreateWindowEx(style, Name, windowName, stlye, x, y, width, height, parent, menu, instance, param );
		if ( !hwnd )
		{
			throw std::exception("Failed to create controls");
		}
		return createControl<WindowsClass>(hwnd);
    }


protected:
	virtual LRESULT processMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		MessageMap::const_iterator i = msgMap.find(message);
		if ( i != msgMap.end() )
		{
			// OutputDebugString(L"found handler\n");
			return i->second(hwnd, message, wParam, lParam);
		}
		// OutputDebugString(L"NO handler\n");
		return WindowsCtrl::processMessage( hwnd, message, wParam, lParam );
	}

	virtual LRESULT processCommand(HWND hwnd, HWND parent_hwnd, unsigned notify, unsigned id)
	{
		CmdMap::const_iterator i = cmdMap.find(notify);
		if ( i != cmdMap.end() )
		{
			// OutputDebugString(L"found cmd handler\n");
			return i->second(hwnd, parent_hwnd, notify, id);
		}
		// OutputDebugString(L"NO cmd handler\n");
		return WindowsCtrl::processCommand(hwnd, parent_hwnd, notify, id);
	}


private:
	MessageMap msgMap;
	CmdMap     cmdMap;
};
