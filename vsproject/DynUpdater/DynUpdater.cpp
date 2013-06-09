#include "stdafx.h"
#include "resource.h"
#include "DynUpdater.h"

#define REFRESHMSG (WM_USER + 2)

TCHAR DynUpdater::ApplicationToolTip[64] = TEXT("Xenonym\nDynamic DNS");
TCHAR DynUpdater::ApplicationClassName[] = L"Xenonym::DDNS";
TCHAR DynUpdater::ApplicationTitle[] = L"Xenonym Dynamic Domain Name Setup";

// static func to get the app icon.
WORD DynUpdater::appIconId() 
{ 
	return IDI_DYNUPDATER;
}

void DynUpdater::initialiseMenu(HMENU menu)
{
	AppendMenu(menu, MF_STRING, REFRESHMSG,  TEXT( "Refresh now" ) );
}

void DynUpdater::processMenu(HMENU menu, HWND hwnd, UINT clicked )
{
	switch( clicked )
	{
	case REFRESHMSG:
		//
		break;
	}
}


