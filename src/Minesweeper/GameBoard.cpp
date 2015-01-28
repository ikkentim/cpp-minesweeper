
#include "stdafx.h"
#include "GameBoard.h"
#include <assert.h>

void GameBoard::GenerateBoard(int size, int mines) {
    boardSize = size;
    boardMines = mines;
    boardCells = new BoardCell[size * size];

    // initialize
    for (int y = 0; y < size; y++)
        for (int x = 0; x < size; x++) {
            BoardCell *cell = GetCell(x, y);
            cell->isBomb = false;
            cell->isFlagged = false;
            cell->isVisible = false;
            cell->neighbouringBombs = 0;
        }

    // generate minefield
    for (int idx = 0; idx < mines; idx++) {
        int x = rand() % size;
        int y = rand() % size;

        BoardCell *cell = GetCell(x, y);

        if (cell->isBomb) {
            idx--;
            continue;
        }

        cell->isBomb = true;

        if (GetCell(x - 1, y)) GetCell(x - 1, y)->neighbouringBombs++;
        if (GetCell(x + 1, y)) GetCell(x + 1, y)->neighbouringBombs++;
        if (GetCell(x, y - 1)) GetCell(x, y - 1)->neighbouringBombs++;
        if (GetCell(x, y + 1)) GetCell(x, y + 1)->neighbouringBombs++;

        if (GetCell(x - 1, y - 1)) GetCell(x - 1, y - 1)->neighbouringBombs++;
        if (GetCell(x + 1, y - 1)) GetCell(x + 1, y - 1)->neighbouringBombs++;
        if (GetCell(x + 1, y + 1)) GetCell(x + 1, y + 1)->neighbouringBombs++;
        if (GetCell(x - 1, y + 1)) GetCell(x - 1, y + 1)->neighbouringBombs++;
    }
}

void GameBoard::Reset() {
    GenerateBoard(boardSize, boardMines);

    isGameOver = false;
}
GameBoard::GameBoard(int size, int mines) {
    GenerateBoard(size, mines);

    isGameOver = false;
}

GameBoard::~GameBoard() {
    delete[] boardCells;
}

void GameBoard::Show(int x, int y) {
    BoardCell *cell = GetCell(x, y);

    if (!cell || cell->isVisible || isGameOver) return;

    cell->isVisible = true;
    cell->isFlagged = false;

    if (cell->isBomb || IsAllCellsVisible() || IsAllBombsFlagged()) isGameOver = true;

    if (cell->neighbouringBombs) return;

    if (IsDiscoverable(x - 1, y)) Show(x - 1, y);
    if (IsDiscoverable(x + 1, y)) Show(x + 1, y);
    if (IsDiscoverable(x, y - 1)) Show(x, y - 1);
    if (IsDiscoverable(x, y + 1)) Show(x, y + 1);
}

void GameBoard::Flag(int x, int y) {
    BoardCell *cell = GetCell(x, y);

    if (!cell || cell->isVisible || isGameOver) return;

    cell->isFlagged = !cell->isFlagged;
}
