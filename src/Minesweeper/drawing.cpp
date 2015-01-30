#include "stdafx.h"
#include "drawing.h"
#include <stdio.h>

// colors
#define COLOR_CONTROL               RGB(0xBD, 0xBD, 0xBD)
#define COLOR_CONTROL_LIGHT         RGB(0xFF, 0xFF, 0xFF)
#define COLOR_CONTROL_DARK          RGB(0x80, 0x80, 0x80)

#define COLOR_EXPLOAD               RGB(0xFF, 0x00, 0x00)
#define COLOR_BLACK_FLAG            RGB(0x00, 0x00, 0x00)
#define COLOR_RED_FLAG              RGB(0xFF, 0x00, 0x00)
#define COLOR_BLACK_BOMB            RGB(0x00, 0x00, 0x00)
#define COLOR_WHITE_BOMB            RGB(0xFF, 0xFF, 0xFF)

#define COLOR_SCOREBOARD_BACKGROUND RGB(0x00, 0x00, 0x00)
#define COLOR_SCOREBOARD_ACTIVE     RGB(0xFF, 0x00, 0x00)
#define COLOR_SCOREBOARD_INACTIVE   RGB(0x7B, 0x00, 0x00)

void DrawShadow(HDC hdc, int x, int y, int width, int height, COLORREF colortl, COLORREF colorbr) {
    POINT Pt[6];

    /* Top-left
    */
    Pt[0].x = x;
    Pt[0].y = y;

    Pt[1].x = x + width - 1 - SHADOW_CORNER_OFFSET;
    Pt[1].y = y;

    Pt[2].x = x + width - SHADOW_SIZE - SHADOW_CORNER_OFFSET;
    Pt[2].y = y + SHADOW_SIZE - 1;

    Pt[3].x = x + SHADOW_SIZE - 1;
    Pt[3].y = y + SHADOW_SIZE - 1;

    Pt[4].x = x + SHADOW_SIZE - 1;
    Pt[4].y = y + height - SHADOW_SIZE - SHADOW_CORNER_OFFSET;

    Pt[5].x = x;
    Pt[5].y = y + height - 1 - SHADOW_CORNER_OFFSET;

    SetDCPenColor(hdc, colortl);
    SetDCBrushColor(hdc, colortl);
    Polygon(hdc, Pt, 6);

    /* Bottom right
    */
    Pt[0].x = x + width - 1;
    Pt[0].y = y + height - 1;

    Pt[1].x = x + SHADOW_CORNER_OFFSET;
    Pt[1].y = y + height - 1;

    Pt[2].x = x + SHADOW_SIZE - 1 + SHADOW_CORNER_OFFSET;
    Pt[2].y = y + height - SHADOW_SIZE;

    Pt[3].x = x + width - SHADOW_SIZE;
    Pt[3].y = y + height - SHADOW_SIZE;

    Pt[4].x = x + width - SHADOW_SIZE;
    Pt[4].y = y + SHADOW_SIZE - 1 + SHADOW_CORNER_OFFSET;

    Pt[5].x = x + width - 1;
    Pt[5].y = y + SHADOW_CORNER_OFFSET;

    SetDCPenColor(hdc, colorbr);
    SetDCBrushColor(hdc, colorbr);

    Polygon(hdc, Pt, 6);
}

void DrawImage(HDC hdc, int x, int y, int width, int height, const unsigned int *map,
    const COLORREF *colorMap, int setSize) {
    for (int oy = 0; oy < height; oy++)
        for (int ox = 0; ox < width; ox++) {
            int idx = oy * width + ox;

            int data = (map[idx / (32 / setSize)] >>
                (((32 / setSize - 1) -
                (idx % (32 / setSize))) * setSize)) & (setSize * 2 - 1);

            if (data) SetPixel(hdc, x + ox, y + oy, colorMap[data - 1]);
        }
}

void DrawCell(HDC hdc, int x, int y) {
    DrawShadow(hdc, x, y, CELL_SIZE, CELL_SIZE, COLOR_CONTROL_LIGHT, COLOR_CONTROL_DARK);
}

