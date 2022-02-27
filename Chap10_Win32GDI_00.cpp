#include "resource.h"
#include <windows.h>
#include "common_gdi.h"

using namespace _special_gdi_routines;


HWND ghWnd = NULL;
BOOL bStartCapt = FALSE;
HINSTANCE hInst = GetModuleHandle(NULL);
HMENU wndMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU1));

_gdi_routines WinGDI;
TCHAR wcszWndName[MAX_PATH] = TEXT("Win32GDI_00");

const WNDCLASSEX WndMainClass();
HWND WndMainCreate();
BOOL WndMainDisplay();
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
extern "C" int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);


const WNDCLASSEX WndMainClass() {
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpszClassName = wcszWndName;
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wcex.lpfnWndProc = (WNDPROC)MainWndProc;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = LoadIcon(NULL, IDC_ICON);
	wcex.hIconSm = NULL;
	wcex.hInstance = hInst;
	
	return wcex;
}


HWND WndMainCreate() {
	const WNDCLASSEX _tmpWin = WndMainClass();

	if (RegisterClassEx( &_tmpWin ) )
		return CreateWindowEx(WS_EX_CLIENTEDGE,wcszWndName, NULL, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0,
			CW_USEDEFAULT, 0, NULL, NULL, hInst, NULL);
	else {
		MessageBox(NULL, "Failed to register window class or attempt to create window failed  !!", "Error Info", MB_OK);
		return NULL;
	}
		
}


BOOL WndMainDisplay()
{
	ghWnd = WndMainCreate();
	if (ghWnd) {
		ShowWindow(ghWnd, SW_SHOW);
		UpdateWindow(ghWnd);
		return TRUE;
	}
	else return FALSE;
	
}





// Entry Point for Application's Program Code
int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _pszcStr, int nCmdShow) {

	if (!WndMainDisplay()) return 0;

	MSG _msg;
	
	while (GetMessage(&_msg, NULL, 0, 0)) {
		if (!TranslateAccelerator(ghWnd, NULL, &_msg)) {
			TranslateMessage(&_msg);
			DispatchMessage(&_msg);
		}
	}
	return (int)_msg.wParam;
}



LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	int xPos = 0, yPos = 0;
	RECT wndRc = { 0,0,0,0 };
	HRGN wndRgn = NULL;      
	HMENU wndSubMenu = NULL;
	LOGPEN PenLog = {};
	LOGBRUSH PicBrush = {}; 
	CHOOSECOLOR ColorsLodge;
	static int PenStrokeSize = 1;
	static COLORREF ccDefault16Colors[16];
	static DWORD ccRGBCurrent;
	TCHAR const* S_TEXT = "This is Sample Exercise of Chapter 10 on Windows' GDI Topics.";
	MENUINFO mnuInfo = {}; mnuInfo.cbSize = sizeof(MENUINFO);
	MENUITEMINFO wndSubMenuInfo = {}; wndSubMenuInfo.cbSize = sizeof(wndSubMenuInfo);
	
	WinGDI.SetHDC(GetDC(hWnd)); 
	WinGDI.SetInstance(hInst); 
	WinGDI.SetHandle(hWnd);

	switch (uMsg) {

	case WM_CREATE:
	{/*
		GetMenuInfo(wndMenu, &mnuInfo);
		mnuInfo.fMask = MIM_STYLE;
		mnuInfo.dwStyle = MNS_NOTIFYBYPOS;
		SetMenuInfo(wndMenu, &mnuInfo);
		SetMenu(hWnd, wndMenu); */

	}break;

	case WM_MENUSELECT:
	{
		UINT mnuItemID = (UINT)LOWORD(wParam);
		HMENU hSubMnu = (HMENU)lParam;
		SetMenuDefaultItem(hSubMnu, mnuItemID, FALSE);
	}break;


	case WM_COMMAND: {
		// selector for chosen menu item
		switch (LOWORD(wParam)) {
				case ID_GDIACTIONS_RECTANGLE:
					WinGDI.draw_rect(10, 10, 100, 100);
				break;

				case ID_GDIACTIONS_CIRCLE:
					WinGDI.draw_arc(110, 10, 170, 80, 112, 12, 120, 20);
					break;
				
				case ID_GDIACTIONS_ELLIPSE:
					WinGDI.draw_ellipse(10, 110, 70, 170); 
					break;

				case ID_GDIACTIONS_BEZIER:
				{
					POINT cPts[4] = { {90,110}, {150,124}, {168,132}, {176,119} };
					WinGDI.draw_poly_bezier(cPts, 4);
				}break;

				case ID_GDIACTIONS_ARCS:
				{
					POINT cPts[4] = { {10,194}, {20,226},{40,146}, {55,194} };
					WinGDI.draw_poly_bezier(cPts, 4);

				}break;

				case ID_GDIACTIONS_FREEFORMARTS:
					MessageBox(hWnd, TEXT("Keep pressing your left mouse button to strike any drawing on the client area, press [Esc] when finished done !"), TEXT("GDI Action Info"), MB_OK);
					bStartCapt = TRUE;
				break;

				case ID_GDITOOLS_USEBRUSH:
					WinGDI.SetBrush();
					break;

				case ID_GDITOOLS_USEPEN:
					WinGDI.SetPen(PenStrokeSize);
					break;

				case ID_BRUSHSTYLE_CROSS:
				{
					PicBrush.lbStyle = BS_HATCHED;
					PicBrush.lbHatch = HS_CROSS;
					PicBrush.lbColor = DIB_RGB_COLORS;
					WinGDI.SetBrushStyle(PicBrush);		
				}
				break;

				case ID_BRUSHSTYLE_HATCH:
				{
					PicBrush.lbStyle = BS_HATCHED;
					PicBrush.lbColor = DIB_RGB_COLORS;
					WinGDI.SetBrushStyle(PicBrush);

				}break;

				case ID_BRUSHSTYLE_DIAGONAL:
				{
					PicBrush.lbStyle = BS_HATCHED;
					PicBrush.lbHatch = HS_FDIAGONAL;
					PicBrush.lbColor = DIB_RGB_COLORS;
					WinGDI.SetBrushStyle(PicBrush);

				}break;

				case ID_BRUSHSTYLE_BIDIAGONAL:
				{
					PicBrush.lbStyle = BS_HATCHED;
					PicBrush.lbHatch = HS_BDIAGONAL;
					PicBrush.lbColor = DIB_RGB_COLORS;
					WinGDI.SetBrushStyle(PicBrush);

				}break;

				case ID_BRUSHSTYLE_CROSSEDDIAGONAL:
				{
					PicBrush.lbStyle = BS_HATCHED;
					PicBrush.lbHatch = HS_DIAGCROSS;
					PicBrush.lbColor = DIB_RGB_COLORS;
					WinGDI.SetBrushStyle(PicBrush);

				}break;
				

				case ID_BRUSHSTYLE_HORIZONTAL:
				{
					PicBrush.lbStyle = BS_HATCHED;
					PicBrush.lbHatch = HS_HORIZONTAL;
					PicBrush.lbColor = DIB_RGB_COLORS;
					WinGDI.SetBrushStyle(PicBrush);

				}break;

				case ID_BRUSHSTYLE_VERTICAL:
				{
					PicBrush.lbStyle = BS_HATCHED;
					PicBrush.lbHatch = HS_VERTICAL;
					PicBrush.lbColor = DIB_RGB_COLORS;
					WinGDI.SetBrushStyle(PicBrush);

				}break;


				case ID_BRUSHSTYLE_SOLIDDOT:
				{
					PicBrush.lbStyle = BS_SOLID;
					PicBrush.lbColor = DIB_RGB_COLORS;
					WinGDI.SetBrushStyle(PicBrush);

				}break;

				// Manage the Menu for Casting Pen Style
				case ID_PENSTYLE_DASH:
				{
					PenLog.lopnColor = WinGDI.GetInternalColorScheme();
					PenLog.lopnStyle = PS_DASH;
					PenLog.lopnWidth = POINT{ PenStrokeSize,0 };
					WinGDI.SetPenStyle(PenLog);
				}break;
				

				case ID_PENSTYLE_DASHDOTDOT:
				{
					PenLog.lopnColor = WinGDI.GetInternalColorScheme();
					PenLog.lopnStyle = PS_DASHDOTDOT;
					PenLog.lopnWidth = POINT{ PenStrokeSize,0 };
					WinGDI.SetPenStyle(PenLog);

				}break;


				case ID_PENSTYLE_DOT:
				{
					PenLog.lopnColor = WinGDI.GetInternalColorScheme();
					PenLog.lopnStyle = PS_DOT;
					PenLog.lopnWidth = POINT{ PenStrokeSize,0 };
					WinGDI.SetPenStyle(PenLog);

				}break;

				case ID_PENSTYLE_SOLID:
				{
					PenLog.lopnColor = WinGDI.GetInternalColorScheme();
					PenLog.lopnStyle = PS_SOLID;
					PenLog.lopnWidth = POINT{ PenStrokeSize,0 };
					WinGDI.SetPenStyle(PenLog);

				}break;

				case ID_GDIACTIONS_TEXT:
					TextOut(WinGDI.GetInternalDC(), 55, 155, S_TEXT, lstrlen(S_TEXT));
				break;


				case ID_GDISETTINGS_COLORBOX:
				{
					ZeroMemory(&ColorsLodge, 0);	
					ColorsLodge.lStructSize = sizeof(ColorsLodge);
					ColorsLodge.hwndOwner = hWnd;
					ColorsLodge.lpCustColors = (LPDWORD)ccDefault16Colors;
					ColorsLodge.rgbResult = ccRGBCurrent;
					ColorsLodge.Flags = CC_FULLOPEN | CC_RGBINIT;

					if (ChooseColor(&ColorsLodge)) WinGDI.SetColorSketch(ColorsLodge.rgbResult);
					else {
							switch(CommDlgExtendedError()) {
								case CDERR_DIALOGFAILURE:
									MessageBox(NULL, TEXT("General DialogBox Failed !"), TEXT("ERR_INFO"), MB_OK + MB_ICONERROR);
									break;

								case CDERR_FINDRESFAILURE:
									MessageBox(NULL, TEXT("Dialog Function Failed to Locate the Resource Handle."), TEXT("ERR_INFO"), MB_OK + MB_ICONERROR);
									break;

								case CDERR_INITIALIZATION:
									MessageBox(NULL, TEXT("Dialog couldn't perform the initialization due to runs out of memory."), TEXT("ERR_INFO"), MB_OK + MB_ICONERROR);
									break;

								case CDERR_LOADRESFAILURE:
									MessageBox(NULL, TEXT("Dialog failed upon loading the needed resources."), TEXT("ERR_INFO"), MB_OK + MB_ICONERROR);
									break;

								case CDERR_LOCKRESFAILURE:
									MessageBox(NULL, TEXT("Dialog failed in procuring the needed resources."), TEXT("ERR_INFO"), MB_OK + MB_ICONERROR);
									break;

								case CDERR_MEMALLOCFAILURE:
									MessageBox(NULL, TEXT("Dialog couldn't reserve memory for its internal structures."), TEXT("ERR_INFO"), MB_OK + MB_ICONERROR);
									break;

								case CDERR_MEMLOCKFAILURE:
									MessageBox(NULL, TEXT("Dialog can't lock the memory associated with a specified handle."), TEXT("ERR_INFO"), MB_OK + MB_ICONERROR);
									break;

								case CDERR_NOHOOK:
									MessageBox(NULL, TEXT("Dialog couldn't find a matched pointer to the hook procedure."), TEXT("ERR_INFO"), MB_OK + MB_ICONERROR);
									break;

								case CDERR_NOTEMPLATE:
									MessageBox(NULL, TEXT("Dialog couldn't find a suitably template resource for its initialization."), TEXT("ERR_INFO"), MB_OK + MB_ICONERROR);
									break;
								case CDERR_REGISTERMSGFAIL:
									MessageBox(NULL, TEXT("Dialog failed due to a call to the RegisterWindowMessage A.P.I."), TEXT("ERR_INFO"), MB_OK + MB_ICONERROR);
									break;
								case CDERR_STRUCTSIZE:
									MessageBox(NULL, TEXT("Dialog failed in its initialization due to an invalid lStructSize's value."), TEXT("ERR_INFO"), MB_OK + MB_ICONERROR);
									break;
								default:
									MessageBox(NULL, TEXT("Unknown system exception might occurred to annul the Dialog's build."), TEXT("ERR_INFO"), MB_OK + MB_ICONERROR);
									break;

						       }
					      }
				}break;
					
				case ID_STROKESIZE_2:
					PenStrokeSize = 2;
					break;

				case ID_STROKESIZE_4:
					PenStrokeSize = 4; break;

				case ID_STROKESIZE_6:
					PenStrokeSize = 6; break;

				case ID_STROKESIZE_8:
					PenStrokeSize = 8; break;


				case ID_GDIACTIONS_ERASEDRAWING:
				{
					UpdateWindow(hWnd);
					GetClientRect(hWnd, &wndRc);
					InvalidateRect(hWnd, &wndRc, TRUE);	
				}
				break;

				default:
					break;
		}

	}break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC _hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);

	}break;


	case WM_LBUTTONDOWN:
		if (MK_LBUTTON == wParam && bStartCapt) {			
			if (wndMenu) {
				wndSubMenu = GetSubMenu(wndMenu, 0);
				GetMenuItemInfo(wndSubMenu, 5, TRUE, &wndSubMenuInfo);
				if (MFS_HILITE == wndSubMenuInfo.fState) {
					GetClientRect(hWnd, &wndRc); 
					SetRect(&wndRc, wndRc.left + 1, wndRc.top + 1, wndRc.right - 1, wndRc.bottom - 1);
					wndRgn = CreateRectRgn(wndRc.left, wndRc.top, wndRc.right, wndRc.bottom);
					SetWindowRgn(hWnd, wndRgn, TRUE);
					SetCapture(GetCapture());
				}
			}
		}
	break;

	case WM_MOUSEMOVE:
		if ( MK_LBUTTON == wParam && bStartCapt ) {
			xPos = LOWORD(lParam); yPos = HIWORD(lParam);
			WinGDI.free_draw(xPos, yPos, WinGDI.GetInternalColorScheme());
		}
	break;


	case WM_LBUTTONUP:
		ReleaseCapture();
	break;

	case WM_KEYDOWN:
	{
		int nVirtKey = (int)wParam;
		if (VK_ESCAPE == nVirtKey) ReleaseCapture(); bStartCapt = FALSE;
		wndSubMenuInfo.fMask = MIIM_STATE;
		wndSubMenuInfo.fState = MFS_UNHILITE;
		SetMenuItemInfo(wndSubMenu, 5, TRUE, &wndSubMenuInfo);

	}break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	};
	return 0;
}
