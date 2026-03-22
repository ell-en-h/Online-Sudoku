#ifndef SUDOKU_BOARD_HPP
#define SUDOKU_BOARD_HPP

#include <vector>
#include <string>
#include <ctime>   
#include <cstdlib> 

class SudokuBoard {
private:
    int grid[9][9];
public:
    SudokuBoard() {
        for(int i = 0; i < 9; ++i)
            for(int j = 0; j < 9; ++j)
                grid[i][j] = 0;
    }
    void initializeRandom(int count = 10) {
        std::srand(std::time(0)); 
        int placed = 0;
        while (placed < count) {
            int r = std::rand() % 9;
            int c = std::rand() % 9;
            int val = (std::rand() % 9) + 1;
            if (grid[r][c] == 0 && isValid(r, c, val)) {
                setCell(r, c, val);
                placed++;
            }
        }
    }
    bool isValid(int r, int c, int val) {
        if (r < 0 || r > 8 || c < 0 || c > 8) return false;
        if (val < 1 || val > 9) return false;
        for (int i = 0; i < 9; ++i) {
            if (grid[r][i] == val || grid[i][c] == val)
                return false;
            if (grid[3*(r/3) + i/3][3*(c/3) + i%3] == val)
                return false;
        }
        return true;
    }
    void setCell(int r, int c, int val) {
        if (r >= 0 && r < 9 && c >= 0 && c < 9) {
            grid[r][c] = val;
        }
    }
    std::string toString() {
        std::string s = "";
        for(int i = 0; i < 9; ++i)
            for(int j = 0; j < 9; ++j)
                s += std::to_string(grid[i][j]);
        return s;
    }
};
#endif
