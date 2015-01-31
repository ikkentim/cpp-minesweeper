#include "stdafx.h"
#include "Minesweeper.h"
#include "drawing.h"
#include "GameBoard.h"
#include <stdlib.h>
#include <time.h>

#define MAX_LOADSTRING 100

#define IDT_SCORETIME_TIMER 1

#define SWEEPER_SIZE   16
#define SWEEPER_MINES  40

#define BOARD_WIDTH (FRAME_WIDTH + SWEEPER_SIZE * CELL_SIZE)
#define BOARD_HEIGHT (FRAME_HEIGHT + SWEEPER_SIZE * CELL_SIZE)

#define RESET_BUTTON_X (BOARD_WIDTH / 2 - RESET_BUTTON_SIZE / 2)
#define RESET_BUTTON_Y (SHADOW_SIZE + FRAME_SIZE + SCOREBOARD_SIZE / 2 - RESET_BUTTON_SIZE / 2)

#define SCORE_NUMBER_Y (SHADOW_SIZE + FRAME_SIZE + SCOREBOARD_SIZE / 2 - SCOREBOARD_NUMBER_HEIGHT / 2)

GameBoard * gameBoard;
bool isButtonDown;
bool isClicking;
int hoverX = -1, hoverY = -1;

HINSTANCE hInst;

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
	wcex.lpszClassName	= L"Minesweeper";
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
   hInst = hInstance;

   RECT winSz = { 0, 0, BOARD_WIDTH, BOARD_HEIGHT };
   AdjustWindowRect(&winSz, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, true);

   HWND hWnd = CreateWindowEx(
       0,
       L"Minesweeper",
       L"Minesweeper",
       WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX |
       WS_CLIPSIBLINGS |
       WS_CLIPCHILDREN,
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

   SetTimer(hWnd, IDT_SCORETIME_TIMER, 1000, (TIMERPROC)NULL);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return true;
}
void InvalidateAll(HWND hWnd) {
    InvalidateRect(hWnd, 0, FALSE);
}

void InvalidateScore(HWND hWnd) {
    RECT rect;
    rect.left = SHADOW_SIZE * 2 + FRAME_SIZE * 2;
    rect.top = SCORE_NUMBER_Y;
    rect.right = rect.left + SCOREBOARD_NUMBER_WIDTH * 3;
    rect.bottom = SCORE_NUMBER_Y + SCOREBOARD_NUMBER_HEIGHT;

    InvalidateRect(hWnd, &rect, FALSE);
}

void InvalidateTime(HWND hWnd) {
    RECT rect;
    rect.right = SHADOW_SIZE * 2 + SWEEPER_SIZE * CELL_SIZE;
    rect.left = rect.right - SCOREBOARD_NUMBER_WIDTH * 3;

    rect.bottom = SCORE_NUMBER_Y + SCOREBOARD_NUMBER_HEIGHT;
    rect.top = SCORE_NUMBER_Y;

    InvalidateRect(hWnd, &rect, FALSE);
}

void InvalidateBoard(HWND hWnd) {
    RECT rect = {
        BOARD_X, BOARD_Y,
        BOARD_X + SWEEPER_SIZE * CELL_SIZE,
        BOARD_Y + SWEEPER_SIZE * CELL_SIZE
    };

    InvalidateRect(hWnd, &rect, FALSE);
}

void InvalidateReset(HWND hWnd) {
    RECT rect = { 
        RESET_BUTTON_X, RESET_BUTTON_Y,
        RESET_BUTTON_X + RESET_BUTTON_SIZE, 
        RESET_BUTTON_Y + RESET_BUTTON_SIZE 
    };

    InvalidateRect(hWnd, &rect, FALSE);
}

