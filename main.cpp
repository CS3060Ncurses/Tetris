// Copyright Dillon Swanson and Jonathan Sterling 2014
// v0.02 12/05/14
#include <ncurses.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <cstdlib>

using namespace std;

// This function will either erase or print a block on the screen. Bool set
// to false for erase and true for print. The y and x coordinated are
// where the erasing or printing should be done in the window.
void paint(WINDOW *win, bool draw, int y, int x);
void alarmFunc(int signal);

bool moveDown = false;

int main() {
    int x1, x2, x3, y1, y2, y3;

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
    
    getmaxyx(gameWin, y1, x1);

//    mvprintw(22, 45, "Size is x: %d, y: %d",x1, y1); 

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
    int x = 1; // x coordinate (column)
    int y = 1; // y coordinate (row)

    while (c != 'q') {
        if (c == KEY_RIGHT && x < 18) {
            paint(gameWin, false, y, x);
            x += 2;
            paint(gameWin, true, y, x);
        }
        if (c == KEY_LEFT && x > 1) {
            paint(gameWin, false, y, x);
            x -= 2;
            paint(gameWin, true, y, x);
        }
        if ((moveDown || c == KEY_DOWN) && y < 22) {
            paint(gameWin, false, y, x);
            y++;
            paint(gameWin, true, y, x);
            moveDown = false;
        }
        c = getch();
    }

    endwin();
    return 0;
}

void paint(WINDOW *win, bool draw, int y, int x) {
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_BLACK);

    int pair = 1; // Default red
    if (draw == FALSE)
        pair = 2; // Change to black
    // Remember that a square in the terminal is actually 2 spaces
    wattron(win, COLOR_PAIR(pair));
    wmove(win, y, x);
    waddch(win, ' '|A_REVERSE); // draw first space
    wmove(win, y, x + 1);
    waddch(win, ' '|A_REVERSE); // draw second space
    wattroff(win, COLOR_PAIR(pair));
    wrefresh(win);
}

void alarmFunc(int signal) {
	moveDown = true;
}

