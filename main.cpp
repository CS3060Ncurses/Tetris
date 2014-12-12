// Copyright Dillon Swanson and Jonathan Sterling 2014
// v0.20 12/11/14
#include <ncurses.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <cstdlib>

#include "Block.h"

using namespace std;

void checkRows(WINDOW* win);
void reDraw(WINDOW* win);
void alarmFunc(int signal);

int masterGrid[22][20] = {8};
bool moveDown = false;

int main() {
    for (int i = 0; i < 22; i++) {
        for (int j = 0; j < 20; j++) {
            masterGrid[i][j] = 8;
        }
    }
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

    int c = 0;
    int blockNum = rand() % 7 + 1;    
    Block* currentBlock = new Block(gameWin, blockNum, 1, 9);
    currentBlock->passMaster(masterGrid);
    bool alive = true;
    while (c != 'q' && alive == true) {
        bool set = false;
        int x = 9;
        int y = 1;
        blockNum = rand() % 7 + 1;
        //blockNum = 1;

        werase(nextBlockWin); // clears the preview window
        wborder(nextBlockWin, 0, 0, 0, 0, 0, 0, 0, 0);
        wrefresh(nextBlockWin);
        Block* nextBlock = new Block(nextBlockWin, blockNum, 3, 4);

        //Print out masterGrid for debugging
        //for (int i = 0; i < 22; i ++) {
          //  for (int j = 0; j < 20; j++) {
            //    if (masterGrid[i][j] != 8) {
              //      wmove(gameWin, (i + 1), (j + 1));
                //    waddch(gameWin, 'x');
                //}
            //}
        //}
        wrefresh(gameWin);
        //if (!currentBlock->spawn(gameWin, y, x))
            //alive = false;
        alive = currentBlock->spawn(gameWin, y, x);
        // Main game loop
        while (c != 'q' && set == false) {
            if (c == KEY_UP) {
                currentBlock->tryRotate(gameWin, y, x);
            } else if (c == KEY_RIGHT) {
                if (currentBlock->tryRight(gameWin, y, x)) 
                    x += 2;
            } else if (c == KEY_LEFT) {
                if (currentBlock->tryLeft(gameWin, y, x))
                    x -= 2;
            } else if ((moveDown || c == KEY_DOWN) && y < 22) { 
                if (!currentBlock->tryDown(gameWin, y, x)) {
                    set = true;
                    for (int i = 0; i < 4; i++) {
                        for (int j = 0; j < 4; j++) {
                            if (currentBlock->grid[currentBlock->rotate][i][j] == 1) {
                                masterGrid[(y - 1) + i][(x - 1) + (j * 2)] = currentBlock->blockID;
                                masterGrid[(y - 1) + i][(x - 1) + (j * 2) + 1] = currentBlock->blockID;
                            }
                        }
                    }
                    checkRows(gameWin);

                    reDraw(gameWin);
                } else {
                    y++;
                }
            } else if (c == ' ') {
		        while (!currentBlock->tryDown(gameWin, y, x))
			        y++;
	        }

            c = getch();
        }
        currentBlock = nextBlock;
        currentBlock->passMaster(masterGrid);
    }

    endwin();
    return 0;
}

void checkRows(WINDOW* win) {
    //bool hole = false;
    //int n = 0;
    for (int i = 0; i < 22; i++) {
        bool hole = false;
        for (int j = 0; j < 20; j++) {
            if (masterGrid[i][j] == 8)
                hole = true;
            //n++;
        }
        if (hole == false) {
            for (int l = i; l > 0; l--)
            {
                for (int k = 0; k < 20; k++){
                    masterGrid[l][k] = masterGrid[l - 1][k];
                }
            }
        }
    }
    //if (hole == false) {
      //  for (int i = 0; i < 22; i++) {
        //    for (int j = 0; j < 20; j++){
          //      if (i < 21)
            //        masterGrid[i][j] = masterGrid[i - 1][j];
            //}
        //}
    //}
    //wmove(win, 9, 9);
    //waddch(win, (char)n);
    //wrefresh(win);
}

void reDraw(WINDOW* win) {
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_GREEN, COLOR_BLACK);
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(7, COLOR_RED, COLOR_BLACK);
    init_pair(8, COLOR_BLACK, COLOR_BLACK);

    for (int i = 0; i < 22; i ++) {
        for (int j = 0; j < 20; j++) {
            //if (masterGrid[i][j] == 8) {
            wattron(win, COLOR_PAIR(masterGrid[i][j]));
            wmove(win, i + 1, j + 1);
            waddch(win, ' '|A_REVERSE);
            int colorz = masterGrid[i][j];
            wattroff(win, COLOR_PAIR(masterGrid[i][j]));
            wrefresh(win);
            //}
        }
    }
}

void alarmFunc(int signal) {
	moveDown = true;
}

