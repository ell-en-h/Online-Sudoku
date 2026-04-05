#ifndef SUDOKU_BOARD_HPP
#define SUDOKU_BOARD_HPP

#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>

class SudokuBoard {
private:
    int grid[9][9];

    bool canSolve(int tempGrid[9][9], int r, int c) {
        if (r == 8 && c == 9) return true;
        if (c == 9) { r++; c = 0; }
        if (tempGrid[r][c] != 0) return canSolve(tempGrid, r, c + 1);
        
        for (int n = 1; n <= 9; n++) {
            if (isSafeInternal(tempGrid, r, c, n)) {
                tempGrid[r][c] = n;
                if (canSolve(tempGrid, r, c + 1)) return true;
                tempGrid[r][c] = 0;
            }
        }
        return false;
    }

    bool isSafeInternal(int g[9][9], int r, int c, int n) const {
        for (int x = 0; x < 9; x++)
            if (g[r][x] == n || g[x][c] == n) return false;
        int sR = r - r % 3, sC = c - c % 3;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (g[i + sR][j + sC] == n) return false;
        return true;
    }

public:
    SudokuBoard() {
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++) grid[i][j] = 0;
    }

    bool isValid(int row, int col, int val) {
        if (row < 0 || row >= 9 || col < 0 || col >= 9 || val < 1 || val > 9) return false;
        if (grid[row][col] != 0) return false;

        if (!isSafeInternal(grid, row, col, val)) return false;

        int temp[9][9];
        for(int i=0; i<9; ++i) for(int j=0; j<9; ++j) temp[i][j] = grid[i][j];
        temp[row][col] = val;
        
        return canSolve(temp, 0, 0); 
    }

    void setCell(int r, int c, int v) { grid[r][c] = v; }

    bool isFull() const {
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++)
                if (grid[i][j] == 0) return false;
        return true;
    }

    std::string toString() const {
        std::string s = "";
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++) s += std::to_string(grid[i][j]);
        return s;
    }

    void initializeRandom(int cellsToKeep) {
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++) grid[i][j] = 0;
        
        static std::mt19937 g(static_cast<unsigned int>(std::time(nullptr)));
        
        int temp[9][9] = {0};
        for (int i = 0; i < 9; i += 3) {
            std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9};
            std::shuffle(nums.begin(), nums.end(), g);
            int idx = 0;
            for (int r = 0; r < 3; r++)
                for (int c = 0; c < 3; c++) grid[i + r][i + c] = nums[idx++];
        }
        
        int fillTemp[9][9];
        for(int i=0; i<9; i++) for(int j=0; j<9; j++) fillTemp[i][j] = grid[i][j];
        canSolve(fillTemp, 0, 0);
        for(int i=0; i<9; i++) for(int j=0; j<9; j++) grid[i][j] = fillTemp[i][j];

        int toRemove = 81 - cellsToKeep;
        while (toRemove > 0) {
            int r = g() % 9;
            int c = g() % 9;
            if (grid[r][c] != 0) {
                grid[r][c] = 0;
                toRemove--;
            }
        }
    }
};
#endif
