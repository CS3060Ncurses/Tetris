#include "Block.h"
#include "Templates.h"

using namespace std;

Block::Block(WINDOW* win, int id, int row, int col) 
{
    rotate = 0; 
    blockID = id;
    setGridBounds(); 
    travPrint(win, blockID, row, col);
    wrefresh(win);
}

void Block::passMaster(int master[22][20]) {
    for (int i = 0; i < 22; i++) {
        for (int j = 0; j < 20; j++) {
            localMaster[i][j] = master[i][j];        
        }
    }
}

bool Block::spawn(WINDOW* win, int row, int col) {
    bool collision = false;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (grid[rotate][i][j] == 1) {
                if (localMaster[(row - 1) + i][(col - 1) + (j * 2)] != 8)
                    collision = true;
            }
        }
    }
    travPrint(win, blockID, row, col);
    return !collision;
}

void Block::tryRotate(WINDOW* win, int row, int col) {
    bool collision = false;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (grid[rotate][i][j] == 2) {
                int y = (row - 1) + i;
                int x = (col - 1) + (j * 2);
                int tmp = localMaster[(row - 1) + i][(col - 1) + (j * 2)];
                if (tmp != 8 || x < 0 || x > 19 || y > 21)
                    collision = true;
            }
        }
    }
    if (collision == false)
        this->moveRotate(win, row, col);
}

void Block::moveRotate(WINDOW* win, int row, int col) {
    travPrint(win, 8, row, col);
    if (rotate < 3)
        this->rotate++;
    else
        this->rotate = 0;
    setGridBounds();
    travPrint(win, blockID, row, col);
}

bool Block::tryDown(WINDOW* win, int row, int col) {
    bool collision = false;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (grid[rotate][i][j] == 1) {
                if (localMaster[row + i][(col - 1) + (j * 2)] != 8)
                    collision = true;
            }
        }
    }
    if (row > (22 - bounds[rotate][1]) || collision == true) {
        return false;
    } else {
        this->moveDown(win, row, col);
        return true;
    }
}

void Block::moveDown(WINDOW* win, int row, int col) {
    travPrint(win, 8, row++, col);
    travPrint(win, blockID, row, col);
}

bool Block::tryRight(WINDOW* win, int row, int col) {
    bool collision = false;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (grid[rotate][i][j] == 1) {
                if (localMaster[(row - 1) + i][(col - 1) + (j * 2) + 2] != 8)
                    collision = true;
            }
        }
    }
    if ((col + (bounds[rotate][2] * 2)) >= 20 || collision == true) {
        return false;
    } else {
        this->moveRight(win, row, col);
        return true;
    }
}

void Block::moveRight(WINDOW* win, int row, int col) {
    travPrint(win, 8, row, col);
    col += 2;
    travPrint(win, blockID, row, col);
}

bool Block::tryLeft(WINDOW* win, int row, int col) {
    bool collision = false;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (grid[rotate][i][j] == 1) {
                if (localMaster[(row - 1) + i][(col - 1) + (j * 2) - 1] != 8)
                    collision = true;
            }
        }
    }
    if ((col + (bounds[rotate][0] * 2)) <= 1 || collision == true) {
        return false;
    } else {
        this->moveLeft(win, row, col);
        return true;
    }
}

void Block::moveLeft(WINDOW* win, int row, int col) {
    travPrint(win, 8, row, col);
    col -= 2;
    travPrint(win, blockID, row, col);
}

void Block::travPrint(WINDOW *win, int color, int row, int col) {
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_GREEN, COLOR_BLACK);
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(7, COLOR_RED, COLOR_BLACK);
    init_pair(8, COLOR_BLACK, COLOR_BLACK);
    for (int i = 0; i < 4; i++) {
        int tmpcol = col;
        for (int j = 0; j < 4; j++) {
            if (grid[rotate][i][j] == 1) {
                wattron(win, COLOR_PAIR(color));
                wmove(win, row, tmpcol);
                waddch(win, ' '|A_REVERSE);
                wmove(win, row, tmpcol + 1);
                waddch(win, ' '|A_REVERSE);
                wattroff(win, COLOR_PAIR(color));
                wrefresh(win);
            }
            tmpcol += 2;
        }
        row++;
    }
}

void Block::setGridBounds() {
    if (blockID == 1) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++)
                grid[rotate][i][j] = gridI[rotate][i][j];
        }
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 3; j++)
                bounds[i][j] = boundsI[i][j];
        }  
    }
    if (blockID == 2) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++)
                grid[rotate][i][j] = gridBL[rotate][i][j];
        }
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 3; j++)
                bounds[i][j] = boundsBL[i][j];
        }  
    }
    if (blockID == 3) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++)
                grid[rotate][i][j] = gridL[rotate][i][j];
        }
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 3; j++)
                bounds[i][j] = boundsL[i][j];
        }  
    }
    if (blockID == 4) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++)
                grid[rotate][i][j] = gridB[rotate][i][j];
        }
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 3; j++)
                bounds[i][j] = boundsB[i][j];
        }  
    }
    if (blockID == 5) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++)
                grid[rotate][i][j] = gridS[rotate][i][j];
        }
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 3; j++)
                bounds[i][j] = boundsS[i][j];
        }  
    }
    if (blockID == 6) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++)
                grid[rotate][i][j] = gridT[rotate][i][j];
        }
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 3; j++)
                bounds[i][j] = boundsT[i][j];
        }  
    }
    if (blockID == 7) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++)
                grid[rotate][i][j] = gridZ[rotate][i][j];
        }
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 3; j++)
                bounds[i][j] = boundsZ[i][j];
        }  
    }
}
