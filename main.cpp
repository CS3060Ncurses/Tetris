// Copyright Dillon Swanson and Jonathan Sterling 2014
// v0.11 12/11/14
#include <ncurses.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <cstdlib>

#include "Block.h"

using namespace std;

void paint(WINDOW *win, int color, int y, int x);
void alarmFunc(int signal);
void spawnBrick(WINDOW* win, int grid[3][3]);

int masterGrid[22][20] = {0};

bool moveDown = false;

int main() {
    initscr();
    WINDOW * gameWin = newwin(24, 22, 5, 13); 
    					     // params are (nlines, ncols, ystart, xstart)
    WINDOW * nextBlockWin = newwin(7, 14, 5, 38);
    WINDOW * infoWin = newwin(12, 14, 13, 38);

    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal needs to be colors to play Tetris!\n");
        exit(1);
    }
    
    start_color(); // Start teh col0rz
    curs_set(0); // Hide the cursor
    noecho(); // Turn off echo
    keypad(stdscr, TRUE); // Enable arrow key input
    nodelay(gameWin, TRUE);
    halfdelay(10); // makes getch() return ERR if no input is detected within
                   // 1 second of the function being called. This is going to
                   // to be replaced for obvious reasons.
    wborder(gameWin, 0, 0, 0, 0, 0, 0, 0, 0);  // creates the border around the specified window
    wborder(nextBlockWin, 0, 0, 0, 0, 0, 0, 0, 0);
    wborder(infoWin, 0, 0, 0, 0, 0, 0, 0, 0);
    
    refresh();
    wrefresh(gameWin); // needed to refresh a specific window
    wrefresh(nextBlockWin);
    wrefresh(infoWin);
    
    // Time Code
    timer_t gameTimer;
    struct sigevent sigev;
    sigev.sigev_notify = SIGEV_SIGNAL;
    sigev.sigev_signo = SIGALRM;
    timer_create(CLOCK_REALTIME, &sigev, &gameTimer);
    signal(SIGALRM, alarmFunc); // depricated function
    struct itimerspec currentSpec;
    struct itimerspec oldSpec;
    currentSpec.it_interval.tv_sec = 0;
    currentSpec.it_interval.tv_nsec = 800000000;
    currentSpec.it_value.tv_sec = 0;
    currentSpec.it_value.tv_nsec = 800000000;
    timer_settime(gameTimer, 0, &currentSpec, &oldSpec);
    // End Time Code

    int c = 0; // Character read from stdin
    //int x = 1; // x coordinate (column)
    //int y = 1; // y coordinate (row)
    
    while (c != 'q') {
    bool set = false;
    int x = 1;
    int y = 1;
    int blockNum = rand() % 7 + 1;
    Block* currentBlock = new Block(gameWin, masterGrid, blockNum, y, x);
    // Main game loop
    while (c != 'q' && set == false) {
        if (c == KEY_UP) {
            currentBlock->tryRotate(gameWin, y, x);
        }
        if (c == KEY_RIGHT) {
            if (currentBlock->tryRight(gameWin, y, x)) 
                x += 2;
        }
        if (c == KEY_LEFT) {
            ///paint(gameWin, MY_BLACK, y, x);
            //x -= 2;
            //paint(gameWin, MY_RED, y, x);
            if (currentBlock->tryLeft(gameWin, y, x))
                x -= 2;
        }
        if ((moveDown || c == KEY_DOWN) && y < 22) { 
            if (!currentBlock->tryDown(gameWin, y, x)) {
                set = true;
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 4; j++) {
                        if (currentBlock->grid[currentBlock->rotate][i][j] == 1) {
                            //masterGrid[y + i][x + (j * 2)] = 1;
                            //masterGrid[y + i][x + (j * 2) + 1] = 1;
                            //masterGrid[y - 1][x - 1] = 1;
                            masterGrid[(y - 1) + i][(x - 1) + (j * 2)] = 1;
                            masterGrid[(y - 1) + i][(x - 1) + (j * 2) + 1] = 1;
                        }
                    }
                }
                //for (int i = 0; i < 3; i++) {
                //    for (int j = 0; j < 3; j++) {
                //        if (currentBlock->grid[currentBlock->rotate][i][j] == 1)
                //            masterGrid[y + i][x + j] = 1;
                //    }
                //}
            } else {
                y++;
            }
        }
	    if (c == ' ') {
	        paint(gameWin, 8, y, x);
	        y = 21;
	        paint(gameWin, 8, y, x);
	        moveDown = false;
	    }
        c = getch();
    }
    }

    endwin();
    return 0;
}

void updateMaster() {

}

void paint(WINDOW *win, int color, int y, int x) {
    init_pair(1, COLOR_BLACK, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);

    wattron(win, COLOR_PAIR(color));
    wmove(win, y, x);
    waddch(win, ' '|A_REVERSE); // draw first space
    wmove(win, y, x + 1);
    waddch(win, ' '|A_REVERSE); // draw second space
    wattroff(win, COLOR_PAIR(color));
    wrefresh(win);
}

void alarmFunc(int signal) {
	moveDown = true;
}

