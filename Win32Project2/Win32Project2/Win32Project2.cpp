// Win32Project2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Win32Project2.h"

#include <cstdlib>

#define BOARD_SIZE 10
#define BOARD_MINES 10
#define COLOR_CONTROL RGB(0xC0, 0xC0, 0xC0)
#define COLOR_CONTROL_LIGHT RGB(0xE0, 0xE0, 0xE0)
#define COLOR_CONTROL_DARK RGB(0x70, 0x70, 0x70)
#define COLOR_TEXT RGB(0x0, 0x0, 0x0)

#define BOARD_MARK_NONE 0
#define BOARD_MARK_MARKED 1
#define BOARD_MARK_OPENED 2

#define MAX_LOADSTRING 100

int * boardState;
int * boardMarks;

HINSTANCE hInst;							
//TCHAR szTitle[MAX_LOADSTRING];				
TCHAR szWindowClass[MAX_LOADSTRING];		

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	boardState = new int[BOARD_SIZE * BOARD_SIZE];
	boardMarks = new int[BOARD_SIZE * BOARD_SIZE];

	// init board
	for (int idx = 0; idx < BOARD_SIZE * BOARD_SIZE; idx++) {
		boardState[idx] = 0;
		boardMarks[idx] = BOARD_MARK_NONE;
	}

	// generate minefield
	for (int idx = 0; idx < BOARD_MINES; idx++) {
		int cell = rand() % (BOARD_SIZE *BOARD_SIZE);
		if (boardState[idx] == 9) {
			idx--;
			continue;
		}

		boardState[cell] = 9;

		int cellx = cell % BOARD_SIZE;
		int celly = cell / BOARD_SIZE;

		if (cellx > 0 && boardState[cellx - 1 + (celly)* BOARD_SIZE] != 9) boardState[cellx - 1 + (celly)* BOARD_SIZE]++;
		if (celly > 0 && boardState[cellx + (celly - 1)* BOARD_SIZE] != 9) boardState[cellx + (celly - 1)* BOARD_SIZE]++;
		if (cellx < BOARD_SIZE - 1 && boardState[cellx + 1 + (celly)* BOARD_SIZE] != 9) boardState[cellx + 1 + (celly)* BOARD_SIZE]++;
		if (celly < BOARD_SIZE - 1 && boardState[cellx + (celly + 1)* BOARD_SIZE] != 9) boardState[cellx + (celly + 1)* BOARD_SIZE]++;
	}

	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	//LoadString(hInstance, IDS_APP_TITLE, L"Minesweeper", MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN32PROJECT2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) {
		return false;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT2));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WIN32PROJECT2);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance;

   hWnd = CreateWindow(szWindowClass, L"Minesweeper", WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd) return false;
   
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	POINT       Pt[6];
	HDC hdc;
	int xPos, yPos;
	switch (message)
	{
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
	case WM_POINTERDOWN:
		xPos = GET_X_LPARAM(lParam);
		yPos = GET_Y_LPARAM(lParam);

		if (IS_POINTER_FIRSTBUTTON_WPARAM(wParam))
		{
			// process pointer down, similar to mouse left button down
		}
		else if (IS_POINTER_SECONDBUTTON_WPARAM(wParam))
		{
			// process pointer down, similar to mouse right button down
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		SelectObject(hdc, GetStockObject(DC_PEN));
		SelectObject(hdc, GetStockObject(DC_BRUSH));

		// background
		SetDCPenColor(hdc, COLOR_CONTROL);
		SetDCBrushColor(hdc, COLOR_CONTROL);

		Rectangle(hdc, 0, 0, 500, 500);
		
		for (int x = 0; x < BOARD_SIZE; x++) {
			for (int y = 0; y < BOARD_SIZE; y++) {
				const int BTN_SIZE = 25;
				const int SHADOW_SIZE = 2;
				int basex = 10 + BTN_SIZE * x;
				int basey = 10 + BTN_SIZE * y;

				//boardMarks[idx] = BOARD_MARK_NONE

				if (boardMarks[x + y * BOARD_SIZE] == BOARD_MARK_NONE ||
					boardMarks[x + y * BOARD_SIZE] == BOARD_MARK_MARKED) {
					// mid
					SetDCPenColor(hdc, COLOR_CONTROL);
					SetDCBrushColor(hdc, COLOR_CONTROL);
					Rectangle(hdc, basex, basey, basex + BTN_SIZE, basey + BTN_SIZE);

					// lt
					Pt[0].x = basex + 0;
					Pt[0].y = basey + 0;

					Pt[1].x = basex + BTN_SIZE;
					Pt[1].y = basey + 0;

					Pt[2].x = basex + BTN_SIZE - SHADOW_SIZE;
					Pt[2].y = basey + SHADOW_SIZE;

					Pt[3].x = basex + SHADOW_SIZE;
					Pt[3].y = basey + SHADOW_SIZE;

					Pt[4].x = basex + SHADOW_SIZE;
					Pt[4].y = basey + BTN_SIZE - SHADOW_SIZE;

					Pt[5].x = basex + 0;
					Pt[5].y = basey + BTN_SIZE;

					SetDCPenColor(hdc, COLOR_CONTROL_LIGHT);
					SetDCBrushColor(hdc, COLOR_CONTROL_LIGHT);
					Polygon(hdc, Pt, 6);

					// rb
					Pt[0].x = basex + BTN_SIZE;
					Pt[0].y = basey + BTN_SIZE;

					Pt[1].x = basex + 0;
					Pt[1].y = basey + BTN_SIZE;

					Pt[2].x = basex + SHADOW_SIZE;
					Pt[2].y = basey + BTN_SIZE - SHADOW_SIZE;

					Pt[3].x = basex + BTN_SIZE - SHADOW_SIZE;
					Pt[3].y = basey + BTN_SIZE - SHADOW_SIZE;

					Pt[4].x = basex + BTN_SIZE - SHADOW_SIZE;
					Pt[4].y = basey + SHADOW_SIZE;

					Pt[5].x = basex + BTN_SIZE;
					Pt[5].y = basey + 0;

					SetDCPenColor(hdc, COLOR_CONTROL_DARK);
					SetDCBrushColor(hdc, COLOR_CONTROL_DARK);

					SetBkMode(hdc, TRANSPARENT);
					Polygon(hdc, Pt, 6);

					if (boardMarks[x + y * BOARD_SIZE] == BOARD_MARK_MARKED) {
						TextOut(hdc, basex + 8, basey + 5, L"F", 1);
					}
				}

				if (boardMarks[x + y * BOARD_SIZE] == BOARD_MARK_OPENED) {
					TCHAR *text = new TCHAR;
					*text = '0' + boardState[x + y * BOARD_SIZE];

					TextOut(hdc, basex + 8, basey + 5, text, 1);
				}
			}
		}

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)true;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)true;
		}
		break;
	}
	return (INT_PTR)false;
}
