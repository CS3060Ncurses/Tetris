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
void paint(bool draw, int y, int x);

int main() {
    initscr();
    
    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal needs to be colors to play Tetris!\n");
        exit(1);
    }
    
    start_color(); // Start teh col0rz
    curs_set(0); // Hide the cursor
    noecho(); // Turn off echo
    keypad(stdscr, TRUE); // Enable arrow key input
    halfdelay(10); // makes getch() return ERR if no input is detected within
                   // 1 second of the function being called. This is going to
                   // to be replaced for obvious reasons.
    refresh();
    
    int c = 0; // Character read from stdin
    int x = 0; // x coordinate (column)
    int y = 0; // y coordinate (row)
    do { // do-while loop, may be replaced for a while loop
        paint(false, y, x); // erase the previous block
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
        paint(true, y, x); // draw the block in the new position
    } while ((c = getch()));

    endwin();
    return 0;
}

void paint(bool draw, int y, int x) {
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_BLACK);
    int pair = 1; // Default red
    if (draw == FALSE)
        pair = 2; // Change to black
    // Remember that a square in the terminal is actually 2 spaces
    attron(COLOR_PAIR(pair));
    move(y, x);
    addch(' '|A_REVERSE); // draw first space
    move(y, x + 1);
    addch(' '|A_REVERSE); // draw second space
    attroff(COLOR_PAIR(pair));
    refresh();
}
