/*
 * ============================================================================
 * FILE: board.h
 * PURPOSE: Board class - Manages the game grid and grid operations
 * 
 * RESPONSIBILITY: Handle the 2D array representation of the game board
 * 
 * SINGLE RESPONSIBILITY: This class is responsible ONLY for:
 *   - Storing the game board state
 *   - Checking cell occupancy
 *   - Clearing complete lines
 *   - Locking pieces to the board
 * 
 * This class DOES NOT:
 *   - Know about SDL/rendering
 *   - Handle collision detection (delegated to Game Engine)
 *   - Manage piece shapes or rotation
 * ============================================================================
 */

#ifndef BOARD_H
#define BOARD_H

#define BOARD_WIDTH 20
#define BOARD_HEIGHT 20
#define EMPTY_CELL 0

class Board {
private:
    int grid[BOARD_HEIGHT][BOARD_WIDTH];
    
public:
    // Constructor and Destructor
    Board();
    ~Board();
    
    // Board manipulation
    void clear();
    void lockPieceToBoard(const int piece[4][4], int pieceX, int pieceY, int pieceType);
    int clearCompleteLines();
    
    // Grid queries
    int getCell(int x, int y) const;
    void setCell(int x, int y, int value);
    bool isCellOccupied(int x, int y) const;
    bool isPositionValid(int x, int y) const;
    
    // Getters
    const int* const* getGrid() const { return (const int* const*)grid; }
};

#endif // BOARD_H
