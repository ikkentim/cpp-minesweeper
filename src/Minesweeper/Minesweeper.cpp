#include "stdafx.h"
#include "Minesweeper.h"
#include "drawing.h"
#include "GameBoard.h"
#include <stdlib.h>
#include <time.h>

#define MAX_LOADSTRING 100

#define SWEEPER_SIZE   16
#define SWEEPER_MINES  40

#define BOARD_WIDTH (BOARD_X + SWEEPER_SIZE * CELL_SIZE + BOARD_SHADOW + BOARD_OFFSET)
#define BOARD_HEIGHT (BOARD_Y + SWEEPER_SIZE * CELL_SIZE + BOARD_SHADOW + BOARD_OFFSET)
GameBoard * gameBoard;

HINSTANCE hInst;
TCHAR szWindowClass[MAX_LOADSTRING];

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

    srand(time(NULL));
    gameBoard = new GameBoard(SWEEPER_SIZE, SWEEPER_MINES);


	MSG msg;
	HACCEL hAccelTable;

	LoadString(hInstance, IDC_MINESWEEPER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance (hInstance, nCmdShow)) return false;
	
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MINESWEEPER));

	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MINESWEEPER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MINESWEEPER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
   hInst = hInstance;

   RECT winSz = { 0, 0, BOARD_WIDTH, BOARD_HEIGHT };
   AdjustWindowRect(&winSz, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, true);

   HWND hWnd = CreateWindowEx(
       0,
       szWindowClass,
       L"Minesweeper",
       WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
       CW_USEDEFAULT,
       CW_USEDEFAULT,
       winSz.right - winSz.left,
       winSz.bottom - winSz.top,
       HWND_DESKTOP, 
       NULL,
       hInstance,
       NULL
       );

   if (!hWnd) return false;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	int wmId, wmEvent, xPos, yPos;
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message) {
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		switch (wmId) {
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
        case ID_FILE_RESTART:
            gameBoard->Reset();
            InvalidateRect(hWnd, 0, TRUE);
            break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
        break;
    case WM_LBUTTONDOWN:
        xPos = GET_X_LPARAM(lParam);
        yPos = GET_Y_LPARAM(lParam);

        gameBoard->Show((xPos - BOARD_X) / CELL_SIZE, (yPos - BOARD_Y) / CELL_SIZE);
        InvalidateRect(hWnd, 0, TRUE);
        break;
    case WM_RBUTTONDOWN:
        xPos = GET_X_LPARAM(lParam);
        yPos = GET_Y_LPARAM(lParam);

        gameBoard->Flag((xPos - BOARD_X) / CELL_SIZE, (yPos - BOARD_Y) / CELL_SIZE);
        InvalidateRect(hWnd, 0, TRUE);
        break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

        SelectObject(hdc, GetStockObject(DC_PEN));
        SelectObject(hdc, GetStockObject(DC_BRUSH));

        DrawBoard(hdc, BOARD_WIDTH, BOARD_HEIGHT, SWEEPER_SIZE * CELL_SIZE, SWEEPER_SIZE * CELL_SIZE);

        for (int y = 0; y < gameBoard->GetSize(); y++)
            for (int x = 0; x < gameBoard->GetSize(); x++) {
                switch (gameBoard->GetCellState(x, y)) {
                case CELL_UNKNOWN:
                    DrawCell(hdc, BOARD_X + x * CELL_SIZE, BOARD_Y + y * CELL_SIZE);
                    break;
                case CELL_VISIBLE:
                    DrawVisibleCell(hdc, BOARD_X + x * CELL_SIZE, BOARD_Y + y * CELL_SIZE,
                        gameBoard->GetNeighbouringBombs(x, y));
                    break;
                case CELL_FLAGGED:
                    DrawFlaggedCell(hdc, BOARD_X + x * CELL_SIZE, BOARD_Y + y * CELL_SIZE);
                    break;
                case CELL_EXPLOAD:
                    DrawBombCell(hdc, BOARD_X + x * CELL_SIZE, BOARD_Y + y * CELL_SIZE, true);
                    break;
                case CELL_BOMB:
                    DrawBombCell(hdc, BOARD_X + x * CELL_SIZE, BOARD_Y + y * CELL_SIZE, false);
                    break;
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

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
