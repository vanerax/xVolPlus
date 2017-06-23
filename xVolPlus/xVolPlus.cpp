// xVolPlus.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "resource.h"
#include <assert.h>
#include "Config.h"
#include "VolumeManager.h"

//#pragma comment( linker, "/subsystem:windows /entry:wWinMainCRTStartup" )
//#pragma comment( linker, "/subsystem:windows /entry:wmainCRTStartup" )

//#pragma comment( linker, "/subsystem:console /entry:wmainCRTStartup" )
//#pragma comment( linker, "/subsystem:console /entry:wWinMainCRTStartup" )

#pragma comment( linker, "/subsystem:windows /entry:wmainCRTStartup" )


// MESSAGE_TRAYICON will be received by wndProc
#define MESSAGE_TRAYICON WM_USER
#define MAX_LOADSTRING 100
#define VOLUME_UP 1
#define VOLUME_DOWN 2

//global variable
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

HINSTANCE g_hInst;
HWND g_hWnd;
HWND g_hConsole;
CVolumeManager *volumeManager;

//declaration
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT HotKeyProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);




//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_XVOLPLUS);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassEx(&wcex);
}


//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   g_hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   g_hWnd = hWnd;

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;


	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		//case IDM_ABOUT:
		//	DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		//	break;
		//case IDM_EXIT:
		//	DestroyWindow(hWnd);
		//	break;

		case ID_MENU_SHOWWINDOW:
			ShowWindow(hWnd, SW_RESTORE);
			SetForegroundWindow(hWnd);
			break;

		case ID_MENU_SHOWCONSOLE:
			ShowWindow(g_hConsole, SW_RESTORE);
			break;

		case ID_MENU_HIDECONSOLE:
			ShowWindow(g_hConsole, SW_HIDE);
			break;

		case ID_MENU_EXIT:
			DestroyWindow(hWnd);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		HDC hdc;

		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
	//unregister hot key
	if ( !UnregisterHotKey(g_hWnd, VOLUME_UP) ) {
		MessageBox(NULL, TEXT("UnRegisterHotKey failed"), TEXT(""), 0);
	}
	
	if ( !UnregisterHotKey(g_hWnd, VOLUME_DOWN) ) {
		MessageBox(NULL, TEXT("UnRegisterHotKey failed"), TEXT(""), 0);
	}
		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		ShowWindow(hWnd, SW_HIDE); //just hide it
		//DestroyWindow(hWnd); //close window
		break;

	case WM_CREATE:
		break;

	case WM_INITDIALOG:
		break;

	case WM_HOTKEY:
		//MessageBox(hWnd, TEXT("key"), _T(""), 0);
		HotKeyProc(hWnd, message, wParam, lParam);
		break;

	//customized message type
	case MESSAGE_TRAYICON:
		if (lParam == WM_LBUTTONDBLCLK){
			ShowWindow(hWnd, SW_RESTORE);
			SetForegroundWindow(hWnd);
		}
		else if (lParam == WM_RBUTTONUP){
			HMENU hMenu;            // top-level menu 
			HMENU hMenuTrackPopup;  // shortcut menu 
			hMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_TRAYMENU)); //MAKEINTRESOURCE());
			hMenuTrackPopup = GetSubMenu(hMenu, 0);

			POINT point;
			GetCursorPos(&point);
			SetForegroundWindow(hWnd); //otherwise menu will not be hiden unless it is clicked
			TrackPopupMenu(hMenuTrackPopup,  TPM_RIGHTBUTTON, point.x, point.y, 0, hWnd, NULL);
			//DestroyMenu(hMenuTrackPopup);
		}
		break;

	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED) ShowWindow(hWnd, SW_HIDE);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return S_OK;
}

LRESULT HotKeyProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	int iHotkeyMsgId = (int) wParam; //   identifier   of   hot   key     
	UINT fuModifiers = (UINT) LOWORD(lParam); //   key-modifier   flags     
	UINT uVirtKey = (UINT) HIWORD(lParam); //   virtual-key   code
	float fVolGap = CConfig::GetVolGap();
	float fLevel;

	try {
	switch (iHotkeyMsgId) {
	case VOLUME_UP:
		fLevel = volumeManager->GetVolumeValue();
		volumeManager->SetMasterVolumeLevelScalar(fLevel + fVolGap);
		break;

	case VOLUME_DOWN:
		fLevel = volumeManager->GetVolumeValue();
		volumeManager->SetMasterVolumeLevelScalar(fLevel - fVolGap);
		break;

	default:
		break;
	}
	}
	catch (int e) {
		CString csMsg;
		csMsg.Format(_T("Failed to set volume. Error code: %d"), e);
		//wsprintf(_T("Failed to set volume. Error code: %d"), e);
		MessageBox(NULL, csMsg, TEXT(""), 0);
	}

	return S_OK;
}

int CALLBACK HandlerRoutine(DWORD dwCtrlType) {
	switch(dwCtrlType)  
    {
	case CTRL_C_EVENT:
		break;

	case CTRL_BREAK_EVENT:
		break;

    case CTRL_CLOSE_EVENT:      
        break;

    default:  
        break;  
    }  
  
    return TRUE;  
}


int _tmain(int argc, _TCHAR* argv[])
{
	CoInitialize(NULL);

	//if (SetConsoleCtrlHandler(HandlerRoutine, TRUE) == FALSE) {	
	//}

	//HMODULE hModule = GetModuleHandle(NULL);
	//HANDLE hProcess = GetCurrentProcess();
	//DWORD dwThreadID = GetCurrentThreadId();
	HINSTANCE hInstance = GetModuleHandle(NULL);
	g_hConsole = GetConsoleWindow();
	ShowWindow(g_hConsole, SW_HIDE);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_XVOLPLUS, szWindowClass, MAX_LOADSTRING);
	
	MyRegisterClass(hInstance);

	// Perform application initialization:
	int nCmdShow = SW_HIDE;
	if (!InitInstance (hInstance, nCmdShow))
	{
		CoUninitialize();
		return FALSE;
	}

	// Add tray
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = g_hWnd;
	//nid.hIcon = (HICON)LoadImage(NULL,_T("xVolume.ico"),IMAGE_ICON,0,0,LR_LOADFROMFILE);
	nid.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	lstrcpy(nid.szTip, szTitle);
	nid.uCallbackMessage = MESSAGE_TRAYICON;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD, &nid);
	

	//register hot key for tuning the volume
	if (!RegisterHotKey(g_hWnd, VOLUME_UP, CConfig::GetModifiers(), CConfig::GetVolUpKey())){
		MessageBox(NULL, TEXT("RegisterHotKey failed"), TEXT(""), 0);
	}
	if (!RegisterHotKey(g_hWnd, VOLUME_DOWN, CConfig::GetModifiers(), CConfig::GetVolDownKey())) {
		MessageBox(NULL, TEXT("RegisterHotKey failed"), TEXT(""), 0);
	}

	//create volume manager instance. Com should be initialized first
	volumeManager = new CVolumeManager();


	MSG msg;
	WPARAM wParam = 1;
	LPARAM lParam = 2;

	//SendMessage(hWnd, WM_USER, wParam, lParam);
	//PostMessage(hWnd, WM_USER, wParam, lParam);

	//PostThreadMessage(dwThreadID, WM_QUIT, wParam, lParam);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		
	}

	delete volumeManager;
	volumeManager = NULL;



	//remove tray icon
	Shell_NotifyIcon(NIM_DELETE, &nid);

	CoUninitialize();
	return 0;
}


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	return 0;
}