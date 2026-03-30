/*
 * ============================================================================
 * FILE: tetromino.cpp
 * PURPOSE: Implementation of Tetromino class
 * ============================================================================
 */

#include "tetromino.h"
#include <cstring>
#include <cstdlib>
#include <ctime>

// Define all 7 Tetromino shapes
const int Tetromino::SHAPES[7][PIECE_SIZE][PIECE_SIZE] = {
    // I-piece (cyan line)
    {
        {0,0,0,0},
        {1,1,1,1},
        {0,0,0,0},
        {0,0,0,0}
    },
    // J-piece (blue L-shape)
    {
        {1,0,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // L-piece (orange L-shape)
    {
        {0,0,1,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // O-piece (yellow square)
    {
        {0,1,1,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // S-piece (green zigzag)
    {
        {0,1,1,0},
        {1,1,0,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // T-piece (purple T-shape)
    {
        {0,1,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // Z-piece (red zigzag)
    {
        {1,1,0,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    }
};

Tetromino::Tetromino() 
    : currentType(0), nextType(0), posX(0), posY(0) {
    // Initialize pieces to empty
    for (int y = 0; y < PIECE_SIZE; y++) {
        for (int x = 0; x < PIECE_SIZE; x++) {
            currentPiece[y][x] = 0;
            nextPiece[y][x] = 0;
        }
    }
    // Seed random number generator
    static bool seeded = false;
    if (!seeded) {
        srand((unsigned)time(NULL));
        seeded = true;
    }
}

Tetromino::~Tetromino() {
    // No dynamic memory
}

void Tetromino::initialize() {
    // Generate the first next piece type
    nextType = rand() % 7;
    // Copy next piece data
    memcpy(nextPiece, SHAPES[nextType], sizeof(nextPiece));
    
    // Spawn the first active piece
    spawnNewPiece();
}

void Tetromino::spawnNewPiece() {
    // Move next piece to become the current piece
    currentType = nextType;
    memcpy(currentPiece, nextPiece, sizeof(currentPiece));
    
    // Generate a new random piece for the next_piece preview
    nextType = rand() % 7;
    memcpy(nextPiece, SHAPES[nextType], sizeof(nextPiece));
    
    // Reset position to top-center (will be adjusted by GameEngine)
    posX = 8;  // (BOARD_WIDTH/2) - 2 = 10 - 2 = 8
    posY = 0;
}

void Tetromino::setPosition(int x, int y) {
    posX = x;
    posY = y;
}

void Tetromino::moveBy(int dx, int dy) {
    posX += dx;
    posY += dy;
}

void Tetromino::rotateCW(int piece[PIECE_SIZE][PIECE_SIZE]) {
    // Rotate 90 degrees clockwise
    // Formula: rotated[x][3-y] = original[y][x]
    int rotated[PIECE_SIZE][PIECE_SIZE];
    for (int y = 0; y < PIECE_SIZE; y++) {
        for (int x = 0; x < PIECE_SIZE; x++) {
            rotated[x][3 - y] = piece[y][x];
        }
    }
    // Copy back
    memcpy(piece, rotated, sizeof(rotated));
}

void Tetromino::copyPiece(const int source[PIECE_SIZE][PIECE_SIZE], 
                          int dest[PIECE_SIZE][PIECE_SIZE]) const {
    for (int y = 0; y < PIECE_SIZE; y++) {
        for (int x = 0; x < PIECE_SIZE; x++) {
            dest[y][x] = source[y][x];
        }
    }
}

void Tetromino::rotate() {
    // Save the original piece configuration in case rotation fails
    int original[PIECE_SIZE][PIECE_SIZE];
    copyPiece(currentPiece, original);
    
    // Rotate the piece
    rotateCW(currentPiece);
    
    // Note: Collision detection and wall-kick is handled by GameEngine
}

int Tetromino::getPieceAt(int x, int y) const {
    if (x < 0 || x >= PIECE_SIZE || y < 0 || y >= PIECE_SIZE) {
        return 0;
    }
    return currentPiece[y][x];
}

const int* Tetromino::getCurrentPiece() const {
    return (const int*)currentPiece;
}

const int* Tetromino::getNextPiece() const {
    return (const int*)nextPiece;
}

const int* Tetromino::getPieceShape(int type) {
    if (type < 0 || type >= 7) {
        return (const int*)SHAPES[0];
    }
    return (const int*)SHAPES[type];
}
