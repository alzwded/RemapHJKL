// RemapHJKL.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "RemapHJKL.h"
#include <shellapi.h>

#define MAX_LOADSTRING 100
#define MY_NOTIFICATION_ICON 2
#define MY_NOTIFY_ICON_MESSAGE_ID (WM_USER + 88)

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

BOOL myState = FALSE;
HWND g_hwnd = NULL;
HHOOK g_hook = NULL;
HICON g_icons[2] = { NULL, NULL };


void Cleanup()
{
	UnregisterHotKey(g_hwnd, 1);
	UnhookWindowsHookEx(g_hook);
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.uID = MY_NOTIFICATION_ICON;
	nid.hWnd = g_hwnd;
	nid.uFlags = 0;
	Shell_NotifyIcon(NIM_DELETE, &nid);
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_REMAPHJKL, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_REMAPHJKL));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	Cleanup();
	
	return (int) msg.wParam;
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_REMAPHJKL));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = NULL;// MAKEINTRESOURCE(IDC_REMAPHJKL);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

#include <fstream>
LRESULT CALLBACK KeyboardHook(int code, WPARAM wParam, LPARAM lParam)
{
	if (code != HC_ACTION) return CallNextHookEx(NULL, code, wParam, lParam);

	if (!myState) return CallNextHookEx(NULL, code, wParam, lParam);

	switch (wParam)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:	{
							PKBDLLHOOKSTRUCT hks = (PKBDLLHOOKSTRUCT)lParam;
							// SendInput
							switch (hks->vkCode) {
                            // this is our hotkey out, and we need it to break out.
                            case '3':
                                break;
                            // disable most keys to avoid confusion
                            case 'D':
                            case 'I':
                            case 'G':
                            case 'M':
                            case 'N':
                            case 'O':
                            case 'P':
                            case 'Q':
                            case 'R':
                            case 'S':
                            case 'T':
                            case 'U':
                            case 'V':
                            case 'W':
                            case 'X':
                            case 'Y':
                            case 'Z':
                            case '1':
                            case '2':
                            case '5':
                            case '7':
                            case '8':
                            case '9':
                                return 1;
                            // keys to process
                            case 'C':
							case 'B':
							case 'F':
							case '0':
							case '6':
                            case 'A':
							case '4':
                            case 'E':
							case 'L':
                            case VK_SPACE:
							case 'K':
							case 'J':
							case 'H':
                            case VK_BACK:
                                {
                                    //DWORD extended = (0x1000000 & lParam) >> 24; // Check if KEYEVENTF_EXTENDEDKEY
                                    BYTE scanCode = static_cast<BYTE>(((0xFF0000u & lParam) >> 16) & 0xFFu);

                                    // Check if KEYEVENTF_KEYUP, otherwise will be set to down
                                    auto dwFlags = (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) ? KEYEVENTF_KEYUP : 0;
                                    dwFlags |= KEYEVENTF_EXTENDEDKEY;
                                    dwFlags |= KEYEVENTF_SCANCODE;

                                    INPUT ip;
                                    ZeroMemory(&ip, sizeof(ip));
                                    ip.type = INPUT_KEYBOARD;
                                    switch (hks->vkCode) {
                                        case VK_BACK:
                                        case 'H':
                                            ip.ki.wVk = VK_LEFT; 
                                            break;
                                        case 'J':
                                            ip.ki.wVk = VK_DOWN;
                                            break;
                                        case 'K':
                                            ip.ki.wVk = VK_UP;
                                            break;
                                        case VK_SPACE:
                                        case 'L':
                                            ip.ki.wVk = VK_RIGHT;
                                            break;
                                        case 'E':
                                        case '4':
                                            ip.ki.wVk = VK_END;
                                            break;
                                        case 'A':
                                        case '6':
                                        case '0':
                                            ip.ki.wVk = VK_HOME;
                                            break;
                                        case 'F':
                                            ip.ki.wVk = VK_NEXT;
                                            break;
                                        case 'B':
                                            ip.ki.wVk = VK_PRIOR;
                                            break;
                                            // I keep running into keyboards without a BRK key
                                        case 'C':
                                            ip.ki.wVk = VK_PAUSE;
                                            break;
                                            // disabled keys to eliminate confusion
                                    }
                                    ip.ki.wScan = MapVirtualKey(ip.ki.wVk, MAPVK_VK_TO_VSC);
                                    ip.ki.dwFlags = dwFlags;
                                    ip.ki.time = 0;
                                    ip.ki.dwExtraInfo = 0;
                                    SendInput(1, &ip, sizeof(ip));
                                    return 1;						}
							}
							break;
	}
	}

	return CallNextHookEx(NULL, code, wParam, lParam);
}

void ShowTip(BOOL active)
{

	NOTIFYICONDATA nid = {};
	ZeroMemory(&nid, sizeof(NOTIFYICONDATA));
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.uID = MY_NOTIFICATION_ICON;
	nid.uFlags = NIF_TIP | NIF_SHOWTIP | NIF_ICON;
	nid.hWnd = g_hwnd;
	nid.uVersion = NOTIFYICON_VERSION_4;

	TCHAR tip[64];
	// TODO inform about current hotkey better
	if (active) {
		wsprintf(tip, _T("RemapHJKL (active) - Click to exit forever"));
		nid.hIcon = g_icons[1];
	} else {
		wsprintf(tip, _T("RemapHJKL (not active) - Click to exit forever"));
		nid.hIcon = g_icons[0];
	}
	_tcsncpy_s(nid.szTip, tip, _tcslen(tip));

	Shell_NotifyIcon(NIM_MODIFY, &nid);
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

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindowEx(WS_EX_TOOLWINDOW, szWindowClass, szTitle, WS_POPUP,
	   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   RegisterHotKey(hWnd, 1, MOD_ALT | MOD_CONTROL | MOD_NOREPEAT, '3'); // TODO cleanup
   g_hook = SetWindowsHookEx(WH_KEYBOARD_LL, &KeyboardHook, hInst, 0);
   g_hwnd = hWnd;

   ShowWindow(hWnd, SW_HIDE);
   UpdateWindow(hWnd);

   NOTIFYICONDATA nid = {};
   ZeroMemory(&nid, sizeof(NOTIFYICONDATA));
   nid.cbSize = sizeof(NOTIFYICONDATA);
   nid.uID = MY_NOTIFICATION_ICON;
   nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_SHOWTIP;
   nid.hWnd = hWnd;
   nid.uVersion = NOTIFYICON_VERSION_4;
   nid.uCallbackMessage = MY_NOTIFY_ICON_MESSAGE_ID;

   g_icons[0] = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
   g_icons[1] = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL_ACTIVE));

   nid.hIcon = g_icons[0];
   HRESULT sniHr = Shell_NotifyIcon(NIM_ADD, &nid);
   nid.uFlags = 0;
   sniHr = Shell_NotifyIcon(NIM_SETVERSION, &nid);

   ShowTip(FALSE);

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
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case MY_NOTIFY_ICON_MESSAGE_ID: {
										auto what = LOWORD(lParam);
										switch (what) {
										case NIN_SELECT:
											PostQuitMessage(0);
											break;
										case NIN_KEYSELECT:
										case WM_CONTEXTMENU:
											return DefWindowProc(hWnd, message, wParam, lParam);
										}
	}
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_HOTKEY:
		myState = !myState;
		ShowTip(myState);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
