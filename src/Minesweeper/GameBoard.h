#pragma once

#define CELL_UNKNOWN    0
#define CELL_VISIBLE    1
#define CELL_FLAGGED    2
#define CELL_EXPLOAD    3
#define CELL_BOMB       4

class GameBoard {
    struct BoardCell {
        int neighbouringBombs;
        bool isFlagged;
        bool isBomb;
        bool isVisible;
    };

public:
    GameBoard(int size,int bombs);
    ~GameBoard();
    void Show(int x, int y);
    void Flag(int x, int y);
    void Reset();
    int GetPlayTime();
    int GetScore() {
        return score;
    }
    int GetSize() {
        return boardSize;
    }
    bool IsGameOver() {
        return isDefeat || isVictory;
    }
    bool IsWon() {
        return isVictory;
    }
    int GetNeighbouringBombs(int x, int y) {
        return GetCell(x, y)->neighbouringBombs;
    }
    int GetCellState(int x, int y) {
        auto cell = GetCell(x, y);


        if (cell->isFlagged && IsGameOver() && cell->isBomb) return CELL_BOMB;
        if (cell->isFlagged) return CELL_FLAGGED;

        if (cell->isVisible && cell->isBomb) return CELL_EXPLOAD;
        if (cell->isVisible || (!cell->isBomb && IsGameOver())) return CELL_VISIBLE;
        if (cell->isBomb && IsGameOver()) return CELL_BOMB;
        
        return CELL_UNKNOWN;
    }
private:
    int score = 0;
    int boardSize;
    int boardMines;
    bool isDefeat;
    bool isVictory;
    bool isStarted;
    time_t startTime;
    time_t endTime;

    BoardCell *boardCells;
    BoardCell *GetCell(int x, int y) {
        if (x < 0 || x >= boardSize ||
            y < 0 || y >= boardSize)
            return NULL;
        int idx = x + y * boardSize;
        return &boardCells[idx];
    }
    void GenerateBoard(int size, int mines);
    void Reveal(int x, int y);
    bool IsDiscoverable(int x, int y) {
        auto cell = GetCell(x, y);

        return cell && !cell->isVisible && 
            !cell->isBomb && !cell->isFlagged;
    }
    bool IsAllCellsVisible() {
        for (int y = 0; y < boardSize; y++)
            for (int x = 0; x < boardSize; x++) {
                auto cell = GetCell(x, y);

                if (!cell->isBomb && !cell->isVisible)
                    return false;
            }

        return true;
    }
};
