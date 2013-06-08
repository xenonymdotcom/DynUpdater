#include "WindowsApp.h"

#include "resource.h"

class MyApp : public WindowsApp<MyApp>
{
public:
	static TCHAR ApplicationToolTip[64];
	static TCHAR ApplicationClassName[];
	static TCHAR ApplicationTitle[];
	static WORD appIconId() { return IDI_DYNUPDATER; }
	static void initialiseMenu(HMENU menu)
	{
		AppendMenu(menu, MF_STRING, ID_TRAY_EXIT,  TEXT( "Exit The Demo" ) );
	}
	static int const WIDTH = 480;
	static int const HEIGHT = 300;
};


