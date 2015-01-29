#include "stdafx.h"

#pragma once

#define CELL_SIZE                   16
#define CELL_SHADOW                 2

#define SHADOW_SIZE                 2
#define SHADOW_CORNER_OFFSET        1

#define SCOREBOARD_SIZE             30
#define FRAME_SIZE                  6

#define SCOREBOARD_NUMBER_WIDTH     13
#define SCOREBOARD_NUMBER_HEIGHT    23

#define BOARD_X                     (SHADOW_SIZE * 2 + FRAME_SIZE)
#define BOARD_Y                     (SHADOW_SIZE * 2 + FRAME_SIZE * 2 \
                                        + SCOREBOARD_SIZE)

#define FRAME_WIDTH                 (SHADOW_SIZE * 4 + FRAME_SIZE * 2)
#define FRAME_HEIGHT                (SHADOW_SIZE * 4 + FRAME_SIZE * 3 \
                                        + SCOREBOARD_SIZE)

void DrawCell(HDC hdc, int x, int y);
void DrawVisibleCell(HDC hdc, int x, int y, int count);
void DrawFlaggedCell(HDC hdc, int x, int y);
void DrawBombCell(HDC hdc, int x, int y, bool hasExploded);
void DrawBoard(HDC hdc, int width, int height, int boardWidth, int boardHeight);
void DrawScoreboardNumbers(HDC hdc, int x, int y, int score, int numbers);