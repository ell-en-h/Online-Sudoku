#ifndef SUDOKU_BOARD_HPP
#define SUDOKU_BOARD_HPP
#include <vector>
#include <string>

class SudokuBoard {
private:
    int grid[9][9];

public:
    SudokuBoard() {
            for(int i = 0; i < 9; ++i)
            for(int j = 0; j < 9; ++j)
                grid[i][j] = 0;
    }
    bool isValid(int r, int c, int val) {
        if (val < 1 || val > 9) 
		return false;
        for (int i = 0; i < 9; ++i) {
            if (grid[r][i] == val || grid[i][c] == val)
		    return false;
            if (grid[3*(r/3) + i/3][3*(c/3) + i%3] == val)
		    return false;
        }
        return true;
    }
    void setCell(int r, int c, int val) { grid[r][c] = val; }
        std::string toString() {
        std::string s = "";
        for(int i = 0; i < 9; ++i)
            for(int j = 0; j < 9; ++j)
                s += std::to_string(grid[i][j]);
        return s;
    }
};
#endif

