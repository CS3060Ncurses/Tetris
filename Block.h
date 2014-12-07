#include <ncurses.h>

using namespace std;
#ifndef BLOCK_H
#define BLOCK_H

class Block {
private:
    int grid[3][3];
public:
    Block();
    Block(WINDOW* win, int id, int row, int col);
    bool tryRight(WINDOW* win);
    bool tryLeft(WINDOW* win);
    bool tryDown(WINDOW* win);
    void moveDown(WINDOW* win, int row, int col);
    void travPrint(WINDOW *win, int color, int row, int col);
};
#endif
