#include "stdafx.h"

#include "DynUpdater.h"

namespace
{
	DynUpdater theApp;
}

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
