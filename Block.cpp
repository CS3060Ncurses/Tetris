#include "Block.h"
#include "Templates.h"
#include "Colors.h"

using namespace std;

Block::Block(WINDOW* win, int id, int row, int col) 
{
    if (id == 0) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++)
                grid[i][j] = grid1[i][j];
        }  
    }

   travPrint(win, MY_RED, row, col);
  
}
bool Block::tryDown(WINDOW* win, int row, int col) {
    if (row > (22 - getLowest())) {
        return false;
    } else {
        this->moveDown(win, row, col);
        return true;
    }
}

void Block::moveDown(WINDOW* win, int row, int col) {
    travPrint(win, MY_BLACK, row++, col);
    travPrint(win, MY_RED, row, col);
}

bool Block::tryRight(WINDOW* win, int row, int col) {
    if (col > (16 - getRightMost())) {
        return false;
    } else {
        this->moveRight(win, row, col);
        return true;
    }
}

void Block::moveRight(WINDOW* win, int row, int col) {
    travPrint(win, MY_BLACK, row, col);
    col += 2;
    travPrint(win, MY_RED, row, col);
}

bool Block::tryLeft(WINDOW* win, int row, int col) {
    if (col < (getLeftMost() + 4)) {
        return false;
    } else {
        this->moveLeft(win, row, col);
        return true;
    }
}

void Block::moveLeft(WINDOW* win, int row, int col) {
    travPrint(win, MY_BLACK, row, col);
    col -= 2;
    travPrint(win, MY_RED, row, col);
}

void Block::travPrint(WINDOW *win, int color, int row, int col) {
    init_pair(1, COLOR_BLACK, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    for (int i = 0; i < 3; i++) {
        int tmpcol = col;
        for (int j = 0; j < 3; j++) {
            if (grid[i][j] == 1) {
                wattron(win, COLOR_PAIR(color));
                wmove(win, row, tmpcol);
                waddch(win, ' '|A_REVERSE); // draw first space
                wmove(win, row, tmpcol + 1);
                waddch(win, ' '|A_REVERSE); // draw second space
                wattroff(win, COLOR_PAIR(color));
                wrefresh(win);
            }
            tmpcol += 2;
        }
        row++;
    }
}

int Block::getLowest() {
    int lowest = 0;
    for (int i = 0; i < 3; i++) {
        bool brick = false;
        for (int j = 0; j < 3; j++) {
            if (grid[i][j] == 1)
                brick = true;
        }
        if (brick == true)
            lowest++;
    }
    return lowest;
}

int Block::getRightMost() {
    int rightest = 0;
    for (int i = 0; i < 3; i++) {
        int tmp = 0;
        for (int j = 0; j < 3; j++) {
            if (grid[i][j] == 1)
                tmp = j;
        }
        if (tmp > rightest)
            rightest = tmp;
    }
    return rightest;
}

int Block::getLeftMost() {
    int leftest = 0;
    for (int i = 0; i < 3; i++) {
        int tmp = 2;
        for (int j = 2; j > -1; j--) {
            if (grid[i][j] == 1)
                tmp = j;
        }
        if (tmp < leftest)
            leftest = tmp;
    }
    return leftest;
}