void InvalidateCell(HWND hWnd, int x, int y) {
    RECT rect = {
        BOARD_X + x * CELL_SIZE,
        BOARD_Y + y * CELL_SIZE,
        BOARD_X + x * CELL_SIZE + CELL_SIZE,
        BOARD_Y + y * CELL_SIZE + CELL_SIZE
    };

    InvalidateRect(hWnd, &rect, FALSE);

}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	int wmId, wmEvent, xPos, yPos, xCell, yCell;
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
            InvalidateAll(hWnd);
            break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
        break;
    case WM_LBUTTONDOWN:
        xPos = GET_X_LPARAM(lParam);
        yPos = GET_Y_LPARAM(lParam);
      
        if (xPos >= RESET_BUTTON_X &&
            xPos < RESET_BUTTON_X + RESET_BUTTON_SIZE &&
            yPos >= RESET_BUTTON_Y &&
            yPos < RESET_BUTTON_Y + RESET_BUTTON_SIZE) {
            InvalidateReset(hWnd);
            isButtonDown = true;
            break;
        }

        if (gameBoard->IsGameOver()) break;

        hoverX = (xPos - BOARD_X) / CELL_SIZE;
        hoverY = (yPos - BOARD_Y) / CELL_SIZE;
        isClicking = true;

        if (gameBoard->GetCellState(hoverX, hoverY) == CELL_UNKNOWN) InvalidateCell(hWnd, hoverX, hoverY);
        InvalidateReset(hWnd);
        break;
    case WM_MOUSEMOVE:
        if (isClicking && !gameBoard->IsGameOver()) {
            xPos = GET_X_LPARAM(lParam);
            yPos = GET_Y_LPARAM(lParam);

            xCell = (xPos - BOARD_X) / CELL_SIZE;
            yCell = (yPos - BOARD_Y) / CELL_SIZE;

            if (xCell != hoverX || yCell != hoverY) {
                if (gameBoard->GetCellState(xCell, yCell) == CELL_UNKNOWN) InvalidateCell(hWnd, xCell, yCell);
                if (gameBoard->GetCellState(hoverX, hoverY) == CELL_UNKNOWN) InvalidateCell(hWnd, hoverX, hoverY);

                hoverX = xCell;
                hoverY = yCell;
            }
        }
        break;
    case WM_LBUTTONUP:
        if (isButtonDown) {
            gameBoard->Reset();
            InvalidateAll(hWnd);
        }

        if (isClicking && !gameBoard->IsGameOver()) {
            xPos = GET_X_LPARAM(lParam);
            yPos = GET_Y_LPARAM(lParam);

            std::stack<int> updatedCells;
            gameBoard->Show((xPos - BOARD_X) / CELL_SIZE, (yPos - BOARD_Y) / CELL_SIZE, &updatedCells);
            InvalidateReset(hWnd);

            while (!updatedCells.empty())
            {
                int updCell = updatedCells.top();
                if (updCell == -1)
                    InvalidateBoard(hWnd);
                else
                    InvalidateCell(hWnd, updCell % SWEEPER_SIZE, updCell / SWEEPER_SIZE);

                updatedCells.pop();
            }

            hoverX = -1;
            hoverY = -1;
        }

        isButtonDown = false;
        isClicking = false;
        break;
    case WM_RBUTTONDOWN:
        if (gameBoard->IsGameOver()) break;

        xPos = GET_X_LPARAM(lParam);
        yPos = GET_Y_LPARAM(lParam);

        gameBoard->Flag((xPos - BOARD_X) / CELL_SIZE, (yPos - BOARD_Y) / CELL_SIZE);
        InvalidateCell(hWnd, (xPos - BOARD_X) / CELL_SIZE, (yPos - BOARD_Y) / CELL_SIZE);
        InvalidateScore(hWnd);
        break;
    case WM_TIMER:
        switch (wParam)
        {
        case IDT_SCORETIME_TIMER:
            InvalidateTime(hWnd);
            break;
        }
        break;
    case WM_ERASEBKGND:
        return 1;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

        SelectObject(hdc, GetStockObject(DC_PEN));
        SelectObject(hdc, GetStockObject(DC_BRUSH));

        DrawBoard(hdc, BOARD_WIDTH, BOARD_HEIGHT, SWEEPER_SIZE * CELL_SIZE, SWEEPER_SIZE * CELL_SIZE);

        DrawScoreboardNumbers(hdc, SHADOW_SIZE * 2 + FRAME_SIZE * 2, 
            SCORE_NUMBER_Y,
            gameBoard->GetScore(), 3);

        DrawScoreboardNumbers(hdc, SHADOW_SIZE * 2 + SWEEPER_SIZE * CELL_SIZE -
            SCOREBOARD_NUMBER_WIDTH * 3,
            SCORE_NUMBER_Y,
            gameBoard->GetPlayTime(), 3);

        DrawResetButton(hdc, RESET_BUTTON_X, RESET_BUTTON_Y, 
            !gameBoard->IsGameOver() && isClicking ? BUTTON_STATE_CLICKING :
            gameBoard->IsGameOver() && gameBoard->IsWon() ? BUTTON_STATE_VICTORY :
            gameBoard->IsGameOver() ? BUTTON_STATE_DEFEAT :
            BUTTON_STATE_NORMAL, isButtonDown);

        for (int y = 0; y < gameBoard->GetSize(); y++)
            for (int x = 0; x < gameBoard->GetSize(); x++) {
                if (isClicking && hoverX == x && hoverY == y && gameBoard->GetCellState(x,y) == CELL_UNKNOWN) {
                    DrawVisibleCell(hdc, BOARD_X + x * CELL_SIZE, BOARD_Y + y * CELL_SIZE, 0);
                    continue;
                }
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
                case CELL_FLAGGED_INVALID:
                    DrawBombCell(hdc, BOARD_X + x * CELL_SIZE, BOARD_Y + y * CELL_SIZE, false, true);
                    break;
                case CELL_INVESTIGATE:
                    DrawInvestigateCell(hdc, BOARD_X + x * CELL_SIZE, BOARD_Y + y * CELL_SIZE);
                    break;
                case CELL_EXPLOAD:
                    DrawBombCell(hdc, BOARD_X + x * CELL_SIZE, BOARD_Y + y * CELL_SIZE, true, false);
                    break;
                case CELL_BOMB:
                    DrawBombCell(hdc, BOARD_X + x * CELL_SIZE, BOARD_Y + y * CELL_SIZE, false, false);
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
