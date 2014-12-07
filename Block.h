#include <ncurses.h>

using namespace std;
#ifndef BLOCK_H
#define BLOCK_H

class Block {
public:
    int grid[3][3];
    Block();
    Block(WINDOW* win, int id, int row, int col);
    bool tryRight(WINDOW* win, int row, int col);
    bool tryLeft(WINDOW* win, int row, int col);
    bool tryDown(WINDOW* win, int row, int col);
    void moveDown(WINDOW* win, int row, int col);
    void moveRight(WINDOW* win, int row, int col);
    void moveLeft(WINDOW* win, int row, int col);
    void travPrint(WINDOW *win, int color, int row, int col);
    int getLowest();
    int getRightMost();
    int getLeftMost();
};
#endif
