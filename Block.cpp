#include "Brick.h"
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
bool Block::tryDown(WINDOW* win) {

}

void Block::moveDown(WINDOW* win, int row, int col) {
    travPrint(win, MY_BLACK, row++, col);
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


