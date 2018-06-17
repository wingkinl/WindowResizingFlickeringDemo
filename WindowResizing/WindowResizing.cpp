// WindowResizing.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "WindowResizing.h"
#include <CommCtrl.h>
#include <time.h>

#if _MSC_VER>=1400
#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HWND g_hwndListLeft = nullptr;
HWND g_hwndListRight = nullptr;

const LONG g_nGap = 5;
LONG g_nFixedWidth = 0;
BOOL g_bFixLeftPanel = FALSE;

LONG GetMinClientWidth()
{
	return g_nFixedWidth + g_nGap * 2;
}

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWRESIZING, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWRESIZING));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWRESIZING));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWRESIZING);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

void GetListRect(RECT& rectList, HWND hWnd, BOOL bLeft)
{
	RECT rectWnd;
	GetClientRect(hWnd, &rectWnd);
	SIZE szWnd = { rectWnd.right - rectWnd.left, rectWnd.bottom - rectWnd.top };
	rectList = rectWnd;
	InflateRect(&rectList, -g_nGap, -g_nGap);
	if (bLeft)
		rectList.right = g_bFixLeftPanel ? rectList.left + g_nFixedWidth : rectList.right - g_nFixedWidth - g_nGap;
	else
		rectList.left = g_bFixLeftPanel ? rectList.left + g_nFixedWidth + g_nGap : rectList.right - g_nFixedWidth;
}

void UpdateFixedPanels(HWND hWnd)
{
	RECT rectWnd;
	GetClientRect(hWnd, &rectWnd);
	g_nFixedWidth = (rectWnd.right - rectWnd.left) / 3;
	if (g_nFixedWidth < 250)
		g_nFixedWidth = 250;
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
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 800, 400, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   UpdateFixedPanels(hWnd);

   RECT rectList;
   GetListRect(rectList, hWnd, TRUE);
   g_hwndListLeft = CreateWindow(WC_LISTVIEW, TEXT("listL"), WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | LVS_REPORT,
	   rectList.left, rectList.top, rectList.right - rectList.left, rectList.bottom - rectList.top, hWnd, nullptr, hInstance, nullptr);
   GetListRect(rectList, hWnd, FALSE);
   g_hwndListRight = CreateWindow(WC_LISTVIEW, TEXT("listR"), WS_CHILD|WS_VISIBLE|WS_BORDER|WS_TABSTOP|LVS_REPORT, 
	   rectList.left, rectList.top, rectList.right-rectList.left, rectList.bottom-rectList.top, hWnd, nullptr, hInstance, nullptr);

   ListView_SetExtendedListViewStyle(g_hwndListLeft, LVS_EX_DOUBLEBUFFER);
   ListView_SetExtendedListViewStyle(g_hwndListRight, LVS_EX_DOUBLEBUFFER);

   ListView_SetBkColor(g_hwndListLeft, RGB(255, 0, 0));
   ListView_SetBkColor(g_hwndListRight, RGB(0, 0, 255));
   LVCOLUMN column = { 0 };
   column.mask = LVCF_TEXT | LVCF_FMT;
   column.pszText = (LPTSTR)TEXT("Index");
   ListView_InsertColumn(g_hwndListLeft, 0, &column);
   ListView_InsertColumn(g_hwndListRight, 0, &column);
   column.pszText = (LPTSTR)TEXT("Value");
   ListView_InsertColumn(g_hwndListLeft, 1, &column);
   ListView_InsertColumn(g_hwndListRight, 1, &column);
   srand((unsigned int)time(NULL));
   for (int ii = 0; ii < 20; ++ii)
   {
	   TCHAR szText[20];
	   LVITEM item = { 0 };
	   item.mask = LVIF_TEXT;
	   item.iItem = ii;
	   item.pszText = szText;
	   _stprintf(szText, TEXT("Left %d"), ii);
	   ListView_InsertItem(g_hwndListLeft, &item);
	   _stprintf(szText, TEXT("Right %d"), ii);
	   ListView_InsertItem(g_hwndListRight, &item);
	   _stprintf(szText, TEXT("%d"), rand());
	   ListView_SetItemText(g_hwndListLeft, ii, 1, szText);
	   _stprintf(szText, TEXT("%d"), rand());
	   ListView_SetItemText(g_hwndListRight, ii, 1, szText);
   }
   ListView_SetColumnWidth(g_hwndListLeft, 0, 100);
   ListView_SetColumnWidth(g_hwndListLeft, 1, LVSCW_AUTOSIZE_USEHEADER);
   ListView_SetColumnWidth(g_hwndListRight, 0, 100);
   ListView_SetColumnWidth(g_hwndListRight, 1, LVSCW_AUTOSIZE_USEHEADER);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

BOOL CanExpandWindow(HWND hWnd)
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	BOOL bExpand = (rect.right - rect.left) == GetMinClientWidth();
	return bExpand;
}

int g_nExpandingShrinking = 0;

