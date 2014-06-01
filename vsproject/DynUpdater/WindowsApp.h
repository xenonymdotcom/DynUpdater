#pragma once

#include "WindowsCtrl.h"

template<typename Manager> class WindowsApp : public WindowsCtrl
{
private:
	HMENU menu;
	NOTIFYICONDATA notifyIconData;

	/// register the windows application class struct
	static bool registerApplicationWindowsClass(HINSTANCE hThisInstance)
	{
		WNDCLASSEX winclz;
		winclz.hInstance = hThisInstance;
		winclz.lpszClassName = Manager::ApplicationClassName;
		winclz.lpfnWndProc = WindowsCtrl::windowsCallback;
		winclz.style = CS_DBLCLKS;
		winclz.cbSize = sizeof (WNDCLASSEX);

		winclz.hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(Manager::appIconId()));
		winclz.hIconSm = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(Manager::appIconId()));
		winclz.hCursor = LoadCursor(0, IDC_ARROW);
		winclz.lpszMenuName = 0;
		winclz.cbClsExtra = 0;
		winclz.cbWndExtra = 0;
		winclz.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(255, 255, 255)));
		return !!RegisterClassEx (&winclz);
	}

	void initNotifyIconData(HWND wnd)
	{
		memset( &notifyIconData, 0, sizeof( NOTIFYICONDATA ) ) ;

	    notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
		notifyIconData.hWnd = wnd;
		notifyIconData.uID = ID_TRAY_APP_ICON;
		notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		notifyIconData.uCallbackMessage = APPICONMSG; //Set up our invented Windows Message
		notifyIconData.hIcon = (HICON)LoadIcon( GetModuleHandle(0), MAKEINTRESOURCE(Manager::appIconId()) );
		_tcsncpy_s(notifyIconData.szTip, Manager::ApplicationToolTip, sizeof(Manager::ApplicationToolTip));
        Shell_NotifyIcon(NIM_ADD, &notifyIconData);
	}

	void createMenu()
	{
        menu = CreatePopupMenu();
		initialiseMenu(menu);
		// add and exit option at the end
		AppendMenu(menu, MF_SEPARATOR, 0, 0 );
		AppendMenu(menu, MF_STRING, ID_TRAY_EXIT,  TEXT( "Exit" ) );
	}
	
	void createControls(HWND wnd, HINSTANCE thisInstance)
	{
		initialiseControls(wnd,thisInstance);
	}

	static LRESULT handleNCHitTest(HWND hwnd, WPARAM wParam, LPARAM lParam)
	{
		LRESULT result = DefWindowProc(hwnd, WM_NCHITTEST, wParam, lParam);
		return (result == HTCLIENT) ? HTCAPTION : result;
	}

	void processOurMessage(HWND hwnd, WPARAM wParam, LPARAM lParam)
	{
        switch(wParam)
        {
        case ID_TRAY_APP_ICON:
			SetForegroundWindow(hwnd);
            break;
        }

        switch(lParam)
        {
		case WM_LBUTTONUP:
			ShowWindow(hwnd, SW_SHOW);
			break;
		case WM_RBUTTONDOWN:
            POINT curPoint ;
            GetCursorPos( &curPoint );
			SetForegroundWindow(hwnd);

			// this will block until the we get an answer
            UINT clicked = TrackPopupMenu( menu,TPM_RETURNCMD | TPM_NONOTIFY,curPoint.x,curPoint.y,0,hwnd,0);

            if (clicked == ID_TRAY_EXIT)
            {
                // quit the application.
                Shell_NotifyIcon(NIM_DELETE, &notifyIconData);
                PostQuitMessage( 0 );
            }
			else
			{
				processMenu( menu, hwnd, clicked );
			}
        }
	}

protected:
	virtual LRESULT processMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		OutputDebugString(L"App processed msg\n");
		switch (message)
		{
		case WM_SYSCOMMAND:
			// the bottom 4 bits are used internally (mask them out)		
			switch( wParam & 0xFFF0 )  
			{
			case SC_MINIMIZE:
			case SC_CLOSE:  
				ShowWindow(hwnd, SW_HIDE);
				return 0;
			}
			break;


			// Our user defined message.
		case APPICONMSG:
			processOurMessage(hwnd, wParam, lParam);
			return 0;

			// intercept the hittest message..
		case WM_NCHITTEST:
			return handleNCHitTest(hwnd, wParam, lParam );

		case WM_CLOSE:
			ShowWindow(hwnd, SW_HIDE);
			return 0;

		case WM_DESTROY:
			PostQuitMessage (0);
			break;

		case WM_QUIT:
			break;
		}

		return DefWindowProc( hwnd, message, wParam, lParam );
	}
	
	// add user items to the menu.
	virtual void initialiseMenu(HMENU menu){}
	// process the actions we added
	virtual void processMenu(HMENU menu, HWND hwnd, UINT clicked ){}

	// add user controls
	virtual void initialiseControls(HWND wnd, HINSTANCE thisInstance){}
	
public:
	// public interface (just this entry point)
	static bool appMain( HINSTANCE hThisInstance
					  , int nCmdShow
					  )
	{
		// register the app class.
		if ( !registerApplicationWindowsClass(hThisInstance) )
		{
			return false;
		}
		HWND wnd = CreateWindowEx ( 0
		                      , Manager::ApplicationClassName
							  , Manager::ApplicationTitle
							  , WS_OVERLAPPEDWINDOW
							  , CW_USEDEFAULT
							  , CW_USEDEFAULT
							  , Manager::WIDTH
							  , Manager::HEIGHT
							  , HWND_DESKTOP
							  , 0
							  , hThisInstance
							  , 0
							  );
		std::shared_ptr<Manager> p = WindowsCtrl::createControl<Manager>(wnd);		
		p->initNotifyIconData(wnd);
		p->createMenu();
		p->createControls(wnd, hThisInstance);
		// show the window we just created.
	    ShowWindow (wnd, nCmdShow);
		return true;
	}
};
