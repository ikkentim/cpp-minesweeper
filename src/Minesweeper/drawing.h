#include "stdafx.h"

#pragma once

#define CELL_SIZE      28
#define CELL_SHADOW    3

#define BOARD_SHADOW 9
#define BOARD_OFFSET 18
#define BOARD_X (BOARD_SHADOW + BOARD_OFFSET)
#define BOARD_Y (43 + BOARD_SHADOW + BOARD_OFFSET)
//add 43
//...
void DrawCell(HDC hdc, int x, int y);
void DrawVisibleCell(HDC hdc, int x, int y, int count);
void DrawFlaggedCell(HDC hdc, int x, int y);
void DrawBombCell(HDC hdc, int x, int y, bool hasExploded);
void DrawBoard(HDC hdc, int width, int height, int boardWidth, int boardHeight);