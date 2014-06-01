#include "stdafx.h"
#include "WindowsCtrl.h"
#include <sstream>

WindowsCtrl::LookupMap WindowsCtrl::lookup;

LRESULT CALLBACK WindowsCtrl::windowsCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// OutputDebugString(L"{{{\n");
	// if we have a command message with 
	if ( (message == WM_COMMAND) && (wParam > 1) )
	{
		HWND cmd_src = (HWND)lParam;
		unsigned notify = (wParam >> 16);
		unsigned id = (wParam  & 0xFFFF);
		LookupMap::iterator i = lookup.find(cmd_src);
		if ( i != lookup.end() )
		{
			std::wostringstream msg;
			msg << "found cmd handler handle 0x" << std::hex << hwnd << " msg:0x" << message << " 0x" << wParam << " 0x" << lParam << "\n"; 
			// OutputDebugString( msg.str().c_str() );
			auto res  = (i->second)->processCommand(cmd_src, hwnd, notify, id);
			// OutputDebugString(L"+}}}\n");
			return res;
		}
	}

	LookupMap::iterator i = lookup.find(hwnd);
	if ( i != lookup.end() )
	{
		std::wostringstream msg;
		msg << "found obj handle 0x" << std::hex << hwnd << " msg:0x" << message << " 0x" << wParam << " 0x" << lParam << "\n"; 
		// OutputDebugString( msg.str().c_str() );
		auto res  = (i->second)->processMessage(hwnd, message, wParam, lParam );
		// OutputDebugString(L"+}}}\n");
		return res;
	}
	else
	{
		std::wostringstream msg;
		msg << "NO obj for handle 0x" << std::hex << hwnd << " msg:0x" << message << " 0x" << wParam << " 0x" << lParam << "\n"; 
		// OutputDebugString( msg.str().c_str() );
	}
	auto rv = DefWindowProc( hwnd, message, wParam, lParam );
	// OutputDebugString(L"-}}}\n");
	return rv;
}

LRESULT WindowsCtrl::handleNCHitTest(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = DefWindowProc(hwnd, WM_NCHITTEST, wParam, lParam);
	return (result == HTCLIENT) ? HTCAPTION : result;
}

LRESULT WindowsCtrl::processMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	std::wostringstream msg;
	msg << "proccessing for handle 0x" << std::hex << hwnd << " msg:0x" << message << " 0x" << wParam << " 0x" << lParam << "\n"; 
	// OutputDebugString( msg.str().c_str() );
	// OutputDebugString(L"Found control\n");

	return DefWindowProc( hwnd, message, wParam, lParam );
}

LRESULT WindowsCtrl::processCommand(HWND hwnd, HWND parent_hwnd, unsigned notify, unsigned id)
{
	return DefWindowProc( parent_hwnd, WM_COMMAND, (WPARAM)((notify<<16)|(id&0xFFFF)), (LPARAM)hwnd );
}

void WindowsCtrl::initialiseMenu(HMENU menu)
{
}

void WindowsCtrl::processMenu(HMENU menu, HWND hwnd, UINT clicked )
{
}

void WindowsCtrl::initialiseControls(HWND wnd, HINSTANCE thisInstance)
{
}
