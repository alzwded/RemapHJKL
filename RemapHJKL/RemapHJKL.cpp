// RemapHJKL.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "RemapHJKL.h"

#define MAX_LOADSTRING 100
#define MY_NOTIFICATION_ICON 2
#define MY_NOTIFY_ICON_MESSAGE_ID (WM_USER + 88)

#define MY_KEYDOWN(hks) (!(hks->flags & (1u<<7)))
#define MY_KEYUP(hks) (hks->flags & (1u<<7))

#define HOTKEY_KEY '3'
//#define HOTKEY_MODS (MOD_ALT | MOD_CONTROL | MOD_NOREPEAT)
#define HOTKEY_MODS (MOD_WIN | MOD_CONTROL | MOD_NOREPEAT)

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
BOOL g_lwin = FALSE;
BOOL g_lctrl = FALSE;
BOOL g_hotkeydown = FALSE;
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

LRESULT CALLBACK KeyboardHook(int code, WPARAM wParam, LPARAM lParam)
{
	if (code != HC_ACTION) return CallNextHookEx(NULL, code, wParam, lParam);
    switch(wParam)
    {
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
        break;
    default:
        return CallNextHookEx(NULL, code, wParam, lParam);
    }

    PKBDLLHOOKSTRUCT hks = (PKBDLLHOOKSTRUCT)lParam;

    // use both WIN and CTRL, because WIN by itself will open the
    // start menu; however, if, while WIN is held down, some other
    // modifier gets pressed, then it will not open the start menu.
    // Otherwise, the modifiers MUST go out for further processing
    // to avoid stuck / sticky keys :-)
    if(hks->vkCode == VK_LWIN)
    {
        g_lwin = MY_KEYDOWN(hks);
        return CallNextHookEx(NULL, code, wParam, lParam);
    }
    else if(hks->vkCode == VK_LCONTROL)
    {
        g_lctrl = MY_KEYDOWN(hks);
        return CallNextHookEx(NULL, code, wParam, lParam);
    }
    else if(hks->vkCode == HOTKEY_KEY && g_lctrl && g_lwin)
    {
        if(MY_KEYDOWN(hks))
        {
            g_hotkeydown = TRUE;
            // allegedly, this doesn't block the thread by waiting for
            // the message to get processed
            PostMessage(g_hwnd, WM_HOTKEY, 0, MAKELPARAM(HOTKEY_MODS, HOTKEY_KEY));
        }
        else
        {
            g_hotkeydown = FALSE;
        }
        return 1;
    }

    // debt -- we swallowed up the down state of this key, the modifier
    // was released in the meantime, so we don't want to issue a phantom
    // keyup
    if(hks->vkCode == HOTKEY_KEY && g_hotkeydown && MY_KEYUP(hks))
    {
        return 1;
    }

	if (!myState) return CallNextHookEx(NULL, code, wParam, lParam);

    // SendInput
    switch (hks->vkCode) {
        // this is our hotkey out, and we need it to break out.
        case HOTKEY_KEY:
            break;
            // leave these available for common operations
        case 'C':
        case 'V':
        case 'X':
        case 'Z':
            break;
            // disable most keys to avoid confusion
        case 'D':
        case 'I':
        case 'G':
        case 'M':
        case 'N':
        case 'O':
        case 'Q':
        case 'S':
        case 'T':
        case 'U':
        case 'W':
        case 'Y':
        case '1':
        case '2':
        case '5':
        case '7':
        case '8':
        case '9':
            return 1;
            // keys to process
        case 'R':
        case 'P':
        case 'B':
        case 'F':
        case '0':
        case '6':
        case 'A':
        case '4':
        case 'E':
        case 'L':
        case 'K':
        case 'J':
        case 'H':
            {
                //DWORD extended = (0x1000000 & lParam) >> 24; // Check if KEYEVENTF_EXTENDEDKEY
                BYTE scanCode = static_cast<BYTE>(((0xFF0000u & lParam) >> 16) & 0xFFu);

                // Check if KEYEVENTF_KEYUP, otherwise will be set to down
                auto dwFlags = 0
                    | ((hks->flags & LLKHF_UP) || (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) ? KEYEVENTF_KEYUP : 0)
                    | KEYEVENTF_SCANCODE /* for some reason, using wVk with SHIFT doesn't work, it only works with wScan */
                    ;

                INPUT ip;
                ZeroMemory(&ip, sizeof(ip));
                ip.type = INPUT_KEYBOARD;
                ip.ki.wVk = static_cast<WORD>(hks->vkCode & 0xFFFFu);
                switch (hks->vkCode) {
                    case 'H':
                        // shennanigans
                        // MapVirtualKey doesn't distinguish
                        // betweek numpad 4 (0x00xx) and the actual
                        // left key (0xE000).
                        //
                        // Pause and break are also fun, break is 0xE0xx,
                        // but pause is 0xE1xx :-)
                        //
                        // So force the prefix
                        ip.ki.wVk = VK_LEFT; 
                        ip.ki.wScan = (0xFF & MapVirtualKey(ip.ki.wVk, MAPVK_VK_TO_VSC_EX)) | 0xE000;
                        dwFlags |= KEYEVENTF_EXTENDEDKEY;
                        break;
                    case 'J':
                        ip.ki.wVk = VK_DOWN;
                        ip.ki.wScan = (0xFF & MapVirtualKey(ip.ki.wVk, MAPVK_VK_TO_VSC_EX)) | 0xE000;
                        dwFlags |= KEYEVENTF_EXTENDEDKEY;
                        break;
                    case 'K':
                        ip.ki.wVk = VK_UP;
                        ip.ki.wScan = (0xFF & MapVirtualKey(ip.ki.wVk, MAPVK_VK_TO_VSC_EX)) | 0xE000;
                        dwFlags |= KEYEVENTF_EXTENDEDKEY;
                        break;
                    case 'L':
                        ip.ki.wVk = VK_RIGHT;
                        ip.ki.wScan = (0xFF & MapVirtualKey(ip.ki.wVk, MAPVK_VK_TO_VSC_EX)) | 0xE000;
                        dwFlags |= KEYEVENTF_EXTENDEDKEY;
                        break;
                    case 'E':
                    case '4':
                        ip.ki.wVk = VK_END;
                        ip.ki.wScan = (0xFF & MapVirtualKey(ip.ki.wVk, MAPVK_VK_TO_VSC_EX)) | 0xE000;
                        dwFlags |= KEYEVENTF_EXTENDEDKEY;
                        break;
                    case 'A':
                    case '6':
                    case '0':
                        ip.ki.wVk = VK_HOME;
                        ip.ki.wScan = (0xFF & MapVirtualKey(ip.ki.wVk, MAPVK_VK_TO_VSC_EX)) | 0xE000;
                        dwFlags |= KEYEVENTF_EXTENDEDKEY;
                        break;
                    case 'F':
                        ip.ki.wVk = VK_NEXT;
                        ip.ki.wScan = (0xFF & MapVirtualKey(ip.ki.wVk, MAPVK_VK_TO_VSC_EX)) | 0xE000;
                        dwFlags |= KEYEVENTF_EXTENDEDKEY;
                        break;
                    case 'B':
                        ip.ki.wVk = VK_PRIOR;
                        ip.ki.wScan = (0xFF & MapVirtualKey(ip.ki.wVk, MAPVK_VK_TO_VSC_EX)) | 0xE000;
                        dwFlags |= KEYEVENTF_EXTENDEDKEY;
                        break;
                        // I keep running into keyboards without a BRK key
                    case 'P':
                        // this is a multibyte scan code for some reason
                        ip.ki.wVk = VK_PAUSE;
                        // doesn't work well with scan codes.
                        dwFlags &= ~(KEYEVENTF_SCANCODE);
                        break;
                    case 'R':
                        ip.ki.wVk = VK_CANCEL;
                        dwFlags &= ~(KEYEVENTF_SCANCODE);
                        //ip.ki.wScan = (0xFF & MapVirtualKey(ip.ki.wVk, MAPVK_VK_TO_VSC_EX)) | 0xE000;
                        //dwFlags |= KEYEVENTF_EXTENDEDKEY;
                        break;
                    default:
                        {
                            UINT outScanCode = MapVirtualKey(ip.ki.wVk, MAPVK_VK_TO_VSC_EX);
                            ip.ki.wScan = LOBYTE(outScanCode);
                            dwFlags |= ((HIBYTE(outScanCode) & 0xE0) ? KEYEVENTF_EXTENDEDKEY : 0);
                            break;
                        }
                }
                UINT outScanCode = MapVirtualKey(ip.ki.wVk, MAPVK_VK_TO_VSC_EX);
                ip.ki.dwFlags = dwFlags;
                ip.ki.time = hks->time;
                ip.ki.dwExtraInfo = hks->dwExtraInfo;
                SendInput(1, &ip, sizeof(ip));
                return 1;
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

   //BOOL result = RegisterHotKey(hWnd, 1, HOTKEY_MODS, HOTKEY_KEY); // TODO cleanup
   //if(!result)
   //{
   //    MessageBox(NULL, _T("Cannot register hotkey, exiting!"), _T("Cannot register hotkey"), MB_OK|MB_ICONERROR);
   //    DestroyWindow(hWnd);
   //    return FALSE;
   //}
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
