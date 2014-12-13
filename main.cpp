//****************************************************
// main.cpp
// Copyright Dillon Swanson and Jonathan Sterling 2014
//  v0.50
//  December 12th, 2014
//  Class: CS 3060 Mike Wright
//  This program simulates the class Tetris game
//****************************************************

#include <ncurses.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <cstdlib>
#include <menu.h>

#include "Block.h"

using namespace std;

// void checkRows(WINDOW* win)
// Purpose: To check all of the rows of the game window to see if
// that there are any completed rows.
// Parameters: A pointer to the window to check.
// Returns: void
void checkRows(WINDOW* win);

// void reDraw(WINDOW* win)
// Purpose: To redraw the game window after making any adjustments
// to it.
// Parameters: A pointer to the window to redraw.
// Returns: void.
void reDraw(WINDOW* win);

// void alarmFunc(int signal)
// Purpose: Signal handler for the game clock.
// Parameters: One integer value that is the signal to be caught.
// Returns: void
void alarmFunc(int signal);

int masterGrid[22][20] = {8};
int lines, gameSpeed = 800000000, WIDTH = 55, HEIGHT = 32;
int x, y;
bool moveDown = false;

int main() {
    initscr();
    WINDOW * gameWin = newwin(24, 22, 5, 13);
    WINDOW * nextBlockWin = newwin(7, 14, 5, 38);
    WINDOW * infoWin = newwin(12, 14, 13, 38);

    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal needs to be colors to play Tetris!\n");
        exit(1);
    }

     getmaxyx(stdscr, y, x);
     if(x < WIDTH || y < HEIGHT)
     {
         endwin();
         printf("ERROR, your terminal is too small. (min %ux%u)\n", WIDTH, HEIGHT);
         return 1;
     }

    
    start_color(); // Start teh col0rz
    curs_set(0); // Hide the cursor
    noecho(); // Turn off echo
    keypad(stdscr, TRUE); // Enable arrow key input
    nodelay(gameWin, TRUE);
    halfdelay(10);
    wborder(gameWin, 0, 0, 0, 0, 0, 0, 0, 0);  // creates the border around the specified window
    wborder(nextBlockWin, 0, 0, 0, 0, 0, 0, 0, 0);
    wborder(infoWin, 0, 0, 0, 0, 0, 0, 0, 0);
    
    refresh();
    wrefresh(gameWin);
    wrefresh(nextBlockWin);
    wrefresh(infoWin);
    
    // Time Code
    timer_t gameTimer;
    struct sigevent sigev;
    sigev.sigev_notify = SIGEV_SIGNAL;
    sigev.sigev_signo = SIGALRM;
    timer_create(CLOCK_REALTIME, &sigev, &gameTimer);
    signal(SIGALRM, alarmFunc);
    struct itimerspec currentSpec;
    struct itimerspec oldSpec;
    currentSpec.it_interval.tv_sec = 0;
    currentSpec.it_interval.tv_nsec = gameSpeed;
    currentSpec.it_value.tv_sec = 0;
    currentSpec.it_value.tv_nsec = gameSpeed;
    timer_settime(gameTimer, 0, &currentSpec, &oldSpec);
    // End Time Code
    
    // Menu Code;
    mvprintw(0, 13, "Press 'q' to quit at any time");
    mvprintw(14, 42, "LINES");
    mvprintw(30, 13, "Use arrow keys to move the blocks\n");
    mvprintw(31, 13, "Use up arrow key to rotate the blocks\n");
    mvprintw(32, 13, "Use spacebar to instantly drop blocks");
    // end Menu Code

    // MAIN GAME LOOP
    bool playing = true;
    while (playing) {
        
        // Reset the masterGrid
        for (int i = 0; i < 22; i++) {
            for (int j = 0; j < 20; j++) {
                masterGrid[i][j] = 8;
            }
        }
        
        reDraw(gameWin);
        int c = 0;
        int blockNum = rand() % 7 + 1;
        bool alive = true;
        lines = 0;
        mvprintw(16, 44, "%d", lines);
        Block* currentBlock = new Block(gameWin, blockNum, 1, 9);
        currentBlock->passMaster(masterGrid);
        int count = 0;
        // Match loop
        while (c != 'q' && alive == true) {
            bool set = false;
            int x = 9;
            int y = 1;

            werase(nextBlockWin); // clears the preview window
            wborder(nextBlockWin, 0, 0, 0, 0, 0, 0, 0, 0);
            wrefresh(nextBlockWin);
            wrefresh(gameWin);

            blockNum = rand() % 7 + 1;
            Block* nextBlock = new Block(nextBlockWin, blockNum, 3, 4);
            alive = currentBlock->spawn(gameWin, y, x);

            // Block loop
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
		    moveDown = false;
                } else if (c == ' ') {
		        while (currentBlock->tryDown(gameWin, y, x))
			    y++;
			set = true;
		for (int i = 0; i < 4; i++) {
                        for (int j = 0; j < 4; j++) {
                            if (currentBlock->grid[currentBlock->rotate][i][j] == 1) {
                                masterGrid[(y - 1) + i][(x - 1) + (j * 2)] = currentBlock->blockID;
                                masterGrid[(y - 1) + i][(x - 1) + (j * 2) + 1] = currentBlock->blockID;
                            }
                        }
                    }
	        }

                c = getch();
            }
            currentBlock = nextBlock;
            currentBlock->passMaster(masterGrid);
        }
            if (c != 'q') {
                mvprintw(3, 13, "Press 'n' to play a new game");
                bool response = false;
                while (!response) {
                    c = getch();
                    if (c == 'n') {
                        response = true;
                    } else if (c == 'q') {
                        response = true;
                        playing = false;
                    }
                }
            } else {
                playing = false;
            }
            mvprintw(3, 13, "                             ");
        }

    endwin();
    return 0;
}

void checkRows(WINDOW* win) {
    for (int i = 0; i < 22; i++) {
        bool hole = false;
        for (int j = 0; j < 20; j++) {
            if (masterGrid[i][j] == 8)
                hole = true;
        }
        if (hole == false) {
            for (int l = i; l > 0; l--)
            {
                for (int k = 0; k < 20; k++){
                    masterGrid[l][k] = masterGrid[l - 1][k];
                }
            }
            lines++;
            mvprintw(16, 44, "%d", lines);
        }
    }
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
            wattron(win, COLOR_PAIR(masterGrid[i][j]));
            wmove(win, i + 1, j + 1);
            waddch(win, ' '|A_REVERSE);
            int colorz = masterGrid[i][j];
            wattroff(win, COLOR_PAIR(masterGrid[i][j]));
            wrefresh(win);
        }
    }
}

void alarmFunc(int signal) {
	moveDown = true;
}

