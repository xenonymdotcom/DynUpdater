#pragma once

#define ID_TRAY_APP_ICON    1001
#define ID_TRAY_EXIT        1002

#define APPICONMSG          (WM_USER + 1)

template<typename Manager> class WindowsApp
{
private:
	static HWND wnd;
	static HMENU menu;
	static NOTIFYICONDATA notifyIconData;

	/// register the windows application class struct
	static bool registerApplicationWindowsClass(HINSTANCE hThisInstance)
	{
		WNDCLASSEX winclz;
		winclz.hInstance = hThisInstance;
		winclz.lpszClassName = Manager::ApplicationClassName;
		winclz.lpfnWndProc = windowsCallback;      /* This function is called by windows */
		winclz.style = CS_DBLCLKS;                 /* Catch double-clicks */
		winclz.cbSize = sizeof (WNDCLASSEX);

		/* Use default icon and mouse-pointer */
		winclz.hIcon = LoadIcon (GetModuleHandle(NULL), MAKEINTRESOURCE(Manager::appIconId()));
		winclz.hIconSm = LoadIcon (GetModuleHandle(NULL), MAKEINTRESOURCE(Manager::appIconId()));
		winclz.hCursor = LoadCursor (NULL, IDC_ARROW);
		winclz.lpszMenuName = NULL;                 /* No menu */
		winclz.cbClsExtra = 0;                      /* No extra bytes after the window class */
		winclz.cbWndExtra = 0;                      /* structure or the window instance */
		winclz.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(255, 255, 255)));
		return !!RegisterClassEx (&winclz);
	}

	static void initNotifyIconData()
	{
		memset( &notifyIconData, 0, sizeof( NOTIFYICONDATA ) ) ;

	    notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
		notifyIconData.hWnd = wnd;
		notifyIconData.uID = ID_TRAY_APP_ICON;
		notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		notifyIconData.uCallbackMessage = APPICONMSG; //Set up our invented Windows Message
		notifyIconData.hIcon = (HICON)LoadIcon( GetModuleHandle(NULL), MAKEINTRESOURCE(Manager::appIconId()) );
		_tcsncpy_s(notifyIconData.szTip, Manager::ApplicationToolTip, sizeof(Manager::ApplicationToolTip));
	}

public:
	static bool appMain( HINSTANCE hThisInstance
					  , int nCmdShow
					  )
	{
		// register the app class.
		if ( !registerApplicationWindowsClass(hThisInstance) )
		{
			return false;
		}
		wnd = CreateWindowEx ( 0
		                      , Manager::ApplicationClassName
							  , Manager::ApplicationTitle
							  , WS_OVERLAPPEDWINDOW
							  , CW_USEDEFAULT
							  , CW_USEDEFAULT
							  , 544          // width
							  , 375          // height
							  , HWND_DESKTOP
							  , NULL
							  , hThisInstance
							  , NULL
							  );
		initNotifyIconData();
        Shell_NotifyIcon(NIM_ADD, &notifyIconData);
		/* Make the window visible on the screen */
	    ShowWindow (wnd, nCmdShow);
		return true;
	}


static LRESULT CALLBACK windowsCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
    case WM_ACTIVATE:
        break;

	case WM_CREATE:
        ShowWindow(wnd, SW_HIDE);
        menu = CreatePopupMenu();
		Manager::initialiseMenu(menu);
        break;

    case WM_SYSCOMMAND:
        /*In WM_SYSCOMMAND messages, the four low-order bits of the wParam parameter 
		are used internally by the system. To obtain the correct result when testing the value of wParam, 
		an application must combine the value 0xFFF0 with the wParam value by using the bitwise AND operator.*/ 
		
		switch( wParam & 0xFFF0 )  
        {
        case SC_MINIMIZE:
        case SC_CLOSE:  
	        ShowWindow(wnd, SW_HIDE);
            return 0;
        }
        break;

        
	// Our user defined message.
    case APPICONMSG:
    {

        switch(wParam)
        {
        case ID_TRAY_APP_ICON:
			SetForegroundWindow(wnd);
            break;
        }


        switch(lParam)
        {
		case WM_LBUTTONUP:
			ShowWindow(wnd, SW_SHOW);
			break;
		case WM_RBUTTONDOWN:
            // Get current mouse position.
            POINT curPoint ;
            GetCursorPos( &curPoint ) ;
			SetForegroundWindow(wnd);

            // TrackPopupMenu blocks the app until TrackPopupMenu returns

            UINT clicked = TrackPopupMenu(menu,TPM_RETURNCMD | TPM_NONOTIFY,curPoint.x,curPoint.y,0,hwnd,NULL);

            SendMessage(hwnd, WM_NULL, 0, 0); // send benign message to window to make sure the menu goes away.
            if (clicked == ID_TRAY_EXIT)
            {
                // quit the application.
                Shell_NotifyIcon(NIM_DELETE, &notifyIconData);
                PostQuitMessage( 0 );
            }
        }
    }
    break;

    // intercept the hittest message..
    case WM_NCHITTEST:
		{
		LRESULT result = DefWindowProc(hwnd, WM_NCHITTEST, wParam, lParam);
		return (result == HTCLIENT) ? HTCAPTION : result;
		}

    case WM_CLOSE:
        ShowWindow(wnd, SW_HIDE);
        return 0;

    case WM_DESTROY:
        PostQuitMessage (0);
        break;

    case WM_QUIT:
		break;
    }

    return DefWindowProc( hwnd, message, wParam, lParam );
}

};

template<typename T> HWND WindowsApp<T>::wnd;
template<typename T> HMENU WindowsApp<T>::menu;
template<typename T> NOTIFYICONDATA WindowsApp<T>::notifyIconData;
