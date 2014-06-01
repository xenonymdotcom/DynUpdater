#pragma once

#include <memory>

#define ID_TRAY_APP_ICON    1001
#define ID_TRAY_EXIT        1002

#define APPICONMSG          (WM_USER + 1)

class WindowsCtrl
{
public:
	typedef std::map<HWND,std::shared_ptr<WindowsCtrl>> LookupMap;

public:
	template <typename T> static std::shared_ptr<T> createControl(HWND wnd)
	{
		std::shared_ptr<T> p = std::make_shared<T>();
		p->attach(wnd);
		lookup[wnd] = p;
		return p;
	}

	static LRESULT CALLBACK windowsCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	HWND getHwnd()
	{
		return this_hwnd;
	}

private:
	static LookupMap lookup;
	
	HWND this_hwnd;
	void attach( HWND hwnd )
	{
		this_hwnd = hwnd;
	}
	
	static LRESULT handleNCHitTest(HWND hwnd, WPARAM wParam, LPARAM lParam);


protected:
	virtual LRESULT processMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	// methods that sub classes are expected to reimplement

	virtual LRESULT processCommand(HWND hwnd, HWND parent_hwnd, unsigned notify, unsigned id);
	// methods that sub classes are expected to reimplement
	
	// add user items to the menu.
	virtual void initialiseMenu(HMENU menu);

	// process the actions we added
	virtual void processMenu(HMENU menu, HWND hwnd, UINT clicked );

	// add user controls
	virtual void initialiseControls(HWND wnd, HINSTANCE thisInstance);
	
public:
};
