#include <ncurses.h>

using namespace std;
#ifndef BLOCK_H
#define BLOCK_H

class Block {
private:
public:
    int grid[4][4][4];
    int bounds[4][3];
    int localMaster[22][20];
    int blockID;
    int rotate;
    int myColor;
    Block();
    Block(WINDOW* win, int id, int row, int col);
    void passMaster(int master[22][20]);
    void setGridBounds();
    void tryRotate(WINDOW* win, int row, int col);
    bool tryRight(WINDOW* win, int row, int col);
    bool tryLeft(WINDOW* win, int row, int col);
    bool tryDown(WINDOW* win, int row, int col);
    void moveRotate(WINDOW* win, int row, int col);
    void moveDown(WINDOW* win, int row, int col);
    void moveRight(WINDOW* win, int row, int col);
    void moveLeft(WINDOW* win, int row, int col);
    void travPrint(WINDOW *win, int color, int row, int col);
};
#endif
