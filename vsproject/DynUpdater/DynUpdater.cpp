#include "stdafx.h"
#include "DynUpdater.h"
#include "windows.h"

namespace
{
	MyApp theApp;
}

HWND WindowsApp<MyApp>::wnd;
HMENU WindowsApp<MyApp>::menu;
NOTIFYICONDATA WindowsApp<MyApp>::notifyIconData;

TCHAR MyApp::ApplicationToolTip[64] = TEXT("Xenonym\nDynamic DNS");
TCHAR MyApp::ApplicationClassName[] = L"Xenonym::DDNS";
TCHAR MyApp::ApplicationTitle[] = L"Xenonym Dynamic Domain Name Setup";

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
	// initialise the app
	if ( !theApp.appMain(hThisInstance, nCmdShow) )
	{
		return 0;
	}

	MSG messages;

	// while there are messages to process
	while (GetMessage (&messages, NULL, 0, 0))
    {
        // transalate the virt key
        TranslateMessage(&messages);
        // send the message to the correct winproc
        DispatchMessage(&messages);
    }

    return messages.wParam;
}
