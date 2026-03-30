/*
 * ============================================================================
 * FILE: board.cpp
 * PURPOSE: Implementation of Board class
 * ============================================================================
 */

#include "board.h"
#include <cstring>

Board::Board() {
    clear();
}

Board::~Board() {
    // No dynamic memory, destructor empty
}

void Board::clear() {
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            grid[y][x] = EMPTY_CELL;
        }
    }
}

void Board::lockPieceToBoard(const int piece[4][4], int pieceX, int pieceY, int pieceType) {
    // LSP COMPLIANCE: All piece types are stored using identical logic
    // No shape gets special storage, validation, or handling
    // typeID is just a data identifier (typeId + 1 to distinguish from empty cells)
    // The Board does NOT make behavior decisions based on piece type
    
    // Lock the piece to the board by storing its type value in board cells
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (piece[y][x]) {
                int boardX = pieceX + x;
                int boardY = pieceY + y;
                // Only place on board if within bounds (identical for all pieces)
                if (boardY >= 0 && boardY < BOARD_HEIGHT && 
                    boardX >= 0 && boardX < BOARD_WIDTH) {
                    grid[boardY][boardX] = pieceType + 1;
                }
            }
        }
    }
}

int Board::clearCompleteLines() {
    int linesCleared = 0;

    // Scan board from bottom to top
    for (int y = BOARD_HEIGHT - 1; y >= 0; y--) {
        // Check if current line is complete (no empty cells)
        int lineComplete = 1;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (!grid[y][x]) {
                // Found an empty cell, line is not complete
                lineComplete = 0;
                break;
            }
        }
        
        if (lineComplete) {
            // Line is complete, increment counter and remove it
            linesCleared++;
            
            // Drop all lines above down by one row
            for (int y2 = y; y2 > 0; y2--) {
                for (int x = 0; x < BOARD_WIDTH; x++) {
                    grid[y2][x] = grid[y2 - 1][x];
                }
            }
            
            // Clear the top line
            for (int x = 0; x < BOARD_WIDTH; x++) {
                grid[0][x] = EMPTY_CELL;
            }
            
            // Increment y to re-check the same row (now contains shifted pieces)
            y++;
        }
    }

    return linesCleared;
}

int Board::getCell(int x, int y) const {
    if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT) {
        return -1;  // Out of bounds
    }
    return grid[y][x];
}

void Board::setCell(int x, int y, int value) {
    if (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT) {
        grid[y][x] = value;
    }
}

bool Board::isCellOccupied(int x, int y) const {
    if (!isPositionValid(x, y)) {
        // Out of bounds cells are considered "occupied" (for collision detection)
        return true;
    }
    return grid[y][x] != EMPTY_CELL;
}

bool Board::isPositionValid(int x, int y) const {
    return (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT);
}
