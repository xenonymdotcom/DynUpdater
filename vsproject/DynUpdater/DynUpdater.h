#include "WindowsApp.h"


class DynUpdater : public WindowsApp<DynUpdater>
{
public:
	static TCHAR ApplicationToolTip[64];
	static TCHAR ApplicationClassName[];
	static TCHAR ApplicationTitle[];
	static WORD appIconId();

	static int const WIDTH = 480;
	static int const HEIGHT = 300;
protected:
	virtual void initialiseMenu(HMENU menu);
	virtual void processMenu(HMENU menu, HWND hwnd, UINT clicked );
};


