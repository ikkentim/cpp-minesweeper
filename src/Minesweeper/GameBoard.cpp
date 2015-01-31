
#include "stdafx.h"
#include "GameBoard.h"
#include <assert.h>
#include <time.h>

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
            cell->isInvestigation = false;
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
    delete[] boardCells;

    GenerateBoard(boardSize, boardMines);

    score = boardMines;
    isStarted = false;
    isVictory = false;
    isDefeat = false;
}
GameBoard::GameBoard(int size, int mines) {
    GenerateBoard(size, mines);

    score = mines;
    isStarted = false;
    isVictory = false;
    isDefeat = false;
}

GameBoard::~GameBoard() {
    delete[] boardCells;
}

void GameBoard::Reveal(int x, int y, std::stack<int> *updatedCells) {
    BoardCell *cell = GetCell(x, y);

    if (!cell || cell->isVisible || IsGameOver()) return;

    cell->isVisible = true;
    cell->isFlagged = false;

    if (cell->isBomb) isDefeat = true;
    if (IsAllCellsVisible()) isVictory = true;
    

    if (updatedCells && !IsGameOver()) updatedCells->push(x + y * boardSize);
    if (updatedCells && IsGameOver()) updatedCells->push(-1);
    
    if (cell->neighbouringBombs) return;

    if (IsDiscoverable(x - 1, y)) Reveal(x - 1, y, updatedCells);
    if (IsDiscoverable(x + 1, y)) Reveal(x + 1, y, updatedCells);
    if (IsDiscoverable(x, y - 1)) Reveal(x, y - 1, updatedCells);
    if (IsDiscoverable(x, y + 1)) Reveal(x, y + 1, updatedCells);

    if (IsDiscoverable(x - 1, y - 1)) Reveal(x - 1, y - 1, updatedCells);
    if (IsDiscoverable(x + 1, y - 1)) Reveal(x + 1, y - 1, updatedCells);
    if (IsDiscoverable(x + 1, y + 1)) Reveal(x + 1, y + 1, updatedCells);
    if (IsDiscoverable(x - 1, y + 1)) Reveal(x - 1, y + 1, updatedCells);
}
void GameBoard::Show(int x, int y, std::stack<int> *updatedCells) {
    BoardCell *cell = GetCell(x, y);

    if (!cell || cell->isVisible || cell->isFlagged || 
        cell->isInvestigation || IsGameOver()) return;

    /* Ensure first move is lucky
     */
    while (!isStarted && cell->isBomb) {
        GenerateBoard(boardSize, boardMines);
        cell = GetCell(x, y);
    }

    if (!isStarted)
        startTime = time(NULL);

    isStarted = true;
    Reveal(x, y, updatedCells);
}

void GameBoard::Flag(int x, int y) {
    BoardCell *cell = GetCell(x, y);

    if (!cell || cell->isVisible || IsGameOver()) return;

    if (!cell->isFlagged && !cell->isInvestigation) {
        cell->isFlagged = true;
        score--;
    }
    else if (cell->isFlagged) {
        cell->isFlagged = false;
        cell->isInvestigation = true;
        score++;
    }
    else if (cell->isInvestigation) {
        cell->isInvestigation = false;
    }
}

int GameBoard::GetPlayTime() {
    if (!isStarted) return 0;
    if (!IsGameOver()) endTime = time(NULL);

    return (int)difftime(endTime, startTime);
}