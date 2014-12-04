// Copyright Dillon Swanson and Jonathan Sterling 2014
// v0.01 12/03/14
// First version testing ncurses library. It simulates the basic tetris engine with a single block
// that gravitates downward at a slow pace. The getch() function with halfdelay() set will more than likely
// be replaced in future versions. Also note that there are no window restrictions set so if you move the
// block out of the bounds of your terminal window you will get some weird effects. Have fun!
#include <ncurses.h>
#include <cstdlib>

// paint(bool, int, int)
// This function will either erase or print a block on the screen. Bool set
// to false for erase and true for print. The y and x coordinated are
// where the erasing or printing should be done in the window.
void paint(WINDOW *win, bool draw, int y, int x);

int main() {
    int x1, x2, x3, y1, y2, y3;

    initscr();
    WINDOW * gameWin = newwin(22, 35, 5, 2);  // creates a new window 
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
    halfdelay(4); // makes getch() return ERR if no input is detected within
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

    int c = 0; // Character read from stdin
    int x = 0; // x coordinate (column)
    int y = 0; // y coordinate (row)
    do { // do-while loop, may be replaced for a while loop
        paint(gameWin, false, y, x); // erase the previous block
        switch(c) {
            case KEY_RIGHT:
                x += 2; // One block unit is actually 2 space characters.
                break;
            case KEY_LEFT:
                x -= 2;
                break;
            case ERR: // If no input is seen, move down. The problem with this
                y++;  // is that ANY input will suspend this.
                break;
        }
        paint(gameWin, true, y, x); // draw the block in the new position
    } while ((c = getch()));

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
    attron(COLOR_PAIR(pair));
//    wmove(win, y, x);
    move(y, x);
    addch(' '|A_REVERSE); // draw first space
//    wmove(win, y, x + 1);
    move(y, x + 1);
    addch(' '|A_REVERSE); // draw second space
    attroff(COLOR_PAIR(pair));
    refresh();
//    wrefresh(win);
}

