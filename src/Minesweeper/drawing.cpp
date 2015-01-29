#include "stdafx.h"
#include "drawing.h"

// colors
#define COLOR_CONTROL               RGB(0xBD, 0xBD, 0xBD)
#define COLOR_CONTROL_LIGHT         RGB(0xFF, 0xFF, 0xFF)
#define COLOR_CONTROL_DARK          RGB(0x7B, 0x7B, 0x7B)

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

    Pt[2].x = x + CELL_SIZE - 1;
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
    POINT Pt[8];
    DWORD  lpPts[] = { 2, 2, 2, 2 };

    SetDCPenColor(hdc, hasExploded ? COLOR_EXPLOAD : COLOR_CONTROL);
    SetDCBrushColor(hdc, hasExploded ? COLOR_EXPLOAD : COLOR_CONTROL);
    Rectangle(hdc, x, y, x + CELL_SIZE, y + CELL_SIZE);

    Pt[0].x = x;
    Pt[0].y = y + CELL_SIZE - 1;

    Pt[1].x = x;
    Pt[1].y = y;

    Pt[2].x = x + CELL_SIZE - 1;
    Pt[2].y = y;

    SetDCPenColor(hdc, COLOR_CONTROL_DARK);
    SetDCBrushColor(hdc, COLOR_CONTROL_DARK);
    Polyline(hdc, Pt, 3);

    const int bombSize = 14;
    const int spikesOffset = 2;
    const int reflectionOffset = 3;
    const int reflectionSize = 2;

    const int offset = (CELL_SIZE - bombSize) / 2;

    const int offsetX = x + offset;
    const int offsetY = y + offset;

    SetDCPenColor(hdc, COLOR_BLACK_BOMB);
    SetDCBrushColor(hdc, COLOR_BLACK_BOMB);

    Ellipse(hdc, offsetX + spikesOffset, offsetY + spikesOffset, 
        offsetX + bombSize - spikesOffset, offsetY + bombSize - spikesOffset);


    Pt[0].x = offsetX + spikesOffset;
    Pt[0].y = offsetY + spikesOffset;

    Pt[1].x = offsetX + bombSize - spikesOffset;
    Pt[1].y = offsetY + bombSize - spikesOffset;

    Pt[2].x = offsetX + bombSize - spikesOffset;
    Pt[2].y = offsetY + spikesOffset;

    Pt[3].x = offsetX + spikesOffset;
    Pt[3].y = offsetY + bombSize - spikesOffset;

    Pt[4].x = offsetX + (bombSize / 2);
    Pt[4].y = offsetY;

    Pt[5].x = offsetX + (bombSize / 2);
    Pt[5].y = offsetY + bombSize;

    Pt[6].x = offsetX;
    Pt[6].y = offsetY + (bombSize / 2);

    Pt[7].x = offsetX + bombSize;
    Pt[7].y = offsetY + (bombSize / 2);

    PolyPolyline(hdc, Pt, lpPts, 4);

    SetDCPenColor(hdc, COLOR_WHITE_BOMB);
    SetDCBrushColor(hdc, COLOR_WHITE_BOMB);

    Ellipse(hdc, offsetX + spikesOffset + reflectionOffset, 
        offsetY + spikesOffset + reflectionOffset,
        offsetX + spikesOffset + reflectionOffset + reflectionSize, 
        offsetY + spikesOffset + reflectionOffset + reflectionSize);

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
        0xEE, 0x48, 0xBA, 0xDA, 0x5C, 0xD6, 0xF7, 0x4A, 0xFE, 0xDE };

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
    int pow = 1;
    for (int i = 0; i < numbers; i++) {
        pow *= 10;
    }

    if (score >= pow)
        score = pow - 1;

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
    POINT Pt[6];

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

    SetDCPenColor(hdc, RGB(0,0,0));
    SetDCBrushColor(hdc, RGB(255,255,0));
    Ellipse(hdc, x + 5, y + 5, x + 5 + 17, y + 5 + 17);

    SetDCPenColor(hdc, RGB(0, 0, 0));
    SetDCBrushColor(hdc, RGB(0, 0, 0));
    switch (status) {
    default:
        Pt[0].x = x + 10;
        Pt[0].y = y + 10;

        Pt[1].x = x + 11;
        Pt[1].y = y + 10;

        Pt[2].x = x + 11;
        Pt[2].y = y + 11;

        Pt[3].x = x + 10;
        Pt[3].y = y + 11;

        Polygon(hdc, Pt, 4);

        Pt[0].x = x + 15;
        Pt[0].y = y + 10;

        Pt[1].x = x + 16;
        Pt[1].y = y + 10;

        Pt[2].x = x + 16;
        Pt[2].y = y + 11;

        Pt[3].x = x + 15;
        Pt[3].y = y + 11;

        Polygon(hdc, Pt, 4);

        Pt[0].x = x + 9;
        Pt[0].y = y + 15;

        Pt[1].x = x + 11;
        Pt[1].y = y + 17;

        Pt[2].x = x + 15;
        Pt[2].y = y + 17;

        Pt[3].x = x + 18;
        Pt[3].y = y + 14;

        Polyline(hdc, Pt, 4);
        break;
    }
}