void ExpandShrinkWindow(HWND hWnd)
{
	RECT rect;
	GetWindowRect(hWnd, &rect);
	BOOL bExpand = CanExpandWindow(hWnd);
	if (g_bFixLeftPanel)
	{
		if (bExpand)
		{
			rect.right += g_nFixedWidth + g_nGap;
		}
		else
		{
			RECT rectTemp;
			GetWindowRect(g_hwndListLeft, &rectTemp);
			ScreenToClient(hWnd, (LPPOINT)&rectTemp);
			ScreenToClient(hWnd, (LPPOINT)&rectTemp + 1);
			rectTemp.right += g_nGap;
			ClientToScreen(hWnd, (LPPOINT)&rectTemp);
			ClientToScreen(hWnd, (LPPOINT)&rectTemp + 1);
			AdjustWindowRectEx(&rectTemp, GetWindowLong(hWnd, GWL_STYLE), TRUE, GetWindowLong(hWnd, GWL_EXSTYLE));
			rect.right = rectTemp.right;
		}
	}
	else
	{
		if (bExpand)
		{
			rect.left -= g_nFixedWidth + g_nGap;
		}
		else
		{
			RECT rectTemp;
			GetWindowRect(g_hwndListRight, &rectTemp);
			ScreenToClient(hWnd, (LPPOINT)&rectTemp);
			ScreenToClient(hWnd, (LPPOINT)&rectTemp + 1);
			rectTemp.left -= g_nGap;
			ClientToScreen(hWnd, (LPPOINT)&rectTemp);
			ClientToScreen(hWnd, (LPPOINT)&rectTemp + 1);
			AdjustWindowRectEx(&rectTemp, GetWindowLong(hWnd, GWL_STYLE), TRUE, GetWindowLong(hWnd, GWL_EXSTYLE));
			rect.left = rectTemp.left;
		}
	}
	g_nExpandingShrinking = 1;
	UINT nFlags = SWP_NOZORDER | SWP_NOACTIVATE;
	SetWindowPos(hWnd, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, nFlags);
	g_nExpandingShrinking = 0;
}

//#define FIX_FLICKERING

LRESULT OnNcCalcSize(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	NCCALCSIZE_PARAMS* lpncsp = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
	LRESULT res;
#ifdef FIX_FLICKERING
	if (wParam && g_nExpandingShrinking && !g_bFixLeftPanel)
	{
		res = DefWindowProc(hWnd, WM_NCCALCSIZE, wParam, lParam);
		GetWindowRect(g_hwndListRight, lpncsp->rgrc + 2);
		lpncsp->rgrc[1] = lpncsp->rgrc[2];
		res = WVR_VALIDRECTS;
	}
	else
#endif // FIX_FLICKERING
	{
		res = DefWindowProc(hWnd, WM_NCCALCSIZE, wParam, lParam);
	}
	return res;
}

inline void TryDeferWindowPosEx(HDWP& hDWP, HWND hWnd, int x, int y, int cx, int cy, UINT nFlags)
{
	if (hDWP)
		hDWP = DeferWindowPos(hDWP, hWnd, nullptr, x, y, cx, cy, nFlags);
	else
		SetWindowPos(hWnd, nullptr, x, y, cx, cy, nFlags);
}

inline void TryDeferWindowPos(HDWP& hDWP, HWND hWnd, const RECT& rect, UINT nFlags)
{
	TryDeferWindowPosEx(hDWP, hWnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, nFlags);
}

void RepositionControls(HWND hWnd)
{
	RECT rect;
	UINT nFlags = SWP_NOZORDER | SWP_NOACTIVATE;
	HDWP hDWP = BeginDeferWindowPos(2);
	if (g_hwndListLeft)
	{
		GetListRect(rect, hWnd, TRUE);
		TryDeferWindowPos(hDWP, g_hwndListLeft, rect, nFlags);
	}
	if (g_hwndListRight)
	{
		GetListRect(rect, hWnd, FALSE);
		TryDeferWindowPos(hDWP, g_hwndListRight, rect, nFlags);
	}
	if (hDWP)
		EndDeferWindowPos(hDWP);
}

void OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	RepositionControls(hWnd);
}

void OnGetMinMaxInfo(HWND hWnd, MINMAXINFO* pMinMaxInfo)
{
	RECT rect = { 0, 0, GetMinClientWidth(), 10 };
	AdjustWindowRectEx(&rect, GetWindowLong(hWnd, GWL_STYLE), TRUE, GetWindowLong(hWnd, GWL_EXSTYLE));
	pMinMaxInfo->ptMinTrackSize.x = rect.right-rect.left;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case ID_WINDOW_EXPAND_SHRINK:
				ExpandShrinkWindow(hWnd);
				break;
			case ID_WINDOW_FIXLEFTPANEL:
			case ID_WINDOW_FIXRIGHTPANEL:
				g_bFixLeftPanel = wmId == ID_WINDOW_FIXLEFTPANEL;
				UpdateFixedPanels(hWnd);
				RepositionControls(hWnd);
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_SIZE:
		OnSize(hWnd, wParam, lParam);
		break;
	case WM_GETMINMAXINFO:
		OnGetMinMaxInfo(hWnd, (MINMAXINFO*)lParam);
		break;
	case WM_NCCALCSIZE:
		return OnNcCalcSize(hWnd, wParam, lParam);
	case WM_INITMENUPOPUP:
		// 'Window' menu
		if (LOWORD(lParam) == 1)
		{
			CheckMenuRadioItem((HMENU)wParam, ID_WINDOW_FIXLEFTPANEL, ID_WINDOW_FIXRIGHTPANEL, g_bFixLeftPanel ? ID_WINDOW_FIXLEFTPANEL : ID_WINDOW_FIXRIGHTPANEL, MF_BYCOMMAND);
			MENUITEMINFO mii = { 0 };
			mii.cbSize = sizeof(MENUITEMINFO);
			mii.fMask = MIIM_STRING;
			mii.dwTypeData = (LPTSTR)(CanExpandWindow(hWnd) ? TEXT("&Expand\tF4") : TEXT("&Shrink\tF4"));
			SetMenuItemInfo((HMENU)wParam, ID_WINDOW_EXPAND_SHRINK, FALSE, &mii);
		}
		break;
	case WM_KEYDOWN:
		if (wParam == VK_F4)
		{
			ExpandShrinkWindow(hWnd);
			break;
		}
		// fall through
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
