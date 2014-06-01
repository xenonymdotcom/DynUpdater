#include "stdafx.h"
#include "resource.h"
#include "DynUpdater.h"
#include "PreDefinedClasses.h"

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

enum
{
    IDC_FILE_EXIT = 10000,
    IDC_BUTTON_SHOW_MSG,
    IDC_EDIT_CTRL,
    IDC_STATIC_TEXT
};


void DynUpdater::initialiseControls(HWND wnd, HINSTANCE thisInstance)
{
	m_staticText = WindowsClass<PredefinedClasses::Static>::create
		(
			  0 
			, L"Type in the edit box to change the title"
			, WS_CHILD | WS_VISIBLE | SS_LEFT
			, 40, 40, 640, 20
			, wnd
			, NULL
			, thisInstance
			, NULL
		);

	auto edit = WindowsClass<PredefinedClasses::Edit>::create
		(
              0
			, NULL
			, WS_VISIBLE|WS_CHILD|WS_BORDER|ES_AUTOHSCROLL|ES_AUTOVSCROLL
			, 40, 80, 243, 23
			, wnd
			, (HMENU)IDC_EDIT_CTRL
			, thisInstance
			, NULL
		);
 
    // Create a group box control
 	auto group = WindowsClass<PredefinedClasses::Button>::create
		(
			  0
			, L"Title edit"
			, WS_CHILD | WS_VISIBLE | BS_GROUPBOX
			, 20, 10, 292, 185
			, wnd
			, NULL
			, thisInstance
			, NULL		
		);
 
    // Create a button control
 	auto button = WindowsClass<PredefinedClasses::Button>::create
		(
			  WS_EX_CLIENTEDGE
			, L"Reset title"
			, BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE
			, 95,130,120,28
			, wnd
			, (HMENU)IDC_BUTTON_SHOW_MSG
			, thisInstance
			, NULL		
		);

	button->onCommand(	BN_CLICKED
						, [&](HWND hwnd, HWND parent_hwnd, unsigned notify, unsigned id) -> LRESULT 
							{
								SetWindowText( m_staticText->getHwnd(), L"BOO" );
								return 0;
							} 
						);
	/*
    // Create underlined text for the static control.
    HFONT hFontBold = CreateFont( 16,0,
                                  0,0,
                                  560,FALSE,FALSE,FALSE,
                                  ANSI_CHARSET,OUT_DEVICE_PRECIS,CLIP_MASK,
                                  ANTIALIASED_QUALITY, DEFAULT_PITCH,
                                  L"MS Outlook");
    // Use normal font for other controls.
    HFONT hFont = CreateFont( 17,0,
                              0,0,
                              550,FALSE,FALSE,FALSE,
                              ANSI_CHARSET,OUT_DEVICE_PRECIS,CLIP_MASK,
                              ANTIALIASED_QUALITY, DEFAULT_PITCH,
                              L"MS Outlook");
    // Set the new font for the controls

	SendMessage(hStaticText, WM_SETFONT, WPARAM (hFont), TRUE);
    SendMessage(hGroupBox, WM_SETFONT, WPARAM (hFontBold), TRUE);
    SendMessage(hButton, WM_SETFONT, WPARAM (hFont), TRUE);
    SendMessage(hEdit, WM_SETFONT, WPARAM (hFont), TRUE);
	*/
}


