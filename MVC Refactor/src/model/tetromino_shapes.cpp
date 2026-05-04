/*
 * ============================================================================
 * FILE: tetromino_shapes.cpp
 * PURPOSE: Implementation of static shape matrices for all Tetromino pieces
 * 
 * These definitions must be in a .cpp file to avoid multiple definitions
 * when the header is included in multiple translation units.
 * ============================================================================
 */

#include "tetromino_shapes.h"

// I-Piece shape matrix
const int IPiece::SHAPE[PIECE_SIZE][PIECE_SIZE] = {
    {0,0,0,0},
    {1,1,1,1},
    {0,0,0,0},
    {0,0,0,0}
};

// J-Piece shape matrix
const int JPiece::SHAPE[PIECE_SIZE][PIECE_SIZE] = {
    {1,0,0,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0}
};

// L-Piece shape matrix
const int LPiece::SHAPE[PIECE_SIZE][PIECE_SIZE] = {
    {0,0,1,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0}
};

// O-Piece shape matrix
const int OPiece::SHAPE[PIECE_SIZE][PIECE_SIZE] = {
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0},
    {0,0,0,0}
};

// S-Piece shape matrix
const int SPiece::SHAPE[PIECE_SIZE][PIECE_SIZE] = {
    {0,1,1,0},
    {1,1,0,0},
    {0,0,0,0},
    {0,0,0,0}
};

// T-Piece shape matrix
const int TPiece::SHAPE[PIECE_SIZE][PIECE_SIZE] = {
    {0,1,0,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0}
};

// Z-Piece shape matrix
const int ZPiece::SHAPE[PIECE_SIZE][PIECE_SIZE] = {
    {1,1,0,0},
    {0,1,1,0},
    {0,0,0,0},
    {0,0,0,0}
};
