#include "stdafx.h"
#include "drawing.h"

// colors
#define COLOR_CONTROL       RGB(0xC9, 0xC9, 0xC9)
#define COLOR_CONTROL_LIGHT RGB(0xE0, 0xE0, 0xE0)
#define COLOR_CONTROL_DARK  RGB(0x70, 0x70, 0x70)
#define COLOR_EXPLOAD       RGB(0xFF, 0x00, 0x00)
#define COLOR_TEXT          RGB(0x00, 0x00, 0x00)
#define COLOR_BLACK_FLAG    RGB(0x00, 0x00, 0x00)
#define COLOR_RED_FLAG      RGB(0xFF, 0x00, 0x00)
#define COLOR_BLACK_BOMB    RGB(0x00, 0x00, 0x00)
#define COLOR_WHITE_BOMB    RGB(0xFF, 0xFF, 0xFF)


void DrawCell(HDC hdc, int x, int y)
{
    POINT Pt[6];

    SetDCPenColor(hdc, COLOR_CONTROL);
    SetDCBrushColor(hdc, COLOR_CONTROL);
    Rectangle(hdc, x, y, x + CELL_SIZE, y + CELL_SIZE);

    // lt
    Pt[0].x = x + 0;
    Pt[0].y = y + 0;

    Pt[1].x = x + CELL_SIZE;
    Pt[1].y = y + 0;

    Pt[2].x = x + CELL_SIZE - CELL_SHADOW;
    Pt[2].y = y + CELL_SHADOW;

    Pt[3].x = x + CELL_SHADOW;
    Pt[3].y = y + CELL_SHADOW;

    Pt[4].x = x + CELL_SHADOW;
    Pt[4].y = y + CELL_SIZE - CELL_SHADOW;

    Pt[5].x = x + 0;
    Pt[5].y = y + CELL_SIZE;

    SetDCPenColor(hdc, COLOR_CONTROL_LIGHT);
    SetDCBrushColor(hdc, COLOR_CONTROL_LIGHT);
    Polygon(hdc, Pt, 6);

    // rb
    Pt[0].x = x + CELL_SIZE;
    Pt[0].y = y + CELL_SIZE;

    Pt[1].x = x + 0;
    Pt[1].y = y + CELL_SIZE;

    Pt[2].x = x + CELL_SHADOW;
    Pt[2].y = y + CELL_SIZE - CELL_SHADOW;

    Pt[3].x = x + CELL_SIZE - CELL_SHADOW;
    Pt[3].y = y + CELL_SIZE - CELL_SHADOW;

    Pt[4].x = x + CELL_SIZE - CELL_SHADOW;
    Pt[4].y = y + CELL_SHADOW;

    Pt[5].x = x + CELL_SIZE;
    Pt[5].y = y + 0;

    SetDCPenColor(hdc, COLOR_CONTROL_DARK);
    SetDCBrushColor(hdc, COLOR_CONTROL_DARK);

    Polygon(hdc, Pt, 6);
}


void DrawVisibleCell(HDC hdc, int x, int y, int count)
{
    SetDCPenColor(hdc, COLOR_CONTROL_DARK);
    SetDCBrushColor(hdc, COLOR_CONTROL);
    Rectangle(hdc, x, y, x + CELL_SIZE, y + CELL_SIZE);

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
		18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, 
        OUT_RASTER_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY,
        DEFAULT_PITCH | FF_SCRIPT, L"Brush Script");

    SelectObject(hdc, font);

    TCHAR *text = new TCHAR;
    *text = '0' + count;

    SetBkMode(hdc, TRANSPARENT);
    TextOut(hdc, x + 10, y + 5, text, 1);

    DeleteObject(font);
}