void DrawVisibleCell(HDC hdc, int x, int y, int count)
{
    SetDCPenColor(hdc, COLOR_CONTROL);
    SetDCBrushColor(hdc, COLOR_CONTROL);
    Rectangle(hdc, x, y, x + CELL_SIZE, y + CELL_SIZE);

    POINT Pt[3];

    Pt[0].x = x;
    Pt[0].y = y + CELL_SIZE - 1;

    Pt[1].x = x;
    Pt[1].y = y;

    Pt[2].x = x + CELL_SIZE;
    Pt[2].y = y;

    SetDCPenColor(hdc, COLOR_CONTROL_DARK);
    SetDCBrushColor(hdc, COLOR_CONTROL_DARK);
    Polyline(hdc, Pt, 3);

    if (!count) return;

    switch (count) {
    case 1:
        SetTextColor(hdc, RGB(0x00, 0x00, 0xFF));
        break;
    case 2:
        SetTextColor(hdc, RGB(0x00, 0x80, 0x00));
        break;
    case 3:
        SetTextColor(hdc, RGB(0xFF, 0x00, 0x00));
        break;
    default:
        SetTextColor(hdc, RGB(0xFF, 0x00, 0x90));
        break;
    }

    HFONT font = CreateFont(
		12, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, 
        OUT_RASTER_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY,
        DEFAULT_PITCH | FF_SCRIPT, L"Brush Script");

    SelectObject(hdc, font);

    TCHAR *text = new TCHAR;
    *text = '0' + count;

    SetBkMode(hdc, TRANSPARENT);
    TextOut(hdc, x + 5, y + 2, text, 1);

    DeleteObject(font);
}

void DrawFlaggedCell(HDC hdc, int x, int y)
{
    POINT Pt[11];

    DrawCell(hdc, x, y);

    const int flagWidth = 8;
    const int flagHeight = 10;

    const int hOffset = (CELL_SIZE - flagWidth) / 2;
    const int vOffset = (CELL_SIZE - flagHeight) / 2;

    const int baseOffsetX = x + hOffset;
    const int baseOffsetY = y + CELL_SIZE - vOffset;

    Pt[0].x = baseOffsetX;
    Pt[0].y = baseOffsetY;

    Pt[1].x = baseOffsetX;
    Pt[1].y = baseOffsetY - 1;

    Pt[2].x = baseOffsetX + 2;
    Pt[2].y = baseOffsetY - 1;

    Pt[3].x = baseOffsetX + 2;
    Pt[3].y = baseOffsetY - 2;

    Pt[4].x = baseOffsetX + 4;
    Pt[4].y = baseOffsetY - 2;

    Pt[5].x = baseOffsetX + 4;
    Pt[5].y = baseOffsetY - 4;

    Pt[6].x = baseOffsetX + 4;
    Pt[6].y = baseOffsetY - 2;

    Pt[7].x = baseOffsetX + 5;
    Pt[7].y = baseOffsetY - 2;

    Pt[8].x = baseOffsetX + 5;
    Pt[8].y = baseOffsetY - 1;

    Pt[9].x = baseOffsetX + 7;
    Pt[9].y = baseOffsetY - 1;

    Pt[10].x = baseOffsetX + 7;
    Pt[10].y = baseOffsetY;

    SetDCPenColor(hdc, COLOR_BLACK_FLAG);
    SetDCBrushColor(hdc, COLOR_BLACK_FLAG);

    Polygon(hdc, Pt, 11);

    const int flagOffsetX = baseOffsetX + 4;
    const int flagOffsetY = baseOffsetY - 5;

    Pt[0].x = flagOffsetX;
    Pt[0].y = flagOffsetY;

    Pt[1].x = flagOffsetX;
    Pt[1].y = flagOffsetY - 4;

    Pt[2].x = flagOffsetX - 1;
    Pt[2].y = flagOffsetY - 4;

    Pt[3].x = flagOffsetX - 1;
    Pt[3].y = flagOffsetY - 3;

    Pt[4].x = flagOffsetX - 3;
    Pt[4].y = flagOffsetY - 3;

    Pt[5].x = flagOffsetX - 3;
    Pt[5].y = flagOffsetY - 2;

    Pt[6].x = flagOffsetX - 4;
    Pt[6].y = flagOffsetY - 2;

    Pt[7].x = flagOffsetX - 3;
    Pt[7].y = flagOffsetY - 2;

    Pt[8].x = flagOffsetX - 3;
    Pt[8].y = flagOffsetY - 1;

    Pt[9].x = flagOffsetX - 1;
    Pt[9].y = flagOffsetY - 1;

    Pt[10].x = flagOffsetX - 1;
    Pt[10].y = flagOffsetY;

    SetDCPenColor(hdc, COLOR_RED_FLAG);
    SetDCBrushColor(hdc, COLOR_RED_FLAG);

    Polygon(hdc, Pt, 11);

}

void DrawBombCell(HDC hdc, int x, int y, bool hasExploded)
{
    const unsigned int bomb[] = {
        0x00040000, 0x01000045, 0x54400555, 0x4005A554, 0x01695505, 0x55555415,
        0x55500555, 0x54005554, 0x00455440, 0x00100000, 0x10000000
    };

    const COLORREF palette[] = {
        RGB(0, 0, 0), RGB(255, 255, 255)
    };

    DrawVisibleCell(hdc, x, y, 0);
    DrawImage(hdc, x + 2, y + 2, 13, 13, bomb, palette, 2);
}

