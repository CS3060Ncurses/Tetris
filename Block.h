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

    // Block()
    // Purpose: Default constructor for the block object.
    // Parameters: none
    // Returns: none
    Block();

    // Block(WINDOW* win, int id. int row, int col)
    // Purpose: Parameterized constructor for the block object.
    // Parameters: A pointer to a window to create the block in, an integer value
    // to identify the block ID (1-7), and two other integer values indicating
    // the row and column where the block will be spawned.
    // Returns: none.
    Block(WINDOW* win, int id, int row, int col);

    // void passMaster(int master[22][20])
    // Purpose: To copy over the master grid from the main game to detect
    // collisions.
    // Parameters: A two-dimensional integer array
    // Returns: void
    void passMaster(int master[22][20]);

    // bool spawn(WINDOW* win, int row, int col)
    // Purpose: A collision checker that checks to see if the spawn space is
    // clear. It will draw the new block regardless.
    // Parameters: A pointer to a window to spawn the block and two integer
    // values indicating where the block is to be spawned.
    // Returns: A boolean value indicating whether or not a collision was
    // detected upon spawning.
    bool spawn(WINDOW* win, int row, int col);

    // void tryRotate(WINDOW* win, int row, int col)
    // Purpose: To use collision detection to determine if a rotation is
    // possible.
    // Parameter: A pointer to a window that the block is rotating in and
    // two integer values indicating the position of the block.
    // Returns: void
    void tryRotate(WINDOW* win, int row, int col);

    // void moveRotate(WINDOW* win, int row, int col)
    // Purpose: To rotate the block int a clockwise fashion.
    // Parameters: A pointer to a window that the block is rotating in and
    // two integer values indicating the position of the block.
    // Returns: void
    void moveRotate(WINDOW* win, int row, int col);

    // bool tryDown(WINDOW* win, int row, int col)
    // Purpose: To use collision detection to determine if a downward 
    // movement is possible.
    // Parameters: A pointer to a window that the block is moving in and
    // two integer values indicating the position of the block.
    // Returns: A boolean value indicating if the movement was successful
    // or not. 
    bool tryDown(WINDOW* win, int row, int col);

    // void moveDown(WINDOW* win, int row, int col)
    // Purpose: To move the block downwards.
    // Parameters: A pointer to a window that the block is moving in and
    // two integer values indicating the position of the block.
    // Returns: void
    void moveDown(WINDOW* win, int row, int col);

    // bool tryRight(WINDOW* win, int row, int col)
    // Purpose: To use collision deteciton to determine if a right movement
    // is possible.
    // Parameters: A pointer to a window that he block is moving in and
    // two integer values indicating the position of the block.
    // Returns: A boolean value indicating if the movement was successful
    // or not.
    bool tryRight(WINDOW* win, int row, int col);

    // void moveRight(WINDOW* win, int row, int col)
    // Purpose: To move the block right.
    // Parameters: A pointer to a window that the block is moving in and
    // two integer values indicating the position of the block.
    // Returns: void
    void moveRight(WINDOW* win, int row, int col);

    // bool tryLeft(WINDOW* win, int row, int col)
    // Purpose: To use collision deteciton to determine if a left movement
    // is possible.
    // Parameters: A pointer to a window that he block is moving in and
    // two integer values indicating the position of the block.
    // Returns: A boolean value indicating if the movement was successful
    // or not.   
    bool tryLeft(WINDOW* win, int row, int col);

    // void moveLeft(WINDOW* win, int row, int col)
    // Purpose: To move the block left.
    // Parameters: A pointer to a window that the block is moving in and
    // two integer values indicating the position of the block.
    // Returns: void
    void moveLeft(WINDOW* win, int row, int col);

    // void travPrint(WINDOW* win, int color, int row, int col)
    // Purpose: To traverse through the Block's grid array and print it
    // out to a window.
    // Parameters: A window to print out to, an integer value indicating 
    // the color code (1-7), and tow integer values indicating the position
    // of the block.
    void travPrint(WINDOW* win, int color, int row, int col);

    // void setGridBounds()
    // Purpose: To copy over the grid and bounds array from the Templates.h 
    // file depending on the blockID determined upon construction.
    // Parameters: none
    // Returns: void
    void setGridBounds();
};
#endif