void DrawFlaggedCell(HDC hdc, int x, int y)
{
    POINT Pt[12];

    DrawCell(hdc, x, y);

    const int flagWidth = 14;
    const int flagHeight = 18;

    const int hOffset = (CELL_SIZE - flagWidth) / 2;
    const int vOffset = (CELL_SIZE - flagHeight) / 2;

    const int baseOffsetX = x + hOffset;
    const int baseOffsetY = y + CELL_SIZE - vOffset;

    Pt[0].x = baseOffsetX;
    Pt[0].y = baseOffsetY;

    Pt[1].x = baseOffsetX;
    Pt[1].y = baseOffsetY - 3;

    Pt[2].x = baseOffsetX + 4;
    Pt[2].y = baseOffsetY - 3;

    Pt[3].x = baseOffsetX + 4;
    Pt[3].y = baseOffsetY - 3 - 2;

    Pt[4].x = baseOffsetX + 4 + 3;
    Pt[4].y = baseOffsetY - 3 - 2;

    Pt[5].x = baseOffsetX + 4 + 3;
    Pt[5].y = baseOffsetY - 3 - 2 - 4;

    Pt[6].x = baseOffsetX + 4 + 3 + 2;
    Pt[6].y = baseOffsetY - 3 - 2 - 4;

    Pt[7].x = baseOffsetX + 4 + 3 + 2;
    Pt[7].y = baseOffsetY - 3 - 2;

    Pt[8].x = baseOffsetX + 4 + 3 + 2 + 1;
    Pt[8].y = baseOffsetY - 3 - 2;

    Pt[9].x = baseOffsetX + 4 + 3 + 2 + 1;
    Pt[9].y = baseOffsetY - 3;

    Pt[10].x = baseOffsetX + 4 + 3 + 2 + 1 + 4;
    Pt[10].y = baseOffsetY - 3;

    Pt[11].x = baseOffsetX + 4 + 3 + 2 + 1 + 4;
    Pt[11].y = baseOffsetY;

    SetDCPenColor(hdc, COLOR_BLACK_FLAG);
    SetDCBrushColor(hdc, COLOR_BLACK_FLAG);

    Polygon(hdc, Pt, 12);

    const int flagOffsetX = x + hOffset + 9;
    const int flagOffsetY = y + vOffset;

    Pt[0].x = flagOffsetX;
    Pt[0].y = flagOffsetY;

    Pt[1].x = flagOffsetX - 4;
    Pt[1].y = flagOffsetY;

    Pt[2].x = flagOffsetX - 4;
    Pt[2].y = flagOffsetY + 2;

    Pt[3].x = flagOffsetX - 4 - 3;
    Pt[3].y = flagOffsetY + 2;

    Pt[4].x = flagOffsetX - 4 - 3;
    Pt[4].y = flagOffsetY + 2 + 2;

    Pt[5].x = flagOffsetX - 4 - 3 - 2;
    Pt[5].y = flagOffsetY + 2 + 2;

    Pt[6].x = flagOffsetX - 4 - 3 - 2;
    Pt[6].y = flagOffsetY + 2 + 2 + 2;

    Pt[7].x = flagOffsetX - 4 - 3;
    Pt[7].y = flagOffsetY + 2 + 2 + 2;

    Pt[8].x = flagOffsetX - 4 - 3;
    Pt[8].y = flagOffsetY + 2 + 2 + 2 + 1;

    Pt[9].x = flagOffsetX - 4;
    Pt[9].y = flagOffsetY + 2 + 2 + 2 + 1;

    Pt[10].x = flagOffsetX - 4;
    Pt[10].y = flagOffsetY + 2 + 2 + 2 + 1 + 2;

    Pt[11].x = flagOffsetX;
    Pt[11].y = flagOffsetY + 2 + 2 + 2 + 1 + 2;

    SetDCPenColor(hdc, COLOR_RED_FLAG);
    SetDCBrushColor(hdc, COLOR_RED_FLAG);

    Polygon(hdc, Pt, 12);

}