void DrawScoreboardNumber(HDC hdc, int x, int y, int number) {
    const int numBmp[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x01, 0x11, 0x01, 0x11, 0x01, 0x11, 0x01, 0x11, 0x01, 0x00, 0x00,
        0x00, 0x12, 0x00, 0x01, 0x11, 0x01, 0x11, 0x01, 0x11, 0x01, 0x00, 0x13, 0x00,
        0x00, 0x02, 0x12, 0x00, 0x01, 0x11, 0x01, 0x11, 0x01, 0x00, 0x13, 0x03, 0x00,
        0x00, 0x12, 0x02, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x03, 0x13, 0x00,
        0x00, 0x02, 0x12, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x13, 0x03, 0x00,
        0x00, 0x12, 0x02, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x03, 0x13, 0x00,
        0x00, 0x02, 0x12, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x13, 0x03, 0x00,
        0x00, 0x12, 0x02, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x03, 0x13, 0x00,
        0x00, 0x02, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x03, 0x00,
        0x00, 0x12, 0x00, 0x04, 0x14, 0x04, 0x14, 0x04, 0x14, 0x04, 0x00, 0x13, 0x00,
        0x00, 0x00, 0x04, 0x14, 0x04, 0x14, 0x04, 0x14, 0x04, 0x14, 0x04, 0x00, 0x00,
        0x00, 0x15, 0x00, 0x04, 0x14, 0x04, 0x14, 0x04, 0x14, 0x04, 0x00, 0x16, 0x00,
        0x00, 0x05, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0x06, 0x00,
        0x00, 0x15, 0x05, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0x06, 0x16, 0x00,
        0x00, 0x05, 0x15, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x16, 0x06, 0x00,
        0x00, 0x15, 0x05, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0x06, 0x16, 0x00,
        0x00, 0x05, 0x15, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x16, 0x06, 0x00,
        0x00, 0x15, 0x05, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0x06, 0x16, 0x00,
        0x00, 0x05, 0x15, 0x00, 0x07, 0x17, 0x07, 0x17, 0x07, 0x00, 0x16, 0x06, 0x00,
        0x00, 0x15, 0x00, 0x07, 0x17, 0x07, 0x17, 0x07, 0x17, 0x07, 0x00, 0x16, 0x00,
        0x00, 0x00, 0x07, 0x17, 0x07, 0x17, 0x07, 0x17, 0x07, 0x17, 0x07, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    const int numActive[] = { 
        0xEE, 0x48, 0xBA, 0xDA, 0x5C, 0xD6, 0xF7, 0x4A, 0xFE, 0xDE, 0x10 };

    /* Fill background
    */
    SetDCPenColor(hdc, COLOR_SCOREBOARD_BACKGROUND);
    SetDCBrushColor(hdc, COLOR_SCOREBOARD_BACKGROUND);
    Rectangle(hdc, x, y, x + SCOREBOARD_NUMBER_WIDTH, y + SCOREBOARD_NUMBER_HEIGHT);


    /* Draw number
    */
    int activeSections = numActive[number];
    for (int oy = 0; oy < SCOREBOARD_NUMBER_HEIGHT; oy++)
        for (int ox = 0; ox < SCOREBOARD_NUMBER_WIDTH; ox++) {
            int px = numBmp[ox + oy * SCOREBOARD_NUMBER_WIDTH];

            if (!px) continue;

            if ((activeSections >> (px & 0x07)) & 0x01 == 0x01) {
                SetPixel(hdc, x + ox, y + oy, COLOR_SCOREBOARD_ACTIVE);
            }
            else {
                
                SetPixel(hdc, x + ox, y + oy, px >> 3 ? COLOR_SCOREBOARD_INACTIVE : COLOR_SCOREBOARD_BACKGROUND);
            }
        }
}

void DrawScoreboardNumbers(HDC hdc, int x, int y, int score, int numbers) {

    DrawShadow(hdc, x - 1, y - 1, SCOREBOARD_NUMBER_WIDTH * numbers + 2,
        SCOREBOARD_NUMBER_HEIGHT + 2, COLOR_CONTROL_DARK, COLOR_CONTROL_LIGHT);

    int pow = 1;
    for (int i = 0; i < numbers; i++) {
        pow *= 10;
    }

    if (score >= pow)
        score = pow - 1;

    if (score < -pow / 10)
        score = -pow / 10 + 1;

    if (score < 0) {
        DrawScoreboardNumber(hdc, x, y, 10);

        // shift
        numbers--;
        score = -score;
        x += SCOREBOARD_NUMBER_WIDTH;
    }

    for (int n = 0; n < numbers; n++) {
        int offset = (numbers - n - 1) * SCOREBOARD_NUMBER_WIDTH;

        DrawScoreboardNumber(hdc, x + offset, y, score % 10);
        score /= 10;
    }
}

void DrawBoard(HDC hdc, int width, int height, int boardWidth, int boardHeight) {
    POINT Pt[6];

    /* Fill background
     */
    SetDCPenColor(hdc, COLOR_CONTROL);
    SetDCBrushColor(hdc, COLOR_CONTROL);
    Rectangle(hdc, 0, 0, width, height);

    /* Frame: Top-left border
     */

    DrawShadow(hdc, 0, 0, width, height, COLOR_CONTROL_LIGHT, COLOR_CONTROL_DARK);
    DrawShadow(hdc, BOARD_X - SHADOW_SIZE, BOARD_Y - SHADOW_SIZE, 
        boardWidth + SHADOW_SIZE * 2, boardHeight + SHADOW_SIZE * 2, 
        COLOR_CONTROL_DARK, COLOR_CONTROL_LIGHT);

    DrawShadow(hdc, SHADOW_SIZE + FRAME_SIZE, SHADOW_SIZE + FRAME_SIZE,
        width - SHADOW_SIZE * 2 - FRAME_SIZE * 2, SCOREBOARD_SIZE, 
        COLOR_CONTROL_DARK, COLOR_CONTROL_LIGHT);
}

void DrawResetButton(HDC hdc, int x, int y, int status, bool isDown) {
    const unsigned int normal[] = {
        0x00055400, 0x0016AA50, 0x001AAAA9, 0x001AAAAA, 0x901AAAAA, 0xA906A5A9,
        0x6A46A96A, 0x5AA5AAAA, 0xAAA96AAA, 0xAAAA5AAA, 0xAAAA96A6, 0xAAA6A46A,
        0x6AA6A41A, 0xA556A901, 0xAAAAA900, 0x1AAAA900, 0x016AA500, 0x00055400,
        0x00000000
    };

    const unsigned int clicking[] = {
        0x00055400, 0x0016AA50, 0x001AAAA9, 0x001AAAAA, 0x901ADEAD, 0xE90695A9,
        0x5A46ADEA, 0xDEA5AAAA, 0xAAA96AAA, 0xAAAA5AAA, 0x56AA96AA, 0xD9EAA46A,
        0x9A9AA41A, 0xAD9EA901, 0xAA56A900, 0x1AAAA900, 0x016AA500, 0x00055400,
        0x00000000
    };

    const unsigned int defeat[] = {
        0x00055400, 0x0016AA50, 0x001AAAA9, 0x001AAAAA, 0x901A66A6, 0x6906A6AA,
        0x6A46A66A, 0x66A5AAAA, 0xAAA96AAA, 0xAAAA5AAA, 0xAAAA96AA, 0x556AA46A,
        0x6AA6A41A, 0x6AAA6901, 0xAAAAA900, 0x1AAAA900, 0x016AA500, 0x00055400,
        0x00000000
    };

    const unsigned int victory[] = {
        0x00055400, 0x0016AA50, 0x001AAAA9, 0x001AAAAA, 0x901AAAAA, 0xA9069555,
        0x5A469559, 0x55A59956, 0x55995A16, 0xA5295AAA, 0xAAAA96AA, 0xAAAAA46A,
        0x6AA6A41A, 0xA556A901, 0xAAAAA900, 0x1AAAA900, 0x016AA500, 0x00055400,
        0x00000000
    };
    const COLORREF palette[] = {
        RGB(0, 0, 0), RGB(255, 255, 0), RGB(80, 80, 0)
    };

    DrawShadow(hdc, x, y, RESET_BUTTON_SIZE, RESET_BUTTON_SIZE, 
        COLOR_CONTROL_DARK, COLOR_CONTROL_DARK);

    if (!isDown) {
        DrawShadow(hdc, x + 1, y + 1, 
            RESET_BUTTON_SIZE - 2, RESET_BUTTON_SIZE - 2,
            COLOR_CONTROL_LIGHT, COLOR_CONTROL_DARK);
    }
    else {
        SetDCPenColor(hdc, COLOR_CONTROL);
        SetDCBrushColor(hdc, COLOR_CONTROL);
        Rectangle(hdc, x + 2, y + 2,
            x + RESET_BUTTON_SIZE - 2, y + RESET_BUTTON_SIZE - 2);
    }

    DrawImage(hdc, x + 5 + (isDown ? 1 : 0), y + 5 + (isDown ? 1 : 0), 17, 17, 
        status == BUTTON_STATE_CLICKING ? clicking :
        status == BUTTON_STATE_DEFEAT ? defeat :
        status == BUTTON_STATE_VICTORY ? victory :
        normal, palette, 2);
}