void DrawBombCell(HDC hdc, int x, int y, bool hasExploded)
{
    SetDCPenColor(hdc, COLOR_CONTROL_DARK);
    SetDCBrushColor(hdc, hasExploded ? COLOR_EXPLOAD : COLOR_CONTROL);
    Rectangle(hdc, x, y, x + CELL_SIZE, y + CELL_SIZE);

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

    POINT Pt[8];
    DWORD  lpPts[] = { 2, 2, 2, 2};

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

#define BOARD_SHADOW 9
void DrawBoard(HDC hdc, int width, int height, int boardWidth, int boardHeight) {
    POINT Pt[6];

    SetDCPenColor(hdc, COLOR_CONTROL);
    SetDCBrushColor(hdc, COLOR_CONTROL);
    Rectangle(hdc, 0, 0, width, height);

    // lt
    Pt[0].x = 0;
    Pt[0].y = 0;

    Pt[1].x = width;
    Pt[1].y = 0;

    Pt[2].x = width - BOARD_SHADOW;
    Pt[2].y = BOARD_SHADOW;

    Pt[3].x = BOARD_SHADOW;
    Pt[3].y = BOARD_SHADOW;

    Pt[4].x = BOARD_SHADOW;
    Pt[4].y = height - BOARD_SHADOW;

    Pt[5].x =0;
    Pt[5].y = height;

    SetDCPenColor(hdc, COLOR_CONTROL_LIGHT);
    SetDCBrushColor(hdc, COLOR_CONTROL_LIGHT);
    Polygon(hdc, Pt, 6);

    // rb
    Pt[0].x = width;
    Pt[0].y = height;

    Pt[1].x = 0;
    Pt[1].y = height;

    Pt[2].x = BOARD_SHADOW;
    Pt[2].y = height - BOARD_SHADOW;

    Pt[3].x = width - BOARD_SHADOW;
    Pt[3].y = height - BOARD_SHADOW;

    Pt[4].x = width - BOARD_SHADOW;
    Pt[4].y = BOARD_SHADOW;

    Pt[5].x = width;
    Pt[5].y = 0;

    SetDCPenColor(hdc, COLOR_CONTROL_DARK);
    SetDCBrushColor(hdc, COLOR_CONTROL_DARK);
    Polygon(hdc, Pt, 6);

    //mid section
    // lt
    Pt[0].x = BOARD_X - BOARD_SHADOW;
    Pt[0].y = BOARD_Y - BOARD_SHADOW;

    Pt[1].x = BOARD_X + boardWidth + BOARD_SHADOW;
    Pt[1].y = BOARD_Y - BOARD_SHADOW;

    Pt[2].x = BOARD_X + boardWidth;
    Pt[2].y = BOARD_Y;

    Pt[3].x = BOARD_X;
    Pt[3].y = BOARD_Y;

    Pt[4].x = BOARD_X;
    Pt[4].y = BOARD_Y + boardHeight;

    Pt[5].x = BOARD_X - BOARD_SHADOW;
    Pt[5].y = BOARD_Y + boardHeight + BOARD_SHADOW;

    SetDCPenColor(hdc, COLOR_CONTROL_DARK);
    SetDCBrushColor(hdc, COLOR_CONTROL_DARK);
    Polygon(hdc, Pt, 6);

    // rb
    Pt[0].x = BOARD_X + boardWidth;
    Pt[0].y = BOARD_Y + boardHeight;

    Pt[1].x = BOARD_X;
    Pt[1].y = BOARD_Y + boardHeight;

    Pt[2].x = BOARD_X - BOARD_SHADOW;
    Pt[2].y = BOARD_Y + boardHeight + BOARD_SHADOW;

    Pt[3].x = BOARD_X + boardWidth + BOARD_SHADOW;
    Pt[3].y = BOARD_Y + boardHeight + BOARD_SHADOW;

    Pt[4].x = BOARD_X + boardWidth + BOARD_SHADOW;
    Pt[4].y = BOARD_Y - BOARD_SHADOW;

    Pt[5].x = BOARD_X + boardWidth;
    Pt[5].y = BOARD_Y;

    SetDCPenColor(hdc, COLOR_CONTROL_LIGHT);
    SetDCBrushColor(hdc, COLOR_CONTROL_LIGHT);
    Polygon(hdc, Pt, 6);
}